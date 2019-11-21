# Ariadne-aware EEPROM Library

This library is meant for Arduino application using the Ariadne bootloader and storing data in the Arduino EEPROM.

The Ariadne bootloader requires space in the EEPROM. Devices which are also using the EEPROM can use the NewEEPROM library instead of the built-in EEPROM library. NewEEPROM will automatically handle offsets for Ariadne data, meaning that you do not need to adjust EEPROM offsets in your program.

If you are using the default EEPROM library for our own purposes, you **must** start writing after `NETEEPROM_END`, available in `NetEEPROM_defs.h`:

```
#define NETEEPROM_END      63
```

Note that use of the Ariadne bootloader does reduce the maximum EEPROM capacity by 63 bytes.

## Using the Library

To use the library, include `NewEEPROM.h` instead of `EEPROM.h`:

```
#include <NewEEPROM.h>
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

## Example Sketches

Three example sketches are provided:

* [eeprom_clear](examples/eeprom_clear/eeprom_clear.ino) - demonstrates erasing all data from the EEPROM, except for Ariadne data
* [eeprom_read](examples/eeprom_read/eeprom_read.ino) - demonstrates reading from the EEPROM
* [eeprom_write](examples/eeprom_write/eeprom_write.ino) - demonstrates writing to the EEPROM
