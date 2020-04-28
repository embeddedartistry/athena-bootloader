# Athena-Aware EEPROM Library

This library is meant for Arduino application using the Athena bootloader and storing data in the Arduino EEPROM.

The Athena bootloader requires space in the EEPROM. Devices which are also using the EEPROM can use the AthenaEEPROM library instead of the built-in EEPROM library. AthenaEEPROM will automatically handle offsets for Athena data, meaning that you do not need to adjust EEPROM offsets in your program.

If you are using the default EEPROM library for our own purposes, you **must** start writing after `NETEEPROM_END`, available in `AthenaEEPROM_defs.h`:

```
#define NETEEPROM_END      63
```

Note that use of the Athena bootloader does reduce the maximum EEPROM capacity by 63 bytes.

In addition to reading and writing data from the EEPROM, this library also enables you to read/write network settings using the Arduino EEPROM. These network settings are used by the Athena bootloader and AthenaEthernetReset library.

You must program the device with the proper network settings in order for the Athena bootloader and AthenaEthernetReset library to properly function.

## Using the Library

To use the library, include `AthenaEEPROM.h` instead of `EEPROM.h`:

```
#include <AthenaEEPROM.h>
```

The library provides a global instance called `EEPROM`. 

You can write a byte with the `write` API:

```
EEPROM.write(addr, val);
```

And read a byte using the `read` API:

```
byte value = EEPROM.read(address);
```

This library automatically handles the offset that protects Athena-specific EEPROM data. The actual size of the EEPROM available to your program is reduced by `ATHENA_OFFSET` bytes, defined in `AthenaEEPROM.h`.

## EEPROM Memory Requirements

This library uses the Arduino EEPROM to store values needed by the bootloader and AthenaEthernetReset server. If you are using the default EEPROM library for our own purposes, please make sure to start writing after `NETEEPROM_END`, available in `AthenaEEPROM_defs.h`:

```
#define NETEEPROM_END      63
```

If you use the AthenaEEPROM library included with the Athena bootloader, then offsets will be handled for you automatically.

## Using the AthenaEEPROM Library

A global instance is declared by the library and should be referenced within your application:

```
AthenaEEPROMClass AthenaEEPROM;
```

The current settings can be printed out to serial:

```
AthenaEEPROM.printAll(&Serial);
```

To update network settings, use the `writeNet` function:

```
byte  mac[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
IPAddress  ip(192, 168, 1, 120);
IPAddress  gateway(192, 168, 1, 1);
IPAddress  subnet(255, 255, 255, 0);

/* Write the new settings values to EEPROM */
AthenaEEPROM.writeNet(mac, ip, gateway, subnet);
```

You can change the TFTP data transfer port from the default setting, if needed, by using `writePort`:

```
word port = 46969;

/* Set the TFTP data transfer port in EEPROM. Change it only if you need to. */
AthenaEEPROM.writePort(port);
```

You can change the AthenaEthernetReset URL "password" using the `writePass` function:

```
/* 
 * This is the "password" for the reset server in case you want to reset your Arduino
 * remotely. This setting has no effect on the bootloader itself. Password can be 36 characters
 * long, at maximum. Any string longer than 36 characters will be truncated.
 * Spaces should not be used 
 */
String password = "athena";

/* Write Reset Server password in EEPROM */
AthenaEEPROM.writePass(password);
```

You can manually set the programmed application image marker as "bad". In this case, the bootloader will think that there is no active application and will remain in bootloader mode until a new sketch is received:

```
/* Set image status to bad, so upon reboot the bootloader won't load the previous sketch */
AthenaEEPROM.writeImgBad();
```

## Example Sketches

Example sketches are provided:

1. [ReadNetworkSettings](examples/ReadNetworkSettings/ReadNetworkSettings.ino)
2. [WriteNetworkSettings](examples/WriteNetworkSettings/WriteNetworkSettings.ino)
3. [eeprom_clear](examples/eeprom_clear/eeprom_clear.ino) - demonstrates erasing all data from the EEPROM, except for Athena data
4. [eeprom_read](examples/eeprom_read/eeprom_read.ino) - demonstrates reading from the EEPROM
5. [eeprom_write](examples/eeprom_write/eeprom_write.ino) - demonstrates writing to the EEPROM

The [ReadNetworkSettings](examples/ReadNetworkSettings/ReadNetworkSettings.ino) sketch uses the AthenaEEPROM library to print out the currently configured network settings:

```
--- Network Settings ---
    MAC: 0xDE.0xAD.0xBE.0xEF.0xFE.0xED
Address: 10.0.1.199
Gateway: 10.0.1.1
 Subnet: 255.255.255.0
--- Tftp Settings ---
Data Port: 46969
(Default)
--- Reset Server Settings ---
Password: athena
  Length: 7
Settings read, you can now upload another sketch.
```

The [WriteNetworkSettings](examples/WriteNetworkSettings/WriteNetworkSettings.ino) sketch can be used to update the network settings for a device. Note that you *must* write network settings to the device before it can be properly used to update devices over Ethernet.
