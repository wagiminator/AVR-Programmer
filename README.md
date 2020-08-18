# AVR Programmer Boards
Collection of AVR Programmer Boards and Accessories

# UPDI Programmer - ATmega8/88/168/328 (SMD)
UPDI programmer (jtag2updi) for the new tinyAVR 0- and 1-series microcontrollers based on ATmega8/88/168/328.

Project Video: https://youtu.be/OVSLsA9-gF0

More information and firmware: https://github.com/ElTangas/jtag2updi

Getting started with the new ATtiny chips: http://www.technoblogy.com/show?2OCH

megaTinyCore: https://github.com/SpenceKonde/megaTinyCore

![IMG_20191208_124317_x.jpg](https://image.easyeda.com/pullimage/PM68XuK188ao1RtIswFbullYaIk90rYgmMzmChER.jpeg)
![IMG_20200514_173213_x.jpg](https://image.easyeda.com/pullimage/IGZjPZZ4m6kjn5Qch3MXRU1gqsGLIccMPUOz08z0.jpeg)

# UPDI Programmer Mini - ATtiny1604/1614 (SMD)
Mini UPDI programmer (jtag2updi) for the new tinyAVR 0- and 1-series microcontrollers based on the ATtiny1604/1614. Program the new ATtiny devices with a new ATtiny device!

How to use a UPDI programmer video: https://youtu.be/OVSLsA9-gF0

More information and firmware: https://github.com/ElTangas/jtag2updi

Getting started with the new ATtiny chips: http://www.technoblogy.com/show?2OCH

megaTinyCore: https://github.com/SpenceKonde/megaTinyCore

![IMG_20200613_173825_x.jpg](https://image.easyeda.com/pullimage/icfYq77BBZRDNswbE3f3vRKpzrFgV8p63hpvzKs4.jpeg)
![IMG_20200613_173920_x.jpg](https://image.easyeda.com/pullimage/qU5N95pMje5yX9TScD6FCIrOMVn27a4kgl2rreWa.jpeg)

# UPDI HV Programmer - ATmega8/88/168/328 (SMD)

HV UPDI (High Voltage Unified Program and Debug Interface) programmer for the new tinyAVR 0- and 1-series microcontrollers built on ATmega8/88/168/328 based on the design and the firmware by Dlloydev (https://github.com/Dlloydev/jtag2updi). The HV UPDI programmer will enable you to use the additional configuration settings for the UPDI pin without the fear of getting locked out from the MCU. It features 3 programming modes: UPDI, HV or PCHV, with the target voltage at 5V.

**Mode Jumper Settings:**

|Mode Jumper|Prog Mode|PA0 Configurations|HV Pulse|Power Cycle|
|-|-|-|-|-|-|
|no jumper|UPDI|UPDI|NO|NO|
|pins 2-3 shorted|HV|UPDI, RESET|YES|NO|
|pins 1-2 shorted|PCHV|UPDI, RESET, GPIO|YES|YES|

**Modes:**

|Mode|Function|
|-|-|
|UPDI Mode|This mode would be used when the UPDI pin is configured as UPDI or for any target device that isn't HV tolerant.|
|HV Mode|This mode applies the 12V UPDI enable sequence (HV pulse) at the start of the programming sequence. This temporarily reconfigures the UPDI/Reset pin to UPDI mode which will remain in this state until the next power on reset (POR). This allows programming to occur when the pin is configured as Reset. A POR needs to occur for any fuse setting changes to take effect.|
|PCHV Mode|Power Cycle High Voltage mode (PCHV) will initiate a power cycle and HV pulse at the start of the programming sequence. At the end of the sequence, a second power cycle will occur which causes any new fuse setting to take effect. The power cycle OFF duration has been set to 10ms. This mode would be used when the UPDI/Reset pin is configured as Reset or as GPIO.|

**Status LED Operation:**

|LED|Status|
|-|-|
|PWR|STEADY ON when programmer is powered|
|PRG|STEADY ON when in programming mode; BLINKING at 4Hz if target overload occurs|
|HV|FLASHING at HV pulses during programming|

![IMG_20200816_165418_x.jpg](https://image.easyeda.com/pullimage/WSaYBqbKyEx85czv25Vj6bLIZKI5R4EZFXKvWk67.jpeg)

# USBasp - ATmega8A (SMD)

USBasp - USB ISP/TPI programmer stick for Atmel AVR microcontrollers with 5V and 3.3V target voltage based on the design by Thomas Fischl. More information can be found at https://www.fischl.de/usbasp/. Windows users may need to install a driver. Of course this is not necessary for linux and mac users.

|Jumper|Function|
|-|-|
|TGTVCC|Close to power target device via ISP header|
|SELFPRG|Close to upload firmware to USBasp itself via ISP header|
|SLOWCLK|Close to upload firmware to target device with slow speed (necessary for factory-new microcontrollers)|

![IMG_20200807_142132_x.jpg](https://image.easyeda.com/pullimage/d0ASArBzXZ8V1ynPEoLOtxn6s97VlMvqZNAw8k3H.jpeg)
![IMG_20200807_142211_x.jpg](https://image.easyeda.com/pullimage/ZjNAFKYcdE5Z6Ej2qIZHsuxyc8uMiJBT51fkMTWi.jpeg)

# USBtinyISP - ATtiny45/85 (SMD)
Simple USB ISP programmer for Atmel AVR microcontrollers based on the ATtiny45/85. More information can be found at https://dicks.home.xs4all.nl/avr/usbtiny/. Windows users may need to install a driver: https://learn.adafruit.com/usbtinyisp/drivers. Of course this is not necessary for linux and mac users.

![IMG_20200705_083217_x.jpg](https://image.easyeda.com/pullimage/qCx6PWBVTSVGHAjXukQMZttNO4id0el6g8eESECR.jpeg)

# USBasp - ATmega8 (DIP)
USBasp - USB ISP/TPI programmer for Atmel AVR microcontrollers based on the design by Thomas Fischl. More information can be found at https://www.fischl.de/usbasp/. Windows users may need to install a driver. Of course this is not necessary for linux and mac users. Close SELFPROG jumper when uploading firmware. If you are burning a fresh microcontroller, close the Slow Serial Clock jumper (SLOWCLK) since they are factory programmed for internal 1MHz oscillator. Close Taget Power jumper (TGTPWR) to power target device from the USBasp.

![IMG_20200111_160216_x.jpg](https://image.easyeda.com/pullimage/7mTvpuRMFyOUBHyYD23iyDAu5gH15Dn6gT4HioRq.jpeg)

# USBtinyISP - ATtiny2313 (DIP)
USB ISP programmer with buffer for Atmel AVR microcontrollers based on the design by ladyada. More information can be found at https://learn.adafruit.com/usbtinyisp. Windows users may need to install a driver: https://learn.adafruit.com/usbtinyisp/drivers. Of course this is not necessary for linux and mac users.

![IMG_20190817_132314_x.jpg](https://image.easyeda.com/pullimage/u9zoNt2S1srkuZI3wce0yCBqCxodPfevkOeClaYS.jpeg)

# USBtinyISP - ATtiny45/85 (DIP)
Simple USB ISP programmer for Atmel AVR microcontrollers. More information can be found at https://dicks.home.xs4all.nl/avr/usbtiny/. Windows users may need to install a driver: https://learn.adafruit.com/usbtinyisp/drivers. Of course this is not necessary for linux and mac users.

![IMG_20190817_152833_x.jpg](https://image.easyeda.com/pullimage/LfcmYyt3g5Lt9UxRwyiwrXzDqmxKaMS0bYB2vpNe.jpeg)

# USB to Serial Converter - CH330N (SMD)
USB to serial converter based on the new ch330 chip. Windows users may need to install a driver: http://www.wch.cn/download/CH341SER_ZIP.html. This is not necessary for linux users.

![IMG_20190721_163936_x.jpg](https://image.easyeda.com/pullimage/AJUqog14Ij64834MfDg9wDvgLqIuHVOqtuximmOm.jpeg)

# AVR Programmer Adapter
AVR programmer adapter for 8-pin ATtiny microcontrollers for converting:
- 6-pin ICSP connectors,
- 10-pin ICSP connectors,
- 8-pin SOP8/SOIC8 adapters,
- 8-pin SOP8/SOIC8 clips,
- 8-pin DIP8 chips.

![IMG_20200614_132142_x.jpg](https://image.easyeda.com/pullimage/2Z1C33oQaDrrEAdvkacl5YyJtu6WFPP9fS3XFmv0.jpeg)
![IMG_20200614_132106_x.jpg](https://image.easyeda.com/pullimage/HYhXpGDUFOx7fKk47dkpx30zrmqlgDVB5G0cT1hW.jpeg)
