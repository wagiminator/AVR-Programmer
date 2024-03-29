# picoAVR - ISP / TPI / UPDI Combo Programmer for AVR MCU based on CH552G
The CH55x-based picoAVR is an ISP, TPI, and UPDI combo programmer for AVR microcontrollers compatible with the USBasp and SerialUPDI. It works fine with the Arduino IDE (Tools -> Programmer -> USBasp / Tools -> Programmer -> SerialUPDI). This combo programmer includes a target voltage selection switch that allows users to choose between 5V and 3.3V for their project needs.

![picoAVR_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoAVR_Programmer/documentation/picoAVR_pic1.jpg)

# Hardware
## Schematic
![picoAVR_wiring.png](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoAVR_Programmer/documentation/picoAVR_wiring.png)

## CH552G 8-bit USB Device Microcontroller
The CH552G is a low-cost, enhanced E8051 core microcontroller compatible with the MCS51 instruction set. It has an integrated USB 2.0 controller with full-speed data transfer (12 Mbit/s) and supports up to 64 byte data packets with integrated FIFO and direct memory access (DMA). The CH552G has a factory built-in bootloader so firmware can be uploaded directly via USB without the need for an additional programming device.

![picoAVR_pic2.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoAVR_Programmer/documentation/picoAVR_pic2.jpg)
![picoAVR_pic3.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoAVR_Programmer/documentation/picoAVR_pic3.jpg)

# Compiling and Installing Firmware
## Preparing the CH55x Bootloader
### Installing Drivers for the CH55x Bootloader
On Linux you do not need to install a driver. However, by default Linux will not expose enough permission to upload your code with the USB bootloader. In order to fix this, open a terminal and run the following commands:

```
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="4348", ATTR{idProduct}=="55e0", MODE="666"' | sudo tee /etc/udev/rules.d/99-ch55x.rules
sudo service udev restart
```

For Windows, you need the [CH372 driver](http://www.wch-ic.com/downloads/CH372DRV_EXE.html). Alternatively, you can also use the [Zadig Tool](https://zadig.akeo.ie/) to install the correct driver. Here, click "Options" -> "List All Devices" and select the USB module. Then install the libusb-win32 driver. To do this, the board must be connected and the CH55x must be in bootloader mode.

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
- Connect the board and make sure the CH55x is in bootloader mode. 
- Run ```make flash``` to compile and upload the firmware. 
- If you don't want to compile the firmware yourself, you can also upload the precompiled binary. To do this, just run ```python3 ./tools/chprog.py picoavr.bin```.

## Compiling and Uploading using the Arduino IDE
### Installing the Arduino IDE and CH55xduino
Install the [Arduino IDE](https://www.arduino.cc/en/software) if you haven't already. Install the [CH55xduino](https://github.com/DeqingSun/ch55xduino) package by following the instructions on the website.

### Compiling and Uploading Firmware
- Adjust the firmware parameters in src/config.h if necessary.
- Copy the .ino and .c files as well as the /src folder together into one folder and name it picoavr. 
- Open the .ino file in the Arduino IDE.
- Go to **Tools -> Board -> CH55x Boards** and select **CH552 Board**.
- Go to **Tools** and choose the following board options:
  - **Clock Source:**   16 MHz (internal)
  - **Upload Method:**  USB
  - **USB Settings:**   USER CODE /w 0B USB RAM
- Connect the board and make sure the CH55x is in bootloader mode. 
- Click **Upload**.

## Installing Drivers for the picoAVR
On Linux, you don't need to install a driver. The implementation includes Windows Compatible ID (WCID) code for automated driver installation on Windows. If this doesn't work, Windows users may need to install a driver manually using the Zadig tool (https://zadig.akeo.ie/). Click on "Options" -> "List all devices" and select the USBasp. Then install the libusb-win32 driver. Now select the SerialUPDI and install the CDC driver. To do this, the picoAVR must be connected to your PC.

# Operating Instructions
- Select the programming voltage (3.3V or 5V) with the voltage selection switch.
- Plug the picoAVR into a USB port on your PC, it should be identified as a USBasp with an additional CDC COM port.
- Connect the picoAVR to the target board via the 6-pin ICSP connector or the 3-pin UPDI connector.  
- The picoAVR should work with any IDE or programming software that supports the USBasp and/or SerialUPDI programmer.

# References, Links and Notes
1. [EasyEDA Design Files](https://oshwlab.com/wagiminator)
2. [WCH: CH552 Datasheet](http://www.wch-ic.com/downloads/CH552DS1_PDF.html)
3. [SDCC Compiler](https://sdcc.sourceforge.net/)
4. [Blinkinlabs: CH55x SDK for SDCC](https://github.com/Blinkinlabs/ch554_sdcc)
5. [Thomas Fischl: USBasp](https://www.fischl.de/usbasp/)
6. [Ralph Doncaster: USBasp](https://github.com/nerdralph/usbasp)
7. [Deqing Sun: CH55xduino](https://github.com/DeqingSun/ch55xduino)

![picoAVR_pic4.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoAVR_Programmer/documentation/picoAVR_pic4.jpg)
![picoAVR_pic5.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoAVR_Programmer/documentation/picoAVR_pic5.jpg)

# License
![license.png](https://i.creativecommons.org/l/by-sa/3.0/88x31.png)

This work is licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License. 
(http://creativecommons.org/licenses/by-sa/3.0/)
