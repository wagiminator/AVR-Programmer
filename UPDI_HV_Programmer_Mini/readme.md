# Mini UPDI HV Programmer Stick
Mini HV UPDI (High-Voltage Unified Program and Debug Interface) programmer for tinyAVR, megaAVR and AVR-Dx microcontrollers built on ATtiny1604/1614 based on the design and the firmware by Dlloydev (https://github.com/Dlloydev/jtag2updi). The Mini HV UPDI programmer will enable you to use the additional configuration settings for the UPDI pin without the fear of getting locked out from the MCU. It features 3 programming modes: UPDI, HV or PCHV, with the target voltage at 5V.

![IMG_20200913_102915_x.jpg](https://image.easyeda.com/pullimage/WHLYr95X94WEz8xDnWDjL9huWchuZqdKR6tdkJLe.jpeg)
![IMG_20200913_102947_x.jpg](https://image.easyeda.com/pullimage/Y3l77uJMz41Lhfrs3PIsd4ozecEohC2g1A3MwJd7.jpeg)

# Uploading Firmware

**SELFPRG Jumper Settings:**

|SELFPRG Jumper|Prog Mode|
|:-|:-|
|no jumper|Normal operation.|
|pins 1-2 shorted|UPDI - Uploading firmware or installing bootloader via UPDI header using another UPDI (HV) programmer. This is necessary when no bootloader is installed.|
|pins 2-3 shorted|USB - Uploading firmware via USB. An installed bootloader is required.|

# Operating Instructions

**MODE Jumper Settings:**

|MODE Jumper|Prog Mode|PA0 Configurations|HV Pulse|Power Cycle|
|:-|:-|:-|:-|:-|
|no jumper|UPDI|UPDI|NO|NO|
|pins 2-3 shorted|HV|UPDI, RESET|YES|NO|
|pins 1-2 shorted|PCHV|UPDI, RESET, GPIO|YES|YES|

**Modes:**

|Mode|Function|
|:-|:-|
|UPDI Mode|This mode would be used when the UPDI pin is configured as UPDI or for any target device that isn't HV tolerant.|
|HV Mode|This mode applies the 12V UPDI enable sequence (HV pulse) at the start of the programming sequence. This temporarily reconfigures the UPDI/Reset pin to UPDI mode which will remain in this state until the next power on reset (POR). This allows programming to occur when the pin is configured as Reset. A POR needs to occur for any fuse setting changes to take effect.|
|PCHV Mode|Power Cycle High-Voltage mode (PCHV) will initiate a power cycle and HV pulse at the start of the programming sequence. At the end of the sequence, a second power cycle will occur which causes any new fuse setting to take effect. The power cycle OFF duration has been set to 10ms. This mode would be used when the UPDI/Reset pin is configured as Reset or as GPIO.|

**Status LED Operation:**

|LED|Status|
|:-|:-|
|PWR|STEADY ON when programmer is powered|
|PRG|STEADY ON when in programming mode|
|HV|FLASHING at HV pulses during programming|

# Further Information
- The CH330N can be replaced with a CH340N.
- More information and source code: https://github.com/Dlloydev/jtag2updi/wiki/DIY-ATtiny-HV-UPDI-Programmer
- Getting started with the new ATtiny chips: http://www.technoblogy.com/show?2OCH
- megaTinyCore: https://github.com/SpenceKonde/megaTinyCore
- MegaCoreX: https://github.com/MCUdude/MegaCoreX
- UPDI Programmer based on ATmega8/88/168/328: https://easyeda.com/wagiminator/y-updi-programmer
- Mini UPDI Programmer based on ATtiny1604: https://easyeda.com/wagiminator/attiny1604-updi-programmer
- Development board for ATtiny x14/x04 series: https://easyeda.com/wagiminator/attiny-814-development-board-smd
- Development board for ATtiny x16/x06 series: https://easyeda.com/wagiminator/y-attiny3216-development-board-smd
- Development board for ATmega4808 series: https://easyeda.com/wagiminator/atmega4808-development-board_copy
