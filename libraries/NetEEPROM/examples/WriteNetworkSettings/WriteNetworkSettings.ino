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
   (if connected) will flash rapdily. The arduino will reset after 4 seconds

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
#include <EEPROM.h>
#include <NetEEPROM.h>

byte  mac[] = {0x12, 0x34, 0x56,
			   0x78, 0x9A, 0xBC};
IPAddress  ip(192, 168, 1, 120);
IPAddress  gateway(192, 168, 1, 1);
IPAddress  subnet(255, 255, 255, 0);
/* Because of limitations in the bootloader, if you set the network settings you have
 * to set the pseudo-random port for the tftp data transfer. If you have more than one arduinos
 * with ethernet behind your router that you want to program from outside your local
 * network you need a unique value for each one value. The initial negotiation of tftp still
 * happens at port [69]. You will also need to forwards these ports from your router.
 * The value set below is the built-in default [46969]. */
uint16_t port = 46969;


/* Preset for Arduino Uno with Ethernet shield.
 * For Arduino Ethernet set this to 9 */
int ledpin = 13;

void setup()
{
	/* Write the new settings values to EEPROM */
	NetEEPROM.writeNet(mac, ip, gateway, subnet, port);
	/* Set image status to bad, so upon reboot, the bootloader won't time out */
	NetEEPROM.writeImgBad();

	pinMode(ledpin, OUTPUT);
	/* Timeout and reset after 4 seconds */
	wdt_enable(WDTO_4S);
}


void loop()
{
	/* Blink an LED on pin defined above rapidly if present
	 * to indicate the settings have been stored in EEPROM. */
	digitalWrite(ledpin, HIGH);
	delay(500);
	digitalWrite(ledpin, LOW);
	delay(500);
}
