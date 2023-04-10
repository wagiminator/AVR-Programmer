# USBasp (ATmega8 DIP)
The USBasp is a versatile USB ISP/TPI programmer stick designed for Atmel AVR microcontrollers with selectable target voltage of 5V and 3.3V. It is based on the design by Thomas Fischl and is an easy-to-use tool that enables you to program your microcontroller projects with ease. You can find more information about the USBasp at https://www.fischl.de/usbasp/.

## Uploading Firmware
- Make sure you have installed [avrdude](https://learn.adafruit.com/usbtinyisp/avrdude).
- Set the TGTPWR and the SELFPROG jumper on the USBasp you want to program.
- Connect your programmer to your PC and to the USBasp you want to program.
- Open a terminal.
- Navigate to the folder with the hex-file.
- Execute the following command (if necessary replace "usbasp" with the programmer you use):
  ```
  avrdude -c usbasp -p m8 -U lfuse:w:0x9f:m -U hfuse:w:0xc9:m -U flash:w:usbasp_m8_2020-09-14.hex
  ```
- Remove the TGTPWR jumper.

## Installing Drivers
On Linux and Mac, you don't need to install a driver. Windows users can use the [Zadig Tool](https://zadig.akeo.ie/) to install the correct driver. Click on "Options" -> "List all devices" and select the "USBasp". Then install the libusb-win32 driver. To do this, the USBasp must be connected to your PC.

## Operating Instructions
If you are flashing a fresh microcontroller, it is important to close the Slow Serial Clock jumper (SLOWCLK) as these microcontrollers are factory programmed for an internal 1MHz oscillator. However, if you use the improved firmware by nerdralph (https://github.com/nerdralph/usbasp), this step is not necessary. To power your target device from the USBasp, simply close the Target Power jumper (TGTPWR). When uploading firmware to the USBasp itself, it is necessary to close the SELFPROG and TGTPWR jumpers.

|Jumper|Function|
|:-|:-|
|TGTPWR|Close to power target device via ISP header|
|SELFPRG|Close to upload firmware to USBasp itself via ISP header|
|SLOWCLK|Close to upload firmware to target device with slow speed (necessary for factory-new microcontrollers); not needed with improved firmware|

![USBasp_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USBasp_DIP/USBasp_pic1.jpg)
![USBasp_pic2.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USBasp_DIP/USBasp_pic2.jpg)
