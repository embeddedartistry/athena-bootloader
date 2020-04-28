/*
 * WriteNetworkSettings
 *
 *  This sketch writes a set of network configuration settings to EEPROM
 *  in such a way that both the Athena bootloader & the AthenaEthernetReset server
 *  can read them back later and share the same settings.
 *
 *  As commented below these are the settings available for modification:
 *
 *    * Gateway IP address
 *    * Subnet mask
 *    * MAC address
 *    * Arduino device IP address
 *
 *  Once the settings have been written to EEPROM, the values will be read
 *  from the EEPROM. The Arduino will reset after 4 seconds.
 */

#include <avr/wdt.h>
#include <AthenaEEPROM.h>

/*
 * You have to set each and every one of these variables below to their correct values
 * for your network. The bootloader won't function correctly if one of these is wrong.
 * For the MAC Address, you can set it to anything you like, but using the one provided
 * with your Ethernet Shield or Arduino Ethernet is strongly suggested.
 */
byte  mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress  ip(192, 168, 1, 120);
IPAddress  gateway(192, 168, 1, 1);
IPAddress  subnet(255, 255, 255, 0);

/*
 * You might want to also change the TFTP transfer port. If you have more than one Arduino
 * your router that you want to program from outside your local network you need a unique value for each device.
 * The initial negotiation of tftp still happens at port [69]. You will also need to forwards these ports from your router.
 * The value set below is the built-in default [46969].
 */
word port = 46969;

/*
 * This is the "password" for the reset server in case you want to reset your Arduino
 * remotely. This setting has no effect on the bootloader itself. Password can be 36 characters
 * long, at maximum. Any string longer than 36 characters will be truncated.
 * Spaces should not be used
 */
String password = "athena";


void setup()
{
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
	/* Wait for serial port to connect. Needed for Leonardo only
	 * but breaks compilation on Arduino 1.0. Leonardo is not
	 * supported by the bootloader yet */
	while(!Serial)
	{
	delay(10);
	}

	/* Write the new settings values to EEPROM */
	EEPROM.writeNet(mac, ip, gateway, subnet);
	/* Set the TFTP data transfer port in EEPROM. Change it only if you need to. */
	EEPROM.writePort(port);
	/* Write Reset Server password in EEPROM */
	EEPROM.writePass(password);

	/* Print the network settings to serial */
	EEPROM.printAll(&Serial);

	/* Set image status to bad, so upon reboot the bootloader won't load the previous sketch */
	EEPROM.writeImgBad();

	/* Timeout and reset after 4 seconds */
	wdt_enable(WDTO_4S);
}


void loop()
{
	delay(500);
}
