# UPDI HV Programmer
HV UPDI (High-Voltage Unified Program and Debug Interface) programmer for tinyAVR, megaAVR and AVR-Dx microcontrollers built on ATmega8/88/168/328 based on the design and the firmware by Dlloydev (https://github.com/Dlloydev/jtag2updi). The HV UPDI programmer will enable you to use the additional configuration settings for the UPDI pin without the fear of getting locked out from the MCU. It features 3 programming modes: UPDI, HV or PCHV, with the target voltage at 5V.

**Mode Jumper Settings:**

|Mode Jumper|Prog Mode|PA0 Configurations|HV Pulse|Power Cycle|
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
|PRG|STEADY ON when in programming mode; BLINKING at 4Hz if target overload occurs|
|HV|FLASHING at HV pulses during programming|

![IMG_20200816_165418_x.jpg](https://image.easyeda.com/pullimage/WSaYBqbKyEx85czv25Vj6bLIZKI5R4EZFXKvWk67.jpeg)
![IMG_20200820_165012_x.jpg](https://image.easyeda.com/pullimage/MiPS7JxxDo3wv8L5noEuWbKuNfCzOo8h9CAsPrfS.jpeg)
![pic_17_2.jpg](https://image.easyeda.com/pullimage/looVl4c4DJbB6DBh1GLoLdgY8OiKGK0J3UIYaWCC.jpeg)

- The CH330N can be replaced with a CH340N.
- More information and source code: https://github.com/Dlloydev/jtag2updi
- Getting started with the new ATtiny chips: http://www.technoblogy.com/show?2OCH
- megaTinyCore: https://github.com/SpenceKonde/megaTinyCore
- MegaCoreX: https://github.com/MCUdude/MegaCoreX
- UPDI Programmer based on ATmega8/88/168/328: https://easyeda.com/wagiminator/y-updi-programmer
- Mini UPDI Programmer based on ATtiny1604: https://easyeda.com/wagiminator/attiny1604-updi-programmer
- Mini UPDI HV Programmer based on ATtiny1604: https://easyeda.com/wagiminator/attiny1604-updi-hv-programmer
- Development board for ATtiny x14/x04 series: https://easyeda.com/wagiminator/attiny-814-development-board-smd
- Development board for ATtiny x16/x06 series: https://easyeda.com/wagiminator/y-attiny3216-development-board-smd
- Development board for ATmega4808 series: https://easyeda.com/wagiminator/atmega4808-development-board_copy
