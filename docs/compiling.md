# Compiling

## Compiling the Bootloader

Precompiled bootloader binaries are provided within this repository. The files are stored using [git-lfs](../README.md#dependencies). If you do not have git-lfs installed, these files will not be true binaries. Instead, they will be text files with a git-lfs hash stored inside. To download them properly, please see [the dependencies section](../README.md#dependencies).

To compile the bootloader, you will need to have [avr-gcc](../README.md#avr-gcc) installed on your computer. If you are using the Arduino IDE or Atmel Studio for AVR, the compiler is already installed.

The bootloader files can be re-compiled from source by navigating to the [bootloader source directory: `avr/bootloaders/athena/src`](../avr/bootloaders/athena/src).

You can build individual targets with `make`:

```
$ cd avr/bootloaders/athena/src
$ make debug328_w5500_isp ISPTOOL=stk500 ISPPORT=/dev/cu.usbserial-14110
```

Here are the supported build targets.

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

You can also run the `makerelease` script to build all variants:

```
$ cd avr/bootloaders/athena/src
$ ./makerelease
```

The script will place the bootloader variants in the [`avr/bootloaders/athena/`](../avr/bootloaders/athena/) directory. The files will be automatically picked up by the Arduino IDE in this location.

Targets can be individually built within the [`avr/bootloaders/athena/src`](../avr/bootloaders/athena/src) directory. A Makefile is provided in that directory. When using the Makefile, note that the `.hex` files which are produced stay in the `src/` directory. They are not automatically pushed up a level. You will need to manually move the file(s) to [`avr/bootloaders/athena/`](../avr/bootloaders/athena) for the Arduino IDE to pick up the new version.

## Compiling Applications

When compiling applications for a board using the Athena bootloader, you need to select the proper board configuration in the Arduino IDE. 

If you are using Atmel Studio, you also need to select the proper board configuration using the Visual Micro board manager. Use the board manager to select the proper Athena board definition for your target. For more information, see this link:

* [Visual Micro: The Board Manager](https://www.visualmicro.com/page/User-Guide.aspx?doc=Board-Manager.html)
