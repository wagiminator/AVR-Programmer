// ===================================================================================
// TPI Functions for for CH551, CH552 and CH554                               * v1.1 *
// ===================================================================================

#include "tpi.h"

__xdata uint8_t TPI_dly_cnt; 

// Transmit/receive one bit via TPI; b must be 1 when receiving
__bit TPI_clockBit(__bit b) {
  __bit result;
  uint8_t cnt;
  PIN_write(PIN_MOSI, b);                 // set DATA according to bit
  cnt = TPI_dly_cnt; while(cnt--);        // delay
  PIN_high(PIN_SCK);                      // CLK high
  cnt = TPI_dly_cnt; while(cnt--);        // delay
  result = PIN_read(PIN_MOSI);            // read DATA
  PIN_low(PIN_SCK);                       // CLK low
  return result;
}

// Connect TPI bus
void TPI_connect(void) {
  uint8_t i;
  
  #ifdef PIN_LED_PRG
  PIN_low(PIN_LED_PRG);                   // turn on status LED
  #endif

  PIN_high(PIN_RESET);                    // RST high
  PIN_output(PIN_RESET);                  // RST to output
  DLY_us(1000);                           // wait a bit
  PIN_low(PIN_RESET);                     // RST low
  DLY_us(5600);                           // wait a bit
  PIN_high(PIN_MOSI);                     // DATA high
  PIN_input_PU(PIN_MOSI);                 // DATA input pullup, open-drain output
  PIN_low(PIN_SCK);                       // CLK low
  PIN_output(PIN_SCK);                    // CLK to output
  
  for(i=32; i; i--) TPI_clockBit(1);      // DATA high for 32 TPI clock cycles
}

// Disconnect TPI bus
void TPI_disconnect(void) {
  TPI_writeByte(TPI_OP_SSTCS(TPISR));
  TPI_writeByte(0);
  DLY_us(3200);
  PIN_high(PIN_RESET);                    // RST high
  DLY_us(1600);                           // wait a bit
  PIN_low(PIN_RESET);                     // RST low
  DLY_us(1600);                           // wait a bit
  PIN_input(PIN_SCK);                     // CLK to input, high-impedance
  PIN_input(PIN_MOSI);                    // DATA to input, high-impedance
  PIN_input(PIN_RESET);                   // RST to input
  
  #ifdef PIN_LED_PRG
  PIN_high(PIN_LED_PRG);                  // turn off status LED
  #endif
}

// Send raw byte via TPI
void TPI_writeByte(uint8_t b) {
  uint8_t i;
  
  // Get parity
  __bit parity;
  ACC = b;
  parity = P;
  
  // Send frame
  TPI_clockBit(0);                        // start bit
  for(i=8; i; i--) {                      // 8 bits
    b >>= 1;                              // LSB first
    TPI_clockBit(CY);                     // data bit
  }
  TPI_clockBit(parity);                   // parity bit
  TPI_clockBit(1);                        // stop bit
  TPI_clockBit(1);                        // stop bit
}

// Receive raw byte via TPI
uint8_t TPI_readByte(void) {
  __bit parity;
  uint8_t result = 0;

  // Wait for start bit
  uint8_t i = 192;                        // wait max 192 TPI clock cycles
  do {
    if(!TPI_clockBit(1)) break;           // start bit found?
  } while(--i);
  if(!i) {                                // no start bit found?
    for(i=26; i; i--) TPI_clockBit(0);    // send 2 breaks (24++ bits)
    TPI_clockBit(1);                      // send hi
    return 0;                             // and return
  }           

  // Read data bits
  for(i=8; i; i--) {                      // 8 bits
    result >>= 1;                         // LSB first
    if(TPI_clockBit(1)) result |= 0x80;   // get data bit
  }
  
  // Receive and check parity
  ACC = result;
  parity = P;
  if(parity != TPI_clockBit(1)) {         // parity missmatch?
    for(i=26; i; i--) TPI_clockBit(0);    // send 2 breaks (24++ bits)
    TPI_clockBit(1);                      // send hi
    return result;                        // and return
  }
  
  // Receive stop bits
  TPI_clockBit(1);                        // receive stop bit
  TPI_clockBit(1);                        // receive stop bit
  return result;
}

// Update PR
void TPI_PR_update(uint16_t pr) {
  TPI_writeByte(TPI_OP_SSTPR(0));
  TPI_writeByte(pr & 0xFF);
  TPI_writeByte(TPI_OP_SSTPR(1));
  TPI_writeByte(pr >> 8);
}

// Read block via TPI into buffer
void TPI_readBlock(uint16_t addr, __xdata uint8_t* buf, uint8_t len) {
  TPI_PR_update(addr);
  while(len--) {
    TPI_writeByte(TPI_OP_SLD_INC);
    *buf++ = TPI_readByte();
  }
}

// Write block from buffer via TPI
void TPI_writeBlock(uint16_t addr, __xdata uint8_t* buf, uint8_t len) {
  TPI_PR_update(addr);
  while(len--) {
    TPI_writeByte(TPI_OP_SOUT(NVMCMD));
    TPI_writeByte(NVMCMD_WORD_WRITE);
    TPI_writeByte(TPI_OP_SST_INC);
    TPI_writeByte(*buf++);
    do {
      TPI_writeByte(TPI_OP_SIN(NVMCSR));
    } while(TPI_readByte() & NVMCSR_BSY);
  }
}
