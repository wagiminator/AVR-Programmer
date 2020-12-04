# USBasp (ATmega8 SMD)
USBasp - USB ISP/TPI programmer stick for Atmel AVR microcontrollers with 5V and 3.3V target voltage based on the design by Thomas Fischl. More information can be found at https://www.fischl.de/usbasp/. Windows users may need to install a driver. Of course this is not necessary for linux and mac users. Close SELFPROG and TGTPWR jumper when uploading firmware. If you are burning a fresh microcontroller, close the Slow Serial Clock jumper (SLOWCLK) since they are factory programmed for internal 1MHz oscillator. This is not necessary if the improved firmware by nerdralph (https://github.com/nerdralph/usbasp) is used. Close Taget Power jumper (TGTPWR) to power target device from the USBasp.

|Jumper|Function|
|:-|:-|
|TGTVCC|Close to power target device via ISP header|
|SELFPRG|Close to upload firmware to USBasp itself via ISP header|
|SLOWCLK|Close to upload firmware to target device with slow speed (necessary for factory-new microcontrollers); not needed with improved firmware|

![IMG_20200807_142132_x.jpg](https://image.easyeda.com/pullimage/d0ASArBzXZ8V1ynPEoLOtxn6s97VlMvqZNAw8k3H.jpeg)
![IMG_20200807_142211_x.jpg](https://image.easyeda.com/pullimage/ZjNAFKYcdE5Z6Ej2qIZHsuxyc8uMiJBT51fkMTWi.jpeg)
![IMG_20200816_174830_x.jpg](https://image.easyeda.com/pullimage/pyJKmLEWedlSWbKNEYuSy8WLDzoAjaKMpxDlgVo6.jpeg)
![IMG_20200816_174955_x.jpg](https://image.easyeda.com/pullimage/KW5XFAbr3ytDBeJn8dDKWLITnnYtqpTK6Ppw7nry.jpeg)
