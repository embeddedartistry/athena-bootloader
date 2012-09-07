#ifndef NetEEPROM_defs_h
#define NetEEPROM_defs_h

#define NETEEPROM_SETTINGS_OFFSET 3
/* EEPROM partitioning */
#define NETEEPROM_SIG_1    0
#define NETEEPROM_SIG_2    1
//#define SIG_3    2 /* This is going to be the port signature byte */
#define NETEEPROM_IMG_STAT 2
#define NETEEPROM_DATA     3
#define NETEEPROM_GW       3
#define NETEEPROM_SN       7
#define NETEEPROM_MAC      11
#define NETEEPROM_IP       17
#define NETEEPROM_PORT     21
/* EERPOM values */
#define NETEEPROM_SIG_1_VAL   (0x55)
#define NETEEPROM_SIG_2_VAL   (0xAA)
//#define NETEEPROM_SIG_3_VAL   (0xCC) /* This is going to be the port signature value */
#define NETEEPROM_IMG_OK_VAL  (0xBB)
#define NETEEPROM_IMG_BAD_VAL (0xFF)

#endif