// ===================================================================================
// Project:   picoAVR ISP + UPDI Programmer based on CH551, CH552, CH554
// Version:   v1.3
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// The CH55x-based picoAVR is an ISP and UPDI programmer for AVR microcontrollers 
// compatible with the USBtinyISP and SerialUPDI. It works fine with the Arduino IDE 
// (Tools -> Programmer -> USBtinyISP / Tools -> Programmer -> SerialUPDI). The
// USBtinyISP part of the firmware is based on Dick Streefland's and Ladyada's 
// original implementation and DeqingSun's CH55x conversion.
//
// References:
// -----------
// - Dick Streefland: https://dicks.home.xs4all.nl/avr/usbtiny/
// - Ladyada: https://learn.adafruit.com/usbtinyisp
// - Deqing Sun: https://github.com/DeqingSun/ch55xduino
// - Blinkinlabs: https://github.com/Blinkinlabs/ch554_sdcc
// - Ralph Doncaster: https://github.com/nerdralph/ch554_sdcc
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
// USBtinyISP with an additional CDC serial interface. Connect the picoAVR to the 
// target board via the 6-pin ICSP connector or the 3-pin UPDI connector.  
// Windows users may need to install a driver for USBtinyISP
// (https://learn.adafruit.com/usbtinyisp) and for CDC (https://zadig.akeo.ie/).
// The picoAVR should work with any IDE or programming software that supports the
// USBtinyISP and/or SerialUPDI programmer.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================

// Libraries
#include "src/system.h"                     // system functions
#include "src/delay.h"                      // delay functions
#include "src/usb_avr.h"                    // AVR ISP and UPDI functions

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
  uint8_t databyte;

  // Setup
  CLK_config();                             // configure system clock
  DLY_ms(10);                               // wait for clock to settle
  AVR_init();                               // init AVR ISP and UPDI
  WDT_start();                              // start watchdog

  // Loop
  while(1) {
    // ISP stuff is handled via USB interrupts.
    // We only need to take care of USB-CDC to UPDI here.
    if(CDC_available()) {                   // something coming in via USB-CDC?
      PIN_low(PIN_LED);                     // turn on LED (active low)
      if(TH1 == 0) {                        // slow speed? -> must be UPDI BREAK
        UPDI_break();                       // send UPDI break frame
        CDC_write(CDC_read());              // return BREAK frame
        CDC_flush();                        // flush OUT buffer
      }
      else {                                // normal data bytes?
        // Send data bytes via UPDI and write echo to USB OUT buffer
        while(CDC_available()) {            // repeat for all incoming bytes
          databyte = CDC_read();            // get data byte
          UPDI_write(databyte);             // send byte via UPDI
          CDC_write(databyte);              // send back echo
        }
        CDC_flush();                        // flush OUT buffer
      }
      PIN_high(PIN_LED);                    // turn off LED (active low)
    }

    if(UPDI_available()) {                  // something coming in via UPDI?
      PIN_low(PIN_LED);                     // turn on LED (active low)
      while(UPDI_available())               // repeat for all incoming bytes
        CDC_write(UPDI_read());             // write received bytes to USB OUT buffer
      CDC_flush();                          // flush USB OUT buffer
      PIN_high(PIN_LED);                    // turn off LED (active low)
    }

    WDT_reset();                            // reset watchdog
  }
}
