#include "NetEEPROM.h"
#include "NetEEPROM_defs.h"


/*
 * Private functions
 */
void NetEEPROMClass::writeSig(void)
{
	EEPROM.write(NETEEPROM_SIG_1, NETEEPROM_SIG_1_VAL); // Set signature 1 to load eeprom settings
	EEPROM.write(NETEEPROM_SIG_2, NETEEPROM_SIG_2_VAL); // Set signature 2
}

IPAddress NetEEPROMClass::readAddr(byte start)
{
	byte octet[4];
	for(byte address = start; address < start + 4; address++)
		octet[address - start] = EEPROM.read(address);
	IPAddress Addr(octet);
	return(Addr);
}

void NetEEPROMClass::readNet(byte settings[NETWORK_SETTINGS_SIZE], word* port)
{
	for(byte address = 0; address < NETWORK_SETTINGS_SIZE; address++)
		settings[address] = EEPROM.read(address + NETEEPROM_SETTINGS_OFFSET);

	*port = readPort();
}

void NetEEPROMClass::printNet(byte settings[NETWORK_SETTINGS_SIZE], word port)
{
	if(sigIsSet()) {
		byte i;

		for(i = 0; i < NETWORK_SETTINGS_SIZE; i++) {
			if(i == 0)
				Serial.print("Gateway: ");
			else if(i == 4) {
				Serial.println();
				Serial.print(" Subnet: ");
			} else if(i == 8) {
				Serial.println();
				Serial.print("    MAC: ");
			} else if(i == 14) {
				Serial.println();
				Serial.print("Address: ");
			} else Serial.print(".");
			if(i >= 8 && i < 14) {
				Serial.print("0x");
				Serial.print(settings[i], HEX);
			} else Serial.print(settings[i]);
		}
		Serial.println();
		Serial.print("   Port: ");
		Serial.print(port);
	} else Serial.print("Using built in settings");
}


/*
 * Public functions
 */
void NetEEPROMClass::eraseSig(void)
{
	EEPROM.write(NETEEPROM_SIG_1, 0xFF); // Unset signature 1 to load built-in settings
	EEPROM.write(NETEEPROM_SIG_2, 0xFF); // Unset signature 2
}

void NetEEPROMClass::writeImgBad(void)
{
	EEPROM.write(NETEEPROM_IMG_STAT, NETEEPROM_IMG_BAD_VAL); // Image status set to invalid
}

void NetEEPROMClass::writeImgOk(void)
{
	EEPROM.write(NETEEPROM_IMG_STAT, NETEEPROM_IMG_OK_VAL); // Image status set to valid
}

void NetEEPROMClass::writeNet(byte* mac, IPAddress ip, IPAddress gw, IPAddress sn, word port)
{
	byte settings[NETWORK_SETTINGS_SIZE] = {
		gw[0], gw[1], gw[2], gw[3],
		sn[0], sn[1], sn[2], sn[3],
		mac[0], mac[1], mac[2],
		mac[3], mac[4], mac[5],
		ip[0], ip[1], ip[2], ip[3]
	};

	for(byte address = 0; address < NETWORK_SETTINGS_SIZE; address++) {
		EEPROM.write(address + NETEEPROM_SETTINGS_OFFSET, settings[address]);
	}
	EEPROM.write(NETEEPROM_PORT, (port & 0xFF));
	EEPROM.write(NETEEPROM_PORT + 1, (port >> 8));

	writeSig();
}


/*
 * Read functions
 */
bool NetEEPROMClass::sigIsSet(void)
{
	if((EEPROM.read(NETEEPROM_SIG_1) == NETEEPROM_SIG_1_VAL)
	        && (EEPROM.read(NETEEPROM_SIG_2) == NETEEPROM_SIG_2_VAL)) return(true);
	else return(false);
}

byte* NetEEPROMClass::readMAC(void)
{
	byte* mac = (byte*)malloc(6 * sizeof(byte));
	for(byte address = NETEEPROM_MAC; address < NETEEPROM_MAC + 6; address++)
		mac[address - NETEEPROM_MAC] = EEPROM.read(address);
	return(mac);
}

IPAddress NetEEPROMClass::readIP(void)
{
	return(readAddr(NETEEPROM_IP));
}

IPAddress NetEEPROMClass::readGW(void)
{
	return(readAddr(NETEEPROM_GW));
}

IPAddress NetEEPROMClass::readSN(void)
{
	return(readAddr(NETEEPROM_SN));
}

word NetEEPROMClass::readPort(void)
{
	byte loByte = EEPROM.read(NETEEPROM_PORT);
	byte hiByte = EEPROM.read(NETEEPROM_PORT + 1);
	return(makeWord(hiByte, loByte));
}


/*
 * Print functions
 */
void NetEEPROMClass::printNet(byte* mac, IPAddress ip, IPAddress gw, IPAddress sn, word port)
{
	if(sigIsSet()) {
		byte i;

		Serial.print("    MAC: ");
		for(i = 0; i < 6; i++) {
			Serial.print("0x");
			Serial.print(mac[i], HEX);
			if(i != 5) Serial.print(".");
			else Serial.println();
		}
		Serial.print("Address: ");
		for(i = 0; i < 4; i++) {
			Serial.print(ip[i]);
			if(i != 3) Serial.print(".");
			else Serial.println();
		}
		Serial.print("Gateway: ");
		for(i = 0; i < 4; i++) {
			Serial.print(gw[i]);
			if(i != 3) Serial.print(".");
			else Serial.println();
		}
		Serial.print(" Subnet: ");
		for(i = 0; i < 4; i++) {
			Serial.print(sn[i]);
			if(i != 3) Serial.print(".");
			else Serial.println();
		}
		Serial.print("   Port: ");
		Serial.print(port);
		Serial.println();
	} else Serial.print("Using built in settings");
}

void NetEEPROMClass::print(void)
{
	printNet(readMAC(), readIP(), readGW(), readSN(), readPort());
}

NetEEPROMClass NetEEPROM;
