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
cable and select the serial port and the appropriate board from the __Tools__ > __Board__ menu. After that you must press the
reset button and the indication LED on __pin 13__ or __pin 9__, in case of Arduino Ethernet, will start blinking rapidly.
This means that the bootloader is running and the Arduino is ready to be programmed. If there is a __valid__ program already
flashed on the Arduino, you have to reprogram the device in the next __5 seconds__. If you don't, the bootloader will
initiate the program that is already in the Arduino. In case there is no program flashed or the program has been marked
as __invalid__, the bootloader will never time out and you can reprogram it at any time. 

After a succesful flashing,

* *Arduino Duemilanove* will automatically start the user's application.
* *Arduino Uno* will do a reset cycle and start the program after the bootloader times out.

This happens because *Uno* has the autoreset feature that resets the board after a serial connection.

Due to "autoreset" for remote tftp programming is being implemented using a watchddog timer timeout, the bootloader
will do a full cycle after every reset, physical or software. For those who want *Adaboot No-Wait Mod*-like functionality,
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
can be used but for the purpose of changing and reading the network settings you can use the included examples. To load
them navigate to __File__ > __Examples__ > __NetEEPROM__ and select one of the examples. You can write the network
settings using the __WriteNetworkSettings__ sketch or print them to the serial using the __ReadNetworkSettings__.

Note that the settings array in the __WriteNetworkSettings__ sketch hasn't got the settings in the usual order but
rather in the order that __W1500__ reads them, so make sure you have put the correct values. If you set the network
settings you also have to set the TFTP data transfer port. The default is good enough but you may need to change it
for the reasons that are listed below in the
[Configuring your Router for Remote Flashing](https://github.com/codebendercc/Ariadne-Bootloader#configuring-your-router-for-remote-flashing)
section.There is also documentation on how use these sketches in the form of comments so be sure to read them.


TFTP Flashing
-------------
Now for the real reason we made this bootloader and why you should use it. First of all you can watch __Ariadne__ in
action in this how-to video for remote flashing using TFTP [here](http://youtu.be/KCHqhV6xPMg). In the video may
notice that the board is being reset by hand. In the next couple of weeks we are going to release the library that will
allow for remote resetting through a simple web server with some security measures. More on that as the library progresses.

Unlike serial flashing that uses __HEX__ files to flash the Arduino, the TFTP server implemented in the bootloader works
with binary files. This means that you have to manually convert your programs to the right format. To do that, first build
your sketch inside _Arduino IDE_ using the __Verify__ button. After that, without exiting the *Arduino IDE* you need
to navigate to the temporary directory where your project was built. That is ```C:\Users\owner\AppData\Local\Temp\```
on *Windows* or ```/tmp``` on *Linux*. There you will find a folder named something like ```build2429295348207572157.tmp```.
That is where the Arduino temporary build files reside. Enter the directory and make sure that there is a ```.elf```
or a ```.hex``` file with the same name as your sketch. That is the file you need to convert. To achieve that you have to
run one of the following commands in a terminal.

* ```avr-objcopy -j .text -j .data -O binary [sketch].elf [sketch].bin```
* ```avr-objcopy -I ihex [sketch].hex -O binary [sketch].bin```

Or,if you have scons installed, you can use the modified ```SConstruct``` script you can find in ```Ariadne-Bootloader/utilities```.
This being based on the *arscons* script, it can be used in two ways. If you used the previous process to generate the _HEX_
file you can just copy the ```SConstruct``` file inside the temporary *Arduino IDE* build directory (as mentioned above)
and run ```scons``` in a terminal inside that directory.

The other way to use it is to copy the ```SConstruct``` script inside the sketch's directory and, as above, run 
```scons``` in a terminal inside that directiry. This way you will build your project outside *Arduino IDE* creating
the ```.bin``` file in the process.


Configuring your Router for Remote Flashing
-------------------------------------------
If you are having troubles flashing your Arduino at home from the road, you probably need to enable
[port forwarding](http://en.wikipedia.org/wiki/Port_forwarding). You need to forward ports __69__ and __46969__ to your
Arduino in your router's configuration. In case you have changed the incoming data port from __46969__ to another port
i.e. __50232__, you have to forward __50232__ port instead of __46969__. This is particularly useful when
you have more than one Arduinos, that you want to flash, behind your router. In addition to this you are going to have
to translate an external port of your choice on the router to the internal port and ip of the Arduino in the local network.
An example is that you have 2 devices, one at *192.168.1.120* and one at *192.168.1.121*. They both listen to port
__69__ for the initial connection. In this case you can translate external port __6969__(any random port will do) on
your router to *192.168.1.120*:__69__ and external port __6970__ to *192.168.1.121*:__69__ and specify these in the
tftp client you are using.

Port Forward has [excellent guides](http://portforward.com/english/routers/port_forwarding/) on how to enable port
forwarding for a vast number of routers.


Codebender
-------------
One of the best ways and easiest ways to use this bootloader is along with [codebender.cc](http://codebender.cc).
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


Acknoledgements
---------------
