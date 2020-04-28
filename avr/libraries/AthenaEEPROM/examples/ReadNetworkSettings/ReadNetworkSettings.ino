/*
 * ReadNetworkSettings
 *
 * This library uses the AthenaEEPROM library to read the
 * network settings used by the Athena bootloader and AthenaEthernetReset server.
 */

#include <AthenaEEPROM.h>

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

	/* Print the network settings to serial */
	EEPROM.printAll(&Serial);

	/* If you want to invalidate the image after first run, uncomment this
	 * Because Duemilanove doesn't have auto-reset, don't uncomment this or
	 * you will invalidate the image before you open the serial console */
	//EEPROM.WriteImgBad();

	Serial.println("Settings read, you can now upload another sketch.");
}

void loop()
{
	delay(500);
}
