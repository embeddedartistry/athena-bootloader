#ifndef NetEEPROM_h
#define NetEEPROM_h

#include <Arduino.h>
#include <EEPROM.h>

#define NETWORK_SETTINGS_SIZE   18

class NetEEPROMClass: public EEPROMClass {
	private:
		/* Write signature bytes in eeprom, informing the bootloader that
		 * the network settings will be loaded from the eeprom */
		void writeSig();
		void readIP(uint8_t ip[4]);
		void readSN(uint8_t sn[4]);
		void readGW(uint8_t gw[4]);
		void readMAC(uint8_t mac[4]);
		uint16_t readPort(void);
	public:
		/* Erase signature bytes in eeprom to revert the bootloader to
		 * built-in settings */
		void eraseSig();
		/* Check if the signature bytes are set, returning 1 if they are
		 * set or 0 if they are unset */
		uint8_t sigIsSet();
		/* Set the flashed image status to bad, to prevent the bootloader
		 * from timing out */
		void writeImgBad();
		/* Set the flashed image status to good, to allow the bootloader
		 * to load the program in memory */
		void writeImgOk();
		/* Write the network settings to EEPROM from a settings array */
		void writeNet(uint8_t settings[NETWORK_SETTINGS_SIZE], uint16_t port);
		/* Write the network settings to EEPROM as individual arrays */
		void writeNet(uint8_t ip[4], uint8_t sn[4], uint8_t gw[4], uint8_t mac[6], uint16_t port);
		/* Read the network settings to a table with size NETWORK_SETTINGS_SIZE
		 * that is given as the only argument */
		void readNet(uint8_t settings[NETWORK_SETTINGS_SIZE], uint16_t* port);
		/* Read the network settings to EEPROM to individual arrays */
		void readNet(uint8_t ip[4], uint8_t sn[4], uint8_t gw[4], uint8_t mac[6], uint16_t* port);
		/* Print the network settings previously loaded in the an array
		 * using the NetEEPROM::Read function */
		void printNet(uint8_t settings[NETWORK_SETTINGS_SIZE], uint16_t* port);
		/* Print the network settings from previously loaded arrays */
		void printNet(uint8_t ip[4], uint8_t sn[4], uint8_t gw[4], uint8_t mac[6], uint16_t* port);
		/* Print the network settings */
		void print(void);
};

extern NetEEPROMClass NetEEPROM;

#endif
