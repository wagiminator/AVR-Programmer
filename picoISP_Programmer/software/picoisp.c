// ===================================================================================
// Project:   picoISP AVR ISP Programmer based on CH551, CH552, CH554
// Version:   v1.0
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// The CH55x-based picoISP is an ISP programmer for AVR microcontrollers compatible 
// with the USBtinyISP. It works fine with AVRdude (avrdude -c usbtiny) and the 
// Arduino IDE (Tools -> Programmer -> USBtinyISP). The firmware is based on 
// Dick Streefland's and ladyada's original implementation and DeqingSun's CH55x 
// conversion.
//
// References:
// -----------
// - Dick Streefland: https://dicks.home.xs4all.nl/avr/usbtiny/
// - ladyada: https://learn.adafruit.com/usbtinyisp
// - Deqing Sun: https://github.com/DeqingSun/ch55xduino
// - Blinkinlabs: https://github.com/Blinkinlabs/ch554_sdcc
// - Ralph Doncaster: https://github.com/nerdralph/ch554_sdcc
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
//
// Compilation Instructions:
// -------------------------
// - Chip:     CH551, CH552 or CH554
// - Clock:    16 MHz internal
// - Adjust the firmware parameters in include/config.h if necessary.
// - Make sure SDCC toolchain and Python3 with PyUSB is installed.
// - Press BOOT button on the board and keep it pressed while connecting it via USB
//   with your PC.
// - Run 'make flash'.
//
// Operating Instructions:
// -----------------------
// Select the programming voltage (3.3V or 5V) with the voltage selection switch.
// Plug the picoISP into a USB port on your PC, it should be identified as a 
// USBtinyISP. Connect the picoISP to the target board via the 6-pin ICSP connector.  
// Windows users may need to install a driver (https://learn.adafruit.com/usbtinyisp).
// The picoISP should work with any IDE or programming software that supports the
// USBtinyISP programmer.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================

// Libraries
#include <system.h>                         // system functions
#include <delay.h>                          // delay functions
#include <usb_isp.h>                        // AVR ISP functions

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
  DLY_ms(5);                                // wait for clock to settle
  ISP_init();                               // init AVR ISP

  // Loop
  while(1);                                 // everything runs on USB interrupts
}
