// ===================================================================================
// USBtinyISP Functions for CH551, CH552 and CH554
// ===================================================================================

#pragma once
#include <stdint.h>
#include "gpio.h"
#include "delay.h"
#include "config.h"
#include "usb_handler.h"

// Generic requests
#define USBTINY_ECHO          0   // echo test
#define USBTINY_READ          1   // read byte (wIndex:address)
#define USBTINY_WRITE         2   // write byte (wIndex:address, wValue:value)
#define USBTINY_CLR           3   // clear bit (wIndex:address, wValue:bitno)
#define USBTINY_SET           4   // set bit (wIndex:address, wValue:bitno)

// Programming requests
#define USBTINY_POWERUP       5   // apply power (wValue:SCK-period, wIndex:RESET)
#define USBTINY_POWERDOWN     6   // remove power from chip
#define USBTINY_SPI           7   // issue SPI command (wValue:c1c0, wIndex:c3c2)
#define USBTINY_POLL_BYTES    8   // set poll bytes for write (wValue:p1p2)
#define USBTINY_FLASH_READ    9   // read flash (wIndex:address)
#define USBTINY_FLASH_WRITE   10  // write flash (wIndex:address, wValue:timeout)
#define USBTINY_EEPROM_READ   11  // read eeprom (wIndex:address)
#define USBTINY_EEPROM_WRITE  12  // write eeprom (wIndex:address, wValue:timeout)
#define USBTINY_DDRWRITE      13  // set port direction
#define USBTINY_SPI1          14  // send single byte via SPI

// Functions
void ISP_init(void);
