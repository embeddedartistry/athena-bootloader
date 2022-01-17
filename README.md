# Athena Bootloader for Arduino

**PSA: DO NOT DOWNLOAD THIS PROJECT AS A ZIP ARCHIVE USING THE GIT INTERFACE. USE THE [LATEST RELEASE](https://github.com/embeddedartistry/athena-bootloader/releases/latest) PAGE**

Athena is an Ethernet- and Serial-capable bootloader for Arduino. Using an Arduino Ethernet board or Wiznet W5x00 shield, you can upload programs to your Arduino over Ethernet. Athena starts a TFTP server on the Arduino board, and files can be transferred using any TFTP client.

Embedded Artistry has forked this bootloader version from the [original project](https://github.com/loathingKernel/ariadne-bootloader), which seems to be no longer actively maintained. We are making many improvements and enhancements to the previous implementation.

Athena is implemented for AVR chips. ARM chips cannot use this bootloader.

Bootloader binary files for different versions can be found on the [Releases](https://github.com/embeddedartistry/athena-bootloader/releases) page.

**Table of Contents:**

1. [Getting the Bootloader](#getting-the-bootloader)
	1. [git-lfs](#git-lfs)
1. [Dependencies](#dependencies)
	1. [avr-gcc](#avr-gcc)
	1. [avrdude](#avrdude)
1. [Supported Boards](#supported-boards)
	1. [Notes on ATMega32U4 Support](#notes-on-atmega32u4-support)
	1. [Notes on WIZnet Shields](#notes-on-wiznet-shields)
	1. [Other Tested Boards](#other-tested-boards)
1. [Repository Structure](#repository-structure)
1. [Quickstart Checklist](#quickstart-checklist)
2. [Related Documents](#related-documents)
1. [Downloading the Bootloader](#downloading-the-bootloader)
1. [Installing the Bootloader Project](#installing-the-bootloader-project)
	1. [Arduino SDK Installation](#arduino-sdk-installation)
	1. [Atmel Studio Installation](#atmel-studio-installation)
1. [Flashing the Bootloader](#flashing-the-bootloader)
	1. [Programming Hardware Requirements](#programming-hardware-requirements)
	2. [Connecting Pins](#connecting-pins)
	3. [Programming with Arduino IDE](#programming-with-arduino-ide)
	4. [Programming with Atmel Studio](#programming-with-atmel-studio)
	4. [Programming with AVRDUDE](#programming-with-avrdude)
1. [Flashing Applications via Serial](#flashing-applications-via-serial)
1. [Flashing Applications via TFTP](#flashing-applications-via-tftp)
	1. [Converting Your Sketch to the Right Format](#converting-your-sketch-to-the-right-format)
	1. [Using a TFTP Client to Upload the Sketch](#using-a-tftp-client-to-upload-the-sketch)
		1. [Windows Upload](#windows-upload)
		1. [Linux and OS X Upload](#linux-and-os-x-upload)
			1. [Linux TFTP Note](#linux-tftp-note)
1. [Enabling Remote Reset and Reprogram Capabilities](#enabling-remote-reset-and-reprogram-capabilities)
	1. [Reprogramming Without The Reset Server Library](#reprogramming-without-the-reset-server-library)
1. [Changing the Ethernet CS Pin](#changing-the-ethernet-cs-pin)
2. [Enabling Ethernet Reset Behavior](#enabling-ethernet-reset-behavior)
1. [Troubleshooting](#troubleshooting)
1. [EEPROM Requirements](#eeprom-requirements)
1. [Test Binaries](#test-binaries)
1. [Supporting Libraries](#supporting-libraries)
1. [Useful References](#useful-references)
1. [Acknowledgments](#acknowledgments)
	1. [Contributors](#contributors)
	2. [Donors](#donors)
1. [License](#license)

## Getting the Bootloader

To download this repository, we recommend cloning with the `--recursive` flag to ensure submodules (e.g., [supporting libraries](#supporting-libraries)) are automatically downloaded.

```
$ git clone https://github.com/embeddedartistry/athena-bootloader.git --recursive
```

You can also download submodules after cloning with:

```
$ git submodule update --init --recursive
```

You can also [access .zip archives from latest release page](https://github.com/embeddedartistry/athena-bootloader/releases/latest). Download the latest ProjectFile .zip archive for the latest release, which contains the full source tree (including submodules) and populated binary files. This is what you should use for populating the Arduino hardware folder.

### git-lfs

This project requires you to use [git-lfs](https://git-lfs.github.com). Images, pre-built bootloader binaries, and other files are stored within git-lfs. If you have git-lfs installed, these files will downloaded correctly when cloning the repository.

If you do not have this installed, please visit [the git-lfs web page](https://git-lfs.github.com) for installation instructions.

If you cloned this repository before installing git-lfs, please run `git lfs pull`. If you have git-lfs installed before cloning the repository, git will automatically perform a `git lfs pull` for you.

When you don't have git-lfs installed, you will see all of the files in the repository. However, files stored with git-lfs will show up as small placeholders. Trying to use these files will result in errors.

### Don't Want to use git-lfs?

If you don't want to use git-lfs, you have the following options:

1. Manually build the bootloader binaries on your machine by running `make` at the top-level of the repository.
1. [Download the latest .zip archive of the bootloader binaries for the latest release](https://github.com/embeddedartistry/athena-bootloader/releases/latest), and copy them to `avr/bootloaders/athena/` folder so they will be properly found by the Arduino IDE. This will replace the placeholders with complete files.
2. [Download the latest ProjectFile .zip archive for the latest release](https://github.com/embeddedartistry/athena-bootloader/releases/latest), which contains the full source tree (including submodules) and populated binary files.

## Dependencies

This project requires Arduino SDK 1.15 or later due to the project's source layout.

The following dependencies are required to compile and use this bootloader:

* [avr-gcc](#avr-gcc)
* [avrdude](#avrdude)

### avr-gcc

If you have the Arduino IDE or Atmel Studio installed on your system, you already have avr-gcc.

For manual installation on Linux/WSL:

```
$ sudo apt-get install gcc-avr binutils-avr avr-libc gdb-avr
```

For manual installation on OS X using [Homebrew](https://brew.sh):

```
brew tap osx-cross/avr
brew install avr-libc
brew install avrdude --with-usb
```

For manual download of the command line tools, see [the compiler page](https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers).

### avrdude

If you have the Arduino IDE installed on your system, you already have avr-gcc.

For manual installation on Linux/WSL:

```
sudo apt-get install avrdude
```

For manual installation on OS X using [Homebrew](https://brew.sh):

```
brew install avrdude --with-usb
```

Files can be [manually downloaded from the AVRDUDE project here](http://savannah.nongnu.org/projects/avrdude).

For information on integrating avrdude with Atmel Studio, see these links:

* [Integrate avrdude with Atmel Studio](https://www.asensar.com/guide/arduino_atemlstudio/integrate-avrdude-with-atmel-studio.html)
* [Arduino to Atmel Studio 7 external programmers, ISP and serial programming](https://www.avrfreaks.net/forum/arduino-atmel-studio-7-external-programmers-isp-and-serial-programming)
* [How to Load Programs to an Arduino UNO from Atmel Studio 7](https://medium.com/jungletronics/how-to-load-programs-to-an-arduino-uno-from-atmel-studio-7-83c8dd8d175d)

## Supported Boards

Officially supported MCUs:

* __ATmega328__
* __ATmega2560__
* __ATmega1284__
* __ATmega32U4__ ([see notes](#notes-on-atmega32u4-support))

Officially supported Ethernet controllers ([see notes](#notes-on-wiznet-shields)):

* __WizNet W5100__
* __WizNet W5200__
* __WizNet W5500__

Officially supported Arduino Boards:

* [Arduino Ethernet][17]
* [Arduino Uno][18]
* [Arduino Duemilanove w/ ATmega328][19]
* [Arduino Mega 2560][20]

### Notes on ATmega32U4 Support

The ATmega32U4 is supported, but the chip cannot support uploads over USB serial. It should be possible to upload sketches over the serial port (non-USB), but this has not been tested. The Caterina bootloader requires the majority of bootloader space to implement USB support. It is unclear whether both USB and Ethernet support can be implemented for this chip.

### Notes on WIZnet Shields

On WIZnet shields pins 11,12, and 13 are **tied up** (by default) with the corresponding pins on the ICSP header.  For more information on handling these pins, see [this document](docs/spi_on_wiznet_ethernet_shields.md).

### Other Tested Boards

These boards have been reported to work with the Athena bootloader:

* [ITead Studio IBoard][21]
* [Sparkfun Ethernet Pro][22]
* [XBoard Relay][28]

If you think another board can be added to this list, please [contact us](mailto:contact@embeddedartistry.com).

## Repository Structure

This repository is structured to follow the [latest Arduino IDE standard for 3rd-party hardware](https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5-3rd-party-Hardware-specification). Here is an overview of the structure:

* [`avr/`](avr): This is where the bootloader code, pre-built binaries, and helper libraries live
	- [`bootloaders/athena/`](avr/bootloaders/athena) contains the pre-built bootloader `.hex` files, as well as [source code for the bootloader](avr/bootloaders/athena/src)
	- [`libraries/`](avr/libraries) contains helper libraries that are used to interact with the bootloader from an Arduino application
	- [`variants/`](avr/variants) contains additional board definitions
* [`docs/`](docs) contains additional reference documentation, images, and GitHub templates
* [`extras/`](extras) contains miscellaneous useful utilities

## Quickstart Checklist

This is the abbreviated checklist for installing and using the bootloader:

1. [Download the bootloader](#download-the-bootloader) and [install it to the proper location](#arduino-sdk-installation)
	1. Just need the bootloader binaries? Check the [releases page](https://github.com/embeddedartistry/athena-bootloader/releases).
1. [Flash the bootloader to the device](flashing-the-bootloader)
2. **DO NOT SKIP:** Program network settings for the device using the [`AthenaEEPROM`](#supporting-libraries) library or [example sketches](https://github.com/embeddedartistry/AthenaEEPROM/tree/master/examples)
	1. More information available in [docs/network_configuration.md](docs/network_configuration.md)
3. Include the [`AthenaEthernetReset`](#supporting-libraries) library in your application (or use the [`ResetServer` example sketch](https://github.com/embeddedartistry/AthenaEthernetReset/blob/master/examples/AthenaResetServer/AthenaResetServer.ino)
	1. Create a reset server and specify a port
	2. Call `.begin()` on the server object in `setup()`
	3. Call `.check()` on the server object in `loop()`
4. When your application is running, trigger a reprogramming mode using curl or a web browser
	1. URL Format: `http://{ip}:{port}/{reset_path}/reprogram`
		1. Example using default settings: `curl 192.168.1.128:8080/athena/reprogram`
	2. The device will respond with:
		1. "Arduino will reset for reprogramming in 2 seconds"
	3. You must use the "password" programmed via the AthenaEEPROM library to successfully enter the programming mode. The default password is `athena`
5. Follow the [TFTP Upload](#flashing-applications-via-tftp) instructions to send a new binary to the device

Not able to connect to the device? Check out the [debugging guide](docs/Debugging.md).

Need to use a custom board file for compiling and uploading sketches? Once you've flashed the bootloader, you can do that safely. You will need to install the [AthenaEthernetReset](https://github.com/embeddedartistry/AthenaEthernetReset) and [AthenaEEPROM](https://github.com/embeddedartistry/AthenaEEPROM) libraries separately, however, because the IDE only picks them up in the bootloader folder if you've selected an Ariadne configuration.

## Related Documents

* [Compiling Instructions](docs/compiling.md) for anyone looking to modify the bootloader
* [AVRDUDE Instructions](docs/avrdude.md) for manually flashing outside of the Arduino IDE or Atmel Studio using AVRDUDE
* [Debugging Guide](docs/Debugging.md) provides pointers for debugging failures to connect to the device
* [Network Configuration](docs/network_configuration.md) provides pointers on configuring the network settings for your device
* [Converting Binaries for TFTP Upload](docs/converting_binaries.md) provides more detailed instructions on the conversion process, with example locations for each OS when using the Arduino IDE

## Downloading the Bootloader

You can clone the repository from GitHub:

```
$ git clone https://github.com/embeddedartistry/athena-bootloader --recursive
```

If you have configured your GitHub account with SSH keys, you can also use the SSH checkout:

```
$ git clone git@github.com:embeddedartistry/athena-bootloader.git --recursive
```

Note that you must use the `--recursive` flag because the project contains submodules. If you cloned without the `--recursive` flag, perform:

```
$ git submodule update --init
```

You can also download a zip archive of the repository from [the GitHub repository page](https://github.com/embeddedartistry/athena-bootloader).

## Installing the Bootloader Project

1. [Arduino SDK Installation](#arduino-sdk-installation)
1. [Atmel Studio Installation](#atmel-studio-installation)

### Arduino SDK Installation

In order for the Arduino IDE to access the information, you need to place the `athena-bootloader` repository within the `hardware/` folder of your "Sketchbook Location". You can find this location by viewing the Arduino IDE settings. The default location is often the `Arduino/` folder in your home directory or in `Documents/`.

For our system, which is OS X, the Sketchbook location is:

```
/Users/embeddedartistry/Documents/Arduino
```

On Windows, it should be in `My Documents\Arduino\`.

Our Sketchbook folder does not have a `hardware` directory by default. We created the folder ourselves:

```
$ mkdir -p /Users/embeddedartistry/Documents/Arduino/hardware
```

You can clone the repository within that directory, or you can use a symbolic link within the `hardware/` folder:

```
$ cd /Users/embeddedartistry/Documents/Arduino/hardware
$ ln -s ~/src/athena-bootloader/ ./athena
```

Once the `athena-bootloader` folder is placed in the `hardware/` folder, Arduino will detect it. Restart the IDE if it is already open. You should now see the Athena bootloader show up in the board list.

![Image showing the Athena bootloader in boards](docs/boards.png "Athena Listed in Boards")

### Atmel Studio Installation

If you are using Atmel Studio, our recommendation is to use the Visual Micro extension. When you are using the Visual Micro add-on, the standard installation instructions will apply. Then, use the Visual Micro board manager to select the proper Athena board definition for your target. 

For more information, see this link:

* [Visual Micro: The Board Manager](https://www.visualmicro.com/page/User-Guide.aspx?doc=Board-Manager.html)

## Flashing the Bootloader

1. [Programming Hardware Requirements](#programming-hardware-requirements)
2. [Connecting Pins](#connecting-pins)
3. [Programming with Arduino IDE](#programming-with-arduino-ide)
4. [Programming with AVRDUDE](#programming-with-avrdude)

### Programming Hardware Requirements

To flash the bootloader to a device, you will need an in-system programmer (ISP). You can purchase an [AVR-ISP][2] or [USBtinyISP][3], which work for most Arduino devices. 

Some devices do not work reliably with the ATMega2560 due to page size requirements. For the ATMega2560, you will need a ISP such as [Atmel's AVRISP mkII][6], [Atmel's AVRISP STK500 USB ISP Programmer](http://wiki.seeedstudio.com/Atmel_AVRISP_STK500_USB_ISP_Programmer/), [Olimex's AVR-ISP-MK2][7], or any similarly-capable programmer.

If you have a spare Arduino, you can build a [ParallelProgrammer][4] or an [ArduinoISP][5].  Nick Gammon has published a [guide][8] for using one Arduino to program another, and his method is reported to work with the ATMega2560.

### Connecting Pins

The programmers should be connected to the `ICSP` pins. On Arduino boards, the ISCP pins are a 2x3 header located near the processor. The board must be powered by an external power supply, by the programmer, or by the USB port. If you are using an _ArduinoISP_, consult [this link][5] for additional instructions.

![Image showing the ISP connected to the board](docs/flashing.png "ISP Connection Image")

### Programming With Arduino IDE

After you have connected the Arduino board and the programmer to your computer launch the Arduino IDE.

First, navigate to the __Tools__ > __Board__ menu and select your target board from the `Athena Bootloader` list (at the bottom in our view).

![Image showing the Athena bootloader in boards](docs/boards.png "Athena Listed in Boards")

Next, navigate to the __Tools__ > __Version__ menu and select Wiznet chip from the list, as well as "Standard" or "Debug" mode. Debug binaries are larger, but contain helpful Serial output for seeing what the bootloader is doing. The *only* difference between Standard and Debug mode is the serial output.

![Image showing the Athena bootloader versions](docs/version.png "Athena Bootloader Versions")

By default, bootloader updates will erase the contents of the EEPROM. Because the Athena EEPROM layout is different from the default layout, this approach is advisable. If you're simply upgrading Athena bootloader versions, you probably want to keep your previously configured device to a new version.

Navigate to the __Tools__ > __Erase EEPROM?__ menu and select `Save` to prevent the EEPROM from being erased.

![Image showing the Erase/Save EEPROM menu](docs/erase_eeprom.png "Erase Menu")

> **Note:** If you are manually flashing the application or need to specify fuse settings, use a high_fuse setting of `0xD8` to erase, and `0xD0` to preserve.

Next, go to the __Tools__ > __Programmer__ menu and select the programmer you are using.

![Image showing the tools menu](docs/tools.png "Programming Tools Menu")

In case you are using _ArduinoISP_, make sure that the selected port in the __Tools__ > __Serial Port__ menu refers to the _ArduinoISP_. You should not select the board that you want to burn the bootloader on. This requirement applies to other serial-based programmers, such as the AVR-ISP 2.0 ("Atmel STK500 Development Board").

To flash the bootloader, select the __Tools__ > __Burn Bootloader__ menu option.

![Image showing the Burn Bootloader menu option](docs/burn.png "Burn Bootloader Menu Option")

Check the Arduino console output to confirm that the bootloader was flashed successfully.

### Programming with Atmel Studio

If you have configured the [Visual Micro board manager](#atmel-studio-implementation), you can configure your programmer and burn in the bootloader in the [visual micro menu](https://www.visualmicro.com/page/User-Guide.aspx?doc=Visual-Micro-Menu.html) similarly to the Arduino IDE.

You can also [manually flash the bootloader in Atmel Studio](https://wspublishing.net/avr-c/restoring-arduino-mega-bootloader/). For this, you will need to manually supply fuse settings for your configuration. These settings can be found in [boards.txt](avr/boards.txt).

### Programming with AVRDUDE

Instructions on manually programming the bootloader with AVRDUDE are described in [docs/avrdude.md](docs/avrdude.md)

## Flashing Applications via Serial

Athena is built off of the standard Arduino AVR bootloaders. Serial upload capabilities are preserved. Serial uploads work even when an Ethernet shield is not installed.

To flash an application over USB/Serial:

1. Plug in the USB cable
2. Select the serial port for the device and the appropriate board from the __Tools__ > __Board__ menu. 
3. Program the device

If your device has auto-reset capabilities, such that a Serial connection resets the device, programming will work with a sketch is running.

If your device does not have auto-reset capabilities, then you will need to press the reset button on the device to enter the bootloader mode. In this case, if there is a __valid__ program already flashed on the Arduino, you have to reprogram the device in the next __5 seconds__. If you don't, the bootloader will initiate the program that is already flashed onto the Arduino. In case there is no program flashed or the program has been marked as __invalid__, the bootloader will never time out and you can reprogram it at any time.

> **Note:** For boards other than the Arduino Mega, the primary LED will start blinking rapidly when the bootloader is running.

After a successful flashing in either case, the bootloader will automatically start the application.

## Flashing Applications via TFTP

Flashing applications via TFTP requires multiple moving parts. This README references the default network settings and assumes that your network is configured so these settings will work.

```
* IP Address:  192.168.1.128
* Subnet Mask: 255.255.255.0
* Gateway:     192.168.1.1
* MAC Address: 0xDE.0xAD.0xBE.0xEF.0xFE.0xED

* TFTP Negotiation Port: 69
* TFTP Data Port: 46969
```

For more information on network configuration, see [docs/network_configuration.md](docs/network_configuration.md)

**If the default values will not work, you MUST configure the network settings for the TFTP server in the bootloader and the AthenaEthernetReset server to work correctly with your network configuration.**

The process flow is:

1. Reset into TFTP mode in the bootloader
2. Connect to the device
2. Upload a binary image
3. After upload completes, the new application auto-boots

### Converting Your Sketch to the Right Format

While Arduino serial flashing uses __HEX__ files, the bootloader TFTP server only works with binary files. You must manually convert your programs the `.bin` file format using `avr-objcopy`.

For detailed instructions, see [docs/converting_binaries.md](docs/converting_binaries.md). Only brief notes are shown here.

The first step is to find the `.hex` file you want to flash to your device. If you're using the Arduino IDE, look for a line in the build output referencing a `.hex` file. It is placed in a temporary folder, such as this:

```
/var/folders/0h/_66m4dqj3p1b3j1xqg10r08w0000gn/T/arduino_build_678002/WebServer.ino.hex 
```

Copy that path and use it in with the following command structure:

Windows:

```
"C:\Program Files\Arduino\hardware\tools\avr\bin\avr-objcopy.exe" -I ihex "C:\path\to\your\program.hex" -O binary program.bin
```

Linux, WSL, and OS X:

```
avr-objcopy -I ihex /path/to/sketch.hex -O binary sketch.bin
```

### Using a TFTP Client to Upload the Sketch

To upload the binary over Ethernet, you will need a TFTP client. All three major OSes have tools that you can use through the command line.

No matter the OS, the following flow must be used:

1. Put the device into bootloader mode, either via the reset button or the Ethernet Reset Server library.
2. Connect to the device using TFTP and the configured port (69 by default)
3. Enable "octet mode" or "binary mode"
3. Send the binary file to the device using the `PUT` command

After the binary is uploaded, the bootloader will automatically run the application.

If the upload fails, wait for the upload process on the bootloader to timeout. That takes about __10 seconds__. For this period you should witness the indication led doing some random blinking. Note that the LED on an Arduino Mega will not blink due to contention with the SCK pin.

After the timeout the TFTP server should restart itself and wait for a new upload.

#### Windows Upload

Windows provides a `tftp` command that can be used:

```
Microsoft Windows XP [Version 5.1.2600]
(C) Copyright 1985-2001 Microsoft Corp.

C:\Documents and Settings\Administrator>tftp

Transfers files to and from a remote computer running the TFTP service.

TFTP [-i] host [GET | PUT] source [destination]

	-i              Specifies binary image transfer mode (also called
									octet). In binary image mode the file is moved literally, byte by byte. Use this mode when transferring binary files.
	host            Specifies the local or remote host.
	GET             Transfers the file destination on the remote host to
									the file source on the local host.
	PUT             Transfers the file source on the local host to
									the file destination on the remote host.
	source          Specifies the file to transfer.
	destination     Specifies where to transfer the file.
```

Here is an example upload command:

```
C:\Documents and Settings\Administrator>tftp -i 192.168.1.128 PUT sketch.bin
```

#### Linux and OS X Upload

Linux and OS X both have a `tftp` command:

```
$ tftp [ip] [port]
```

For the default bootloader, the command would be:

```
$ tftp 192.168.1.128 69
```

Or, because 69 is the default TFTP port, you could simply say:

```
$ tftp 192.168.1.128
```

You should be greeted by the TFTP prompt:

```
tftp>
```

Enable binary/octet mode to tell TFTP to send binary data:

```
tftp> mode octet
```

You can optionally enable verbose output settings:

```
tftp> trace
tftp> verbose
```

To send the sketch to the board, issue a `PUT` command:

```
tftp> put [sketch].bin
```

You should see your TFTP client sending packets. A correct output sample of the TFTP client uploading the *blink* sketch is below:

```
tftp> mode octet
tftp> trace
Trace mode on.
tftp> verbose
Verbose mode on.
tftp> put blink.bin
sent WRQ <file: blink.bin, mode: octet <>>
received ACK <block: 0>
sent DATA <block: 1, size: 512>
received ACK <block: 1>
sent DATA <block: 2, size: 512>
received ACK <block: 2>
sent DATA <block: 3, size: 512>
received ACK <block: 3>
sent DATA <block: 4, size: 512>
received ACK <block: 4>
sent DATA <block: 5, size: 42>
received ACK <block: 5>
tftp>
```

##### Linux TFTP Note

Some __Linux__ distributions, like __Fedora/RedHat__, require you to load the `ip_conntrack_tftp` module. If you don't, the TFTP client won't be able to acknowledge (__ACK__) the incoming packets. Other distributions, such as __Arch__, don't require this step. 

To load `ip_conntrack_tftp`, run the following:

```
$ sudo modprobe ip_conntrack_tftp
```

## Enabling Remote Reset and Reprogram Capabilities

The [AthenaEthernetReset](#supporting-libraries) library is a requirement for your program if you want to support remotely enabling binary uploads. [A README is provided](https://github.com/embeddedartistry/AthenaEthernetReset/blob/master/README.md) and there is an [example sketch showing the usage of the library](https://github.com/embeddedartistry/AthenaEthernetReset/blob/master/examples/AthenaResetServer/AthenaResetServer.ino).

The following steps must be used to implement a remote upload capability for your device:

1. Flash the bootloader to the device
2. Program network settings for the device using the [AthenaEEPROM](#supporting-libraries) library or examples
3. Include the AthenaEthernetReset library in your application
	1. Create a reset server and specify a port
	2. Call `.begin()` on the server object in `setup()`
	3. Call `.check()` on the server object in `loop()`
4. Trigger a reset using curl or a web browser
	1. URL Format: `http://{ip}:{port}/{password}/reprogram`
		1. Example: `curl 192.168.1.128:8080/athena/reprogram`
		2. Note that the `reset` endpoint does not provide a reprogramming opportunity.
	2. The device will respond with:
		1. "Arduino will reset for reprogramming in 2 seconds"
	3. You must use the "password" programmed via the AthenaEEPROM library to successfully enter the programming mode. The default value is `athena`.
5. Follow the [TFTP Upload](#flashing-applications-via-tftp) instructions to send a new binary to the device

### Reprogramming Without The Reset Server Library

If you don't want to use the EthernetReset server (because you have your own web server), you can trigger update mode manually with the following function calls:

```
EEPROM.enableUpdateMode(); // in AthenaEEPROM library
watchdogReset(); // from avr/wdt.h
```

After the device resets, it will wait for a firmware update over TFTP or Serial. This mode will not time out. If the device is manually reset, it will boot once more into the image currently installed on the board.

## Changing the Ethernet CS Pin

The bootloader is programmed to use the default CS pin to talk to the Ethernet controller. If your board is using a custom pin, you can change the pin that the bootloader will use by programming the EEPROM:

```
EEPROM.setEthernetCSPin(53); // Uses Arduino pin number
```

## Enabling Ethernet Reset Behavior

By default, the bootloader does not know about a reset pin. If your hardware requires the bootloader to perform a reset during system boot, you can specify a reset pin using the EEPROM:

```
EEPROM.setEthernetResetPin(53); // Uses Arduino pin number
```

## Troubleshooting

If you're having problems connecting to the device, check out [docs/Debugging.md](docs/Debugging.md) for troubleshooting tips.

If you see an error like the one below, you only have placeholder git-lfs files. Please see the [git-lfs](#git-lfs) section above for instructions.

```
avrdude: ERROR: No valid record found in Intel Hex file "C:\Users\XXX\Documents\Arduino\hardware\athena-bootloader-11.1.8\avr/bootloaders/athena/athena_debug2560_w5500.hex"
avrdude: read from file 'C:\Users\XXX\Documents\Arduino\hardware\athena-bootloader-11.1.8\avr/bootloaders/athena/athena_debug2560_w5500.hex' failed
Error while burning bootloader.
```

If you see an error like the one below, your submodules are not populated. Please see the instructions on [Getting the Bootloader](#getting-the-bootloader).

```
Invalid library found in C:\Users\XXX\Documents\Arduino\hardware\athena-bootloader-11.1.8\avr\libraries\AthenaEEPROM: no headers files (.h) found in C:\Users\XXX\Documents\Arduino\hardware\athena-bootloader-11.1.8\avr\libraries\AthenaEEPROM
Invalid library found in C:\Users\XXX\Documents\Arduino\hardware\athena-bootloader-11.1.8\avr\libraries\AthenaEthernetReset: no headers files (.h) found in C:\Users\XXX\Documents\Arduino\hardware\athena-bootloader-11.1.8\avr\libraries\AthenaEthernetReset
Invalid library found in C:\Users\XXX\Documents\Arduino\libraries\Archive: no headers files (.h) found in C:\Users\XXX\Documents\Arduino\libraries\Archive
```

## EEPROM Requirements

Athena uses the Arduino EEPROM to store values needed by the bootloader and AthenaEthernetReset server. If you are using the default EEPROM library for our own purposes, please make sure to start writing after `NETEEPROM_END`, available in `AthenaEEPROM_defs.h`:

```
#define NETEEPROM_END      63
```

For more information, see the [AthenaEEPROM library](#supporting-libraries).

If you use the [AthenaEEPROM](#supporting-libraries) library included with the Athena bootloader, then offsets will be handled for you automatically and you will not need to adjust your code. Note, however, that the full EEPROM space is not available.

## Test Binaries

For testing purposes, you can find a __blink__ sketch in binary form inside the [`extras/tests/blink`](extras/tests/blink) folder. 

The __fade__ sketch in the [`extras/tests/fade`](extras/tests/fade) folder will also give you a view of what a failed upload looks like. This sketch fails because it is written in plain __C__ and not in __Arduino__. That way it lacks some "signatures" the bootloader uses to validate _Arduino_ sketches. 

## Supporting Libraries

Supporting libraries are included with the bootloader. These libraries are meant to facilitate interacting with the Athena bootloader from Arduino applications.

These libraries are packaged with the bootloader and will automatically detected by the Arduino IDE when an Athena board configuration is used. If you are using a custom board configuration, you may also want to install these libraries separately so that they are available even when an Athena board configuration is not selected.

* [AthenaEEPROM](https://github.com/embeddedartistry/AthenaEEPROM) (found at `avr/libraries/AthenaEEPROM`) is a patched EEPROM library that protect the memory space used by the Athena bootloader. This library also enables you to read/write the bootloader network settings.
	- Use this library in place of the default EEPROM library to prevent settings from being overwritten
* [AthenaEthernetReset](https://github.com/embeddedartistry/AthenaEthernetReset) (found at `avr/libraries/AthenaEthernetReset`) is a library that can be used to create an HTTP server in an application that enables you to:
	- Remotely restart the Arduino application
	- Restart into the bootloader in programming mode, which waits for the a binary over TFTP

The [AthenaEthernetReset](https://github.com/embeddedartistry/AthenaEthernetReset/) library is a requirement for your program if you want to support remotely enabling binary uploads. [A README is provided](https://github.com/embeddedartistry/AthenaEthernetReset/blob/master/README.md) and there is an [example sketch showing the usage of the library](https://github.com/embeddedartistry/AthenaEthernetReset/blob/master/examples/AthenaResetServer/AthenaResetServer.ino).

## Useful References

* [AVR Beginners](http://avrbeginners.net) is an excellent resource for anyone working on this bootloader who is new to the AVR architecture

## Acknowledgments

The Embedded Artistry version of the Athena bootloader is derived from [loathingkernel/ariadne-bootloader](https://github.com/loathingKernel/ariadne-bootloader). Credit goes to its maintainer, [Stelios Tsampas](https://github.com/loathingKernel).

The Ariadne bootloader was built upon these open source projects:

* [TFTP-Bootloader][23] (The original base for this project)
* [Optiboot][24] (Bootloader for the __ATmega328__)
* [Arduino-stk500v2-bootloader][25] (Bootloader for the __ATmega2560__)

### Contributors

* [AkhileshThorat](https://github.com/AkhileshThorat)

The following contributors were listed in the [original project](https://github.com/loathingKernel/ariadne-bootloader) README:

* [mharizanov][26] (Commenting some of the initial *Arduino* code, enabling Stelios to take on the project)
* [follower][27] (Sketches served as a starting point for the included __AthenaEEPROM__ and __AthenaEthernetReset__ libraries)
* [Arjen Hiemstra](https://github.com/arjenhiemstra) (Provided support for W5200 and W5500)
* [per1234](https://github.com/per1234) (Testing and tech support in Arduino Forums)

### Donors 

The following donors were listed in the [original project](https://github.com/loathingKernel/ariadne-bootloader) README:

* Hachi Manzur (AVRISP mkII programmer, testing)

## License

Athena is released under the [GPLv2, GNU General Public License][99].

[2]: http://www.atmel.com/dyn/products/tools_card.asp?tool_id=2726
[3]: http://www.ladyada.net/make/usbtinyisp/
[4]: http://arduino.cc/en/Hacking/ParallelProgrammer
[5]: http://arduino.cc/en/Tutorial/ArduinoISP
[6]: http://www.atmel.com/tools/AVRISPMKII.aspx
[7]: https://www.olimex.com/Products/AVR/Programmers/AVR-ISP-MK2/
[8]: http://www.gammon.com.au/forum/?id=11635
[9]: https://github.com/codebendercc/Athena-Bootloader#configuring-your-router-for-remote-flashing
[10]: http://youtu.be/KCHqhV6xPMg
[11]: http://en.wikipedia.org/wiki/Port_forwarding
[12]: http://portforward.com/english/routers/port_forwarding/
[13]: http://codebender.cc
[17]: http://www.arduino.cc/en/Main/ArduinoBoardEthernet
[18]: http://www.arduino.cc/en/Main/ArduinoBoardUno
[19]: http://www.arduino.cc/en/Main/ArduinoBoardDuemilanove
[20]: http://arduino.cc/en/Main/arduinoBoardMega2560
[21]: http://imall.iteadstudio.com/im120410001.html
[22]: https://www.sparkfun.com/products/10536
[23]: https://github.com/arduino/TFTP-Bootloader
[24]: https://code.google.com/p/optiboot/
[25]: https://github.com/msproul/Arduino-stk500v2-bootloader
[26]: https://github.com/mharizanov
[27]: https://github.com/follower
[28]: https://wiki.dfrobot.com/X-Board_Relay_SKU_DFR0222_
[99]: https://www.gnu.org/licenses/gpl-2.0.html
