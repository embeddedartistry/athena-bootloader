#ifndef NetEEPROM_h
#define NetEEPROM_h

#include <Arduino.h>
#include <EEPROM.h>
#include <IPAddress.h>

#define NETWORK_SETTINGS_SIZE   18

#define DEBUG
#ifdef DEBUG
#define DBG(c) c
#else
#define DBG(c)
#endif

class NetEEPROMClass {

	private:
		/*
		 * Network functions
		 */
		/** Write signature bytes in eeprom, informing the bootloader that
		 *  the network settings will be loaded from the eeprom */
		void writeNetSig();
		/** Generic function to write IP addresses to EEPROM */
		void writeAddr(IPAddress addr, byte start);
		/** Basic method to read addresses used in public functions */
		IPAddress readAddr(byte start);
		/** Print the network settings in a standartized way. Serial needs to be
		 *  initalized in the sketch */
		void printNet(byte* mac, IPAddress ip, IPAddress gw, IPAddress sn);

		/*
		 * Port functions
		 */
		/** Write port signature byte to enable reading the TFTP data port from the bootloader */
		void writePortSig();
		/** Print the port number to the serial. Serial needs to be initalized in the sketch */
		void printPort(word port);

		/*
		 * Password functions
		 */
		/** Write password signature to enable reading of password from EEPROM */
		void writePassSig();
		/** Print password stored in EEPROM to serial. Serial has to be initalized in the sketch */
		void printPass(String passwd);


	public:
		/** @name Image Status
		 * Image status functions
		 */
		/** Set the flashed image status to bad, to prevent the bootloader
		 *  from timing out */
		void writeImgBad(void);
		/** Set the flashed image status to good, to allow the bootloader
		 *  to load the program in memory */
		void writeImgOk(void);

		/** @name Networking
		 * Network functions
		 */
		/** Erase signature bytes in eeprom to revert the bootloader to
		 *  built-in networking settings */
		void eraseNetSig(void);
		/** Write MAC address to EEPROM */
		void writeMAC(byte* mac);
		/** Write IP to EEPROM */
		void writeIP(IPAddress ip);
		/** Write gateway to EEPROM */
		void writeGW(IPAddress gw);
		/** Write subnet mask to EEPROM */
		void writeSN(IPAddress sn);
		/** Write the full network settings to the EEPROM and set the netwrk
		 *  settings signature in order to have the bootloader detect them */
		void writeNet(byte* mac, IPAddress ip, IPAddress gw, IPAddress sn);
		/** Query if the bootloader uses the custom network settings. True if set */
		bool netSigIsSet(void);
		/** Read the MAC address from EEPROM and return a pointer to allocated memory*/
		byte* readMAC(void);
		/** Read IP */
		IPAddress readIP(void);
		/** Read Gateway */
		IPAddress readGW(void);
		/** Read Subnet Mask */
		IPAddress readSN(void);

		/** @name Tftp
		 * Port functions
		 */
		/** Erase port signature byte to disable reading the TFTP data port from the bootloader */
		void erasePortSig(void);
		/** Write the port value to EEPROM */
		void writePort(word port);
		/** Query if the port signature is set. Returns true if port is set in the EEPROM */
		bool portSigIsSet(void);
		/** Read the port from EEPROM */
		word readPort(void);

		/** @name Password
		 * Password functions
		 */
		/** Erase password signature byte to disable password in the bootloader.
		 *  No default pass exists */
		void erasePassSig(void);
		/** Write the password to the EEPROM */
		void writePass(String passwd);
		/** Query if the password is set */
		bool passSigIsSet(void);
		/** Read the password from EEPROM */
		String readPass(void);

		/** @name Generic
		 * General purpose functions
		 */
		/** Print all bootloader settings */
		void print(void);
};

extern NetEEPROMClass NetEEPROM;

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4;
