// ===================================================================================
// USBasp Functions for CH551, CH552 and CH554
// ===================================================================================

#include "isp.h"
#include "tpi.h"
#include "ch554.h"
#include "config.h"
#include "usb_asp.h"
#include "usb_handler.h"

__bit    prog_address_newmode = 0;
uint8_t  prog_state = PROG_STATE_IDLE;
uint8_t  prog_blockflags;
uint8_t  prog_pagecounter;
uint32_t prog_address;
uint16_t prog_nbytes = 0;
uint16_t prog_pagesize;

extern void USB_EP0_IN(void);             // EP0 IN buffer
extern void USB_EP0_OUT(void);            // EP0 OUT buffer

// ===================================================================================
// ASP-Specific USB Handler Functions
// ===================================================================================

// Init ASP
void ASP_init(void) {
  ISP_disconnect();                       // disconnect ISP/TPI bus
  USB_init();                             // setup USB
  UEP1_T_LEN = 0;
}

// Setup ASP endpoints
void ASP_setup(void) {
  UEP1_DMA    = EP1_ADDR;                 // EP1 data transfer address
  UEP1_CTRL   = bUEP_AUTO_TOG             // EP1 Auto flip sync flag
              | UEP_T_RES_NAK;            // EP1 IN transaction returns NAK
  UEP4_1_MOD  = bUEP1_TX_EN;              // EP1 TX enable (0x40)
}

// Reset ASP parameters
void ASP_reset(void) {
  UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
}

// ===================================================================================
// ASP-Specific USB Control Transfers
// ===================================================================================

// Handle non-standard control requests
uint8_t ASP_control(void) {
  uint8_t len, i;
  
  if((USB_setupBuf->bRequestType & USB_REQ_TYP_MASK) == USB_REQ_TYP_VENDOR) {
    USB_MSG_flags = USB_FLG_USE_USER_RW;
    switch(SetupReq) {
      case USBASP_FUNC_CONNECT:
        prog_address_newmode = 0;         // set compatibility mode
        ISP_setSpeed(ISP_sck);            // set SPI clock frequency
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
      case USBASP_FUNC_READEEPROM:
        if(!prog_address_newmode)
          prog_address = *(uint16_t*) &EP0_buffer[2];
        prog_nbytes    = *(uint16_t*) &EP0_buffer[6];
        if(SetupReq == USBASP_FUNC_READFLASH)
             prog_state = PROG_STATE_READFLASH;
        else prog_state = PROG_STATE_READEEPROM;        
        len = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
        for(i=0; i<len; i++) {
          if(prog_state == PROG_STATE_READFLASH)
               EP0_buffer[i] = ISP_readFlash(prog_address);
          else EP0_buffer[i] = ISP_readEEPROM(prog_address);
          prog_address++;
        }
        if(len < EP0_SIZE) prog_state = PROG_STATE_IDLE;
        SetupLen -= len;        
        return len;

      case USBASP_FUNC_WRITEFLASH:
      case USBASP_FUNC_WRITEEEPROM:
        if(!prog_address_newmode)
          prog_address = *(uint16_t*) &EP0_buffer[2];
        prog_nbytes    = *(uint16_t*) &EP0_buffer[6];

        if(SetupReq == USBASP_FUNC_WRITEFLASH) {
          prog_pagesize   = EP0_buffer[4];
          prog_blockflags = EP0_buffer[5] & 0x0F;
          prog_pagesize  += (((uint16_t)EP0_buffer[5] & 0xF0) << 4);
          if(prog_blockflags & PROG_BLOCKFLAG_FIRST)
            prog_pagecounter = prog_pagesize;
          prog_state = PROG_STATE_WRITEFLASH;
        }
        else {
          prog_pagesize   = 0;
          prog_blockflags = 0;
          prog_state = PROG_STATE_WRITEEEPROM;
        }
        return 0;

      case USBASP_FUNC_SETLONGADDRESS:
        prog_address_newmode = 1;
        prog_address = *(uint32_t*) &EP0_buffer[2];
        return 0;

      case USBASP_FUNC_SETISPSCK:
        ISP_sck = EP0_buffer[2];
        EP0_buffer[0] = 0;
        return 1;

      case USBASP_FUNC_TPI_CONNECT:
        TPI_dly_cnt = *(uint16_t*) &EP0_buffer[2];
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
      case USBASP_FUNC_TPI_WRITEBLOCK:
        prog_address = *(uint16_t*) &EP0_buffer[2];
        prog_nbytes  = *(uint16_t*) &EP0_buffer[6];
        if(SetupReq == USBASP_FUNC_TPI_READBLOCK)
             prog_state = PROG_STATE_TPI_READ;
        else prog_state = PROG_STATE_TPI_WRITE;
        return 0;

      #ifdef WCID_VENDOR_CODE
      case WCID_VENDOR_CODE:
        len = WCID_FEATURE_DESCR[0];
        if(USB_setupBuf->wIndexL == 0x04) {
          USB_MSG_flags = USB_FLG_MSGPTR_IS_ROM;
          pDescr = WCID_FEATURE_DESCR;
          len = WCID_FEATURE_DESCR[0];
          return len;
        }
        return 0xFF;
      #endif

      default:
        return 0xFF;
    }
  }
  else return 0xFF;
}

// ===================================================================================
// ASP-Specific USB Endpoint Handlers
// ===================================================================================

// Endpoint 0 IN handler
void ASP_EP0_IN(void) {
  uint8_t len, i;
  if(USB_MSG_flags & USB_FLG_USE_USER_RW) {
    len = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
    switch(prog_state) {
      case PROG_STATE_READFLASH:
      case PROG_STATE_READEEPROM:
        for(i=0; i<len; i++) {
          if(prog_state == PROG_STATE_READFLASH)
               EP0_buffer[i] = ISP_readFlash(prog_address);
          else EP0_buffer[i] = ISP_readEEPROM(prog_address);
          prog_address++;
        }
        if(len < EP0_SIZE) prog_state = PROG_STATE_IDLE;
        SetupLen  -= len;
        UEP0_T_LEN = len;
        UEP0_CTRL ^= bUEP_T_TOG;
        return;

      case PROG_STATE_TPI_READ:
        TPI_readBlock(prog_address, EP0_buffer, len);
        prog_address += len;
        SetupLen     -= len;
        UEP0_T_LEN    = len;
        UEP0_CTRL    ^= bUEP_T_TOG;
        return;

      default:
        UEP0_T_LEN = 0;
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        return;
    }
  }
  else USB_EP0_IN();
}

// Endpoint 0 OUT handler
void ASP_EP0_OUT(void) {
  uint8_t len, i;
  if(USB_MSG_flags & USB_FLG_USE_USER_RW) {
    len = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
    switch(prog_state) {
      case PROG_STATE_WRITEFLASH:
      case PROG_STATE_WRITEEEPROM:
        for(i=0; i<len; i++) {
          if(prog_state == PROG_STATE_WRITEFLASH) {
            if(prog_pagesize == 0)
              ISP_writeFlash(prog_address, EP0_buffer[i], 1);
            else {
              ISP_writeFlash(prog_address, EP0_buffer[i], 0);
              prog_pagecounter--;
              if(prog_pagecounter == 0) {
                ISP_flushPage(prog_address, EP0_buffer[i]);
                prog_pagecounter = prog_pagesize;
              }
            }
          }
          else ISP_writeEEPROM(prog_address, EP0_buffer[i]);

          prog_nbytes--;
          if(prog_nbytes == 0) {
            prog_state = PROG_STATE_IDLE;
            if((prog_blockflags & PROG_BLOCKFLAG_LAST) && (prog_pagecounter != prog_pagesize))
              ISP_flushPage(prog_address, EP0_buffer[i]);
            SetupLen = len;
            break;
          }
          prog_address++;
        }
        SetupLen  -= len;
        UEP0_T_LEN = (SetupLen > 0) ? len : 0;
        UEP0_CTRL ^= bUEP_R_TOG;
        return;

      case PROG_STATE_TPI_WRITE:
        TPI_writeBlock(prog_address, EP0_buffer, len);
        prog_address += len;
        prog_nbytes  -= len;
        if(prog_nbytes <= 0) {
          prog_state = PROG_STATE_IDLE;
          SetupLen = len;
        }
        SetupLen  -= len;
        UEP0_T_LEN = (SetupLen > 0) ? len : 0;
        UEP0_CTRL ^= bUEP_R_TOG;
        return;

      default:
        UEP0_T_LEN = 0;
        UEP0_CTRL |= UEP_R_RES_ACK | UEP_T_RES_ACK;
        return;
    }
  }
  else USB_EP0_OUT();
}

// Endpoint 1 IN handler
void ASP_EP1_IN(void) {
  UEP1_T_LEN = 0;
  UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;
}
