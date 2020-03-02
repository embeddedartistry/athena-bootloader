# Ariadne Bootloader for Arduino

Ariadne is an Ethernet- and Serial-capable bootloader for Arduino. Using an Arduino Ethernet board or Wiznet W5x00 shield, you can upload programs to your Arduino over Ethernet. Ariadne starts a TFTP server on the Arduino board, and files can be transferred using any TFTP client.

Ariadne is implemented for AVR chips. ARM chips cannot use this bootloader.

**Table of Contents:**

1. [Dependencies](#dependencies)
	1. [git-lfs](#git-lfs)
	2. [avr-gcc](#avr-gcc)
	3. [avrdude](#avrdude)
1. [Supported Boards](#supported-boards)
	1. [Notes on ATMega32U4 Support](notes-on-atmega32u4-support)
	2. [Other Tested Boards](#other-tested-boards)
1. [Repository Structure](#repository-structure)
1. [Quickstart Checklist](#quickstart-checklist)
1. [Downloading the Bootloader](#downloading-the-bootloader)
1. [Arduino SDK Installation](#arduino-sdk-installation)
1. [Atmel Studio Installation](#atmel-studio-installation)
1. [Compiling the Bootloader](#compiling-the-bootloader)
1. [Flashing the Bootloader](#flashing-the-bootloader)
	1. [Programming Hardware Requirements](#programming-hardware-requirements)
	2. [Connecting Pins](#connecting-pins)
	3. [Programming with Arduino IDE](#programming-with-arduino-ide)
	4. [Programming with AVRDUDE](#programming-with-avrdude)
1. [Compiling Applications](#compiling-applications)
1. [Flashing Applications via Serial](#flashing-applications-via-serial)
1. [Flashing Applications via TFTP](#flashing-applications-via-tftp)
	1. [Device Network Settings](#device-network-settings)
		2. [Configuring Network Settings After Flashing](#configuring-network-settings-after-flashing)
		1. [Overriding Default Settings](#overriding-default-settings)
	1. [Configuring Your Router](#configuring-your-router)
		1. [Supporting Multiple Arduino Devices Behind One Router](#supporting-multiple-arduino-devices-behind-one-router)
	1. [Converting Your Sketch to the Right Format](#converting-your-sketch-to-the-right-format)
		2. [Windows Conversion](#windows-conversion)
		3. [Linux and OS X Conversion](#linux-and-wsl-conversion)
		4. [OS X Conversion](#os-x-conversion)
	1. [Using a TFTP Client to Upload the Sketch](#using-a-tftp-client-to-upload-the-sketch)
		1. [Windows Upload](#windows-upload)
		1. [Linux and OS X Upload](#linux-and-os-x-upload)
			1. [Linux TFTP Note](#linux-tftp-note)
1. [Enabling Remote Reset and Reprogram Capabilities](#enabling-remote-reset-and-reprogram-capabilities)
1. [EEPROM Requirements](#eeprom-requirements)
1. [Test Binaries](#test-binaries)
1. [Supporting Libraries](#supporting-libraries)
1. [Useful References](#useful-references)
1. [Acknowledgments](#acknowledgments)
	1. [Contributors](#contributors)
	2. [Donors](#donors)
1. [License](#license)

## Dependencies

This project requires Arduino SDK 1.15 or later due to the project's source layout.

The following dependencies are required to compile and use this bootloader:

* [git-lfs](#git-lfs)
* [avr-gcc](#avr-gcc)
* [avrdude](#avrdude)

### git-lfs

This project requires you to use [git-lfs](https://git-lfs.github.com). Images, pre-built bootloader binaries, and other files are stored within git-lfs.

If you do not have this installed, please visit [the git-lfs web page](https://git-lfs.github.com) for installation instructions.

If you cloned this repository before installing git-lfs, please run `git lfs pull`. If you have git-lfs installed before cloning the repository, git will automatically perform a `git lfs pull` for you.

When you don't have git-lfs installed, you will see all of the files in the repository. However, files stored with git-lfs will show up as small placeholders. Trying to use these files will result in errors.

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
* __ATmega32U4__ ([see notes](notes-on-atmega32u4-support))

Officially supported Ethernet controllers:

* __WizNet W5100__
* __WizNet W5200__
* __WizNet W5500__

Officially supported Arduino Boards:

* [Arduino Ethernet](17)
* [Arduino Uno][18]
* [Arduino Duemilanove w/ ATmega328][19]
* [Arduino Mega 2560][20]

### Notes on ATmega32U4 Support

The ATmega32U4 is supported, but the chip cannot support uploads over USB serial. It should be possible to upload sketches over the serial port (non-USB), but this has not been tested. The Caterina bootloader requires the majority of bootloader space to implement USB support. It is unclear whether both USB and Ethernet support can be implemented for this chip.

### Other Tested Boards

These boards have been reported to work with the Ariadne bootloader:

* [ITead Studio IBoard][21]
* [Sparkfun Ethernet Pro][22]
* [XBoard Relay][28]

If you think another board can be added to this list, please [contact us](contact@embeddedartistry.com).

## Repository Structure

This repository is structured to follow the [latest Arduino IDE standard for 3rd-party hardware](https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5-3rd-party-Hardware-specification). Here is an overview of the structure:

* [`avr/`](avr): This is where the bootloader code, pre-built binaries, and helper libraries live
	- [`bootloaders/ariadne/`](avr/bootloaders/ariadne) contains the pre-built bootloader `.hex` files, as well as [source code for the bootloader](avr/bootloaders/ariadne/src)
	- [`libraries/`](avr/libraries) contains helper libraries that are used to interact with the bootloader from an Arduino application
	- [`variants/`](avr/variants) contains additional board definitions
* [`docs/`](docs) contains additional reference documentation, images, and GitHub templates
* [`extras/`](extras) contains miscellaneous useful utilities

## Quickstart Checklist

This is the abbreviated checklist for installing and using the bootloader:

1. [Download the bootloader](#download-the-bootloader) and [install it to the proper location](#arduino-sdk-installation)
1. [Flash the bootloader to the device](flashing-the-bootloader)
2. **DO NOT SKIP:** Program network settings for the device using the [`NetEEPROM`](#supporting-libraries) library or examples
3. [Compile the application using a bootloader board definition](#compiling-applications)
3. Include the [`EthernetReset`](#supporting-libraries) library in your application (or use the example sketch)
	1. Create a reset server and specify a port
	2. Call `.begin()` on the server object in `setup()`
	3. Call `.check()` on the server object in `loop()`
4. When your application is running, trigger a reset using curl or a web browser
	1. URL Format: `http://{ip}:{port}/{reset_path}/reprogram`
		1. Example: `curl 10.0.1.199:8080/ariadne/reprogram`
	2. The device will respond with:
		1. "Arduino will reset for reprogramming in 2 seconds"
	3. You must use the "password" programmed via the NetEEPROM library to successfully enter the programming mode
5. Follow the [TFTP Upload](#flashing-applications-via-tftp) instructions to send a new binary to the device

## Downloading the Bootloader

You can clone the repository from GitHub:

```
$ git clone https://github.com/embeddedartistry/ariadne-bootloader
```

You can also download a zip archive of the repository from [the GitHub repository page](https://github.com/embeddedartistry/ariadne-bootloader).

## Arduino SDK Installation

In order for the Arduino IDE to access the information, you need to place the `ariadne-bootloader` repository within the `hardware/` folder of your "Sketchbook Location". You can find this location by viewing the Arduino IDE settings. The default location is often the `Arduino/` folder in your home directory or in `Documents/`.

For our system, which is OS X, the Sketchbook location is:

```
/Users/embeddedartistry/Documents/Arduino
```

Our Sketchbook folder does not have a `hardware` directory by default. We created the folder ourselves:

```
$ mkdir -p /Users/embeddedartistry/Documents/Arduino/hardware
```

You can clone the repository within that directory, or you can use a symbolic link within the `hardware/` folder:

```
$ cd /Users/embeddedartistry/Documents/Arduino/hardware
$ ln -s ~/src/ariadne-bootloader/ ./ariadne
```

Once the `ariadne-bootloader` folder is placed in the `hardware/` folder, Arduino will detect it. Restart the IDE if it is already open. You should now see the Ariadne bootloader show up in the board list.

![Image showing the Ariadne bootloader in boards](docs/boards.png "Ariadne Listed in Boards")

## Atmel Studio Installation

If you are using Atmel Studio, our recommendation is to use the Visual Micro extension. When you are using the Visual Micro add-on, the standard installation instructions will apply. Then, use the Visual Micro board manager to select the proper Ariadne board definition for your target. 

For more information, see this link:

* [Visual Micro: The Board Manager](https://www.visualmicro.com/page/User-Guide.aspx?doc=Board-Manager.html)

## Compiling the Bootloader

Precompiled bootloader binaries are provided within this repository. The files are stored using [git-lfs](#dependencies). If you do not have git-lfs installed, these files will not be true binaries. Instead, they will be text files with a git-lfs hash stored inside. To download them properly, please see [the dependencies section](#dependencies).

To compile the bootloader, you will need to have [avr-gcc](#avr-gcc) installed on your computer. If you are using the Arduino IDE or Atmel Studio for AVR, the compiler is already installed.

The bootloader files can be re-compiled from source by navigating to the [bootloader source directory: `avr/bootloaders/ariadne/src`](avr/bootloaders/ariadne/src).

Within that directory, run the `makerelease` script to build all variants:

```
$ ./makerelease
```

The script will place the bootloader variants in the [`avr/bootloaders/ariadne/`](avr/bootloaders/ariadne/) directory. The files will be automatically picked up by the Arduino IDE in this location.

Targets can be individually built within the [`avr/bootloaders/ariadne/src`](avr/bootloaders/ariadne/src) directory. A Makefile is provided in that directory. When using the Makefile, note that the `.hex` files which are produced stay in the `src/` directory. They are not automatically pushed up a level. You will need to manually move the file(s) to [`avr/bootloaders/ariadne/`](avr/bootloaders/ariadne) for the Arduino IDE to pick up the new version.

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

First, navigate to the __Tools__ > __Board__ menu and select your target board from the `Ariadne Bootloader` list (at the bottom in our view).

![Image showing the Ariadne bootloader in boards](docs/boards.png "Ariadne Listed in Boards")

Next, navigate to the __Tools__ > __Version__ menu and select Wiznet chip from the list, as well as "Standard" or "Debug" mode. Debug binaries are larger, but contain helpful Serial output for seeing what the bootloader is doing.

![Image showing the Ariadne bootloader versions](docs/version.png "Ariadne Bootloader Versions")

Next, go to the __Tools__ > __Programmer__ menu and select the programmer you are using.

![Image showing the tools menu](docs/tools.png "Programming Tools Menu")

In case you are using _ArduinoISP_, make sure that the selected port in the __Tools__ > __Serial Port__ menu refers to the _ArduinoISP_. You should not select the board that you want to burn the bootloader on. This requirement applies to other serial-based programmers, such as the AVR-ISP 2.0 ("Atmel STK500 Development Board").

To flash the bootloader, select the __Tools__ > __Burn Bootloader__ menu option.

![Image showing the Burn Bootloader menu option](docs/burn.png "Burn Bootloader Menu Option")

Check the Arduino console output to confirm that the bootloader was flashed successfully.

### Programming with AVRDUDE

COMING SOON

## Compiling Applications

When compiling applications for a board using the Ariadne bootloader, you need to select the proper board configuration in the Arduino IDE. 

If you are using Atmel Studio, you also need to select the proper board configuration using the Visual Micro board manager. Use the board manager to select the proper Ariadne board definition for your target. For more information, see this link:

* [Visual Micro: The Board Manager](https://www.visualmicro.com/page/User-Guide.aspx?doc=Board-Manager.html)

## Flashing Applications via Serial

Ariadne is built off of the standard Arduino AVR bootloaders. Serial upload capabilities are preserved. Serial uploads work even when an Ethernet shield is not installed.

To flash an application over USB/Serial:

1. Plug in the USB cable
2. Select the serial port for the device and the appropriate board from the __Tools__ > __Board__ menu. 
3. Program the device

If your device has auto-reset capabilities, such that a Serial connection resets the device, programming will work with a sketch is running.

If your device does not have auto-reset capabilities, then you will need to press the reset button on the device to enter the bootloader mode.

If there is a __valid__ program already flashed on the Arduino, you have to reprogram the device in the next __10 seconds__. If you don't, the bootloader will initiate the program that is already flashed onto the Arduino. In case there is no program flashed or the program has been marked as __invalid__, the bootloader will never time out and you can reprogram it at any time.

For boards other than the Arduino Mega, the primary LED will start blinking rapidly when the bootloader is running.

After a successful flashing, the bootloader will automatically start the application.

## Flashing Applications via TFTP

Flashing applications via TFTP requires multiple moving parts. 

Follow these instructions to configure your device:

1. [Configure device network settings](#device-network-settings)

### Device Network Settings

When you flash the bootloader, default network settings are used:

```
* IP Address:  192.168.1.128
* Subnet Mask: 255.255.255.0
* Gateway:     192.168.1.1
* MAC Address: 0xDE.0xAD.0xBE.0xEF.0xFE.0xED

* TFTP Negotiation Port: 69
* TFTP Data Port: 46969
```

These default settings are applied whenever the bootloader is flashed. If you have already programmed network settings in the EEPROM, they will be discarded when you re-flash the bootloader.

These settings can be [overridden during the build process](#overriding-default-settings), or they can be [changed after the bootloader is flashing using an application](#changing-network-settings-after-flashing).

**You must configure the network settings for the TFTP server in the bootloader and the EthernetReset server to work correctly.**

#### Configuring Network Settings After Flashing

Network Settings can be changed after the bootloader is flashing using the [__NetEEPROM__ library](avr/libraries/NetEEPROM). This library is provided with the bootloader and will be automatically detected by the Arduino IDE.

This library can be included in your application to support reading and writing network settings. Please refer to the library for documentation on its usage.

You  also use the provided example sketches, [_WriteNetworkSettings_](avr/libraries/NetEEPROM/examples/WriteNetworkSettings/WriteNetworkSettings.ino) and [_ReadNetworkSettigns_](avr/libraries/NetEEPROM/examples/ReadNetworkSettings/ReadNetworkSettings.ino), to see how the NetEEPROM library is used. These sketches can be used to adjust the network settings of your device. The sketches are commented and also meant to serve as documentation.

You can access these files [within this repository](avr/libraries/NetEEPROM/examples/), or by using the Arduino IDE. Inside of the IDE, navigate to navigate to the __File__ > __Examples__ > __NetEEPROM (Ariadne-Bootloader)__ menu and select the desired example.

![Image showing the NetEEPROM example sketches](docs/neteeprom_sketches.png "Ariadne NetEEPROm example sketches")

The settings array in the __WriteNetworkSettings__ sketch are placed in the order that Wiznet chips read them. Make sure to keep the values in this order.
If you configure the network settings, you also need to configure the TFTP data transfer port. Default settings do not need to be changed if you want to use them.

#### Overriding Default Settings

The default settings can be modified by adjusting the values in [`net.h`](avr/bootloaders/ariadne/src/net.h):

```
#ifndef IP_ADDR
#define IP_ADDR     10,0,1,199
#endif

#ifndef SUB_MASK
#define SUB_MASK    255,255,255,0
#endif

#ifndef GW_ADDR
#define GW_ADDR     10,0,1,1
#endif

#ifndef MAC_ADDR
#define MAC_ADDR    0xDE,0xAD,0xBE,0xEF,0xFE,0xED
#endif
```

The default values can also be overridden by the build system. You can adjust the [Makefile](avr/bootloaders/ariadne/src/Makefile) flags variables to include definitions for these values.

### Configuring Your Router

Ethernet updates may succeed while you are on the same network as the Arduino, but fail when you are connecting from a different network.

If this happens to you, you likely need to enable [port forwarding][11] for ports __69__ and __46969__ (or your manually configured port). Port Forward has [excellent guides][12] on how to enable port forwarding for a vast number of routers.

#### Supporting Multiple Arduino Devices Behind One Router

If you have multiple Arduino devices behind the router, you may need to configure different port forwarding settings on your router for each Arduino. You will configure the port forwarding so that each Arduino device has a different external port on the router. Each external port is forwarded to the proper internal port and IP of the Arduino on the local network.

For example, let's say you have 2 devices, one at *192.168.1.128* and one at *192.168.1.129*. They both listen to port __69__ for the initial connection. In this case you can forward an external port (e.g., __6969__) on the router to `192.168.1.128:69`. You'll forward another external port (e.g., __6970__) to `192.168.1.129:69`. 

When you are connecting via TFTP, you will open the connection using the router's public IP address (e.g. `136.20.14.135`) and the external ports, __6969__ and __6970__.

### Converting Your Sketch to the Right Format

While Arduino serial flashing uses __HEX__ files, the bootloader TFTP server only works with binary files. Y must manually convert your programs to the right format. 

First, build your sketch inside _Arduino IDE_ using the __Verify__ button. Next, without exiting the _Arduino IDE_, you need to navigate to the temporary directory where your project was built. On all platforms, you can find out the temporary build folder by checking `Show verbose output during compilation` in _Arduino IDE_'s preferences menu. The path for the temporary build output directory will be included at the end of the compilation output.

For example:

```
/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-size -A /var/folders/0h/_66m4dqj3p1b3j1xqg10r08w0000gn/T/arduino_build_678002/WebServer.ino.elf
Sketch uses 12774 bytes (4%) of program storage space. Maximum is 258048 bytes.
Global variables use 797 bytes (9%) of dynamic memory, leaving 7395 bytes for local variables. Maximum is 8192 bytes.
```

Look for a line mentioning a file in `.elf` or `.hex`. In the example above, we can see `/var/folders/0h/_66m4dqj3p1b3j1xqg10r08w0000gn/T/arduino_build_678002/WebServer.ino.elf`. The temporary folder the build output is stored in is `/var/folders/0h/_66m4dqj3p1b3j1xqg10r08w0000gn/T/arduino_build_678002/`.

If we look at the contents of the folder, we can see there is a corresponding `.hex` file:

```
$ ls /var/folders/0h/_66m4dqj3p1b3j1xqg10r08w0000gn/T/arduino_build_678002/
WebServer.ino.eep                 core
WebServer.ino.elf                 includes.cache
WebServer.ino.hex                 libraries
WebServer.ino.with_bootloader.hex preproc
build.options.json                sketch
```

We'll convert that to a binary using `avr-objcopy`

#### Windows Conversion

On Windows, the temporary directory is often something like:

```
C:\Documents and Settings\Administrator\Local Settings\Temp\build5571819468326525374.tmp\Blink.cpp.hex
```

To convert the file to a binary, use the following `avr-objcopy` pattern:

```
"C:\Program Files\Arduino\hardware\tools\avr\bin\avr-objcopy.exe" -I ihex "C:\path\to\your\program.hex" -O binary program.bin
```

Using the example above, we would say:

```
"C:\Program Files\Arduino\hardware\tools\avr\bin\avr-objcopy.exe" -I ihex
"C:\Documents and Settings\Administrator\Local Settings\Temp\build5571819468326525374.tmp\Blink.cpp.hex"
-O binary Blink.cpp.bin
```

The output will be placed int he current directory.

#### Linux and WSL Conversion

For Linux, the file is likely to be in your `/tmp` directory. The form of `avr-objcopy` to use is:

```
avr-objcopy -I ihex /path/to/sketch.hex -O binary sketch.bin
```

#### OS X Conversion

For OS X, the file location is likely within `/var/folders/`. The form of `avr-objcopy` to use is:

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

The [EthernetReset](#supporting-libraries) library is a requirement for your program if you want to support remotely enabling binary uploads. [A README is provided](avr/libraires/EthernetReset.md) and there is an [example sketch showing the usage of the library](avr/libraries/EthernetReset/examples/ResetServer/ResetServer.ino).

The following steps must be used to implement a remote upload capability for your device:

1. Flash the bootloader to the device
2. Program network settings for the device using the [NetEEPROM](#supporting-libraries) library or examples
3. Include the EthernetReset library in your application
	1. Create a reset server and specify a port
	2. Call `.begin()` on the server object in `setup()`
	3. Call `.check()` on the server object in `loop()`
4. Trigger a reset using curl or a web browser
	1. URL Format: `http://{ip}:{port}/{reset_path}/reprogram`
		1. Example: `curl 10.0.1.199:8080/ariadne/reprogram`
	2. The device will respond with:
		1. "Arduino will reset for reprogramming in 2 seconds"
	3. You must use the "password" programmed via the NetEEPROM library to successfully enter the programming mode
5. Follow the [TFTP Upload](#flashing-applications-via-tftp) instructions to send a new binary to the device

## EEPROM Requirements

Ariadne uses the Arduino EEPROM to store values needed by the bootloader and EthernetReset server. If you are using the default EEPROM library for our own purposes, please make sure to start writing after `NETEEPROM_END`, available in `NetEEPROM_defs.h`:

```
#define NETEEPROM_END      63
```

For more information, see the [NetEEPROM library](#supporting-libraries).

If you use the [NewEEPROM](#supporting-libraries) library included with the Ariadne bootloader, then offsets will be handled for you automatically and you will not need to adjust your code. Note, however, that the full EEPROM space is not available.

## Test Binaries

For testing purposes, you can find a __blink__ sketch in binary form inside the [`extras/tests/blink`](extras/tests/blink) folder. 

The __fade__ sketch in the [`extras/tests/fade`](extras/tests/fade) folder will also give you a view of what a failed upload looks like. This sketch fails because it is written in plain __C__ and not in __Arduino__. That way it lacks some "signatures" the bootloader uses to validate _Arduino_ sketches. 

## Supporting Libraries

Supporting libraries are included with the bootloader and will be automatically detected by the Arduino IDE.

These libraries are meant to facilitate interacting with the Ariadne bootloader from Arduino applications.

* [NewEEPROM](avr/libraries/NewEEPROM) is a patched EEPROM library that protect the memory space used by the Ariadne bootloader
	- Use this library in place of the default EEPROM library to prevent settings from being overwritten
* [NetEEPROM](avr/libraries/NetEEPROM) is a library that enables you to read/write the bootloader network settings
* [EthernetReset](avr/libraries/EthernetReset) is a library that can be used to create an HTTP server in an application that enables you to:
	- Remotely restart the Arduino application
	- Restart into the bootloader in programming mode, which waits for the a binary over TFTP

The [EthernetReset](avr/libraries/EthernetReset) library is a requirement for your program if you want to support remotely enabling binary uploads. [A README is provided](avr/libraires/EthernetReset.md) and there is an [example sketch showing the usage of the library](avr/libraries/EthernetReset/examples/ResetServer/ResetServer.ino).

## Useful References

* [AVR Beginners](http://avrbeginners.net) is an excellent resource for anyone working on this bootloader who is new to the AVR architecture

## Acknowledgments

The Embedded Artistry version of the Ariadne bootloader is derived from [loathingkernel/ariadne-bootloader](https://github.com/loathingKernel/ariadne-bootloader). Credit goes to its maintainer, [Stelios Tsampas](https://github.com/loathingKernel).

The Ariadne bootloader is built upon these open source projects:

* [TFTP-Bootloader][23] (The original base for this project)
* [Optiboot][24] (Bootloader for the __ATmega328__)
* [Arduino-stk500v2-bootloader][25] (Bootloader for the __ATmega2560__)

### Contributors

The following contributors were listed in the [original project](https://github.com/loathingKernel/ariadne-bootloader) README:

* [mharizanov][26] (Commenting some of the initial *Arduino* code, enabling Stelios to take on the project)
* [follower][27] (Sketches served as a starting point for the included __NetEEPROM__ and __EthernetReset__ libraries)
* [Arjen Hiemstra](https://github.com/arjenhiemstra) (Provided support for W5200 and W5500)
* [per1234](https://github.com/per1234) (Testing and tech support in Arduino Forums)

### Donors 

The following donors were listed in the [original project](https://github.com/loathingKernel/ariadne-bootloader) README:

* Hachi Manzur (AVRISP mkII programmer, testing)

## License

Ariadne is released under the [GPLv2, GNU General Public License][99].

[2]: http://www.atmel.com/dyn/products/tools_card.asp?tool_id=2726
[3]: http://www.ladyada.net/make/usbtinyisp/
[4]: http://arduino.cc/en/Hacking/ParallelProgrammer
[5]: http://arduino.cc/en/Tutorial/ArduinoISP
[6]: http://www.atmel.com/tools/AVRISPMKII.aspx
[7]: https://www.olimex.com/Products/AVR/Programmers/AVR-ISP-MK2/
[8]: http://www.gammon.com.au/forum/?id=11635
[9]: https://github.com/codebendercc/Ariadne-Bootloader#configuring-your-router-for-remote-flashing
[10]: http://youtu.be/KCHqhV6xPMg
[11]: http://en.wikipedia.org/wiki/Port_forwarding
[12]: http://portforward.com/english/routers/port_forwarding/
[13]: http://codebender.cc
[17]: www.arduino.cc/en/Main/ArduinoBoardEthernet
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
