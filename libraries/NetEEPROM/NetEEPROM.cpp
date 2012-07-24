#include "NetEEPROM.h"
#include "NetEEPROM_defs.h"

void NetEEPROMClass::writeSig()
{
	write(NETEEPROM_SIG_1, NETEEPROM_SIG_1_VAL); // Set signature 1 to load eeprom settings
	write(NETEEPROM_SIG_2, NETEEPROM_SIG_2_VAL); // Set signature 2
}

void NetEEPROMClass::eraseSig()
{
	write(NETEEPROM_SIG_1, 0xFF); // Unset signature 1 to load built-in settings
	write(NETEEPROM_SIG_2, 0xFF); // Unset signature 2
}

uint8_t NetEEPROMClass::sigIsSet()
{
	if((read(NETEEPROM_SIG_1)==NETEEPROM_SIG_1_VAL)
		&& (read(NETEEPROM_SIG_2)==NETEEPROM_SIG_2_VAL)) return(1);
	else return(0);
}

void NetEEPROMClass::writeImgBad()
{
	write(NETEEPROM_IMG_STAT, NETEEPROM_IMG_BAD_VAL); // Image status set to invalid
}

void NetEEPROMClass::writeImgOk()
{
	write(NETEEPROM_IMG_STAT, NETEEPROM_IMG_OK_VAL); // Image status set to valid
}

void NetEEPROMClass::writeNet(uint8_t settings[NETWORK_SETTINGS_SIZE], uint16_t port)
{
	for(int address = 0; address < NETWORK_SETTINGS_SIZE; address++) {
		write(address+NETEEPROM_SETTINGS_OFFSET, settings[address]);
	}
	write(NETEEPROM_PORT, (port&0xFF));
	write(NETEEPROM_PORT+1, (port>>8));

	writeSig();
}

void NetEEPROMClass::writeNet(uint8_t ip[4], uint8_t sn[4], uint8_t gw[4], uint8_t mac[6], uint16_t port)
{
	uint8_t settings[NETWORK_SETTINGS_SIZE] =
					{gw[0], gw[1], gw[2], gw[3],
					 sn[0], sn[1], sn[2], sn[3],
					 mac[0], mac[1], mac[2],
					 mac[3], mac[4], mac[5],
					 ip[0], ip[1], ip[2], ip[3]};

	writeNet(settings, port);
}

void NetEEPROMClass::readIP(uint8_t ip[4])
{
	for(int address = NETEEPROM_IP; address < NETEEPROM_IP+4; address++)
		ip[address-NETEEPROM_IP] = read(address);
}

void NetEEPROMClass::readSN(uint8_t sn[4])
{
	for(int address = NETEEPROM_SN; address < NETEEPROM_SN+4; address++)
		sn[address-NETEEPROM_SN] = read(address);
}

void NetEEPROMClass::readGW(uint8_t gw[4])
{
	for(int address = NETEEPROM_GW; address < NETEEPROM_GW+4; address++)
		gw[address-NETEEPROM_GW] = read(address);
}

void NetEEPROMClass::readMAC(uint8_t mac[6])
{
	for(int address = NETEEPROM_MAC; address < NETEEPROM_MAC+6; address++)
		mac[address-NETEEPROM_MAC] = read(address);
}

uint16_t NetEEPROMClass::readPort(void)
{
	uint8_t loByte = read(NETEEPROM_PORT);
	uint8_t hiByte = read(NETEEPROM_PORT+1);
	return((hiByte<<8) + loByte);
}

void NetEEPROMClass::readNet(uint8_t settings[NETWORK_SETTINGS_SIZE], uint16_t* port)
{
	for(int address = 0; address < NETWORK_SETTINGS_SIZE; address++)
		settings[address] = read(address+NETEEPROM_SETTINGS_OFFSET);

	*port = readPort();
}

void NetEEPROMClass::readNet(uint8_t ip[4], uint8_t sn[4], uint8_t gw[4], uint8_t mac[6], uint16_t* port)
{
	readIP(ip);
	readSN(sn);
	readGW(gw);
	readMAC(mac);
	*port = readPort();
}

void NetEEPROMClass::printNet(uint8_t settings[NETWORK_SETTINGS_SIZE], uint16_t* port)
{
	if(sigIsSet()) {
		uint8_t i;

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
		Serial.print(*port);
	} else Serial.print("Using built in settings");
}

void NetEEPROMClass::printNet(uint8_t ip[4], uint8_t sn[4], uint8_t gw[4], uint8_t mac[6], uint16_t *port)
{
	if(sigIsSet()) {
		uint8_t i;

		Serial.print("Address: ");
		for(i=0; i<4; i++) {
			Serial.print(ip[i]);
			if(i!=3) Serial.print(".");
			else Serial.println();
		}
		Serial.print(" Subnet: ");
		for(i=0; i<4; i++) {
			Serial.print(sn[i]);
			if(i!=3) Serial.print(".");
			else Serial.println();
		}
		Serial.print("Gateway: ");
		for(i=0; i<4; i++) {
			Serial.print(gw[i]);
			if(i!=3) Serial.print(".");
			else Serial.println();
		}
		Serial.print("    MAC: ");
		for(i=0; i<6; i++){
			Serial.print("0x");
			Serial.print(mac[i], HEX);
			if(i!=5) Serial.print(".");
			else Serial.println();
		}
		Serial.print("   Port: ");
		Serial.print(*port);
		Serial.println();
	} else Serial.print("Using built in settings");
}


void NetEEPROMClass::print(void)
{
	uint8_t  settings[NETWORK_SETTINGS_SIZE];
	uint16_t port;

	readNet(settings, &port);
	printNet(settings, &port);
}

NetEEPROMClass NetEEPROM;
