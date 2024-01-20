// ===================================================================================
// ISP Functions for for CH551, CH552 and CH554
// ===================================================================================

#pragma once
#include <stdint.h>
#include "gpio.h"
#include "delay.h"
#include "config.h"

// ISP SCK speed identifiers
#define USBASP_ISP_SCK_AUTO       0
#define USBASP_ISP_SCK_0_5        1     // 500 Hz
#define USBASP_ISP_SCK_1          2     //   1 kHz
#define USBASP_ISP_SCK_2          3     //   2 kHz
#define USBASP_ISP_SCK_4          4     //   4 kHz
#define USBASP_ISP_SCK_8          5     //   8 kHz
#define USBASP_ISP_SCK_16         6     //  16 kHz
#define USBASP_ISP_SCK_32         7     //  32 kHz
#define USBASP_ISP_SCK_93_75      8     //  93.75 kHz
#define USBASP_ISP_SCK_187_5      9     // 187.5  kHz
#define USBASP_ISP_SCK_375        10    // 375 kHz
#define USBASP_ISP_SCK_750        11    // 750 kHz
#define USBASP_ISP_SCK_1500       12    // 1.5 MHz
#define USBASP_ISP_SCK_3000       13    //   3 MHz

// Functions
void ISP_connect(void);                 // Connect ISP bus
void ISP_disconnect(void);              // Disconnect ISP bus
uint8_t ISP_transmit(uint8_t data);     // Transmit and receive one byte via ISP
uint8_t ISP_enterProgrammingMode(void); // Enter programming mode
static void ISP_updateExtended(uint32_t address);       // Update extended address byte
uint8_t ISP_readFlash(uint32_t address);                                  // Read flash
uint8_t ISP_writeFlash(uint32_t address, uint8_t data, uint8_t pollmode); // Write flash
uint8_t ISP_flushPage(uint32_t address, uint8_t pollvalue);               // Flush page
uint8_t ISP_readEEPROM(uint16_t address);                                 // Read EEPROM
uint8_t ISP_writeEEPROM(uint16_t address, uint8_t data);                  // Write EEPROM
void ISP_command(__xdata uint8_t* cmd, __xdata uint8_t* res);  // Issue one SPI command
