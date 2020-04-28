/*
 * Reset Web Server
 *
 * This simple sketch that creates a web server on the Arduino that can be used for resetting the device.
 * The server will run on the port specified in the constructor.
 *
 * The server can be used to remotely reset a device or remotely enter programming mode. This is done through a GET request to specific URLs:
 *   http://{ip}:{port}/{reset_path}/reset
 *   http://{ip}:{port}/{reset_path}/reprogram
 *
 * The first URL will perform a normal reset of the program.
 * The second URL will invalidate the existing sketch so the bootloader won't time out. The bootloader will wait until a new image is uploaded.
 *
 * Instead of a password, a {reset_path} is used. If the path is correct, you can access the commands.
 *
 * This reset path matches the "password" that is configured by the AthenaEEPROM library. You should override the default password, but do not let
 * your team's password be revealed in any public-facing repositories.
 *
 * Circuit:
 *  Ethernet shield attached to pins 10, 11, 12, 13
 *
 * Originally created 14 Sep 2012 by Stelios Tsampas
 * Updated in 2019 by Embedded Artistry LLC
 */

// Dependencies - required here if you're using an older Arduino IDE which does
// not automatically pick up dependencies from the library headers. In this case,
// all libraries must be included in the Sketch.
// For newer Arduino versions, you only need the library header.
#include <Ethernet.h>
#include <AthenaEEPROM.h>
#include <SPI.h>
// Library Header
#include <AthenaEthernetReset.h>

/* Create the reset server at the specified port.*/
AthenaEthernetReset reset(8080);

void setup()
{
	// Open serial communications and wait for port to open:
  // This is for debugging only, Serial isn't required.
  Serial.begin(115200);
  while(!Serial) delay(10);
  Serial.println("Starting Ethernet Reset Server Example");

	/*
	* The Arduino/Ethernet Shield + the server are being configured using the settings already stored in the EEPROM.
	* If you haven't configured any settings, the default values will be used.
	* The "begin()" command takes care of everything, from initializing the shield to starting the web server.
	* This is why you should always start it before any other server you might want to have.
	*/
	reset.begin();
}


void loop()
{
	/* After the reset server is running the only thing needed is this command at the start of
	 * the "loop()" function in your sketch and it will take care of checking for any reset or
	 * reprogram requests */
	reset.check();
}
