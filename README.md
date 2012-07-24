Ariadne-Bootloader for Arduino and WizNet W5100
===============================================

Bootloader for Arduino with Ethernet
------------------------------------
This is a beta stage bootloader for Arduino Ethernet board and the regular
Arduino (Uno, Duemilanove /w Atmega 328) with Ethernet Shield. It is based on previous unfinished work by the Arduino
developers. The bootloader implements a TFTP server on the Arduino board and flashing works
using any regular TFTP client.

There are two ways to get this bootloader, both of them are really simple. You can either burn your Arduino with
this new bootloader (using the guide below), or you can claim the Pioneer perk on codebender's
[IndieGoGo campaign](http://www.indiegogo.com/codebender?a=803683) and get an Arduino preloaded with our bootloader
delivered straight to your door.



The files and folders in this repository
----------------------------------------
The structure of this repository is made to follow the standarts of the Arduino IDE.
This way you can simply copy the folders in your sketchbook and be ready

* hardware: This is where the bootloader resides.
* java-client: Demo client for the bootloader. Inherited by the initial project. Untested and probably non-functional
* libraries: Helper libraries to support functions of the bootloader
* utilities: Various stuff used for development and debugging


Downloading and installing files
--------------------------------
First of all, you need to clone or download the repository. To clone the repository
you need to have git installed, then you can run
```git clone https://github.com/codebendercc/Ariadne-Bootloader.git```
in a directory.
This way you can later update your local repository by running
```git pull```
inside the **Ariadne-Bootloader** directory.

In case you want to avoid this hassle, you can use the **ZIP** button at the top of the page
to download the latest snapshot of the repository in a zip archive and extract it.

After that you have to copy the **hardware** and **libraries** folders inside your sketchbook folder.
Take extra care during coping not to overwrite any other files. Restart the Arduino IDE to load
the new boards and libraries.


Burning of the bootloader
-------------------------
To burn the bootloader, you will need an [AVR-ISP](http://www.atmel.com/dyn/products/tools_card.asp?tool_id=2726)
(in-system programmer), [USBtinyISP](http://www.ladyada.net/make/usbtinyisp/) or you can build a
[ParallelProgrammer](http://arduino.cc/en/Hacking/ParallelProgrammer) or an
[ArduinoISP](http://arduino.cc/en/Tutorial/ArduinoISP).
The first three programmers should be connected to the ICSP pins (the 2 by 3 pin header) and make sure you plug
it in the right way. The board must be powered by an external power supply or the USB port. In the case of _ArduinoISP_
you should consult the above link for further instructions on how to build and use.

After you have connected the Arduino board and the programmer to your computer launch the Arduino IDE.
Navigate to the __Tools__ > __Board__ menu and select ```Arduino Duemilanove/Uno(ATmega328) w/ Ariadne Bootloader```
if you have an Arduino Duemilanove or Uno with an Ethernet Shield or ```Arduino Ethernet w/ Ariadne Bootloader```
for Arduino Ethernet. Then go to __Tools__ > __Programmer__ and select the programmer you are using.
In case you are using _ArduinoISP_, make sure that the selected port in the __Tools__ > __Serial Port__ menu refers to
the _ArduinoISP_ and not the board that you want to burn the bootloader on. Now, just launch the __Tools__ > __Burn Bootloader__
command and wait for about 15 seconds for the operation to complete.


Serial Flashing
---------------
Coming soon.

Configuring Network Settings
----------------------------
Coming soon.

TFTP Flashing
-------------
You can watch the how-to video for remote flashing using TFTP [here](http://youtu.be/KCHqhV6xPMg).
We will add more info Soon™.

Codebender
-------------
One of the best ways to use this bootloader is along with [codebender.cc](http://codebender.cc). Using it is easy.
Just register, enter your Arduino's IP (external IP for those in corporate or home networks behind NAT) and flash.

Configuring your Router for Remote Flashing
-------------------------------------------
If you are having troubles flashing your Arduino at home from the road, you probably need to enable
[port forwarding](http://en.wikipedia.org/wiki/Port_forwarding). You need to forward ports 69 and 46969 to your
Arduino in your router's configuration.

Port Forward has [excellent guides](http://portforward.com/english/routers/port_forwarding/) on how to enable port
forwarding for a vast number of routers.