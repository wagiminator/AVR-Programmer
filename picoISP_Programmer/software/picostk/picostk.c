// ===================================================================================
// Project:   picoSTK AVR ISP Programmer based on CH551, CH552, CH554
// Version:   v1.1
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// The CH55x-based picoSTK is an ISP programmer for AVR microcontrollers compatible 
// with the STK500v1 and ArduinoISP. It works fine with AVRdude (avrdude -c stk500v1)
// and the Arduino IDE (Tools -> Programmer -> Arduino as ISP). The firmware is based 
// on Randall Bohn's original ArduinoISP implementation and includes Ralph Doncaster's
// auto-clock extension for the USBasp.
//
// References:
// -----------
// - Randall Bohn: https://github.com/rsbohn/ArduinoISP
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
// Plug the picoSTK into a USB port on your PC, it should be identified as a 
// CDC Serial COM port. Connect the picoSTK to the target board via the 6-pin ICSP 
// connector. The picoISP should work with any IDE or programming software that 
// supports the STK500v1 or Arduino as ISP programmer.
// Windows users may need to install a CDC driver manually using the Zadig tool 
// (https://zadig.akeo.ie/).


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================

// Libraries
#include "src/system.h"                     // system functions
#include "src/delay.h"                      // delay functions
#include "src/isp.h"                        // ISP functions

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
  ISP_init();                               // init ISP

  // Loop
  while(1) avrisp();                        // handle incoming commands
}
