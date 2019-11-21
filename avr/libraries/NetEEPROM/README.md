# Ariadne Network Settings Programming Library

This library is meant for Arduino application using the Ariadne bootloader. The library enables you to read/write network settings using the Arduino EEPROM. These network settings are used by the Ariadne bootloader and EthernetReset library.

You must program the device with the proper network settings in order for the Ariadne bootloader and EthernetReset library to properly function.

## EEPROM Memory Requirements

This library uses the Arduino EEPROM to store values needed by the bootloader and EthernetReset server. If you are using the default EEPROM library for our own purposes, please make sure to start writing after `NETEEPROM_END`, available in `NetEEPROM_defs.h`:

```
#define NETEEPROM_END      63
```

If you use the NewEEPROM library included with the Ariadne bootloader, then offsets will be handled for you automatically.

## Using the NetEEPROM Library

A global instance is declared by the library and should be referenced within your application:

```
NetEEPROMClass NetEEPROM;
```

The current settings can be printed out to serial:

```
NetEEPROM.printAll(&Serial);
```

To update network settings, use the `writeNet` function:

```
byte  mac[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
IPAddress  ip(192, 168, 1, 120);
IPAddress  gateway(192, 168, 1, 1);
IPAddress  subnet(255, 255, 255, 0);

/* Write the new settings values to EEPROM */
NetEEPROM.writeNet(mac, ip, gateway, subnet);
```

You can change the TFTP data transfer port from the default setting, if needed, by using `writePort`:

```
word port = 46969;

/* Set the TFTP data transfer port in EEPROM. Change it only if you need to. */
NetEEPROM.writePort(port);
```

You can change the EthernetReset URL "password" using the `writePass` function:

```
/* 
 * This is the "password" for the reset server in case you want to reset your Arduino
 * remotely. This setting has no effect on the bootloader itself. Password can be 36 characters
 * long, at maximum. Any string longer than 36 characters will be truncated.
 * Spaces should not be used 
 */
String password = "ariadne";

/* Write Reset Server password in EEPROM */
NetEEPROM.writePass(password);
```

You can manually set the programmed application image marker as "bad". In this case, the bootloader will think that there is no active application and will remain in bootloader mode until a new sketch is received:

```
/* Set image status to bad, so upon reboot the bootloader won't load the previous sketch */
NetEEPROM.writeImgBad();
```

## Example Sketches

Two example sketches are provided:

1. [ReadNetworkSettings](examples/ReadNetworkSettings/ReadNetworkSettings.ino)
2. [WriteNetworkSettings](examples/WriteNetworkSettings/WriteNetworkSettings.ino)

The [ReadNetworkSettings](examples/ReadNetworkSettings/ReadNetworkSettings.ino) sketch uses the NetEEPROM library to print out the currently configured network settings:

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
Password: ariadne
  Length: 7
Settings read, you can now upload another sketch.
```

The [WriteNetworkSettings](examples/WriteNetworkSettings/WriteNetworkSettings.ino) sketch can be used to update the network settings for a device. Note that you *must* write network settings to the device before it can be properly used to update devices over Ethernet.
