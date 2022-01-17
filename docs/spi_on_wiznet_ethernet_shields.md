# SPI Pins Configuration on WIZnet Shields

from the [Ethernet](https://www.arduino.cc/en/Reference/Ethernet) library

> The Arduino board communicates with the shield using the SPI bus.
This is on digital pins 11, 12, and 13 on the Uno and pins 50, 51, 
and 52 on the Mega. On both boards, pin 10 is used as SS. On the Mega,
the hardware SS pin, 53, is not used to select the Ethernet controller
chip, but it must be kept as an output or the SPI interface won't work.


On WIZnet shields pins 11,12, and 13 are **tied up** (by default) with the
corresponding pins on the ICSP header.

## WIZnet Shield + Arduino Mega

When you assembly a Winznet Shield (e.g, [W5500 Ethernet Shield](https://docs.wiznet.io/Product/Open-Source-Hardware/w5500_ethernet_shield))
on an Arduino Mega, you will have a **contention between pins !!!** For example, pin 13, is used for both LED_BUILTIN and the SCLK pin.  

WIZnet have on their shields "SPI Jumpers", which are **mounted by default**:
- Mount: SPI signals are connected to D11(MOSI), D12(MISO), D13(SCLK) and ICSP 
connector
- Not mounted: SPI signals are connected to only the ICSP connector

So, if you wish to use pins 11, 12 and 13 on a Mega setup for a purpose other than SPI, you will have to unmount the SPI jumpers.
  
![W5500 Shield SPI pins](img/W5500_shield_spi_pins.png)

![W5500 Shield SPI pins](img/W5500_shield_jumper.png)


## Arduino Ethernet Shield 2 + Arduino Mega

The above mentioned contention is not the case for the [Arduino Ethernet Shield Rev 2](https://docs.arduino.cc/hardware/ethernet-shield-rev2).

![sd](img/arduino_ethernet_shield_rev2_schematics.png)

