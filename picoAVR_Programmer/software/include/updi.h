// ===================================================================================
// SerialUPDI Functions for CH551, CH552 and CH554                            * v1.0 *
// ===================================================================================

#pragma once
#include <stdint.h>
#include "ch554.h"
#include "delay.h"
#include "gpio.h"
#include "config.h"

// UPDI Macros
#define UPDI_ready()      (TI)    // ready to send data?
#define UPDI_available()  (RI)    // data byte received?

// Setup UART for UPDI
inline void UPDI_init(void) {
  SM0    = 1;                     // UART0 9 data bits
  SM1    = 1;                     // UART0 BAUD rate by timer
//SM2    = 0;                     // UART0 no multi-device comm
//TCLK   = 0;                     // UART0 transmit clock: TIMER1
  PCON  |= SMOD;                  // UART0 fast BAUD rate
  TMOD  |= bT1_M1;                // TIMER1 8-bit auto-reload
  T2MOD |= bTMR_CLK | bT1_CLK;    // TIMER1 fast clock selection
//TH1    = UPDI_BAUD_SET;         // TIMER1 configure for BAUD rate
  TR1    = 1;                     // TIMER1 start
  REN    = 1;                     // enable RX
//TI     = 1;                     // UART0 set transmit complete flag
  PIN_input(PIN_RXD);             // avoid too many pullups on UPDI line
}

// Transmit a data byte, return echo
inline uint8_t UPDI_write(uint8_t data) {
//while(!TI);                     // wait for last byte to be transmitted
//TI   = 0;                       // clear transmitted flag
  ACC  = data;                    // load data byte into accu to check parity
  TB8  = P;                       // set parity bit as 9th bit for UART
  SBUF = data;                    // start transmitting data byte + parity
  while(!RI);                     // wait for echo
  RI = 0;                         // clear received flag
  DLY_us(16);                     // second stop bit + inter-byte delay
  return SBUF;                    // return echo
}

// Receive a data byte
inline uint8_t UPDI_read(void) {
//while(!RI);                     // wait for databyte
  RI = 0;                         // clear received flag
  return SBUF;                    // return received data byte
}

// Send a BREAK frame
inline void UPDI_break(void) {
  PIN_low(PIN_TXD);
  DLY_ms(25);                     // hold UPDI low for 25ms -> BREAK
  PIN_high(PIN_TXD);
  DLY_us(16);
  RI = 0;                         // clear received flag
}

// Set BAUD rate (currently limited to max 230400 BAUD)
// We don't need to be precise here as the AVR adapts with auto-baud.
inline void UPDI_setBAUD(uint32_t baud) {
  if      (baud >= 230400)              TH1 = (uint8_t)(256 - (FREQ_SYS / 16 / 230400));
  else if (baud <= FREQ_SYS / 16 / 256) TH1 = 0;
  else                                  TH1 = (uint8_t)(256 - (FREQ_SYS / 16 / baud));
}
