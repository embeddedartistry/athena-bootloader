#ifndef NetEEPROM_h
#define NetEEPROM_h

#include <Arduino.h>
#include <EEPROM.h>
#include <IPAddress.h>

#define NETWORK_SETTINGS_SIZE   18

class NetEEPROMClass
{
	private:
		/* Write signature bytes in eeprom, informing the bootloader that
		 * the network settings will be loaded from the eeprom */
		void writeNetSig();
		/* Same as above but for the port */
		void writePortSig();
		/* Same for password */
		void writePassSig();

		/* Basic method to read addresses used in public functions */
		IPAddress readAddr(byte start);

		void read(byte settings[NETWORK_SETTINGS_SIZE], word* port);
		void print(byte settings[NETWORK_SETTINGS_SIZE], word port);
		/* Print the network settings from previously loaded arrays */
		void print(byte* mac, IPAddress ip, IPAddress gw, IPAddress sn, word port);

	public:
		/* Erase signature bytes in eeprom to revert the bootloader to
		 * built-in settings */
		void eraseNetSig(void);
		void erasePortSig(void);
		void erasePassSig(void);
		/* Set the flashed image status to bad, to prevent the bootloader
		 * from timing out */
		void writeImgBad(void);
		/* Set the flashed image status to good, to allow the bootloader
		 * to load the program in memory */
		void writeImgOk(void);
		/* Write the network settings to EEPROM as individual arrays */
		void writeNet(byte* mac, IPAddress ip, IPAddress gw, IPAddress sn);
		void writeNet(byte* mac, IPAddress ip, IPAddress gw, IPAddress sn, word port);

		/* Check if the signature bytes are set, returning 1 if they are
		 * set or 0 if they are unset */
		bool netSigIsSet(void);
		bool portSigIsSet(void);
		bool passSigIsSet(void);
		byte* readMAC(void);
		IPAddress readIP(void);
		IPAddress readGW(void);
		IPAddress readSN(void);
		/* Function to return only the port */
		word readPort(void);

		/* Print the network settings */
		void print(void);
};

extern NetEEPROMClass NetEEPROM;

#endif
