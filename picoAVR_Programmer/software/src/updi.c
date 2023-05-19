// ===================================================================================
// SerialUPDI Functions for CH551, CH552 and CH554                            * v1.2 *
// ===================================================================================

#include "updi.h"

__xdata uint8_t  UPDI_buffer[64];             // UPDI RX ring buffer
volatile uint8_t UPDI_readPointer  = 0;       // UPDI RX buffer read pointer
volatile uint8_t UPDI_writePointer = 0;       // UPDI RX buffer write pointer
volatile __bit   UPDI_readyFlag    = 1;       // UPDI ready to write flag

// ===================================================================================
// UART Interrupt Service Routine
// ===================================================================================
#pragma save
#pragma nooverlay
void UART_interrupt(void) {
  if(RI) {                                    // RX complete?
    UPDI_buffer[UPDI_writePointer++] = SBUF;  // push received byte to buffer...
    UPDI_writePointer &= 63;                  // increase ring buffer pointer
    RI = 0;                                   // clear RX interrupt flag
  }
  if(TI) {                                    // TX complete?
    REN = 1;                                  // enable RX again
    TI  = 0;                                  // clear TX interrupt flag
    UPDI_readyFlag = 1;                       // set ready to write flag
  }
}
#pragma restore
