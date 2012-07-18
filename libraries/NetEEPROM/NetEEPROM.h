#include <avr/wdt.h>
#include <EEPROM.h>
#include <USBAPI.h>

#define EEPROM_SETTINGS_SIZE 18

class NetEEPROM {
	public:
		void WriteSig();
		void EraseSig();
		uint8_t SigIsSet();
		void WriteImgBad();
		void WriteImgOk();
		void Write(uint8_t table[18]);
		void Write(uint8_t ip[4], uint8_t sb[4], uint8_t gw[4], uint8_t mac[6]);
		void Read(uint8_t* table);
};
