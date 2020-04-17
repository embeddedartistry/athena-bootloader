# Network Configuration

1. [Device Settings](#device-settings)
    1. [Overriding Default Settings When Building the Bootloader](#overriding-default-settings-when-buildilg-the-bootloader)
    2. [Configuring Network Settings After Flashing](#configuring-network-settings-after-flashing)
1. [Configuring Your Router](#configuring-your-router)
    1. [Supporting Multiple Arduino Devices Behind One Router](#supporting-multiple-arduino-devices-behind-one-router)

## Device Settings

When you flash the bootloader, default network settings are used:

```
* IP Address:  192.168.1.128
* Subnet Mask: 255.255.255.0
* Gateway:     192.168.1.1
* MAC Address: 0xDE.0xAD.0xBE.0xEF.0xFE.0xED

* TFTP Negotiation Port: 69
* TFTP Data Port: 46969
```

These default settings are applied whenever the bootloader is flashed. If you have already programmed network settings in the EEPROM, they will be discarded when you re-flash the bootloader.

These settings can be [overridden during the build process](#overriding-default-settings-when-buildilg-the-bootloader), or they can be [changed after the bootloader is flashing using an application](#configuring-network-settings-after-flashing).

**If the default values will not work, you MUST configure the network settings for the TFTP server in the bootloader and the EthernetReset server to work correctly with your network configuration.**

### Configuring Network Settings After Flashing

Network Settings can be changed after the bootloader is flashing using the [__NetEEPROM__ library](../avr/libraries/NetEEPROM). This library is provided with the bootloader and will be automatically detected by the Arduino IDE.

This library can be included in your application to support reading and writing network settings. Please refer to the library for documentation on its usage.

You  also use the provided example sketches, [_WriteNetworkSettings_](../avr/libraries/NetEEPROM/examples/WriteNetworkSettings/WriteNetworkSettings.ino) and [_ReadNetworkSettigns_](../avr/libraries/NetEEPROM/examples/ReadNetworkSettings/ReadNetworkSettings.ino), to see how the NetEEPROM library is used. These sketches can be used to adjust the network settings of your device. The sketches are commented and also meant to serve as documentation.

You can access these files [within this repository](../avr/libraries/NetEEPROM/examples/), or by using the Arduino IDE. Inside of the IDE, navigate to navigate to the __File__ > __Examples__ > __NetEEPROM (Ariadne-Bootloader)__ menu and select the desired example.

![Image showing the NetEEPROM example sketches](neteeprom_sketches.png "Ariadne NetEEPROm example sketches")

The settings array in the __WriteNetworkSettings__ sketch are placed in the order that Wiznet chips read them. Make sure to keep the values in this order.
If you configure the network settings, you also need to configure the TFTP data transfer port. Default settings do not need to be changed if you want to use them.

### Overriding Default Settings When Building the Bootloader

The default settings can be modified by adjusting the values in [`net.h`](../avr/bootloaders/ariadne/src/net.h):

```
#ifndef IP_ADDR
#define IP_ADDR DEFAULT_IP_ADDR
#endif

#ifndef SUB_MASK
#define SUB_MASK DEFAULT_SUB_MASK
#endif

#ifndef GW_ADDR
#define GW_ADDR DEFAULT_GW_ADDR
#endif

#ifndef MAC_ADDR
#define MAC_ADDR DEFAULT_MAC_ADDR
#endif
```

The default values can also be overridden by the build system. You can adjust the [Makefile](../avr/bootloaders/ariadne/src/Makefile) flags variables to include definitions for these values.

## Configuring Your Router

Ethernet updates may succeed while you are on the same network as the Arduino, but fail when you are connecting from a different network.

If this happens to you, you likely need to enable [port forwarding][11] for ports __69__ and __46969__ (or your manually configured port). Port Forward has [excellent guides][12] on how to enable port forwarding for a vast number of routers.

### Supporting Multiple Arduino Devices Behind One Router

If you have multiple Arduino devices behind the router, you may need to configure different port forwarding settings on your router for each Arduino. You will configure the port forwarding so that each Arduino device has a different external port on the router. Each external port is forwarded to the proper internal port and IP of the Arduino on the local network.

For example, let's say you have 2 devices, one at *192.168.1.128* and one at *192.168.1.129*. They both listen to port __69__ for the initial connection. In this case you can forward an external port (e.g., __6969__) on the router to `192.168.1.128:69`. You'll forward another external port (e.g., __6970__) to `192.168.1.129:69`. 

When you are connecting via TFTP, you will open the connection using the router's public IP address (e.g. `136.20.14.135`) and the external ports, __6969__ and __6970__.
