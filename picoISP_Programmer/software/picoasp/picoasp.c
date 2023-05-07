// ===================================================================================
// Project:   picoASP AVR ISP Programmer based on CH551, CH552, CH554
// Version:   v0.2
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// !!! This firmware version is experimental and still in development !!!
// ToDo: fix broken pipes; make TPI work
//
// Description:
// ------------
// The CH55x-based picoASP is an ISP/TPI programmer for AVR microcontrollers compatible 
// with the USBasp. It works fine with AVRdude (avrdude -c usbasp) and the Arduino IDE 
// (Tools -> Programmer -> USBasp). The firmware is based on Thomas Fischl's original
// implementation for the ATmega8 and Ralph Doncaster's auto-clock extension.
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
// Plug the picoASP into a USB port on your PC, it should be identified as a 
// USBasp. Connect the picoASP to the target board via the 6-pin ICSP connector.  
// The picoASP should work with any IDE or programming software that supports the
// USBasp programmer.
// The implementation includes Windows Compatible ID (WCID) code for automated driver 
// installation. If this doesn't work, Windows users may need to install a driver 
// manually using the Zadig tool (https://zadig.akeo.ie/). However, functionality, 
// especially with newer Windows versions, is not guaranteed.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================

// Libraries
#include "src/system.h"                     // system functions
#include "src/delay.h"                      // delay functions
#include "src/usb_asp.h"                    // AVR ISP functions

// Prototypes for used interrupts
void USB_interrupt(void);
void USB_ISR(void) __interrupt(INT_NO_USB) {
  USB_interrupt();
}

// ===================================================================================
// Main Function
// ===================================================================================
void main(void) {
  // Setup
  CLK_config();                             // configure system clock
  DLY_us(10000);                            // wait for clock to settle
  ASP_init();                               // init AVR ISP

  // Loop
  while(1);                                 // everything runs on USB interrupts
}
