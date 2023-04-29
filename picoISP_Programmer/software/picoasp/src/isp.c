// ===================================================================================
// ISP Functions for for CH551, CH552 and CH554
// ===================================================================================

#include "isp.h"
#include "gpio.h"
#include "delay.h"
#include "config.h"

uint8_t ISP_hiaddr;
uint8_t ISP_sck = USBASP_ISP_SCK_AUTO;

// Connect ISP bus
void ISP_connect(void) {
  #ifdef PIN_LED
  PIN_low(PIN_LED);                       // turn on onboard LED
  #endif
  SPI0_CTRL = 0x60;                       // enable SPI interface, mode 0
  PIN_low(PIN_RESET);                     // RST to output low
  PIN_output(PIN_RESET);
  P1_DIR_PU |= ((1<<5)|(1<<7));           // MOSI, SCK to output
  ISP_hiaddr = 0xFF;
}

// Disconnect ISP bus
void ISP_disconnect(void) {
  P1_DIR_PU &= ~((1<<5)|(1<<6)|(1<<7));   // set all the pins to Hi-Z
  P1_MOD_OC &= ~((1<<5)|(1<<6)|(1<<7));
  PIN_input(PIN_RESET);
  SPI0_CTRL = 0;                          // disable SPI interface
  ISP_sck = USBASP_ISP_SCK_AUTO;          // reset SCK speed
  #ifdef PIN_LED
  PIN_high(PIN_LED);                      // turn off onboard LED
  #endif
}

// Transmit and receive one byte via ISP
uint8_t ISP_transmit(uint8_t data) {
  SPI0_DATA = data;
  while(!S0_FREE);
  return SPI0_DATA;
}

// Issue one SPI command
void ISP_command(__xdata uint8_t* cmd, __xdata uint8_t* res) {
  uint8_t n = 4;
  while(n--) {
    SPI0_DATA = *cmd++;
    while(!S0_FREE);
    *res++ = SPI0_DATA;
  }
}

// Set SPI clock frequency
void ISP_setSpeed(uint8_t option) {
  if(option == USBASP_ISP_SCK_AUTO)
    option = USBASP_ISP_SCK_1500;
  if(option > USBASP_ISP_SCK_3000)
    option = USBASP_ISP_SCK_3000;

  switch(option) {
    case USBASP_ISP_SCK_3000:
      SPI0_CK_SE = (uint8_t)((F_CPU / 3000000) - 1);
      break;
    case USBASP_ISP_SCK_1500:
      SPI0_CK_SE = (uint8_t)((F_CPU / 1500000) - 1);
      break;
    case USBASP_ISP_SCK_750:
      SPI0_CK_SE = (uint8_t)((F_CPU /  750000) - 1);
      break;
    case USBASP_ISP_SCK_375:
      SPI0_CK_SE = (uint8_t)((F_CPU /  375000) - 1);
      break;
    case USBASP_ISP_SCK_187_5:
      SPI0_CK_SE = (uint8_t)((F_CPU /  187500) - 1);
      break;
    case USBASP_ISP_SCK_93_75:
      SPI0_CK_SE = (uint8_t)((F_CPU /   93750) - 1);
      break;
    default:
      SPI0_CK_SE = 255;
      break;
  }
}

// Enter programming mode
uint8_t ISP_enterProgrammingMode(void) {
  uint8_t check, tries;

  if (ISP_sck == USBASP_ISP_SCK_AUTO)
    ISP_sck = USBASP_ISP_SCK_1500;

  do {
    tries = 3;
    do {
      P1_DIR_PU &= ~((1<<5)|(1<<7));          // MOSI, SCK to input
      PIN_high(PIN_RESET);                    // RST to high
      DLY_us(320);                            // RST high for 320us
      PIN_low(PIN_RESET);                     // RST to low
      P1_DIR_PU |= ((1<<5)|(1<<7));           // MOSI, SCK to output
      DLY_us(20000);                          // wait 20ms

      ISP_transmit(0xAC);                     // issue ISP command
      ISP_transmit(0x53);
      check = ISP_transmit(0);
      ISP_transmit(0);

      if(check == 0x53) return 0;             // correct answer? -> success
    } while(--tries);
    ISP_setSpeed(--ISP_sck);                  // try lower speed
  } while(ISP_sck >= USBASP_ISP_SCK_32);
  return 1;                                   // error: device doesn't answer
}

// Update extended address byte
static void ISP_updateExtended(uint32_t address) {
  uint8_t curr_hiaddr = (address >> 17);
  if(ISP_hiaddr != curr_hiaddr) {             // check if extended address byte is changed
    ISP_hiaddr = curr_hiaddr;
    ISP_transmit(0x4D);                       // load extended address byte
    ISP_transmit(0x00);
    ISP_transmit(ISP_hiaddr);
    ISP_transmit(0x00);
  }
}

// Read flash
uint8_t ISP_readFlash(uint32_t address) {
  ISP_updateExtended(address);
  ISP_transmit(0x20 | ((address & 1) << 3));
  ISP_transmit(address >> 9);
  ISP_transmit(address >> 1);
  return ISP_transmit(0);
}

// Write flash
uint8_t ISP_writeFlash(uint32_t address, uint8_t data, uint8_t pollmode) {
  uint8_t retries;
  ISP_updateExtended(address);
  ISP_transmit(0x40 | ((address & 1) << 3));
  ISP_transmit(address >> 9);
  ISP_transmit(address >> 1);
  ISP_transmit(data);

  if(pollmode == 0)
    return 0;

  if(data == 0x7F) {
    DLY_us(4800);
    return 0;
  }
  else {
    retries = 0;
    while(--retries) {
      if(ISP_readFlash(address) != 0x7F)
        return 0;
      DLY_us(32);
    }
    return 1;                                 // error
  }
}

// Flush page
uint8_t ISP_flushPage(uint32_t address, uint8_t pollvalue) {
  uint8_t retries;
  ISP_updateExtended(address);
  ISP_transmit(0x4C);                         // write page
  ISP_transmit(address >> 9);
  ISP_transmit(address >> 1);
  ISP_transmit(0);

  if(pollvalue == 0xFF) {
    DLY_us(4800);
    return 0;
  }
  else {
    retries = 0;
    while(--retries) {
      if(ISP_readFlash(address) != 0xFF)
        return 0;
      DLY_us(32);
    }
    return 1;                                 // error
  }
}

// Read EEPROM
uint8_t ISP_readEEPROM(uint16_t address) {
  ISP_transmit(0xA0);
  ISP_transmit(address >> 8);
  ISP_transmit(address);
  return ISP_transmit(0);
}

// Write EEPROM
uint8_t ISP_writeEEPROM(uint16_t address, uint8_t data) {
  ISP_transmit(0xC0);
  ISP_transmit(address >> 8);
  ISP_transmit(address);
  ISP_transmit(data);
  DLY_us(9600);
  return 0;
}
