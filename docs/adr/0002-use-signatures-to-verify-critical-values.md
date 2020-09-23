# 2. Use Signatures to Verify Critical Values

Date: 2020-09-23

## Status

Accepted 

(Documenting observed behavior)

## Context

The bootloader needs a way to know whether the values programmed in the EEPROM can be relied upon. If they can't be, then default values need to be used.

## Decision

"Signatures" are used to tell the bootloader/library code that the values programmed in the EEPROM are valid. If the EEPROM values don't match, then the bootloader/libraries can't assume that the EEPROM values actually correspond to an Athena-related value, and the defaults are used. For example, the erased EEPROM values are `0xff`, so if you didn't have the signature and the bootloader just assumed the value was reasonable, it would try to self-assign an invalid IP address: 255.255.255.255.

For example, the MAC address, gateway address, IP address, and subnet address values are controlled by a two-byte signature. These signature values must be present in order for the values programmed in these fields to be used by the bootloader.

```
eeprom_write_byte(NETEEPROM_SIG_1, NETEEPROM_SIG_1_VALUE);
eeprom_write_byte(NETEEPROM_SIG_2, NETEEPROM_SIG_2_VALUE);
```

## Consequences

Users have to know about the signatures in case they want to manually program the bootloader EEPROM space.

The AthenaEEPROM library (included in the bootloader repo) handles IP address programming (and other fields) properly, writing the signatures for you under the hood. This should be the preferred route for most users.
