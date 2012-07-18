/*
 * ReadNetworkSettings
 *
 *
 */

#include <avr/wdt.h>
#include <Arduino.h>
#include <USBAPI.h>
#include <EEPROM.h>
#include <NetEEPROM.h>

NetEEPROM neteeprom;
uint8_t settings[EEPROM_SETTINGS_SIZE];

// Preset for Arduino Uno with ethernet shield
// For Arduino Ethernet set this to 9
int ledpin = 13;

void setup()
{
	neteeprom.Read(settings);

	// initialize serial and wait for port to open:
	Serial.begin(9600);
	while(!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}
	Serial.println();

	if(neteeprom.SigIsSet()) {
		uint8_t i;

		for(i = 0; i < EEPROM_SETTINGS_SIZE; i++) {
			if(i == 0)
				Serial.print("Gateway: ");
			else if(i == 4) {
				Serial.println();
				Serial.print(" Subnet: ");
			}
			else if(i == 8) {
				Serial.println();
				Serial.print("    MAC: ");
			}
			else if(i == 14) {
				Serial.println();
				Serial.print("Address: ");
			}
			else Serial.print(".");

			if (i >= 8 && i < 14) {
				Serial.print("0x");
				Serial.print(settings[i], HEX);
			} else Serial.print(settings[i]);
		}
	} else {
		Serial.print("Using built in settings");
	}

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
