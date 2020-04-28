/*
 * AthenaEEPROM.cpp - Athena Bootloader helper library
 * Copyright (c) 2012 Stylianos Tsampas.  All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "AthenaEEPROM.h"
#include "AthenaEEPROM_defs.h"

/****************************************************
 * Definitions                                      *
 ****************************************************/

AthenaEEPROMClass::AthenaEEPROMClass()
{
	if(read(ATHENA_SIGPOS, 0) == ATHENA_SIGVAL)
	{
		_offset = ATHENA_OFFSET;
	}
	else
	{
		_offset = NO_OFFSET;
	}
}

/*
 * General EEPROM Interface
 */
uint8_t AthenaEEPROMClass::read(int address, uint8_t offset)
{
	return eeprom_read_byte((unsigned char*)address + offset);
}

void AthenaEEPROMClass::write(int address, uint8_t value, uint8_t offset)
{
	eeprom_write_byte((unsigned char*)address + offset, value);
}

uint8_t AthenaEEPROMClass::read(int address)
{
	return read(address, _offset);
}

void AthenaEEPROMClass::write(int address, uint8_t value)
{
	write(address, value, _offset);
}

/*
 * Netowork functions
 */
void AthenaEEPROMClass::writeNetSig(void)
{
	write(NETEEPROM_SIG_1, NETEEPROM_SIG_1_VALUE, 0); // Set signature 1 to load eeprom settings
	write(NETEEPROM_SIG_2, NETEEPROM_SIG_2_VALUE, 0); // Set signature 2
}

void AthenaEEPROMClass::writeAddr(IPAddress addr, byte start)
{
	for(byte i = 0; i < 4; i++)
		write(i + start, addr[i], 0);
}

void AthenaEEPROMClass::writeIP(IPAddress ip)
{
	writeAddr(ip, NETEEPROM_IP);
}

void AthenaEEPROMClass::writeGW(IPAddress gw)
{
	writeAddr(gw, NETEEPROM_GW);
}

void AthenaEEPROMClass::writeSN(IPAddress sn)
{
	writeAddr(sn, NETEEPROM_SN);
}

IPAddress AthenaEEPROMClass::readAddr(byte start)
{
	byte octet[4];
	for(byte i = 0; i < 4; i++)
		octet[i] = read(i, start);
	IPAddress Addr(octet);
	return (Addr);
}

/*
 * Port functions
 */
void AthenaEEPROMClass::writePortSig()
{
	write(NETEEPROM_SIG_3, NETEEPROM_SIG_3_VALUE, 0);
}

/*
 * Password functions
 */
void AthenaEEPROMClass::writePassSig()
{
	write(NETEEPROM_SIG_4, NETEEPROM_SIG_4_VALUE, 0);
}

/****************************************************
 * User API                                         *
 ****************************************************/

/*
 * Image status functions
 */
void AthenaEEPROMClass::writeImgBad(void)
{
	write(NETEEPROM_IMG_STAT, NETEEPROM_IMG_BAD_VALUE, 0); // Image status set to invalid
}

void AthenaEEPROMClass::writeImgOk(void)
{
	write(NETEEPROM_IMG_STAT, NETEEPROM_IMG_OK_VALUE, 0); // Image status set to valid
}

/* Netowork functions */
void AthenaEEPROMClass::eraseNetSig(void)
{
	write(NETEEPROM_SIG_1, 0, 0); // Unset signature 1 to load built-in settings
	write(NETEEPROM_SIG_2, 0, 0); // Unset signature 2
}

void AthenaEEPROMClass::writeMAC(byte* mac)
{
	for(byte i = 0; i < 6; i++)
		write(i + NETEEPROM_MAC, mac[i], 0);
}

void AthenaEEPROMClass::writeNet(byte* mac, IPAddress ip, IPAddress gw, IPAddress sn)
{
	writeMAC(mac);
	writeIP(ip);
	writeGW(gw);
	writeSN(sn);

	writeNetSig();
}

bool AthenaEEPROMClass::netSigIsSet(void)
{
	if((read(NETEEPROM_SIG_1, 0) == NETEEPROM_SIG_1_VALUE) &&
	   (read(NETEEPROM_SIG_2, 0) == NETEEPROM_SIG_2_VALUE))
		return (true);
	else
		return (false);
}

byte* AthenaEEPROMClass::readMAC(void)
{
	byte default_mac[6] = {DEFAULT_MAC_ADDR};
	byte* mac = (byte*)malloc(6 * sizeof(byte));

	memcpy((void*)mac, (void*)default_mac, 6);

	if(netSigIsSet())
		for(byte i = 0; i < 6; i++)
			mac[i] = read(i, NETEEPROM_MAC);

	return (mac);
}

IPAddress AthenaEEPROMClass::readIP(void)
{
	IPAddress ip(DEFAULT_IP_ADDR);
	if(netSigIsSet())
		ip = readAddr(NETEEPROM_IP);
	return ip;
}

IPAddress AthenaEEPROMClass::readGW(void)
{
	IPAddress gw(DEFAULT_GW_ADDR);
	if(netSigIsSet())
		gw = readAddr(NETEEPROM_GW);
	return (gw);
}

IPAddress AthenaEEPROMClass::readSN(void)
{
	IPAddress sn(DEFAULT_SUB_MASK);
	if(netSigIsSet())
		sn = readAddr(NETEEPROM_SN);
	return (sn);
}

void AthenaEEPROMClass::printNet(HardwareSerial* serial)
{
	serial->println("--- Network Settings ---");

	byte* mac = readMAC();
	serial->print("    MAC: ");
	for(uint8_t i = 0; i < 6; i++)
	{
		serial->print("0x");
		serial->print(mac[i], HEX);
		if(i != 5)
			serial->print(".");
		else
			serial->println();
	}
	free(mac);

	serial->print("Address: ");
	serial->println(readIP());

	serial->print("Gateway: ");
	serial->println(readGW());

	serial->print(" Subnet: ");
	serial->println(readSN());
}

/*
 * Port functions
 */
void AthenaEEPROMClass::erasePortSig(void)
{
	write(NETEEPROM_SIG_3, 0, 0);
}

void AthenaEEPROMClass::writePort(word port)
{
	write(NETEEPROM_PORT, (port & 0xFF), 0);
	write(NETEEPROM_PORT + 1, (port >> 8), 0);

	writePortSig();
}

bool AthenaEEPROMClass::portSigIsSet(void)
{
	if(read(NETEEPROM_SIG_3, 0) == NETEEPROM_SIG_3_VALUE)
		return (true);
	else
		return (false);
}

word AthenaEEPROMClass::readPort(void)
{
	if(portSigIsSet())
	{
		byte loByte = read(NETEEPROM_PORT, 0);
		byte hiByte = read(NETEEPROM_PORT + 1, 0);
		return (makeWord(hiByte, loByte));
	}
	else
		return (46969);
}

void AthenaEEPROMClass::printPort(HardwareSerial* serial)
{
	serial->println("--- Tftp Settings ---");

	serial->print("Data Port: ");
	serial->println(readPort());
}

/*
 * Password functions
 */
void AthenaEEPROMClass::erasePassSig(void)
{
	write(NETEEPROM_SIG_4, 0, 0);
}

void AthenaEEPROMClass::writePass(String passwd)
{
	uint8_t i;
	uint8_t pass_size = passwd.length();

	if(pass_size > (NETEEPROM_END - NETEEPROM_PASS))
		pass_size = NETEEPROM_END - NETEEPROM_PASS;

	for(i = 0; i < pass_size; i++)
		write(i, passwd.charAt(i), NETEEPROM_PASS);
	write(pass_size, '\0', NETEEPROM_PASS);

	writePassSig();
}

bool AthenaEEPROMClass::passSigIsSet(void)
{
	if(read(NETEEPROM_SIG_4, 0) == NETEEPROM_SIG_4_VALUE)
		return (true);
	else
		return (false);
}

String AthenaEEPROMClass::readPass(void)
{
	String password;

	if(passSigIsSet())
	{
		uint8_t c, i = 0;
		char passwd[NETEEPROM_END - NETEEPROM_PASS + 1];

		while((c = read(i, NETEEPROM_PASS)) != '\0')
			passwd[i++] = c;
		passwd[i] = '\0';

		password = passwd;
	}
	else
		password = "athena";

	return password;
}

void AthenaEEPROMClass::printPass(HardwareSerial* serial)
{
	serial->println("--- Reset Server Settings ---");

	serial->print("Password: ");
	serial->println(readPass());

	serial->print("  Length: ");
	serial->println(readPass().length());
}

/*
 * General print function
 */
void AthenaEEPROMClass::print(HardwareSerial* serial)
{
	if(netSigIsSet())
		printNet(serial);
	if(portSigIsSet())
		printPort(serial);
	if(passSigIsSet())
		printPass(serial);
}

void AthenaEEPROMClass::printAll(HardwareSerial* serial)
{
	printNet(serial);
	if(!netSigIsSet())
		serial->println("(Defaults)");

	printPort(serial);
	if(!portSigIsSet())
		serial->println("(Default)");

	printPass(serial);
	if(!passSigIsSet())
		serial->println("(No password)");
}

AthenaEEPROMClass EEPROM;
