// ===================================================================================
// USBtinyISP and SerialUPDI Functions for CH551, CH552 and CH554             * v1.0 *
// ===================================================================================

#include "gpio.h"
#include "config.h"
#include "usb_avr.h"
#include "usb_handler.h"

// ===================================================================================
// Variables and Defines
// ===================================================================================

// Initialize line coding
__xdata CDC_LINE_CODING_TYPE CDC_lineCodingB = {
  .baudrate = 230400,       // baudrate 57600
  .stopbits = 2,            // 2 stopbits
  .parity   = 2,            // parity even
  .databits = 8             // 8 databits
};

volatile uint8_t sck_period = 10;         // SCK period in microseconds (1..250)
volatile uint8_t poll1;                   // first poll byte for write
volatile uint8_t poll2;                   // second poll byte for write
volatile __xdata uint16_t address;        // read/write address
volatile __xdata uint16_t timeout;        // write timeout in usec
volatile uint8_t cmd0;                    // current read/write command byte
volatile __xdata uint8_t cmd[4];          // SPI command buffer
volatile __xdata uint8_t res[4];          // SPI result buffer

volatile __xdata uint8_t CDC_controlLineState  = 0; // control line state
volatile __xdata uint8_t CDC_EP3_readByteCount = 0; // number of data bytes in IN buffer
volatile __xdata uint8_t CDC_EP3_readPointer   = 0; // data pointer for fetching
volatile __bit CDC_EP3_writeBusyFlag = 0;           // flag of whether upload pointer is busy
__xdata uint8_t CDC_EP3_writePointer = 0;           // data pointer for writing

#define CDC_DTR_flag  (CDC_controlLineState & 1)
#define CDC_RTS_flag  ((CDC_controlLineState >> 1) & 1)

extern uint16_t SetupLen;                 // USB handler
extern void USB_EP0_IN(void);             // EP0 IN buffer
extern void USB_EP0_OUT(void);            // EP0 OUT buffer

// ===================================================================================
// ISP Functions
// ===================================================================================

// Connect ISP bus
inline void ISP_connect(void) {
  PIN_low(PIN_LED);                       // turn on onboard LED
  PIN_low(PIN_RESET);                     // RST to output low
  PIN_output(PIN_RESET);
  P1_DIR_PU |= ((1<<5)|(1<<7));           // MOSI, SCK to output
}

// Disconnect ISP bus
void ISP_disconnect(void) {
  P1_DIR_PU &= ~((1<<5)|(1<<6)|(1<<7));   // set all the pins to Hi-Z
  P1_MOD_OC &= ~((1<<5)|(1<<6)|(1<<7));
  PIN_input(PIN_RESET);
  PIN_high(PIN_LED);                      // turn off onboard LED
}

// Set SPI clock frequency
inline void ISP_setSpeed(void) {
  if(sck_period <= (uint8_t)(255000000 / FREQ_SYS))
    SPI0_CK_SE = (uint8_t)(FREQ_SYS / 1000000) * sck_period;
  else SPI0_CK_SE = 255;
}

// Issue one SPI command.
static void ISP_spi(uint8_t* cmd, uint8_t* res) {
  uint8_t i;
  for(i=4; i; i--) {
    SPI0_DATA = *cmd++;
    while(!S0_FREE);
    *res++ = SPI0_DATA;
  }
}

// Create and issue a read or write SPI command.
static void ISP_spi_rw(void) {
  uint16_t a;
  a = address++;
  if(cmd0 & 0x80) a <<= 1;                // EEPROM
  cmd[0] = cmd0;
  if(a & 1) cmd[0] |= 0x08;
  cmd[1] = a >> 9;
  cmd[2] = a >> 1;
  ISP_spi(cmd, res);
}

// ===================================================================================
// CDC UPDI Functions
// ===================================================================================

// Check number of bytes in the IN buffer
uint8_t CDC_available(void) {
  return CDC_EP3_readByteCount;
}

// Check if OUT buffer is ready to be written
__bit CDC_ready(void) {
  return(!CDC_EP3_writeBusyFlag);
}

// Flush the OUT buffer
void CDC_flush(void) {
  if(!CDC_EP3_writeBusyFlag && CDC_EP3_writePointer > 0) {  // not busy and buffer not empty?
    UEP3_T_LEN = CDC_EP3_writePointer;                      // number of bytes in OUT buffer
    UEP3_CTRL  = UEP3_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_ACK; // respond ACK
    CDC_EP3_writeBusyFlag = 1;                              // busy for now
    CDC_EP3_writePointer  = 0;                              // reset write pointer
  }
}

// Write single character to OUT buffer
void CDC_write(char c) {
  while(CDC_EP3_writeBusyFlag);                             // wait for ready to write
  EP3_buffer[MAX_PACKET_SIZE + CDC_EP3_writePointer++] = c; // write character
  if(CDC_EP3_writePointer == EP3_SIZE) CDC_flush();         // flush if buffer full
}

// Read single character from IN buffer
char CDC_read(void) {
  char data;
  while(!CDC_EP3_readByteCount);                            // wait for data
  data = EP3_buffer[CDC_EP3_readPointer++];                 // get character
  if(--CDC_EP3_readByteCount == 0)                          // dec number of bytes in buffer
    UEP3_CTRL = UEP3_CTRL & ~MASK_UEP_R_RES | UEP_R_RES_ACK;// request new data if empty
  return data;
}

// ===================================================================================
// Setup pico AVR
// ===================================================================================

// Setup picoAVR
void AVR_init(void) {
  ISP_disconnect();                       // setup ISP pins
  USB_init();                             // setup USB
  UEP1_T_LEN = 0;
  UEP2_T_LEN = 0;
  UEP3_T_LEN = 0;
  SPI0_SETUP = 0;                         // setup SPI
  SPI0_CTRL  = 0x60;                      // mode 0
  UPDI_init();                            // setup UPDI
}

// ===================================================================================
// ISP/CDC-Specific USB Handler Functions
// ===================================================================================

// Setup ISP/CDC endpoints
void ISP_setup(void) {
  UEP1_DMA    = EP1_ADDR;                   // EP1 data transfer address
  UEP2_DMA    = EP2_ADDR;                   // EP2 data transfer address
  UEP3_DMA    = EP3_ADDR;                   // EP3 data transfer address
  UEP1_CTRL   = bUEP_AUTO_TOG               // EP1 Auto flip sync flag
              | UEP_T_RES_NAK;              // EP1 IN transaction returns NAK
  UEP2_CTRL   = bUEP_AUTO_TOG               // EP2 Auto flip sync flag
              | UEP_T_RES_NAK;              // EP2 IN transaction returns NAK
  UEP3_CTRL   = bUEP_AUTO_TOG               // EP3 Auto flip sync flag
              | UEP_T_RES_NAK               // EP3 IN transaction returns NAK
              | UEP_R_RES_ACK;              // EP3 OUT transaction returns ACK
  UEP4_1_MOD  = bUEP1_TX_EN;                // EP1 TX enable
  UEP2_3_MOD  = bUEP2_TX_EN                 // EP2 TX enable
              | bUEP3_RX_EN | bUEP3_TX_EN;  // EP3 double buffer
}

// Reset ISP/CDC parameters
void ISP_reset(void) {
  UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
  UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
  UEP3_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
  CDC_EP3_readByteCount = 0;                // reset received bytes counter
  CDC_EP3_writeBusyFlag = 0;                // reset write busy flag
}

// Handle non-standard control requests
uint8_t ISP_control(void) {
  uint8_t i;
  uint16_t returnLen;
  if((USB_setupBuf->bRequestType & USB_REQ_TYP_MASK) == USB_REQ_TYP_VENDOR) {
    USB_MSG_flags = USB_FLG_USE_USER_RW;
    switch(SetupReq) {
      case USBTINY_ECHO:                  // not fully tested yet...
        EP0_buffer[1] = 0x21;
        return 8;

      case USBTINY_READ:                  // not implemented yet, not necessary?
        EP0_buffer[0] = 0x00;
        return 1;

      case USBTINY_WRITE:                 // not implemented yet, not necessary?
      case USBTINY_CLR:
      case USBTINY_SET:
        return 0;

      case USBTINY_POWERUP:
        // Set SPI clock frequency
        sck_period = EP0_buffer[2];
        ISP_setSpeed();

        // Connect ISP bus
        ISP_connect();
        return 0;

      case USBTINY_POWERDOWN:
        // Disconnect ISP bus
        ISP_disconnect();
        return 0;

      case USBTINY_SPI:
        returnLen = *((uint16_t*)(EP0_buffer + 2)); // use returnLen to store first 2 byte temporarily
        ISP_spi(EP0_buffer + 2, EP0_buffer + 0);
        return 4;

      case USBTINY_POLL_BYTES:            // not fully tested yet...
        ISP_spi(EP0_buffer + 2, EP0_buffer + 0);
        poll1 = EP0_buffer[2];
        poll2 = EP0_buffer[3];
        return 0;

      case USBTINY_FLASH_READ:
      case USBTINY_EEPROM_READ:
        address = *(uint16_t*) &EP0_buffer[4];
        if(SetupReq == USBTINY_FLASH_READ) cmd0 = 0x20;
        else cmd0 = 0xa0;
        returnLen = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
        for(i=0; i<returnLen; i++) {
          ISP_spi_rw();
          EP0_buffer[i] = res[3];
        }
        SetupLen -= returnLen;
        return (uint8_t)returnLen;

      case USBTINY_FLASH_WRITE:
      case USBTINY_EEPROM_WRITE:
        address = *(uint16_t*) &EP0_buffer[4];
        timeout = *(uint16_t*) &EP0_buffer[2];
        if(SetupReq == USBTINY_FLASH_WRITE) cmd0 = 0x40;
        else cmd0 = 0xc0;
        returnLen = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
        return (uint8_t)returnLen;

      default:
        return 0xFF;
    }
  }
  else if((USB_setupBuf->bRequestType & USB_REQ_TYP_MASK) == USB_REQ_TYP_CLASS) {
    switch(USB_setupBuf->bRequest) {
      case GET_LINE_CODING:                   // 0x21  currently configured
          for(i=0; i<sizeof(CDC_lineCodingB); i++)
            EP0_buffer[i] = ((uint8_t*)&CDC_lineCodingB)[i]; // transmit line coding to host
          return sizeof(CDC_lineCodingB);
      case SET_CONTROL_LINE_STATE:            // 0x22  generates RS-232/V.24 style control signals
        CDC_controlLineState = EP0_buffer[2]; // read control line state
        PIN_write(PIN_LED, !CDC_RTS_flag);    // set LED according to RTS flag
        return 0;
      case SET_LINE_CODING:                   // 0x20  Configure
        return 0;            
      default:
        return 0xFF;                          // command not supported
    }
  }
  else return 0xFF;
}

// Endpoint 0 IN handler
void ISP_EP0_IN(void) {
  uint8_t i;
  uint8_t returnLen;
  if(USB_MSG_flags & USB_FLG_USE_USER_RW) {
    switch(SetupReq) {
      case USBTINY_FLASH_READ:
      case USBTINY_EEPROM_READ:
        returnLen = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
        for(i=0; i<returnLen; i++) {
          ISP_spi_rw();
          EP0_buffer[i] = res[3];
        }
        SetupLen -= returnLen;
        UEP0_T_LEN = returnLen;
        UEP0_CTRL ^= bUEP_T_TOG;
        break;
      default:
        UEP0_T_LEN = 0;
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        break;
    }
  }
  else USB_EP0_IN();
}

// Endpoint 0 OUT handler
void ISP_EP0_OUT(void) {
  uint8_t i, r;
  uint8_t returnLen;
  uint16_t usec;
  if(USB_MSG_flags & USB_FLG_USE_USER_RW) {
    switch(SetupReq) {
      case USBTINY_FLASH_WRITE:
      case USBTINY_EEPROM_WRITE:
        returnLen = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
        for(i=0; i<returnLen; i++) {
          cmd[3] = EP0_buffer[i];
          ISP_spi_rw();
          cmd[0] ^= 0x60;                 // turn write into read
          for(usec=0; usec<timeout; usec+=32*sck_period) {
            // when timeout > 0, poll until byte is written
            ISP_spi(cmd,res);
            r = res[3];
            if(r == cmd[3] && r != poll1 && r != poll2) break;
          }
        }
        SetupLen -= returnLen;
        UEP0_T_LEN = (SetupLen > 0) ? returnLen : 0;
        UEP0_CTRL ^= bUEP_R_TOG;
        break;
      default:
        UEP0_T_LEN = 0;
        UEP0_CTRL |= UEP_R_RES_ACK | UEP_T_RES_ACK;
        break;
    }
  }
  else if(SetupReq == SET_LINE_CODING) {         // set line coding
    if(U_TOG_OK) {
      // Set CDC line coding
      for(i=0; i<((sizeof(CDC_lineCodingB)<=USB_RX_LEN)?sizeof(CDC_lineCodingB):USB_RX_LEN); i++)
        ((uint8_t*)&CDC_lineCodingB)[i] = EP0_buffer[i];    // receive line coding from host
      UPDI_setBAUD(CDC_lineCoding -> baudrate);             // set baud rate
      UEP0_T_LEN = 0;
      UEP0_CTRL |= UEP_R_RES_ACK | UEP_T_RES_ACK;           // send 0-length packet
    }
  }
  else {
    UEP0_T_LEN = 0;
    UEP0_CTRL |= UEP_R_RES_ACK | UEP_T_RES_NAK;             // respond Nak
  }
}

// Endpoint 1 IN handler
void ISP_EP1_IN(void) {
  UEP1_T_LEN = 0;
  UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK; // default NAK
}

// Endpoint 2 IN handler
void CDC_EP2_IN(void) {
  UEP2_T_LEN = 0;
  UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK; // default NAK
}

// Endpoint 3 IN handler (bulk data transfer to host)
void CDC_EP3_IN(void) {
  UEP3_T_LEN = 0;                                           // no data to send anymore
  UEP3_CTRL = UEP3_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_NAK;  // respond NAK by default
  CDC_EP3_writeBusyFlag = 0;                                // clear busy flag
}

// Endpoint 3 OUT handler (bulk data transfer from host)
void CDC_EP3_OUT(void) {
  if(U_TOG_OK) {                                            // discard unsynchronized packets
    CDC_EP3_readByteCount = USB_RX_LEN;                     // set number of received data bytes
    CDC_EP3_readPointer = 0;                                // reset read pointer for fetching
    if(CDC_EP3_readByteCount) 
      UEP3_CTRL = UEP3_CTRL & ~MASK_UEP_R_RES | UEP_R_RES_NAK; // respond NAK after a packet. Let main code change response after handling.
  }
}
