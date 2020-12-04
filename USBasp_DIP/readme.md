# USBasp (ATmega8 DIP) 
USBasp - USB ISP/TPI programmer for Atmel AVR microcontrollers based on the design by Thomas Fischl. More information can be found at https://www.fischl.de/usbasp/. Windows users may need to install a driver. Of course this is not necessary for linux and mac users. Close SELFPROG and TGTPWR jumper when uploading firmware. If you are burning a fresh microcontroller, close the Slow Serial Clock jumper (SLOWCLK) since they are factory programmed for internal 1MHz oscillator. This is not necessary if the improved firmware by nerdralph (https://github.com/nerdralph/usbasp) is used. Close Taget Power jumper (TGTPWR) to power target device from the USBasp.

![IMG_20200111_160216_x.jpg](https://image.easyeda.com/pullimage/7mTvpuRMFyOUBHyYD23iyDAu5gH15Dn6gT4HioRq.jpeg)
![IMG_20200802_171824_x.jpg](https://image.easyeda.com/pullimage/366NwOCkSOOd821Kcmq0IatIjqJkbxV2InShmAvD.jpeg)
