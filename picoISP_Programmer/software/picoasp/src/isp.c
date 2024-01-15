// ===================================================================================
// ISP Functions for for CH551, CH552 and CH554
// ===================================================================================

#include "isp.h"

uint8_t ISP_hiaddr;
__bit ISP_HWSPI;                          // 0: use software SPI, 1: use hardware SPI

// Connect ISP bus
void ISP_connect(void) {
  #ifdef PIN_LED_PRG
  PIN_low(PIN_LED_PRG);                   // turn on status LED
  #endif
  ISP_hiaddr = 0xFF;
}

// Disconnect ISP bus
void ISP_disconnect(void) {
  P1_DIR_PU &= ~((1<<5)|(1<<6)|(1<<7));   // set all the pins to Hi-Z
  P1_MOD_OC &= ~((1<<5)|(1<<6)|(1<<7));
  PIN_input(PIN_RESET);
  SPI0_CTRL = 0;                          // disable SPI interface
  #ifdef PIN_LED_PRG
  PIN_high(PIN_LED_PRG);                  // turn off status LED
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

// Issue one SPI command
void ISP_command(__xdata uint8_t* cmd, __xdata uint8_t* res) {
  uint8_t n = 4;
  if(ISP_HWSPI) {
    while(n--) {
      SPI0_DATA = *cmd++;
      while(!S0_FREE);
      *res++ = SPI0_DATA;
    }
    return;
  }
  while(n--) *res++ = ISP_SW_transmit(*cmd++);
}

// Connect ISP bus and enter programming mode with auto-clock
// (Thanks to Ralph Doncaster)
uint8_t ISP_enterProgrammingMode(void) {
  uint8_t check, tries;
  PIN_output(PIN_RESET);                      // RESET pin to output
  ISP_HWSPI  = 1;                             // start with hardware SPI
  SPI0_CTRL  = 0x60;                          // enable SPI interface, mode 0
  SPI0_CK_SE = 4;                             // start with ~1.5MHz
  do {
    if(SPI0_CK_SE == 255) {                   // already at slowest hardware speed?
      ISP_HWSPI = 0;                          // use software SPI now
      SPI0_CTRL = 0;                          // disable SPI interface
      PIN_low(PIN_SCK);                       // SCK low
    }
    SPI0_CK_SE = (SPI0_CK_SE << 1) + 1;       // slow down hw speed
    tries = 2;
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
  } while(ISP_HWSPI);                         // repeat until slowest speed is reached
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
