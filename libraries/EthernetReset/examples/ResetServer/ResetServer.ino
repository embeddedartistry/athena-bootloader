/*
 Reset Web Server

 A simple sketch that creates 2 web servers on Arduino. The first one on port 80 is used
 for showing the values of the analog pins and it is the standard example found in the Arduino
 wiki. The second one run on the same port the Ariadne bootloader uses for uploading the sketch
 to the Arduino, in this case the standard 46969. This server is used for remote resetting and
 reprogramming thourgh a GET request to specific URLs.
 * http://{ip}/{reset_path}/reset
 * http://{ip}/{reset_path}/reprogram
 The first on will do a normal reset and after the bootloader times out the existing sketch
 will be ran. The second one will invalidate the existing sketch so the bootloader won't time out.
 {reset_path} is used instead of a password. If the path is correct you can access the commands
 "reset" and "reprogram". That is why you should pay extra attention to not let that path in your
 sketch if want to show it to the world. This is not as secure as a username and a password but
 it is more space efficient.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 14 Sep 2012
 by Stelios Tsampas

 */

#include <NewEEPROM.h>
#include <NetEEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetReset.h>

/* Create the reset server. This way your reser server will be at the port you
 * have speciefied in the bootloader for remote uploading. For more information on that
 * look at the "NetEEPROM" library in the "WriteNetworkSettings" sketch.
 * If you want to use your own port, create the object as this
 * "EthernetReset reset(reset_path, port);" where port is a number, i.e. 81 */
EthernetReset reset(8080);
EthernetServer server(80);

void setup()
{
	// Open serial communications and wait for port to open:
	// This is for debugging. you can safely omit this line.
	Serial.begin(9600);
	/* For now the Arduino EthShield and the server are being configured using the
	 * settings already stored in the EEPROM and are the same with the ones for Ariadne bootloader.
	 * This means that you *MUST* have updated the network settings on your Arduino with the
	 * "WriteNetworkSettings" sketch.
	 * The "begin()" command takes care of everything, from initializing the EthShield to
	 * starting the web server for resetting. This is why you should always start it before any other
	 * server you might want to have */
	reset.begin();
}


void loop()
{
	/* After the reset server is running the only thing needed is this command at the start of
	 * the "loop()" function in your sketch and it will take care of checking for any reset or
	 * reprogram requests */
	reset.check();

}

