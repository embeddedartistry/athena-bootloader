Ariadne Bootloader for Arduino and WizNet W5100
===============================================

Bootloader for Arduino with Ethernet
------------------------------------
This is a beta stage bootloader for Arduino Ethernet board and the regular
Arduino with Ethernet Shield. It is based on previous unfinished work by the Arduino
developers. The bootloader implements a TFTP server on the Arduino board and flashing works
using any regular TFTP client.

There are two ways to get this bootloader, both of them are really simple. You can either burn your Arduino with
this new bootloader (using the guide below), or you can claim the Pioneer perk on codebender's
[IndieGoGo campaign](http://www.indiegogo.com/codebender?a=803683) and get an Arduino preloaded with our bootloader
delivered straight to your door.



The Files and Folders in this Repository
----------------------------------------
The structure of this repository is made to follow the standarts of the Arduino IDE.
This way you can simply copy the folders in your sketchbook and be ready

* hardware: This is where the bootloader resides.
* java-client: Demo client for the bootloader. Inherited by the initial project. Untested and probably non-functional
* libraries: Helper libraries to support functions of the bootloader
* utilities: Various stuff used for development and debugging


Downloading and Installing Files
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


Burning the Bootloader
----------------------
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
Ariadne bootloader supports flashing through serial like any other regular bootloader. Using this way of uploading is
built upon the very good *Optiboot* bootloader so it should be pretty straight forward to use. Just plug in the USB
cable and select the serial port and the appropriate board from the __Tools__ > __Board__ menu. After plugging in the cable,
press the reset button and the indication LED on __pin 13__ or in case of Ethernet __pin 9__ will start blinking rapidly.
This means that the bootloader is running and the Arduino is ready to be programmed. If the bootloader doesn't receive
anything for a *5 sec* period and there is a valid program in the memory, the bootloader will time out and run the user's
application. In case there is not program or the program has been marked invalid the bootloader will never time out.
After flashing is successful *Arduino Duemilanove* will automatically start the user's application but *Arduino Uno* will
do a reset cycle and start the program after the bootloader times out. This happens because *Uno* has the autoreset 
feature that resets the board after flashing.

Due to autoreset for remote tftp programming is being implemented using a watchddog timer timeout, the bootloader
will do a full run after every reset, physical or software. For those who want *Adaboot No-Wait Mod*-like functionality,
we have been testing some options on how to circumvent these limitations, but they still need refinement.


Configuring Network Settings
----------------------------
The default built-in network settings of the bootloader are listed below.
* ```IP Address:  192.168.1.128```
* ```Subnet Mask: 255.255.255.0```
* ```Gateway:     192.168.1.1```
* ```MAC Address: 0xDE.0xAD.0xBE.0xEF.0xFE.0xED```
* ```TFTP Data Port: 46969```

These can be changed using our __NetEEPROM__ library. The library is going to have it's own documentation on how it
can be used but for the purpose of changing and reading the settings you can use the included examples. To load them
navigate to __File__ > __Examples__ > __NetEEPROM__ and select one of the examples. You can write the network settings
using the __WriteNetworkSettings__ sketch or read the to the serial using the __ReadNetworkSettings__.

Note that the settings array in the __WriteNetworkSettings__ sketch hasn't got the settings in the usual order but
rather in the order that __W1500__ reads them, so make sure you have put the correct values. If you set the network
settings you have also to set the TFTP data transfer port. The default is good enough but you may need to change it
for the reasons that are listed below in the ```Configuring your Router for Remote Flashing``` section.There is also
documentation on how use these sketches in the form of comments so be sure to read them.


TFTP Flashing
-------------
You can watch the how-to video for remote flashing using TFTP [here](http://youtu.be/KCHqhV6xPMg).
We will add more info Soon™.


Configuring your Router for Remote Flashing
-------------------------------------------
If you are having troubles flashing your Arduino at home from the road, you probably need to enable
[port forwarding](http://en.wikipedia.org/wiki/Port_forwarding). You need to forward ports __69__ and __46969__ to your
Arduino in your router's configuration. In case you have changed the incoming data port from __46969__ to another port
i.e. __50232__, you are going to have to forward __50232__ port instead of __46969__. This is particularly useful when
you have more than one Arduinos, that you want to flash, behind your router. In addition to this you are going to have
to translate a port of your choice on the router to the port and ip of the Arduino in the local network. An example is
that you have 2 devices, one at *192.168.1.120* and one at *192.168.1.121*. They both listen to port __69__ for the
initial connection. In this case you can translate port __6969__(any random port will do) on your router to
*192.168.1.120*:__69__ and port __6970__ to *192.168.1.121*:__69__ and specify these in the tftp client you are using.

Port Forward has [excellent guides](http://portforward.com/english/routers/port_forwarding/) on how to enable port
forwarding for a vast number of routers.


Codebender
-------------
One of the best ways to use this bootloader is along with [codebender.cc](http://codebender.cc). Using it is easy.
Just register, enter your Arduino's IP (external IP for those in corporate or home networks behind NAT) and flash.


Supported Boards
----------------
Right now the __ATmega328__ processor and the __WizNet W5100__ ethernet controller are supported. That means
that your Arduino Uno, Arduino Duemilanove, Arduino Ethernet or any Arduino compatible board using these chipsets
can be burned with the Ariadne bootloader. If you have the know-how you can probably compile the bootloader for other
processors but note that we haven't tested it. The following list will be updated over time.
* Arduino Ethernet
* Arduino Uno
* Arduino Duemilanove w/ ATmega328


Roadmap
-------
Right now the main focus for the first packaged release is bug fixing and improve existing functionality. That is why we
encourage you to use the bootloader and report any bugs, misbehaviours or feature requests here on github. There is
also on going work to work on the Arduino Mega and support for Arduino Leonardo is planned after that. Support for
other ethernet or wifi controllers is being discussed but after the bootloader has been stabilized.