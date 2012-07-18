/*
 * WriteNetworkSettings

   This sketch writes a set of network configuration settings to EEPROM
   in such a way that both the `tftpboot` bootloader & a sketch
   can read them back later and share the same settings.

   As commented below these are the settings available for modification:

     * Gateway IP address
     * Subnet mask
     * MAC address
     * Arduino device IP address

   Once the settings have been written to EEPROM, an LED on pin 9
   (if connected) will flash rapdily.

   The default settings used by the bootloader:

      192,168,1,254,                       // Gateway IP address
      255,255,255,0,                       // Subnet mask
      0xDE,0xAD,0xBE,0xEF,0xFE,0xED,       // MAC address
      192,168,1,128,                       // Arduino device IP address


   Settings suitable for link-local use:

      169,254,1,1,                         // Gateway IP address
      255,255,0,0,                         // Subnet mask
      0x12,0x34,0x45,0x78,0x9A,0xBC,       // MAC address
      169,254,254,169,                     // Arduino device IP address

 */

#include <avr/wdt.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <NetEEPROM.h>

// TODO: Allow settings to be specified as individual variables.
// TODO: Wrap this all in a library.

NetEEPROM neteeprom;
uint8_t settings[EEPROM_SETTINGS_SIZE] = {
	192,168,1,254,                 // Gateway IP address
	255,255,255,0,                 // Subnet mask
	0x12,0x34,0x45,0x78,0x9A,0xBC, // MAC address
	192,168,1,20,                  // Arduino device IP address
};

// Preset for Arduino Uno with ethernet shield
// For Arduino Ethernet set this to 9
int ledpin = 13;

void setup()
{
	neteeprom.WriteSig();
	neteeprom.WriteImgBad();
	neteeprom.Write(settings);

	pinMode(ledpin, OUTPUT);
	// Timeout and reset after 4 seconds
	wdt_enable(WDTO_4S);
}


void loop()
{
	// Blink an LED on pin defined above rapidly if present
	// to indicate the settings have been stored in EEPROM.
	digitalWrite(ledpin, HIGH);
	delay(500);
	digitalWrite(ledpin, LOW);
	delay(500);
}
