# USBasp (ATmega8 SMD)
The USBasp is a versatile USB ISP/TPI programmer stick designed for Atmel AVR microcontrollers with selectable target voltage of 5V and 3.3V. It is based on the design by Thomas Fischl and is an easy-to-use tool that enables you to program your microcontroller projects with ease. You can find more information about the USBasp at https://www.fischl.de/usbasp/. Windows users may need to install a driver in order to use the USBasp, however, linux and mac users can use it without any additional setup.

If you are flashing a fresh microcontroller, it is important to close the Slow Serial Clock jumper (SLOWCLK) as these microcontrollers are factory programmed for an internal 1MHz oscillator. However, if you use the improved firmware by nerdralph (https://github.com/nerdralph/usbasp), this step is not necessary. To power your target device from the USBasp, simply close the Target Power jumper (TGTPWR). When uploading firmware to the USBasp itself, it is necessary to close the SELFPROG and TGTPWR jumpers.

|Jumper|Function|
|:-|:-|
|TGTVCC|Close to power target device via ISP header|
|SELFPRG|Close to upload firmware to USBasp itself via ISP header|
|SLOWCLK|Close to upload firmware to target device with slow speed (necessary for factory-new microcontrollers); not needed with improved firmware|

![USBasp_SMD_pic1.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USBasp_SMD/USBasp_SMD_pic1.jpg)
![USBasp_SMD_pic2.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USBasp_SMD/USBasp_SMD_pic2.jpg)
![USBasp_SMD_pic3.jpg](https://raw.githubusercontent.com/wagiminator/AVR-Programmer/master/USBasp_SMD/USBasp_SMD_pic3.jpg)
