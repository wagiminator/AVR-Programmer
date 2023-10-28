// ===================================================================================
// USBtinyISP Functions for CH551, CH552 and CH554
// ===================================================================================

#include "gpio.h"
#include "delay.h"
#include "config.h"
#include "usb_isp.h"
#include "usb_handler.h"

uint8_t poll1;                    // first poll byte for write
uint8_t poll2;                    // second poll byte for write
uint16_t address;                 // read/write address
uint16_t timeout;                 // write timeout in usec
uint16_t spi_period;              // time to transmit 4 bytes via SPI in us
uint8_t cmd0;                     // current read/write command byte
__xdata uint8_t cmd[4];           // SPI command buffer
__xdata uint8_t res[4];           // SPI result buffer

__bit ISP_HWSPI;                  // 0: use software SPI, 1: use hardware SPI

extern void USB_EP0_IN(void);     // EP0 IN buffer
extern void USB_EP0_OUT(void);    // EP0 OUT buffer

// ===================================================================================
// SPI Functions
// ===================================================================================

// Disconnect ISP bus
void ISP_disconnect(void) {
  P1_DIR_PU &= ~((1<<5)|(1<<6)|(1<<7));   // set all the pins to Hi-Z
  P1_MOD_OC &= ~((1<<5)|(1<<6)|(1<<7));
  PIN_input(PIN_RESET);
  #ifdef PIN_LED_PRG
  PIN_high(PIN_LED_PRG);                  // turn off status LED
  #endif
}

// Setup USBtinyISP
void ISP_init(void) {
  ISP_disconnect();                       // setup pins
  USB_init();                             // setup USB
  UEP1_T_LEN = 0;
  #ifdef PIN_LED_USB
  PIN_low(PIN_LED_USB);                   // turn on status LED
  #endif
}

// Transmit and receive one byte via Software SPI @ 15625Hz (should be slow enough).
// This might be necessary because hardware SPI is too fast.
uint8_t ISP_SW_transmit(uint8_t data) {
  uint8_t i;
  for(i=8; i; i--) {
    data <<= 1;
    PIN_write(PIN_MOSI, CY);
    PIN_high(PIN_SCK);
    DLY_us(32);
    data |= PIN_read(PIN_MISO);
    PIN_low(PIN_SCK);
    DLY_us(32);
  }
  return data;
}

// Transmit and receive one byte via Hardware ISP
uint8_t ISP_transmit(uint8_t data) {
  if(ISP_HWSPI) {
    SPI0_DATA = data;
    while(!S0_FREE);
    return SPI0_DATA;
  }
  return ISP_SW_transmit(data);
}

// Issue one SPI command.
static void ISP_spi(__xdata uint8_t* cmd, __xdata uint8_t* res) {
  uint8_t n = 4;
  while(n--) *res++ = ISP_transmit(*cmd++);
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

// Connect ISP bus and enter programming mode with auto-clock
// (Thanks to Ralph Doncaster)
uint8_t ISP_connect(void) {
  uint8_t check, tries;
  #ifdef PIN_LED_PRG
  PIN_low(PIN_LED_PRG);                   // turn on status LED
  #endif
  PIN_output(PIN_RESET);                  // RESET pin to output
  ISP_HWSPI  = 1;                         // start with hardware SPI
  SPI0_CTRL  = 0x60;                      // enable SPI interface, mode 0
  SPI0_CK_SE = 3;                         // start with ~1.5MHz
  do {
    if(SPI0_CK_SE == 255) {               // already at slowest hardware speed?
      ISP_HWSPI = 0;                      // use software SPI now
      SPI0_CTRL = 0;                      // disable SPI interface
      PIN_low(PIN_SCK);                   // SCK low
    }
    SPI0_CK_SE = (SPI0_CK_SE << 1) + 1;   // slow down hw speed
    tries = 2;
    do {
      P1_DIR_PU &= ~((1<<5)|(1<<7));      // MOSI, SCK to input
      PIN_high(PIN_RESET);                // RST to high
      DLY_us(320);                        // RST high for 320us
      PIN_low(PIN_RESET);                 // RST to low
      P1_DIR_PU |= ((1<<5)|(1<<7));       // MOSI, SCK to output
      DLY_us(20000);                      // wait 20ms

      ISP_transmit(0xAC);                 // issue ISP command
      ISP_transmit(0x53);
      check = ISP_transmit(0);
      ISP_transmit(0);
      if(check == 0x53) return 0;         // correct answer? -> success

    } while(--tries);
  } while(ISP_HWSPI);                     // repeat until slowest speed is reached
  return 1;                               // error: device doesn't answer
}

// ===================================================================================
// ISP-Specific Setup and Reset Functions
// ===================================================================================

// Setup ISP endpoints
void ISP_setup(void) {
  UEP1_DMA    = (uint16_t)EP1_buffer;     // EP1 data transfer address
  UEP1_CTRL   = bUEP_AUTO_TOG             // EP1 Auto flip sync flag
              | UEP_T_RES_NAK;            // EP1 IN transaction returns NAK
  UEP4_1_MOD  = bUEP1_TX_EN;              // EP1 TX enable (0x40)
}

// Reset ISP parameters
void ISP_reset(void) {
  UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
}

// ===================================================================================
// ISP-Specific USB Control Transfers
// ===================================================================================

// Handle non-standard control requests
uint8_t ISP_control(void) {
  uint8_t len, i;
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
        ISP_connect();                    // connect ISP bus
        if(ISP_HWSPI) spi_period = ((uint16_t)SPI0_CK_SE + 1) * (32000000 / F_CPU);
        else spi_period = (32 * 64);
        return 0;

      case USBTINY_POWERDOWN:
        ISP_disconnect();                 // disconnect ISP bus
        return 0;

      case USBTINY_SPI:
        ISP_spi(EP0_buffer + 2, EP0_buffer + 0);
        return 4;

      case USBTINY_SPI1:
        EP0_buffer[0] = ISP_transmit(EP0_buffer[2]);
        return 1;

      case USBTINY_POLL_BYTES:
        ISP_spi(EP0_buffer + 2, EP0_buffer + 0);
        poll1 = EP0_buffer[2];
        poll2 = EP0_buffer[3];
        return 0;

      case USBTINY_FLASH_READ:
      case USBTINY_EEPROM_READ:
        address = *(uint16_t*) &EP0_buffer[4];
        if(SetupReq == USBTINY_FLASH_READ) cmd0 = 0x20;
        else cmd0 = 0xa0;
        len = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
        for(i=0; i<len; i++) {
          ISP_spi_rw();
          EP0_buffer[i] = res[3];
        }
        SetupLen -= len;
        return len;

      case USBTINY_FLASH_WRITE:
      case USBTINY_EEPROM_WRITE:
        address = *(uint16_t*) &EP0_buffer[4];
        timeout = *(uint16_t*) &EP0_buffer[2];
        if(SetupReq == USBTINY_FLASH_WRITE) cmd0 = 0x40;
        else cmd0 = 0xc0;
        len = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
        return len;

      default:
        return 0xFF;
    }
  }
  else return 0xFF;
}

// ===================================================================================
// ISP-Specific USB Endpoint Handlers
// ===================================================================================

// Endpoint 0 IN handler
void ISP_EP0_IN(void) {
  uint8_t len, i;
  if(USB_MSG_flags & USB_FLG_USE_USER_RW) {
    switch(SetupReq) {
      case USBTINY_FLASH_READ:
      case USBTINY_EEPROM_READ:
        len = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
        for(i=0; i<len; i++) {
          ISP_spi_rw();
          EP0_buffer[i] = res[3];
        }
        SetupLen  -= len;
        UEP0_T_LEN = len;
        UEP0_CTRL ^= bUEP_T_TOG;
        break;
      default:
        UEP0_T_LEN = 0;
        UEP0_CTRL  = UEP_R_RES_ACK | UEP_T_RES_NAK;
        break;
    }
  }
  else USB_EP0_IN();
}

// Endpoint 0 OUT handler
void ISP_EP0_OUT(void) {
  uint8_t len, i, r;
  uint16_t usec;
  if(USB_MSG_flags & USB_FLG_USE_USER_RW) {
    switch(SetupReq) {
      case USBTINY_FLASH_WRITE:
      case USBTINY_EEPROM_WRITE:
        len = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
        for(i=0; i<len; i++) {
          cmd[3] = EP0_buffer[i];
          ISP_spi_rw();
          cmd[0] ^= 0x60;                 // turn write into read
          for(usec=0; usec<timeout; usec+=spi_period) {
            // when timeout > 0, poll until byte is written
            ISP_spi(cmd, res);
            r = res[3];
            if(r == cmd[3] && r != poll1 && r != poll2) break;
          }
        }
        SetupLen -= len;
        UEP0_T_LEN = (SetupLen > 0) ? len : 0;
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
