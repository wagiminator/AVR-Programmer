// ===================================================================================
// USBasp Functions for CH551, CH552 and CH554                                * v1.2 *
// ===================================================================================

#include "usb_asp.h"

static __bit    prog_address_newmode = 0;
static uint8_t  prog_blockflags;
static uint8_t  prog_pagecounter;
static uint32_t prog_address;
static uint16_t prog_pagesize;

// ===================================================================================
// ASP-Specific USB Handler Functions
// ===================================================================================

// Init ASP
void ASP_init(void) {
  ISP_disconnect();                       // disconnect ISP/TPI bus
  USB_init();                             // setup USB
}

// Setup/reset ASP endpoints
void ASP_EP_init(void) {
  UEP3_DMA    = (uint16_t)EP3_buffer;     // EP3 data transfer address
  UEP3_CTRL   = bUEP_AUTO_TOG             // EP3 Auto flip sync flag
              | UEP_T_RES_NAK;            // EP3 IN transaction returns NAK
  UEP2_3_MOD |= bUEP3_TX_EN;              // EP3 TX enable
  UEP3_T_LEN  = 0;                        // EP3 nothing to transmit
}

// ===================================================================================
// ASP-Specific USB Control Transfers
// ===================================================================================

// Handle VENDOR SETUP requests
uint8_t ASP_control(void) {
  uint8_t len, i;
  len = USB_SetupLen >= EP0_SIZE ? EP0_SIZE : USB_SetupLen;
  
  switch(USB_SetupReq) {

    case USBASP_FUNC_CONNECT:
      prog_address_newmode = 0;         // set compatibility mode
      ISP_connect();                    // connect ISP bus
      return 0;

    case USBASP_FUNC_DISCONNECT:
      ISP_disconnect();                 // disconnect ISP bus
      return 0;

    case USBASP_FUNC_ENABLEPROG:
      EP0_buffer[0] = ISP_enterProgrammingMode();
      return 1;

    case USBASP_FUNC_TRANSMIT:
      ISP_command(EP0_buffer + 2, EP0_buffer + 0);
      return 4;

    case USBASP_FUNC_READFLASH:
      if(!prog_address_newmode) prog_address = *(uint16_t*) &EP0_buffer[2];  
      for(i=0; i<len; i++) EP0_buffer[i] = ISP_readFlash(prog_address++);
      return len;

    case USBASP_FUNC_READEEPROM:
      if(!prog_address_newmode) prog_address = *(uint16_t*) &EP0_buffer[2];      
      for(i=0; i<len; i++) EP0_buffer[i] = ISP_readEEPROM(prog_address++);
      return len;

    case USBASP_FUNC_WRITEFLASH:
      if(!prog_address_newmode) prog_address = *(uint16_t*) &EP0_buffer[2];
      prog_pagesize   = EP0_buffer[4];
      prog_blockflags = EP0_buffer[5] & 0x0F;
      prog_pagesize  += (((uint16_t)EP0_buffer[5] & 0xF0) << 4);
      if(prog_blockflags & PROG_BLOCKFLAG_FIRST)
        prog_pagecounter = prog_pagesize;
      return 0;

    case USBASP_FUNC_WRITEEEPROM:
      if(!prog_address_newmode) prog_address = *(uint16_t*) &EP0_buffer[2];
      return 0;

    case USBASP_FUNC_SETLONGADDRESS:
      prog_address_newmode = 1;
      prog_address = *(uint32_t*) &EP0_buffer[2];
      return 0;

    case USBASP_FUNC_SETISPSCK:
      EP0_buffer[0] = 0;
      return 1;

    case USBASP_FUNC_TPI_CONNECT:
      TPI_dly_cnt = EP0_buffer[2]; //*(uint16_t*) &EP0_buffer[2];
      TPI_connect();
      return 0;

    case USBASP_FUNC_TPI_DISCONNECT:
      TPI_disconnect();
      return 0;

    case USBASP_FUNC_TPI_RAWREAD:
      EP0_buffer[0] = TPI_readByte();
      return 1;

    case USBASP_FUNC_TPI_RAWWRITE:
      TPI_writeByte(EP0_buffer[2]);
      return 0;

    case USBASP_FUNC_TPI_READBLOCK:
      prog_address = *(uint16_t*) &EP0_buffer[2];
      TPI_readBlock(prog_address, EP0_buffer, len);
      prog_address += len;
      return len;

    case USBASP_FUNC_TPI_WRITEBLOCK:
      prog_address = *(uint16_t*) &EP0_buffer[2];
      return 0;

    case USBASP_FUNC_GETCAPABILITIES:
      *(uint32_t*) &EP0_buffer[0] = (uint32_t)USBASP_CAP_0_TPI;
      return 4;

    #ifdef WCID_VENDOR_CODE
    case WCID_VENDOR_CODE:
      if(USB_SetupBuf->wIndexL == 0x04) {
        USB_pDescr = WCID_FEATURE_DESCR;
        len = WCID_FEATURE_DESCR[0];
        if(USB_SetupLen > len) USB_SetupLen = len;
        len = USB_SetupLen >= EP0_SIZE ? EP0_SIZE : USB_SetupLen;
        USB_EP0_copyDescr(len);
        return len;
      }
      return 0;
    #endif

    default:
      return 0xff;
  }
}

// ===================================================================================
// ASP-Specific USB IN Transfers
// ===================================================================================

// Endpoint 0 VENDOR IN handler
void ASP_EP0_IN(void) {
  uint8_t len, i;
  len = USB_SetupLen >= EP0_SIZE ? EP0_SIZE : USB_SetupLen;

  switch(USB_SetupReq) {

    case USBASP_FUNC_READFLASH:
      for(i=0; i<len; i++) EP0_buffer[i] = ISP_readFlash(prog_address++);
      break;

    case USBASP_FUNC_READEEPROM:
      for(i=0; i<len; i++) EP0_buffer[i] = ISP_readEEPROM(prog_address++);
      break;

    case USBASP_FUNC_TPI_READBLOCK:
      TPI_readBlock(prog_address, EP0_buffer, len);
      prog_address += len;
      break;

    #ifdef WCID_VENDOR_CODE
    case WCID_VENDOR_CODE:
      USB_EP0_copyDescr(len);                             
      break;
    #endif

    default:
      UEP0_CTRL = bUEP_R_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
      return;
  }

  USB_SetupLen -= len;
  UEP0_T_LEN    = len;
  UEP0_CTRL    ^= bUEP_T_TOG;
}

// ===================================================================================
// ASP-Specific USB OUT Transfers
// ===================================================================================

// Endpoint 0 VENDOR OUT handler
void ASP_EP0_OUT(void) {
  uint8_t len, i;
  len = USB_SetupLen >= USB_RX_LEN ? USB_RX_LEN : USB_SetupLen;

  switch(USB_SetupReq) {

    case USBASP_FUNC_WRITEFLASH:
      for(i=0; i<len; i++) {
        if(prog_pagesize == 0) ISP_writeFlash(prog_address, EP0_buffer[i], 1);
        else {
          ISP_writeFlash(prog_address, EP0_buffer[i], 0);
          if(--prog_pagecounter == 0) {
            ISP_flushPage(prog_address, EP0_buffer[i]);
            prog_pagecounter = prog_pagesize;
          }
        }
        prog_address++;
      }
      USB_SetupLen -= len;
      if(USB_SetupLen == 0) {
        if((prog_blockflags & PROG_BLOCKFLAG_LAST) && (prog_pagecounter != prog_pagesize))
          ISP_flushPage(prog_address, EP0_buffer[i]);
      }
      UEP0_CTRL ^= bUEP_R_TOG;
      return;

    case USBASP_FUNC_WRITEEEPROM:
      for(i=0; i<len; i++) ISP_writeEEPROM(prog_address++, EP0_buffer[i]);
      USB_SetupLen -= len;
      UEP0_CTRL ^= bUEP_R_TOG;
      return;

    case USBASP_FUNC_TPI_WRITEBLOCK:
      TPI_writeBlock(prog_address, EP0_buffer, len);
      prog_address += len;
      USB_SetupLen -= len;
      UEP0_CTRL ^= bUEP_R_TOG;
      return;

    default:
      UEP0_CTRL = bUEP_T_TOG | UEP_T_RES_ACK | UEP_R_RES_ACK;
      return;
  }
}
