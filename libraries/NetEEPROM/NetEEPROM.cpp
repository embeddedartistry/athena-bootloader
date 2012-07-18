#include "NetEEPROM.h"


void NetEEPROM::WriteSig()
{
	EEPROM.write(0, 0x55); // Set signature 1 to load eeprom settings
	EEPROM.write(1, 0xAA); // Set signature 2
}

void NetEEPROM::EraseSig()
{
	EEPROM.write(0, 0xFF); // Unset signature 1 to load built-in settings
	EEPROM.write(1, 0xFF); // Unset signature 2
}

uint8_t NetEEPROM::SigIsSet()
{
	if((EEPROM.read(0)==0x55) && (EEPROM.read(1)==0xAA)) return(1);
	else return(0);
}

void NetEEPROM::WriteImgBad()
{
	EEPROM.write(2, 0xFF); // Image status set to invalid
}

void NetEEPROM::WriteImgOk()
{
	EEPROM.write(2, 0xBB); // Image status set to valid
}

void NetEEPROM::Write(uint8_t table[])
{
	for(int address = 0; address < EEPROM_SETTINGS_SIZE; address++) {
		EEPROM.write(address+3, table[address]);
	}
}

void NetEEPROM::Write(uint8_t ip[4], uint8_t sb[4], uint8_t gw[4], uint8_t mac[6])
{
	uint8_t table[18] = {gw[0], gw[1], gw[2], gw[3],
							sb[0], sb[1], sb[2], sb[3],
							mac[0], mac[1], mac[2],
							mac[3], mac[4], mac[5],
							ip[0], ip[1], ip[2], ip[3]};

	Write(table);
}

void NetEEPROM::Read(uint8_t *table)
{
	for(int address = 0; address < EEPROM_SETTINGS_SIZE; address++) {
		table[address] = EEPROM.read(address+3);
	}
}
