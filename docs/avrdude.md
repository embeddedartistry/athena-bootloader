# Manual Programming with AVRDUDE

The [Makefile](avr/bootloaders/athena/src/Makefile) for this project enables you to manually flash the bootloader to your device using `avrdude`. All build targets will be flashed using the same target name with the `_isp` prefix.

You will need to specify an `ISPTOOL` and `ISPPORT` value when you invoke the command. These values should correspond to the adapter you are using on your system.

```
$ cd avr/bootloaders/athena/src
$ make debug328_w5500_isp ISPTOOL=stk500 ISPPORT=/dev/cu.usbserial-14110
```

Here are the supported build targets. Append `_isp` to invoke `avrdude` on the target.

* Standard Bootloaders
    - atmega1284_w5100
    - atmega1284_w5200
    - atmega1284_w5500
    - atmega2560_w5100
    - atmega2560_w5200
    - atmega2560_w5500
    - atmega328_ethernet
    - atmega328_w5100
    - atmega328_w5200
    - atmega328_w5500
* Debug bootloaders
    - debug1284_w5100
    - debug1284_w5200
    - debug1284_w5500
    - debug2560_w5100
    - debug2560_w5200
    - debug2560_w5500
    - debug328_w5100
    - debug328_w5200
    - debug328_w5500

## Analyzing Arduino IDE Output

If you're not sure about the proper settings to supply, check the Arduino debug output when burning a bootloader when the IDE is properly configured for your tool. You will be able to find the `ISPTOOL` value by looking for the `-c` flag (`-cstk500` below). The `ISPPORT` can be identified by finding the `-P` flag. (Note that the port will likely change every time you cycle the USB connection of the programmer. It is almost always listed as a USB serial device).

```
/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude -C/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf -v -patmega328p -cstk500 -P/dev/cu.usbserial-14110 -Uflash:w:/Users/pjohnston/Documents/Arduino/hardware/athena/avr/bootloaders/athena/athena_debug328_w5500.hex:i -Ulock:w:0x0F:m 
```


### Full Output

Here is full output for a successful flashing run using an STK500 adapter and programming the Arduino Uno bootloader for the W5500 board.

```
/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude -C/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf -v -patmega328p -cstk500 -P/dev/cu.usbserial-14110 -Uflash:w:/Users/pjohnston/Documents/Arduino/hardware/athena/avr/bootloaders/athena/athena_debug328_w5500.hex:i -Ulock:w:0x0F:m 

avrdude: Version 6.3-20190619
         Copyright (c) 2000-2005 Brian Dean, http://www.bdmicro.com/
         Copyright (c) 2007-2014 Joerg Wunsch

         System wide configuration file is "/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf"
Writing |  ***failed;  
################################################## | 100% 0.04s

avrdude: 1 bytes of efuse written
avrdude: verifying efuse memory against 0x05:
avrdude: load data efuse data from input file 0x05:
avrdude: input file 0x05 contains 1 bytes
avrdude: reading on-chip efuse data:

Reading | ################################################## | 100% 0.00s

avrdude: verifying ...
avrdude: WARNING: invalid value for unused bits in fuse "efuse", should be set to 1 according to datasheet
This behaviour is deprecated and will result in an error in future version
You probably want to use 0xfd instead of 0x05 (double check with your datasheet first).
avrdude: 1 bytes of efuse verified
avrdude: reading input file "0xD8"
avrdude: writing hfuse (1 bytes):

Writing | ################################################## | 100% 0.00s

avrdude: 1 bytes of hfuse written
avrdude: verifying hfuse memory against 0xD8:
avrdude: load data hfuse data from input file 0xD8:
avrdude: input file 0xD8 contains 1 bytes
avrdude: reading on-chip hfuse data:

Reading | ################################################## | 100% 0.00s

avrdude: verifying ...
avrdude: 1 bytes of hfuse verified
avrdude: reading input file "0xFF"
avrdude: writing lfuse (1 bytes):

Writing | ################################################## | 100% 0.00s

avrdude: 1 bytes of lfuse written
avrdude: verifying lfuse memory against 0xFF:
avrdude: load data lfuse data from input file 0xFF:
avrdude: input file 0xFF contains 1 bytes
avrdude: reading on-chip lfuse data:

Reading | ################################################## | 100% 0.00s

avrdude: verifying ...
avrdude: 1 bytes of lfuse verified

avrdude done.  Thank you.

         User configuration file is "/Users/pjohnston/.avrduderc"
         User configuration file does not exist or is not a regular file, skipping

         Using Port                    : /dev/cu.usbserial-14110
         Using Programmer              : stk500
         AVR Part                      : ATmega328P
         Chip Erase delay              : 9000 us
         PAGEL                         : PD7
         BS2                           : PC2
         RESET disposition             : dedicated
         RETRY pulse                   : SCK
         serial program mode           : yes
         parallel program mode         : yes
         Timeout                       : 200
         StabDelay                     : 100
         CmdexeDelay                   : 25
         SyncLoops                     : 32
         ByteDelay                     : 0
         PollIndex                     : 3
         PollValue                     : 0x53
         Memory Detail                 :

                                  Block Poll               Page                       Polled
           Memory Type Mode Delay Size  Indx Paged  Size   Size #Pages MinW  MaxW   ReadBack
           ----------- ---- ----- ----- ---- ------ ------ ---- ------ ----- ----- ---------
           eeprom        65    20     4    0 no       1024    4      0  3600  3600 0xff 0xff
           flash         65     6   128    0 yes     32768  128    256  4500  4500 0xff 0xff
           lfuse          0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           hfuse          0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           efuse          0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           lock           0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           calibration    0     0     0    0 no          1    0      0     0     0 0x00 0x00
           signature      0     0     0    0 no          3    0      0     0     0 0x00 0x00

         Programmer Type : STK500V2
         Description     : Atmel STK500
         Programmer Model: STK500
         Hardware Version: 2
         Firmware Version Master : 2.10
         Topcard         : Unknown
         Vtarget         : 4.7 V
         SCK period      : 8.7 us
         Varef           : 0.0 V
         Oscillator      : 3.686 MHz

avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.01s

avrdude: Device signature = 0x1e950f (probably m328p)
avrdude: NOTE: "flash" memory has been specified, an erase cycle will be performed
         To disable this feature, specify the -D option.
avrdude: erasing chip
avrdude: reading input file "/Users/pjohnston/Documents/Arduino/hardware/athena/avr/bootloaders/athena/athena_debug328_w5500.hex"
avrdude: writing flash (32684 bytes):

Writing | ################################################## | 100% 0.00s

avrdude: 32684 bytes of flash written
avrdude: verifying flash memory against /Users/pjohnston/Documents/Arduino/hardware/athena/avr/bootloaders/athena/athena_debug328_w5500.hex:
avrdude: load data flash data from input file /Users/pjohnston/Documents/Arduino/hardware/athena/avr/bootloaders/athena/athena_debug328_w5500.hex:
avrdude: input file /Users/pjohnston/Documents/Arduino/hardware/athena/avr/bootloaders/athena/athena_debug328_w5500.hex contains 32684 bytes
avrdude: reading on-chip flash data:

Reading | ################################################## | 100% 0.00s

avrdude: verifying ...
avrdude: 32684 bytes of flash verified
avrdude: reading input file "0x0F"
avrdude: writing lock (1 bytes):

Writing | ################################################## | 100% 0.01s

avrdude: 1 bytes of lock written
avrdude: verifying lock memory against 0x0F:
avrdude: load data lock data from input file 0x0F:
avrdude: input file 0x0F contains 1 bytes
avrdude: reading on-chip lock data:

Reading | ################################################## | 100% 0.00s

avrdude: verifying ...
avrdude: 1 bytes of lock verified

avrdude done.  Thank you.
```
