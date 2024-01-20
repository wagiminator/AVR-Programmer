// ===================================================================================
// Project:   picoAVR ISP + TPI + UPDI Programmer based on CH551, CH552, CH554
// Version:   v1.4
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// !!! THIS FIRMWARE IS STILL IN DEVELOPMENT !!!
//
// Description:
// ------------
// The CH55x-based picoAVR is an ISP, TPI and UPDI programmer for AVR microcontrollers 
// compatible with the USBasp and SerialUPDI. It works fine with the Arduino IDE 
// (Tools -> Programmer -> USBasp / Tools -> Programmer -> SerialUPDI). The USBasp part
// of the firmware is based on Thomas Fischl's original implementation for the ATmega8 
// and Ralph Doncaster's auto-clock extension.
//
// References:
// -----------
// - Thomas Fischl: https://www.fischl.de/usbasp/
// - Ralph Doncaster: https://github.com/nerdralph/usbasp
// - Blinkinlabs: https://github.com/Blinkinlabs/ch554_sdcc
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
//
// Compilation Instructions:
// -------------------------
// - Chip:  CH551, CH552 or CH554
// - Clock: 16 MHz internal
// - Adjust the firmware parameters in src/config.h if necessary.
// - Make sure SDCC toolchain and Python3 with PyUSB is installed.
// - Press BOOT button on the board and keep it pressed while connecting it via USB
//   with your PC.
// - Run 'make flash' immediatly afterwards.
// - To compile the firmware using the Arduino IDE, follow the instructions in the 
//   .ino file.
//
// Operating Instructions:
// -----------------------
// Select the programming voltage (3.3V or 5V) with the voltage selection switch.
// Plug the picoAVR into a USB port on your PC, it should be identified as a 
// USBasp with an additional CDC serial interface. Connect the picoAVR to the 
// target board via the 6-pin ICSP connector or the 3-pin UPDI connector. 
//
// The implementation includes Windows Compatible ID (WCID) code for automated driver 
// installation on Windows. If this doesn't work, Windows users may need to install a 
// driver manually using the Zadig tool (https://zadig.akeo.ie/). 
// However, functionality, especially with newer Windows versions, is not guaranteed.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================

// Libraries
#include "src/avr.h"                        // AVR ISP and UPDI functions

// Prototypes for used interrupts
void USB_interrupt(void);
void USB_ISR(void) __interrupt(INT_NO_USB) {
  USB_interrupt();
}

void UART_interrupt(void);
void UART_ISR(void) __interrupt(INT_NO_UART0) {
  UART_interrupt();
}

// ===================================================================================
// Main Function
// ===================================================================================
void main(void) {
  // Variables
  uint8_t  databyte;
  uint8_t  rtsstate = 0;
  uint32_t baudrate = 115200;

  // Setup
  CLK_config();                             // configure system clock
  DLY_us(10000);                            // wait for clock to settle
  AVR_init();                               // init programming interfaces

  // Loop
  while(1) {
    // ISP stuff is handled via USB interrupts.
    // We only need to take care of USB-CDC to UPDI here.

    // Transfer data from USB to UPDI
    if(CDC_available()) {                   // something coming in via USB-CDC?
      while(CDC_available()) {              // repeat for all incoming bytes
        databyte = CDC_read();              // get data byte
        UPDI_write(databyte);               // send byte via UPDI
        CDC_write(databyte);                // send back echo
      }
      CDC_flush();                          // flush OUT buffer
    }

    // Transfer data from UPDI to USB
    if(UPDI_available() && CDC_getDTR()) {  // something coming in via UPDI?
      while(UPDI_available())               // repeat for all incoming bytes
        CDC_write(UPDI_read());             // write received bytes to USB OUT buffer
      CDC_flush();                          // flush USB OUT buffer
    }

    // Handle BAUD rate
    if(CDC_getBAUD() != baudrate) {
      baudrate = CDC_getBAUD();
      UPDI_setBAUD(baudrate);
    }

    // Handle status LEDs
    #ifdef PIN_LED_USB
    PIN_write(PIN_LED_USB, !USB_ENUM_OK);   // control USB GOOD status LED
    #endif
    #ifdef PIN_LED_PRG
    if(CDC_getRTS() != rtsstate) {
      rtsstate = CDC_getRTS();
      PIN_write(PIN_LED_PRG, !rtsstate);    // control PROG status LED
    }
    #endif
  }
}
