#include "NetEEPROM.h"
#include "NetEEPROM_defs.h"


/*
 * Image status functions
 */
void NetEEPROMClass::writeImgBad(void)
{
	EEPROM.write(NETEEPROM_IMG_STAT, NETEEPROM_IMG_BAD_VALUE); // Image status set to invalid
}

void NetEEPROMClass::writeImgOk(void)
{
	EEPROM.write(NETEEPROM_IMG_STAT, NETEEPROM_IMG_OK_VALUE); // Image status set to valid
}



/*
 * Netowork functions
 */
void NetEEPROMClass::writeNetSig(void)
{
	EEPROM.write(NETEEPROM_SIG_1, NETEEPROM_SIG_1_VALUE); // Set signature 1 to load eeprom settings
	EEPROM.write(NETEEPROM_SIG_2, NETEEPROM_SIG_2_VALUE); // Set signature 2
}

void NetEEPROMClass::eraseNetSig(void)
{
	EEPROM.write(NETEEPROM_SIG_1, 0xFF); // Unset signature 1 to load built-in settings
	EEPROM.write(NETEEPROM_SIG_2, 0xFF); // Unset signature 2
}

void NetEEPROMClass::writeMAC(byte* mac)
{
	for(byte i = 0; i < 6; i++)	EEPROM.write(i + NETEEPROM_MAC, mac[i]);
}

void NetEEPROMClass::writeAddr(IPAddress addr, byte start)
{
	for(byte i = 0; i < 4; i++)	EEPROM.write(i + start, addr[i]);
}

void NetEEPROMClass::writeIP(IPAddress ip)
{
	writeAddr(ip, NETEEPROM_IP);
}

void NetEEPROMClass::writeGW(IPAddress gw)
{
	writeAddr(gw, NETEEPROM_GW);
}

void NetEEPROMClass::writeSN(IPAddress sn)
{
	writeAddr(sn, NETEEPROM_SN);
}

void NetEEPROMClass::writeNet(byte* mac, IPAddress ip, IPAddress gw, IPAddress sn)
{
	writeMAC(mac);
	writeIP(ip);
	writeGW(gw);
	writeSN(sn);

	writeNetSig();
}

bool NetEEPROMClass::netSigIsSet(void)
{
	if((EEPROM.read(NETEEPROM_SIG_1) == NETEEPROM_SIG_1_VALUE)
		&& (EEPROM.read(NETEEPROM_SIG_2) == NETEEPROM_SIG_2_VALUE)) return(true);
	else return(false);
}

byte* NetEEPROMClass::readMAC(void)
{
	byte default_mac[6] = {DEFAULT_MAC_ADDR};
	byte* mac = (byte*)malloc(6 * sizeof(byte));

	memcpy((void*)mac, (void*)default_mac, 6);

	if(netSigIsSet())
		for(byte i = 0; i < 6; i++) mac[i] = EEPROM.read(i + NETEEPROM_MAC);

	return(mac);
}

IPAddress NetEEPROMClass::readAddr(byte start)
{
	byte octet[4];
	for(byte i = 0; i < 4; i++) octet[i] = EEPROM.read(i + start);
	IPAddress Addr(octet);
	return(Addr);
}

IPAddress NetEEPROMClass::readIP(void)
{
	IPAddress ip(DEFAULT_IP_ADDR);
	if(netSigIsSet()) ip = readAddr(NETEEPROM_IP);
	return ip;
}

IPAddress NetEEPROMClass::readGW(void)
{
	IPAddress gw(DEFAULT_GW_ADDR);
	if(netSigIsSet()) gw = readAddr(NETEEPROM_GW);
	return(gw);
}

IPAddress NetEEPROMClass::readSN(void)
{
	IPAddress sn(DEFAULT_SUB_MASK);
	if(netSigIsSet()) sn = readAddr(NETEEPROM_SN);
	return(sn);
}

void NetEEPROMClass::printNet(byte* mac, IPAddress ip, IPAddress gw, IPAddress sn)
{
	if(netSigIsSet()) {
		byte i;

		Serial.print("    MAC: ");
		for(i = 0; i < 6; i++) {
			Serial.print("0x");
			Serial.print(mac[i], HEX);
			if(i != 5) Serial.print(".");
			else Serial.println();
		}
		Serial.print("Address: ");
		Serial.println(ip);
		Serial.print("Gateway: ");
		Serial.println(gw);
		Serial.print(" Subnet: ");
		Serial.println(sn);
		Serial.println();
	} else Serial.print("Using built in settings");
}



/*
 * Port functions
 */
void NetEEPROMClass::writePortSig()
{
	EEPROM.write(NETEEPROM_SIG_3, NETEEPROM_SIG_3_VALUE);
}

void NetEEPROMClass::erasePortSig(void)
{
	EEPROM.write(NETEEPROM_SIG_3, 0xFF);
}

void NetEEPROMClass::writePort(word port)
{
	EEPROM.write(NETEEPROM_PORT, (port & 0xFF));
	EEPROM.write(NETEEPROM_PORT + 1, (port >> 8));

	writePortSig();
}

bool NetEEPROMClass::portSigIsSet(void)
{
	if(EEPROM.read(NETEEPROM_SIG_3) == NETEEPROM_SIG_3_VALUE) return(true);
	else return(false);
}

word NetEEPROMClass::readPort(void)
{
	if(portSigIsSet()){
		byte loByte = EEPROM.read(NETEEPROM_PORT);
		byte hiByte = EEPROM.read(NETEEPROM_PORT + 1);
		return(makeWord(hiByte, loByte));
	} else return(46969);
}



/*
 * Password functions
 */
void NetEEPROMClass::writePassSig()
{
	EEPROM.write(NETEEPROM_SIG_4, NETEEPROM_SIG_4_VALUE);
}

void NetEEPROMClass::erasePassSig(void)
{
	EEPROM.write(NETEEPROM_SIG_4, 0xFF);
}

void NetEEPROMClass::writePass(String passwd)
{
	uint8_t i;
	uint8_t pass_size = passwd.length();
	if(pass_size > (NETEEPROM_END-NETEEPROM_PASS)) pass_size = NETEEPROM_END-NETEEPROM_PASS;

	EEPROM.write(NETEEPROM_PSIZE, pass_size);

	for(i=0; i<=pass_size; i++) EEPROM.write(NETEEPROM_PASS+i, passwd.charAt(i));

	writePassSig();
}

bool NetEEPROMClass::passSigIsSet(void)
{
	if(EEPROM.read(NETEEPROM_SIG_4) == NETEEPROM_SIG_4_VALUE) return(true);
	else return(false);
}

String NetEEPROMClass::readPass(void)
{
	String password;

	if(passSigIsSet()) {
		uint8_t pass_size = EEPROM.read(NETEEPROM_PSIZE);
		DBG(Serial.print("Password size: ");
			Serial.println(pass_size);
		)
		char passwd[pass_size];

		for(uint8_t i=0; i<pass_size; i++) {
			passwd[i] = EEPROM.read(NETEEPROM_PASS+i);
		}
		password = passwd;

	} else password = "";

	return password;
}



/*
 * General print function
 */
void NetEEPROMClass::print(void)
{
	printNet(readMAC(), readIP(), readGW(), readSN());
}

NetEEPROMClass NetEEPROM;
