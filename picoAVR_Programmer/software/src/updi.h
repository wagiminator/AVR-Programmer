// ===================================================================================
// SerialUPDI Functions for CH551, CH552 and CH554                            * v1.2 *
// ===================================================================================

#pragma once
#include <stdint.h>
#include "ch554.h"
#include "delay.h"
#include "gpio.h"
#include "config.h"

// UPDI Macros
#define UART_ready()      (TI)                // ready to send data?
#define UART_available()  (RI)                // data byte received?
#define UPDI_ready()      (UPDI_readyFlag)    // ready to send data?
#define UPDI_available()  (UPDI_readPointer != UPDI_writePointer) // something in buffer?

#define UPDI_BAUD         115200
#define UPDI_BAUD_SET     (uint8_t)(256 - (((F_CPU / 8 / UPDI_BAUD) + 1) / 2))

// Variables
extern __xdata uint8_t  UPDI_buffer[];
extern volatile uint8_t UPDI_readPointer;
extern volatile uint8_t UPDI_writePointer;
extern volatile __bit   UPDI_readyFlag;

// Setup UART for UPDI
inline void UPDI_init(void) {
  SM0    = 1;                     // UART0 9 data bits
  SM1    = 1;                     // UART0 BAUD rate by timer
//SM2    = 0;                     // UART0 no multi-device comm
//TCLK   = 0;                     // UART0 transmit clock: TIMER1
  PCON  |= SMOD;                  // UART0 fast BAUD rate
  TMOD  |= bT1_M1;                // TIMER1 8-bit auto-reload
  T2MOD |= bTMR_CLK | bT1_CLK;    // TIMER1 fast clock selection
  TH1    = UPDI_BAUD_SET;         // TIMER1 configure for BAUD rate
  TR1    = 1;                     // TIMER1 start
  REN    = 1;                     // enable RX
//TI     = 1;                     // UART0 set transmit complete flag
  ES     = 1;                     // enable UART0 interrupt
  PIN_input(PIN_RXD);             // avoid too many pullups on UPDI line
}

// Transmit a data byte
inline uint8_t UPDI_write(uint8_t data) {
  while(!UPDI_readyFlag);         // wait for transmission to complete
  DLY_us(12);                     // second stop bit + inter-byte delay
  REN  = 0;                       // disable RX to avoid echo (set again in ISR)
  if(TH1 == 0) {                  // slow speed? -> must be UPDI BREAK
    PIN_low(PIN_TXD);
    DLY_us(25000);                // hold UPDI low for 25ms -> BREAK
    PIN_high(PIN_TXD);
    REN = 1;                      // enable RX again
    DLY_us(2000);
  }
  else {                          // normal data byte?
    ACC  = data;                  // load data byte into accu to check parity
    TB8  = P;                     // set parity bit as 9th bit for UART
    SBUF = data;                  // start transmitting data byte + parity
    UPDI_readyFlag = 0;           // clear ready flag
  }
}

// Receive a data byte
inline uint8_t UPDI_read(void) {
  uint8_t result = UPDI_buffer[UPDI_readPointer++]; // pop received byte from buffer
  UPDI_readPointer &= 63;         // increase ring buffer pointer
  return result;                  // return byte
}

// Set BAUD rate (currently limited to max 230400 BAUD)
inline void UPDI_setBAUD(uint32_t baud) {
  if(baud <= F_CPU / 16 / 256) TH1 = 0;
  else TH1 = UPDI_BAUD_SET;
/*
  if      (baud >= 230400)            TH1 = (uint8_t)(256 - ((F_CPU / 8 / 230400) + 1) / 2);
  else if (baud <= F_CPU / 16 / 256)  TH1 = 0;
  else                                TH1 = (uint8_t)(256 - ((F_CPU / 8 / baud) + 1) / 2);
*/
}

void UART_interrupt(void);
