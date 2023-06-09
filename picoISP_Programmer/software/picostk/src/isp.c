// ===================================================================================
// ISP Functions for for CH551, CH552 and CH554
// ===================================================================================

#include "isp.h"

// ===================================================================================
// SPI Functions
// ===================================================================================

__bit ISP_HWSPI;                          // 0: use software SPI, 1: use hardware SPI

// Disconnect ISP bus
void ISP_disconnect(void) {
  P1_DIR_PU &= ~((1<<5)|(1<<6)|(1<<7));   // set all the pins to Hi-Z
  P1_MOD_OC &= ~((1<<5)|(1<<6)|(1<<7));
  PIN_input(PIN_RESET);
}

// Setup ISP
void ISP_init(void) {
  ISP_disconnect();                       // setup pins
  CDC_init();                             // setup USB-CDC
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

// Connect ISP bus and enter programming mode with auto-clock
// (Thanks to Ralph Doncaster)
uint8_t ISP_enterProgrammingMode(void) {
  uint8_t check, tries;
  PIN_output(PIN_RESET);                      // RESET pin to output
  ISP_HWSPI  = 1;                             // start with hardware SPI
  SPI0_CTRL  = 0x60;                          // enable SPI interface, mode 0
  SPI0_CK_SE = 0;                             // start with fastest speed
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

// ===================================================================================
// ArduinoISP Functions
// ===================================================================================
// Copyright (c) 2008-2011 Randall Bohn.
// Modifications by David A. Mellis (2009) and Stefan Wagner (2023).
// If you require a license, see http://www.opensource.org/licenses/bsd-license.php

__bit error = 0;            // error flag
__bit pmode = 0;            // programming mode flag

uint16_t _addr;             // address for reading and writing, set by STK_SET_ADDR cmd
__xdata uint8_t buff[256];  // global block storage

typedef struct param {
  //uint8_t devicecode;
  //uint8_t revision;
  //uint8_t progtype;
  //uint8_t parmode;
  //uint8_t polling;
  //uint8_t selftimed;
  //uint8_t lockbytes;
  //uint8_t fusebytes;
  //uint16_t flashpoll;
  //uint16_t eeprompoll;
  uint16_t pagesize;
  //uint16_t eepromsize;
  //uint32_t flashsize;
} parameter;

__xdata parameter param;

void read_bytes(uint16_t n) {
  uint16_t x;
  for(x = 0; x < n; x++) {
    buff[x] = CDC_read();
  }
}

uint8_t spi_transaction(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  ISP_transmit(a);
  ISP_transmit(b);
  ISP_transmit(c);
  return ISP_transmit(d);
}

void reply_sync(void) {
  if(CDC_read() == CRC_EOP) {
    CDC_write(STK_INSYNC);
    CDC_write(STK_OK);
  } else {
    error = 1;
    CDC_write(STK_NOSYNC);
  }
}

void reply_b(uint8_t b) {
  if(CDC_read() == CRC_EOP) {
    CDC_write(STK_INSYNC);
    CDC_write(b);
    CDC_write(STK_OK);
  } else {
    error = 1;
    CDC_write(STK_NOSYNC);
  }
}

void get_parameters(uint8_t c) {
  switch(c) {
    case 0x80:
      reply_b(HWVER);
      break;
    case 0x81:
      reply_b(SWMAJ);
      break;
    case 0x82:
      reply_b(SWMIN);
      break;
    case 0x93:
      reply_b('S'); // serial programmer
      break;
    default:
      reply_b(0);
  }
}

void set_parameters(void) {
  // call this after reading parameter packet into buff[]
  //param.devicecode = buff[0];
  //param.revision   = buff[1];
  //param.progtype   = buff[2];
  //param.parmode    = buff[3];
  //param.polling    = buff[4];
  //param.selftimed  = buff[5];
  //param.lockbytes  = buff[6];
  //param.fusebytes  = buff[7];

  // following are 16 bits (big endian)
  //param.flashpoll  = ((uint16_t)buff[ 8] << 8) + buff[ 9];
  //param.eeprompoll = ((uint16_t)buff[10] << 8) + buff[11];
  param.pagesize   = ((uint16_t)buff[12] << 8) + buff[13];
  //param.eepromsize = ((uint16_t)buff[14] << 8) + buff[15];

  /*
  // 32 bits flashsize (big endian)
  param.flashsize = ( ((uint32_t)buff[16] << 24)
                    + ((uint32_t)buff[17] << 16)
                    + ((uint32_t)buff[18] <<  8)
                    + ((uint32_t)buff[19]);
  */
}

void start_pmode(void) {
  if(CDC_read() != CRC_EOP) {
    CDC_write(STK_NOSYNC);
    error = 1;
    return;
  }
  CDC_write(STK_INSYNC);
  #ifdef PIN_LED_PRG
  PIN_low(PIN_LED_PRG);
  #endif
  pmode = !ISP_enterProgrammingMode();
  if(pmode) CDC_write(STK_OK);
  else {
    ISP_disconnect();
    CDC_write(STK_FAILED);
    error = 1;
  }
}

void end_pmode(void) {
  ISP_disconnect();
  pmode = 0;
}

void universal(void) {
  read_bytes(4);
  reply_b(spi_transaction(buff[0], buff[1], buff[2], buff[3]));
}

void flash(uint8_t hilo, uint16_t addr, uint8_t data) {
  spi_transaction(0x40 + (hilo << 3), addr >> 8, addr, data);
}

void commit(uint16_t addr) {
  spi_transaction(0x4C, addr >> 8, addr, 0);
  DLY_us(4800);
}

uint16_t current_page(uint16_t addr) {
  if(param.pagesize == 32)  return addr & 0xFFF0;
  if(param.pagesize == 64)  return addr & 0xFFE0;
  if(param.pagesize == 128) return addr & 0xFFC0;
  if(param.pagesize == 256) return addr & 0xFF80;
  return addr;
}

uint8_t write_flash(uint16_t length) {
  uint16_t x, page;
  if(param.pagesize < 1) return STK_FAILED;
  page = current_page(_addr);
  x = 0;
  while(x < length) {
    if(page != current_page(_addr)) {
      commit(page);
      page = current_page(_addr);
    }
    flash(0, _addr, buff[x++]);
    flash(1, _addr, buff[x++]);
    _addr++;
  }
  commit(page);
  return STK_OK;
}

uint8_t write_eeprom(uint16_t length) {
  uint16_t x;
  for(x = 0; x < length; x++) {
    spi_transaction(0xC0, 0x00, (_addr << 1) + x, buff[x]);
    DLY_us(9600);
  } 
  return STK_OK;
}

void program_page(void) {
  uint8_t result, memtype;
  uint16_t length;
  result = STK_FAILED;
  length = (uint16_t)CDC_read() << 8;
  length += CDC_read();
  if(length > 256) {
    CDC_write(STK_FAILED);
    error = 1;
    return;
  }

  memtype = CDC_read();
  read_bytes(length);
  if(CDC_read() != CRC_EOP) {
    CDC_write(STK_NOSYNC);
    error = 1;
    return;
  }

  CDC_write(STK_INSYNC);
  if(memtype == 'F') result = write_flash(length);
  if(memtype == 'E') result = write_eeprom(length);
  CDC_write(result);
  if(result != STK_OK) error = 1;
}

uint8_t flash_read(uint8_t hilo, uint16_t addr) {
  return spi_transaction(0x20 + (hilo << 3), addr >> 8, addr, 0);
}

uint8_t flash_read_page(uint16_t length) {
  uint16_t x;
  for(x = 0; x < length; x += 2) {
    CDC_write(flash_read(0, _addr));
    CDC_write(flash_read(1, _addr));
    _addr++;
  }
  return STK_OK;
}

uint8_t eeprom_read_page(uint16_t length) {
  uint16_t x;
  for(x = 0; x < length; x++) {
    CDC_write(spi_transaction(0xA0, 0x00, (_addr << 1) + x, 0xFF));
  }
  return STK_OK;
}

void read_page(void) {
  uint8_t result, memtype;
  uint16_t length;
  result = STK_FAILED;
  length = (uint16_t)CDC_read() << 8;
  length += CDC_read();
  memtype = CDC_read();
  if(CDC_read() != CRC_EOP) {
    CDC_write(STK_NOSYNC);
    error = 1;
    return;
  }
  CDC_write(STK_INSYNC);
  if(memtype == 'F') result = flash_read_page(length);
  if(memtype == 'E') result = eeprom_read_page(length);
  CDC_write(result);
  if(result != STK_OK) error = 1;
}

void read_signature(void) {
  if(CDC_read() != CRC_EOP) {
    CDC_write(STK_NOSYNC);
    error = 1;
    return;
  }
  CDC_write(STK_INSYNC);
  CDC_write(spi_transaction(0x30, 0x00, 0x00, 0x00));
  CDC_write(spi_transaction(0x30, 0x00, 0x01, 0x00));
  CDC_write(spi_transaction(0x30, 0x00, 0x02, 0x00));
  CDC_write(STK_OK);
}

void avrisp(void) {
  // Set status LEDs
  #ifdef PIN_LED_PRG
  PIN_write(PIN_LED_PRG, !pmode);
  #endif
  #ifdef PIN_LED_ERR
  PIN_write(PIN_LED_ERR, !error);
  #endif

  // Read and handle commands
  uint8_t avrch = CDC_read();
  switch(avrch) {
    case STK_GET_SYNC:                  // get in sync
      reply_sync();
      break;
    case STK_GET_SIGNON:                // get sign on
      if(CDC_read() == CRC_EOP) {
        CDC_write(STK_INSYNC);
        CDC_print("AVR ISP");
        CDC_write(STK_OK);
      }
      break;
    case STK_GET_PARM:                  // 0x41
      get_parameters(CDC_read());
      break;
    case STK_SET_PARM:                  // 0x42
      read_bytes(20);
      set_parameters();
      reply_sync();
      break;
    case STK_SET_PARM_EXT:              // extended parameters - ignore for now
      read_bytes(5);
      reply_sync();
      break;
    case STK_PMODE_START:               // 0x50
      start_pmode();
      break;
    case STK_PMODE_END:                 // 0x51
      error = 0;
      end_pmode();
      reply_sync();
      break;
    case STK_SET_ADDR:                  // 0x55
      _addr  = (uint16_t)CDC_read();
      _addr += (uint16_t)CDC_read() << 8;
      reply_sync();
      break;
    case STK_UNIVERSAL:                 // UNIVERSAL 0x56
      universal();
      break;
    case STK_PROG_FLASH:                // STK_PROG_FLASH  ???
      CDC_read();
      CDC_read();
      reply_sync();
      break;
    case STK_PROG_DATA:                 // STK_PROG_DATA  ???
      CDC_read();
      reply_sync();
      break;
    case STK_PROG_PAGE:                 // STK_PROG_PAGE
      program_page();
      break;
    case STK_READ_PAGE:                 // STK_READ_PAGE
      read_page();    
      break;
    case STK_READ_SIGN:                 // STK_READ_SIGN
      read_signature();
      break;
    case CRC_EOP:                       // need to get back in sync
      CDC_write(STK_NOSYNC);
      error = 1;
      break;
    default:                            // everything else is unknown
      if(CDC_read() == CRC_EOP) 
        CDC_write(STK_UNKNOWN);
      else
        CDC_write(STK_NOSYNC);
  }
  CDC_flush();
}
