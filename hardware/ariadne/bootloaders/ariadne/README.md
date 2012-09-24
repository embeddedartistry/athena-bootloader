THIS IS THE OLD README. IT REMAINS AS IT MAY HAVE SOME MORE INFORMATION. OUTDATED
=================================================================================

Ethernet Bootloader for the ATmega328P / W5100
==============================================

Flashing bootloader:
--------------------
During development an [ArduinoISP](http://arduino.cc/en/Tutorial/ArduinoISP "ArduinoISP")
was used to burn the bootloader on the board and that is what is supported in
the default Makefile. If you have another ISP and since you are here it is safe
assume that you know how to edit the Makefile to support your ISP. After
connecting your ISP all you need to do is "make install" in the firmware
directory.

The size of the bootloader is currently 2375 bytes without any debug messages.
With full debugging enabled it is well over 4k so you can't enable all messages
at the same time.


Configuring your network:
-------------------------
The bootloader's' default address is 192.168.1.250. Configure your computer
network card to a static address of 192.168.1.1 with a subnet of 255.255.255.0.
By default, the bootloader assumes an internet gateway is at address
192.168.1.1. The default network settings can be changed at the top of the
"net.h" file.

The bootloader listens to port 69 for incoming connections and switches to
port 46969 for the transfers. So if you need to flash remotely, outside your
local network you should forward these 2 ports in your router to the Arduino.


Converting "sketches" to the right format:
----------------------------------------
The bootloader accepts raw binary images, starting at address 0x0000.
These can be generated using avr-objcopy, part of WinAVR / AVR-GCC, using the
"-O binary" option.
Example: avr-objcopy -j .text -j .data -O binary [app].elf [app].bin

There is also a modified version of the arscons SConstruct script for the scons
build system that will automatically build the ino sketch and generate the
needed binary. To use the script create a folder with the same (case sensitive)
name of your [app].ino file and put the SConstruct and the [app].ino files in that
folder. After that, navigate to the folder using a terminal and run the "scons"
command.


Uploading firmware manually:
----------------------------
1.  Check the target board is powered, and connected to the computer ethernet.
2.  Verify the computer network settings: Static IP of 192.168.1.1, Subnet of 255.255.255.0.
3.  In a console window: tftp 192.168.1.250
4.  At the tftp> prompt: Make sure the tftp client is in octet mode using the
    mode octet" command. Here you can also put the "trace" and "verbose" commands
    for some more output.
5.  Push reset button to start the bootloader. The LED will blink rapidly.
6.  While the LED blinks, "put [app].bin" (where app.bin is your application binary image)
7.  The board will be reprogrammed. Type quit to exit tftp.


Flash codes:
------------
* Rapid blinking: Ethernet bootloader is running.
* Slow fading: Test application is running.

The bootloader waits for 5 seconds to be flashed. If the board is programmed
after the 5 seconds the bootloader will timeout and start the user application.
Otherwise it will restart the 5 second timeout.


Debugging:
----------
By default the debug interface is disabled. To enable it uncomment the "_DEBUG"
defines in debug.h for the subsystem you want. For any of them to work the plain
"_DEBUG" should be enabled. After flashing the bootloader, helpful messages
will be outputted to the serial port of the Arduino board at 115200 bps speed.
If debugging is enabled the binary image validations are disabled because the
the test applications do not conform with the validation statements.


Version history
---------------
* 0.1: First internal release by the Arduino project. Supports uploads on tftp.
* 0.2: Tftp upload fixed and working. More debugging support
