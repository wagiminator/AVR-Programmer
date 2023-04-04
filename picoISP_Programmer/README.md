# picoISP Programmer for AVR MCU based on CH552E
The CH55x-based picoISP is a simple and cheap ISP programmer for AVR microcontrollers with voltage selection switch (5V and 3.3V) and compatible with the [USBtinyISP](https://learn.adafruit.com/usbtinyisp). It works fine with AVRdude (avrdude -c usbtiny) and the Arduino IDE (Tools -> Programmer -> USBtinyISP). The firmware is based on Dick Streefland's and Ladyada's original implementation and DeqingSun's CH55x conversion.

![picoISP_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoISP_Programmer/documentation/picoISP_pic1.jpg)

# Hardware
## Schematic
![picoISP_wiring.png](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoISP_Programmer/documentation/picoISP_wiring.png)

## CH552E 8-bit USB Device Microcontroller
The CH552E is a low-cost, enhanced E8051 core microcontroller compatible with the MCS51 instruction set. It has an integrated USB 2.0 controller with full-speed data transfer (12 Mbit/s) and supports up to 64 byte data packets with integrated FIFO and direct memory access (DMA). The CH552E has a factory built-in bootloader so firmware can be uploaded directly via USB without the need for an additional programming device.

![picoISP_pic2.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoISP_Programmer/documentation/picoISP_pic2.jpg)
![picoISP_pic3.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoISP_Programmer/documentation/picoISP_pic3.jpg)

# Compiling and Installing Firmware
## Preparing the CH55x Bootloader
### Installing Drivers for the CH55x Bootloader
On Linux you do not need to install a driver. However, by default Linux will not expose enough permission to upload your code with the USB bootloader. In order to fix this, open a terminal and run the following commands:

```
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="4348", ATTR{idProduct}=="55e0", MODE="666"' | sudo tee /etc/udev/rules.d/99-ch55x.rules
sudo service udev restart
```

On Windows you will need the [Zadig tool](https://zadig.akeo.ie/) to install the correct driver. Click "Options" and "List All Devices" to select the USB module, then install the libusb-win32 driver. To do this, the board must be connected and the CH55x must be in bootloader mode.

### Entering CH55x Bootloader Mode
A brand new chip starts automatically in bootloader mode as soon as it is connected to the PC via USB. Once firmware has been uploaded, the bootloader must be started manually for new uploads. To do this, the board must first be disconnected from the USB port and all voltage sources. Now press the BOOT button and keep it pressed while reconnecting the board to the USB port of your PC. The chip now starts again in bootloader mode, the BOOT button can be released and new firmware can be uploaded within the next couple of seconds.

## Compiling and Uploading using the makefile
### Installing SDCC Toolchain for CH55x
Install the [SDCC Compiler](https://sdcc.sourceforge.net/). In order for the programming tool to work, Python3 must be installed on your system. To do this, follow these [instructions](https://www.pythontutorial.net/getting-started/install-python/). In addition [pyusb](https://github.com/pyusb/pyusb) must be installed. On Linux (Debian-based), all of this can be done with the following commands:

```
sudo apt install build-essential sdcc python3 python3-pip
sudo pip install pyusb
```

### Compiling and Uploading Firmware
- Open a terminal.
- Navigate to the folder with the makefile. 
- Connect the board and make sure the CH55x ist in bootloader mode. 
- Run ```make flash``` to compile and upload the firmware. 
- If you don't want to compile the firmware yourself, you can also upload the precompiled binary. To do this, just run ```python3 ./tools/chprog.py picoisp.bin```.

## Compiling and Uploading using the Arduino IDE
### Installing the Arduino IDE and CH55xduino
Install the [Arduino IDE](https://www.arduino.cc/en/software) if you haven't already. Install the [CH55xduino](https://github.com/DeqingSun/ch55xduino) package by following the instructions on the website.

### Compiling and Uploading Firmware
- Copy the .ino and .c files as well as the /src folder together into one folder and name it picoisp. 
- Open the .ino file in the Arduino IDE.
- Go to **Tools -> Board -> CH55x Boards** and select **CH552 Board**.
- Go to **Tools** and choose the following board options:
  - **Clock Source:**   16 MHz (internal)
  - **Upload Method:**  USB
  - **USB Settings:**   USER CODE /w 266B USB RAM
- Connect the board and make sure the CH55x ist in bootloader mode. 
- Click **Upload**.

## Installing Drivers for the picoISP
On Linux you do not need to install a driver. Windows users may need to install the usbtiny driver (https://learn.adafruit.com/usbtinyisp).

# Operating Instructions
- Select the programming voltage (3.3V or 5V) with the voltage selection switch.
- Plug the picoISP into a USB port on your PC, it should be identified as a USBtinyISP.
- Connect the picoISP to the target board via the 6-pin ICSP connector.  
- The picoISP should work with any IDE or programming software that supports the USBtinyISP programmer.

# References, Links and Notes
1. [EasyEDA Design Files](https://oshwlab.com/wagiminator/ch552-picoisp-programmer)
2. [WCH: CH552 Datasheet](http://www.wch-ic.com/downloads/CH552DS1_PDF.html)
3. [SDCC Compiler](https://sdcc.sourceforge.net/)
4. [Blinkinlabs: CH55x SDK for SDCC](https://github.com/Blinkinlabs/ch554_sdcc)
5. [Dick Streefland: USBtiny](https://dicks.home.xs4all.nl/avr/usbtiny/)
6. [Ladyada: USBtinyISP](https://learn.adafruit.com/usbtinyisp)
7. [Deqing Sun: CH55xduino](https://github.com/DeqingSun/ch55xduino)

![picoISP_pic4.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoISP_Programmer/documentation/picoISP_pic4.jpg)

# License
![license.png](https://i.creativecommons.org/l/by-sa/3.0/88x31.png)

This work is licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License. 
(http://creativecommons.org/licenses/by-sa/3.0/)
