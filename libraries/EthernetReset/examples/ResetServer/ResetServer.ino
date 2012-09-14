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

 Analog pin server example
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <EEPROM.h>
#include <NetEEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetReset.h>

/* This is your "password". Set it to something long, unique and not
 * easily guessable. And remember to remove it before releasing your sketch */
char reset_path[] = "reset_server_random_path";
/* Create the reset server. This way your reser server will be at the port you
 * have speciefied in the bootloader for remote uploading. For more information on that
 * look at the "NetEEPROM" library in the "WriteNetworkSettings" sketch.
 * If you want to use your own port, create the object as this
 * "EthernetReset reset(reset_path, port);" where port is a number, i.e. 81 */
EthernetReset reset(reset_path);

EthernetServer server(80);


void setup()
{
// Open serial communications and wait for port to open:
	Serial.begin(9600);
	//while(!Serial) {
	//	; // wait for serial port to connect. Needed for Leonardo only
	//}

	/* For now the Arduino EthShield and the server are being configured using the
	 * settings already stored in the EEPROM and are the same with the ones for Ariadne bootloader.
	 * This means that you *MUST* have updated the network settings on your Arduino with the
	 * "WriteNetworkSettings" sketch.
	 * The "begin()" command takes care of everything, from initializing the EthShield to
	 * starting the web server for resetting. This is why you should always start it before any other
	 * server you might want to have */
	reset.begin();
	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());
}


void loop()
{
	/* After the reset server is running the only thing needed is this command at the start of
	 * the "loop()" function in your sketch and it will take care of checking for any reset or
	 * reprogram requests */
	reset.check();

	// listen for incoming clients
	EthernetClient client = server.available();
	if(client) {
		Serial.println("new client");
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while(client.connected()) {
			if(client.available()) {
				char c = client.read();
				Serial.write(c);
				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				if(c == '\n' && currentLineIsBlank) {
					// send a standard http response header
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connnection: close");
					client.println();
					client.println("<!DOCTYPE HTML>");
					client.println("<html>");
					// add a meta refresh tag, so the browser pulls again every 5 seconds:
					client.println("<meta http-equiv=\"refresh\" content=\"5\">");
					// output the value of each analog input pin
					for(int analogChannel = 0; analogChannel < 6; analogChannel++) {
						int sensorReading = analogRead(analogChannel);
						client.print("analog input ");
						client.print(analogChannel);
						client.print(" is ");
						client.print(sensorReading);
						client.println("<br />");
					}
					client.println("</html>");
					break;
				}
				if(c == '\n') {
					// you're starting a new line
					currentLineIsBlank = true;
				} else if(c != '\r') {
					// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}
		// give the web browser time to receive the data
		delay(10);
		// close the connection:
		client.stop();
		Serial.println("client disonnected");
	}
}

