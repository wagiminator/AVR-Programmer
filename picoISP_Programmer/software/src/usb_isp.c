// ===================================================================================
// USBtinyISP Functions for CH551, CH552 and CH554
// ===================================================================================

#include "gpio.h"
#include "config.h"
#include "usb_isp.h"
#include "usb_handler.h"

volatile uint8_t sck_period = 10;         // SCK period in microseconds (1..250)
volatile uint8_t poll1;                   // first poll byte for write
volatile uint8_t poll2;                   // second poll byte for write
volatile __xdata uint16_t address;        // read/write address
volatile __xdata uint16_t timeout;        // write timeout in usec
volatile uint8_t cmd0;                    // current read/write command byte
volatile __xdata uint8_t cmd[4];          // SPI command buffer
volatile __xdata uint8_t res[4];          // SPI result buffer
extern uint16_t SetupLen;                 // USB handler
extern void USB_EP0_IN(void);             // EP0 IN buffer
extern void USB_EP0_OUT(void);            // EP0 OUT buffer

// ===================================================================================
// SPI Functions
// ===================================================================================

// Connect ISP bus
inline void ISP_connect(void) {
  #ifdef PIN_LED
  PIN_low(PIN_LED);                       // turn on onboard LED
  #endif
  PIN_low(PIN_RESET);                     // RST to output low
  PIN_output(PIN_RESET);
  P1_DIR_PU |= ((1<<5)|(1<<7));           // MOSI, SCK to output
}

// Disconnect ISP bus
void ISP_disconnect(void) {
  P1_DIR_PU &= ~((1<<5)|(1<<6)|(1<<7));   // set all the pins to Hi-Z
  P1_MOD_OC &= ~((1<<5)|(1<<6)|(1<<7));
  PIN_input(PIN_RESET);
  #ifdef PIN_LED
  PIN_high(PIN_LED);                      // turn off onboard LED
  #endif
}

// Setup USBtinyISP
void ISP_init(void) {
  ISP_disconnect();                       // setup pins
  USB_init();                             // setup USB
  UEP1_T_LEN = 0;
  SPI0_SETUP = 0;                         // setup SPI
  SPI0_CTRL = 0x60;                       // mode 0
}

// Set SPI clock frequency
inline void ISP_setSpeed(void) {
  if(sck_period <= (uint8_t)(255000000 / F_CPU))
    SPI0_CK_SE = (uint8_t)(F_CPU / 1000000) * sck_period;
  else SPI0_CK_SE = 255;
}

// Issue one SPI command.
static void ISP_spi(uint8_t* cmd, uint8_t* res, uint8_t n) {
  while(n--) {
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
  ISP_spi(cmd, res, 4);
}

// ===================================================================================
// ISP-Specific USB Handler Functions
// ===================================================================================

// Setup ISP endpoints
void ISP_setup(void) {
  UEP1_DMA    = EP1_ADDR;                 // EP1 data transfer address
  UEP1_CTRL   = bUEP_AUTO_TOG             // EP1 Auto flip sync flag
              | UEP_T_RES_NAK;            // EP1 IN transaction returns NAK
  UEP4_1_MOD  = bUEP1_TX_EN;              // EP1 TX enable (0x40)
}

// Reset ISP parameters
void ISP_reset(void) {
  UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
}

// Handle non-standard control requests
uint8_t ISP_control(void) {
  uint8_t i;
  uint8_t returnLen;
  if((USB_setupBuf->bRequestType & USB_REQ_TYP_MASK) == USB_REQ_TYP_VENDOR) {
    USB_MSG_flags = USB_FLG_USE_USER_RW;
    switch(SetupReq) {
      case USBTINY_ECHO:
        return 8;                         // just send it back

      case USBTINY_READ:
        return 1;                         // just send a dummy byte

      case USBTINY_WRITE:
      case USBTINY_CLR:
      case USBTINY_SET:
      case USBTINY_DDRWRITE:
        return 0;                         // not necessary

      case USBTINY_POWERUP:
        sck_period = EP0_buffer[2];
        ISP_setSpeed();                   // set SPI clock frequency
        ISP_connect();                    // connect ISP bus
        return 0;

      case USBTINY_POWERDOWN:
        ISP_disconnect();                 // disconnect ISP bus
        return 0;

      case USBTINY_SPI:
        ISP_spi(EP0_buffer + 2, EP0_buffer + 0, 4);
        return 4;

      case USBTINY_SPI1:
        ISP_spi(EP0_buffer + 2, EP0_buffer + 0, 1);
        return 1;

      case USBTINY_POLL_BYTES:
        ISP_spi(EP0_buffer + 2, EP0_buffer + 0, 4);
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
        return returnLen;

      case USBTINY_FLASH_WRITE:
      case USBTINY_EEPROM_WRITE:
        address = *(uint16_t*) &EP0_buffer[4];
        timeout = *(uint16_t*) &EP0_buffer[2];
        if(SetupReq == USBTINY_FLASH_WRITE) cmd0 = 0x40;
        else cmd0 = 0xc0;
        returnLen = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
        return returnLen;

      default:
        return 0xFF;
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
            ISP_spi(cmd, res, 4);
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
  else USB_EP0_OUT();
}

// Endpoint 1 IN handler
void ISP_EP1_IN(void) {
  UEP1_T_LEN = 0;
  UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;
}
