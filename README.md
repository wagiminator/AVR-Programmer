# AVR Programmer Boards
Collection of AVR Programmer Boards and Accessories. AVR development boards can be found [here](https://github.com/wagiminator/AVR-Development-Boards). Special programmers can be found here:
- [tinyHVSP: high-voltage fuse resetter for 8-pin ATtinys](https://github.com/wagiminator/ATtiny84-TinyHVSP)
- [tinyCalibrator: oscillator calibrator for 8-pin ATtinys](https://github.com/wagiminator/ATtiny84-TinyCalibrator)
- [tinyICOC: in-circuit oscillator calibrator for AVR microcontrollers](https://github.com/wagiminator/ATtiny84-TinyICOC)
- [picoDAP: CMSIS-DAP compliant SWD Programmer](https://github.com/wagiminator/CH552-picoDAP)

## [SerialUPDI Programmer - CH340N](https://github.com/wagiminator/AVR-Programmer/tree/master/SerialUPDI_Programmer)
Simple and cheap UPDI programmer for tinyAVR, megaAVR and AVR-Dx microcontrollers with voltage selection switch (5V and 3.3V). The programmer works with [pyupdi](https://github.com/mraardvark/pyupdi) as well as with the Arduino IDE as ["SerialUPDI"](https://github.com/SpenceKonde/megaTinyCore). It is based on the USB-to-serial chip [CH340N](https://datasheet.lcsc.com/lcsc/2101130932_WCH-Jiangsu-Qin-Heng-CH340N_C506813.pdf).

![SerialUPDI_Programmer_pic3.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/SerialUPDI_Programmer/SerialUPDI_Programmer_pic3.jpg)

## [SerialUPDI HV Programmer - CH340E](https://github.com/wagiminator/AVR-Programmer/tree/master/SerialUPDI_HV_Programmer)
The simple and cheap High-Voltage Serial UPDI Programmer is a device designed specifically for tinyAVR microcontrollers. This programmer includes a target voltage selection switch that allows users to choose between 5V and 3.3V for their project needs. With this device, you can use the additional configuration settings for the UPDI pin without worrying about getting locked out from the MCU. The programmer operates similarly to a standard SerialUPDI programmer and is compatible with popular development environments like the Arduino IDE, where it appears as "SerialUPDI". The device utilizes the MT3608 Boost Converter IC which generates the 12V pulse required for high-voltage activation of the UPDI pin.

![SerialUPDI_HV_Prog_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/SerialUPDI_HV_Programmer/documentation/SerialUPDI_HV_Prog_pic1.jpg)

## [picoISP Programmer - CH552E](https://github.com/wagiminator/AVR-Programmer/tree/master/picoISP_Programmer)
The CH55x-based picoISP is a simple and cheap ISP programmer for AVR microcontrollers with voltage selection switch (5V and 3.3V). Since it is compatible with the [USBtinyISP](https://learn.adafruit.com/usbtinyisp) it works fine with AVRdude and the Arduino IDE.

![picoISP_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/picoISP_Programmer/documentation/picoISP_pic1.jpg)

## [USB to Serial Converter Stick - CH340E](https://github.com/wagiminator/AVR-Programmer/tree/master/USB_to_Serial_Stick)
Simple and cheap USB-to-Serial converter based on [CH340E](https://datasheet.lcsc.com/lcsc/2008191806_WCH-Jiangsu-Qin-Heng-CH340E_C99652.pdf) with voltage selection switch (5V and 3.3V).

![USB2Serial_Stick_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USB_to_Serial_Stick/USB2Serial_Stick_pic1.jpg)

## [USBasp - ATmega8A](https://github.com/wagiminator/AVR-Programmer/tree/master/USBasp_SMD)
USB ISP/TPI Programmer Stick for Atmel AVR microcontrollers with 5V and 3.3V target voltage based on the design by [Thomas Fischl](https://www.fischl.de/usbasp/).

![USBasp_SMD_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USBasp_SMD/USBasp_SMD_pic1.jpg)

## [USBtinyISP - ATtiny45/85](https://github.com/wagiminator/AVR-Programmer/tree/master/USBtinyISP_ATtiny45_SMD)
Simple USB ISP Programmer Stick for Atmel AVR microcontrollers based on the ATtiny45/85. More information can be found at https://dicks.home.xs4all.nl/avr/usbtiny/.

![USBtinyISP_t45_SMD_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USBtinyISP_ATtiny45_SMD/USBtinyISP_t45_SMD_pic1.jpg)

## [USBasp - ATmega8 (DIP)](https://github.com/wagiminator/AVR-Programmer/tree/master/USBasp_DIP)
USB ISP/TPI Programmer for Atmel AVR microcontrollers based on the design by [Thomas Fischl](https://www.fischl.de/usbasp/).

![USBasp_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USBasp_DIP/USBasp_pic1.jpg)

## [USBtinyISP - ATtiny2313 (DIP)](https://github.com/wagiminator/AVR-Programmer/tree/master/USBtinyISP_ATtiny2313_DIP)
USB ISP Programmer with buffer for Atmel AVR microcontrollers based on the design by [ladyada](https://learn.adafruit.com/usbtinyisp).

![USBtinyISP_t2313_DIP_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USBtinyISP_ATtiny2313_DIP/USBtinyISP_t2313_DIP_pic1.jpg)

## [USBtinyISP - ATtiny45/85 (DIP)](https://github.com/wagiminator/AVR-Programmer/tree/master/USBtinyISP_ATtiny45_DIP)
Simple USB ISP Programmer for Atmel AVR microcontrollers. More information can be found at https://dicks.home.xs4all.nl/avr/usbtiny/.

![USBtinyISP_t45_DIP_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USBtinyISP_ATtiny45_DIP/USBtinyISP_t45_DIP_pic1.jpg)

## [USB to Serial Converter - CH330N](https://github.com/wagiminator/AVR-Programmer/tree/master/USB_to_Serial_Converter)
USB to serial converter based on the CH330N (or CH340N) chip with 5V or 3.3V logic.

![USB_to_Serial_Converter_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USB_to_Serial_Converter/USB_to_Serial_Converter_pic1.jpg)

## [AVR Programmer Adapter](https://github.com/wagiminator/AVR-Programmer/tree/master/AVR_Programmer_Adapter)
AVR programmer adapter for 8-pin ATtiny microcontrollers for converting:
- 6-pin ICSP connectors,
- 10-pin ICSP connectors,
- 8-pin SOP8/SOIC8 adapters,
- 8-pin SOP8/SOIC8 clips,
- 8-pin DIP8 chips.

![AVR_Programmer_Adapter_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/AVR_Programmer_Adapter/AVR_Programmer_Adapter_pic1.jpg)
![AVR_Programmer_Adapter_pic2.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/AVR_Programmer_Adapter/AVR_Programmer_Adapter_pic2.jpg)

## [AVR Programmer Adapter Maxi](https://github.com/wagiminator/AVR-Programmer/tree/master/AVR_Programmer_Adapter_Maxi)
The AVR Programmer Adapter Maxi makes it easy to program SMD ATtinys and ATmegas in SOIC-8, SOIC-14 and TQFP-32 packages.

![AVR_Programmer_Adapter_Maxi_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/AVR_Programmer_Adapter_Maxi/AVR_Programmer_Adapter_Maxi_pic1.jpg)
![AVR_Programmer_Adapter_Maxi_pic2.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/AVR_Programmer_Adapter_Maxi/AVR_Programmer_Adapter_Maxi_pic2.jpg)

## [PyUPDI Programmer - CH330N](https://github.com/wagiminator/AVR-Programmer/tree/master/PyUPDI_Programmer)
Probably the simplest UPDI programmer for tinyAVR, megaAVR and AVR-Dx microcontrollers. The programmer works with [pyupdi](https://github.com/mraardvark/pyupdi) as well as with the Arduino IDE as ["SerialUPDI"](https://github.com/SpenceKonde/megaTinyCore). It is based on the USB-to-serial chip [CH330N](https://datasheet.lcsc.com/szlcsc/2008191734_WCH-Jiangsu-Qin-Heng-CH330N_C108996.pdf) or [CH340N](https://datasheet.lcsc.com/lcsc/2101130932_WCH-Jiangsu-Qin-Heng-CH340N_C506813.pdf).

![PyUPDI_Programmer_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/PyUPDI_Programmer/PyUPDI_Programmer_pic1.jpg)

## [UPDI Programmer - ATmega8/88/168/328](https://github.com/wagiminator/AVR-Programmer/tree/master/UPDI_Programmer)
UPDI Programmer (jtag2updi) for tinyAVR, megaAVR and AVR-Dx microcontrollers built on ATmega8/88/168/328 based on the design and firmware by [ElTangas](https://github.com/ElTangas/jtag2updi).

![UPDI_Programmer_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/UPDI_Programmer/UPDI_Programmer_pic1.jpg)

## [UPDI Programmer Mini - ATtiny1604/1614](https://github.com/wagiminator/AVR-Programmer/tree/master/UPDI_Programmer_Mini)
Mini UPDI Programmer Stick (jtag2updi) for tinyAVR, megaAVR and AVR-Dx microcontrollers built on ATtiny1604/1614 based on the design and firmware by [ElTangas](https://github.com/ElTangas/jtag2updi). Program the new ATtiny devices with a new ATtiny device!

![UPDI_Programmer_Mini_pic2.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/UPDI_Programmer_Mini/UPDI_Programmer_Mini_pic2.jpg)
![UPDI_Programmer_Mini_pic3.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/UPDI_Programmer_Mini/UPDI_Programmer_Mini_pic3.jpg)

## [UPDI HV Programmer - ATmega8/88/168/328](https://github.com/wagiminator/AVR-Programmer/tree/master/UPDI_HV_Programmer)
HV UPDI (High-Voltage Unified Program and Debug Interface) Programmer for tinyAVR, megaAVR and AVR-Dx microcontrollers built on ATmega8/88/168/328 based on the design and the firmware by [Dlloydev](https://github.com/Dlloydev/jtag2updi). The HV UPDI Programmer will enable you to use the additional configuration settings for the UPDI pin without the fear of getting locked out from the MCU. It features 3 programming modes: UPDI, HV or PCHV, with the target voltage at 5V.

![UPDI_HV_Programmer_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/UPDI_HV_Programmer/UPDI_HV_Programmer_pic1.jpg)

## [UPDI HV Programmer Mini - ATtiny1604/1614](https://github.com/wagiminator/AVR-Programmer/tree/master/UPDI_HV_Programmer_Mini)
Mini HV UPDI (High-Voltage Unified Program and Debug Interface) Programmer Stick for tinyAVR, megaAVR and AVR-Dx microcontrollers built on ATtiny1604/1614 based on the design and the firmware by [Dlloydev](https://github.com/Dlloydev/jtag2updi). The Mini HV UPDI Programmer Stick will enable you to use the additional configuration settings for the UPDI pin without the fear of getting locked out from the MCU. It features 3 programming modes: UPDI, HV or PCHV, with the target voltage at 5V.

![UPDI_HV_Programmer_Mini_pic2.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/UPDI_HV_Programmer_Mini/UPDI_HV_Programmer_Mini_pic2.jpg)
![UPDI_HV_Programmer_Mini_pic3.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/UPDI_HV_Programmer_Mini/UPDI_HV_Programmer_Mini_pic3.jpg)
