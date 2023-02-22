// ===================================================================================
// USBtinyISP and SerialUPDI Functions for CH551, CH552 and CH554             * v1.0 *
// ===================================================================================

#pragma once
#include <stdint.h>
#include "ch554.h"
#include "updi.h"

// Generic requests
#define USBTINY_ECHO            0     // echo test
#define USBTINY_READ            1     // read byte (wIndex:address)
#define USBTINY_WRITE           2     // write byte (wIndex:address, wValue:value)
#define USBTINY_CLR             3     // clear bit (wIndex:address, wValue:bitno)
#define USBTINY_SET             4     // set bit (wIndex:address, wValue:bitno)

// Programming requests
#define USBTINY_POWERUP         5     // apply power (wValue:SCK-period, wIndex:RESET)
#define USBTINY_POWERDOWN       6     // remove power from chip
#define USBTINY_SPI             7     // issue SPI command (wValue:c1c0, wIndex:c3c2)
#define USBTINY_POLL_BYTES      8     // set poll bytes for write (wValue:p1p2)
#define USBTINY_FLASH_READ      9     // read flash (wIndex:address)
#define USBTINY_FLASH_WRITE     10    // write flash (wIndex:address, wValue:timeout)
#define USBTINY_EEPROM_READ     11    // read eeprom (wIndex:address)
#define USBTINY_EEPROM_WRITE    12    // write eeprom (wIndex:address, wValue:timeout)

// CDC class requests
#define SET_LINE_CODING         0x20  // host configures line coding
#define GET_LINE_CODING         0x21  // host reads configured line coding
#define SET_CONTROL_LINE_STATE  0x22  // generates RS-232/V.24 style control signals

// USB message flags
#define USB_FLG_USE_USER_RW     1

// Functions
void AVR_init(void);                  // setup picoAVR
void CDC_flush(void);                 // flush OUT buffer
char CDC_read(void);                  // read single character from IN buffer
void CDC_write(char c);               // write single character to OUT buffer
uint8_t CDC_available(void);          // check number of bytes in the IN buffer
__bit CDC_ready(void);                // check if OUT buffer is ready to be written

// CDC Line Coding
typedef struct _CDC_LINE_CODING_TYPE {
  uint32_t baudrate;                  // baud rate
  uint8_t  stopbits;                  // number of stopbits (0:1bit,1:1.5bits,2:2bits)
  uint8_t  parity;                    // parity (0:none,1:odd,2:even,3:mark,4:space)
  uint8_t  databits;                  // number of data bits (5,6,7,8 or 16)
} CDC_LINE_CODING_TYPE, *PCDC_LINE_CODING_TYPE;

extern __xdata CDC_LINE_CODING_TYPE CDC_lineCodingB;
#define CDC_lineCoding ((PCDC_LINE_CODING_TYPE)CDC_lineCodingB)
