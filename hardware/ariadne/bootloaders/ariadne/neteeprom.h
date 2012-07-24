#include <avr/eeprom.h>

#define EEPROM_SIG_1    ((uint8_t*)0)
#define EEPROM_SIG_2    ((uint8_t*)1)
//#define EEPROM_SIG_3    ((uint8_t*)2) /* This is going to be the port signature byte */
#define EEPROM_IMG_STAT ((uint8_t*)2)
#define EEPROM_DATA     ((uint8_t*)3)
#define EEPROM_GW       ((uint8_t*)3)
#define EEPROM_SN       ((uint8_t*)7)
#define EEPROM_MAC      ((uint8_t*)11)
#define EEPROM_IP       ((uint8_t*)17)
#define EEPROM_PORT     ((uint8_t*)21)

#define EEPROM_SETTINGS_SIZE 18
#define REGISTER_BLOCK_SIZE  28

#define EEPROM_SIG_1_VALUE   (0x55)
#define EEPROM_SIG_2_VALUE   (0xAA)
//#define EEPROM_SIG_3_VAL     (0xCC) /* This is going to be the port signature value */
#define EEPROM_IMG_OK_VALUE  (0xBB)
#define EEPROM_IMG_BAD_VALUE (0xFF)
