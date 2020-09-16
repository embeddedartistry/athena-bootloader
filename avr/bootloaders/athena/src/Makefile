# Name: Makefile
# Author: Stelios Tsampas, from work by optiboot
# Copyright: optiboot
# License: GPL http://www.gnu.org/licenses/gpl-2.0.html
# Project: athena bootloader
# Function: tftp implementation and flasher
# Version: 0.2 tftp flashing functional, serial flashing

# Instructions
#
# To make bootloader .hex file:
# make diecimila
# make lilypad
# make ng
# etc...
#
# To burn bootloader .hex file:
# make diecimila_isp
# make lilypad_isp
# make ng_isp
# etc...

# program name should not be changed...
PROGRAM    = athena

# The default behavior is to build using tools that are in the users
# current path variables, but we can also build using an installed
# Arduino user IDE setup, or the Arduino source tree.
# Uncomment this next lines to build within the arduino environment,
# using the arduino-included avrgcc toolset (mac and pc)
# ENV ?= arduino
# ENV ?= arduinodev
# OS ?= macosx
# OS ?= windows

# enter the parameters for the avrdude isp tool
# for now they default to the AVRISP MKII on linux
#ISPTOOL    = avrispmkii
ISPTOOL    ?= usbtiny
ISPPORT	   ?= usb
ISPSPEED   ?= #-b38400

MCU_TARGET = atmega328p
LDSECTIONS = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe

# Build environments
# Start of some ugly makefile-isms to allow optiboot to be built
# in several different environments.  See the README.TXT file for
# details.

# default
fixpath = $(1)

ifeq ($(ENV), arduino)
# For Arduino, we assume that we're connected to the optiboot directory
# included with the arduino distribution, which means that the full set
# of avr-tools are "right up there" in standard places.
TOOLROOT = ../../../tools
GCCROOT = $(TOOLROOT)/avr/bin/
AVRDUDE_CONF = -C$(TOOLROOT)/avr/etc/avrdude.conf

ifeq ($(OS), windows)
# On windows, SOME of the tool paths will need to have backslashes instead
# of forward slashes (because they use windows cmd.exe for execution instead
# of a unix/mingw shell?)  We also have to ensure that a consistent shell
# is used even if a unix shell is installed (ie as part of WINAVR)
fixpath = $(subst /,\,$1)
SHELL = cmd.exe
endif

else ifeq ($(ENV), arduinodev)
# Arduino IDE source code environment.  Use the unpacked compilers created
# by the build (you'll need to do "ant build" first.)
ifeq ($(OS), macosx)
TOOLROOT = ../../../../build/macosx/work/Arduino.app/Contents/Resources/Java/hardware/tools
endif
ifeq ($(OS), windows)
TOOLROOT = ../../../../build/windows/work/hardware/tools
endif

GCCROOT = $(TOOLROOT)/avr/bin/
AVRDUDE_CONF = -C$(TOOLROOT)/avr/etc/avrdude.conf

else
GCCROOT =
AVRDUDE_CONF =
endif
#
# End of build environment code.


# the efuse should really be 0xf8; since, however, only the lower
# three bits of that byte are used on the atmega168, avrdude gets
# confused if you specify 1's for the higher bits, see:
# http://tinker.it/now/2007/02/24/the-tale-of-avrdude-atmega168-and-extended-bits-fuses/
#
# similarly, the lock bits should be 0xff instead of 0x3f (to
# unlock the bootloader section) and 0xcf instead of 0x2f (to
# lock it), but since the high two bits of the lock byte are
# unused, avrdude would get confused.

ISPFUSES = $(GCCROOT)avrdude $(AVRDUDE_CONF) -c $(ISPTOOL) \
           -p $(MCU_TARGET) -P $(ISPPORT) $(ISPSPEED) \
           -e -Ulock:w:0x3F:m -Uefuse:w:0x$(EFUSE):m \
           -Uhfuse:w:0x$(HFUSE):m -Ulfuse:w:0x$(LFUSE):m
ISPFLASH = $(GCCROOT)avrdude $(AVRDUDE_CONF) -c $(ISPTOOL) \
           -p $(MCU_TARGET) -P $(ISPPORT) $(ISPSPEED) \
           -Uflash:w:$(PROGRAM)_$(TARGET).hex -Ulock:w:0x0F:m

STK500   = "C:\Program Files\Atmel\AVR Tools\STK500\Stk500.exe"
STK500-1 = $(STK500) -e -d$(MCU_TARGET) -pf -vf -if$(PROGRAM)_$(TARGET).hex \
	   -lFF -LFF -f$(HFUSE)$(LFUSE) -EF8 -ms -q -cUSB -I200kHz -s -wt
STK500-2 = $(STK500) -d$(MCU_TARGET) -ms -q -lCF -LCF -cUSB -I200kHz -s -wt

OBJ      = main.o spi.o ethernet/net.o ethernet/tftp.o ethernet/validate.o debug/debug.o watchdog.o serial.o util.o optiboot/optiboot.o stk500boot/stk500boot.o
OPTIMIZE = -Os -flto -fuse-linker-plugin -fno-inline-small-functions -fno-split-wide-types -mno-interrupts -mrelax
OPTIMIZE += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fno-jump-tables -std=gnu99 #-save-temps
INCLUDES = -I . -I debug -I ethernet -I optiboot -I stk500boot -isystem ../../../libraries
DEFS     =
LIBS     =

CC       = $(GCCROOT)avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS  = -g -Wall -Wextra -Wstrict-prototypes $(OPTIMIZE) $(INCLUDES) -mmcu=$(MCU_TARGET) -DF_CPU=$(AVR_FREQ) $(DEFS)
override LDFLAGS = $(LDSECTIONS) -Wl,--relax -Wl,--gc-sections -flto -fuse-linker-plugin

OBJCOPY = $(GCCROOT)avr-objcopy
OBJDUMP = $(call fixpath,$(GCCROOT)avr-objdump)

SIZE    = $(GCCROOT)avr-size


# Build with the debug messages enabled
# Use it only if you have already seen the source code
debug328_w5100: TARGET = debug328_w5100
debug328_w5100: MCU_TARGET = atmega328p
debug328_w5100: CFLAGS += '-D__WIZ_W5100__' '-DDEBUG_MAIN=1' '-DDEBUG_NET=1' '-DDEBUG_TFTP=1' '-DBAUD_RATE=115200'
debug328_w5100: AVR_FREQ = 16000000L
debug328_w5100: LDSECTIONS = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
debug328_w5100: $(PROGRAM)_debug328_w5100.hex
debug328_w5100: $(PROGRAM)_debug328_w5100.lst
debug328_w5100_isp: debug328_w5100
debug328_w5100_isp: TARGET = debug328_w5100
debug328_w5100_isp: MCU_TARGET = atmega328p
# Low power xtal (16MHz) 16KCK/14CK+65ms
debug328_w5100_isp: LFUSE = FF
# 4096 byte boot, SPIEN
# D8 = Erase EEPROM, D0 = Save EEPROM
debug328_w5100_isp: HFUSE = D0
# 2.7V brownout
debug328_w5100_isp: EFUSE = 05
debug328_w5100_isp: isp

# Build with the debug messages enabled
# Use it only if you have already seen the source code
debug328_w5200: TARGET = debug328_w5200
debug328_w5200: MCU_TARGET = atmega328p
debug328_w5200: CFLAGS += '-D__WIZ_W5200__' '-DDEBUG_MAIN=1' '-DDEBUG_NET=1' '-DDEBUG_TFTP=1' '-DBAUD_RATE=115200'
debug328_w5200: AVR_FREQ = 16000000L
debug328_w5200: LDSECTIONS = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
debug328_w5200: $(PROGRAM)_debug328_w5200.hex
debug328_w5200: $(PROGRAM)_debug328_w5200.lst
debug328_w5200_isp: debug328_w5200
debug328_w5200_isp: TARGET = debug328_w5200
debug328_w5200_isp: MCU_TARGET = atmega328p
# Low power xtal (16MHz) 16KCK/14CK+65ms
debug328_w5200_isp: LFUSE = FF
# 4096 byte boot, SPIEN
# D8 = Erase EEPROM, D0 = Save EEPROM
debug328_w5200_isp: HFUSE = D0
# 2.7V brownout
debug328_w5200_isp: EFUSE = 05
debug328_w5200_isp: isp

# Build with the debug messages enabled
# Use it only if you have already seen the source code
debug328_w5500: TARGET = debug328_w5500
debug328_w5500: MCU_TARGET = atmega328p
debug328_w5500: CFLAGS += '-D__WIZ_W5500__' '-DDEBUG_MAIN=1' '-DDEBUG_NET=1' '-DDEBUG_TFTP=1' '-DBAUD_RATE=115200'
debug328_w5500: AVR_FREQ = 16000000L
debug328_w5500: LDSECTIONS = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
debug328_w5500: $(PROGRAM)_debug328_w5500.hex
debug328_w5500: $(PROGRAM)_debug328_w5500.lst
debug328_w5500_isp: debug328_w5500
debug328_w5500_isp: TARGET = debug328_w5500
debug328_w5500_isp: MCU_TARGET = atmega328p
# Low power xtal (16MHz) 16KCK/14CK+65ms
debug328_w5500_isp: LFUSE = FF
# 4096 byte boot, SPIEN
# D8 = Erase EEPROM, D0 = Save EEPROM
debug328_w5500_isp: HFUSE = D0
# 2.7V brownout
debug328_w5500_isp: EFUSE = 05
debug328_w5500_isp: isp

# Debug build for Arduino Mega2560
debug2560_w5100: TARGET = debug2560_w5100
debug2560_w5100: MCU_TARGET = atmega2560
debug2560_w5100: CFLAGS += '-D__WIZ_W5100__' '-DDEBUG_MAIN=1' '-DDEBUG_NET=1' '-DDEBUG_TFTP=1' '-DBAUD_RATE=115200'
debug2560_w5100: AVR_FREQ = 16000000L
debug2560_w5100: LDSECTIONS  = -Wl,--section-start=.text=0x3e000
debug2560_w5100: $(PROGRAM)_debug2560_w5100.hex
debug2560_w5100: $(PROGRAM)_debug2560_w5100.lst
debug2560_w5100_isp: debug2560_w5100
debug2560_w5100_isp: TARGET = debug2560_w5100
debug2560_w5100_isp: MCU_TARGET = atmega2560
# Low power xtal (16MHz) 16KCK/14CK+65ms
debug2560_w5100_isp: LFUSE = FF
# 4069 byte boot
# D8 = Erase EEPROM, D0 = Save EEPROM
debug2560_w5100_isp: HFUSE = D0
# 2.7V brownout
debug2560_w5100_isp: EFUSE = FD
debug2560_w5100_isp: isp

# Debug build for Arduino Mega2560
debug2560_w5200: TARGET = debug2560_w5200
debug2560_w5200: MCU_TARGET = atmega2560
debug2560_w5200: CFLAGS += '-D__WIZ_W5200__' '-DDEBUG_MAIN=1' '-DDEBUG_NET=1' '-DDEBUG_TFTP=1' '-DBAUD_RATE=115200'
debug2560_w5200: AVR_FREQ = 16000000L
debug2560_w5200: LDSECTIONS  = -Wl,--section-start=.text=0x3e000
debug2560_w5200: $(PROGRAM)_debug2560_w5200.hex
debug2560_w5200: $(PROGRAM)_debug2560_w5200.lst
debug2560_w5200_isp: debug2560_w5200
debug2560_w5200_isp: TARGET = debug2560_w5200
debug2560_w5200_isp: MCU_TARGET = atmega2560
# Low power xtal (16MHz) 16KCK/14CK+65ms
debug2560_w5200_isp: LFUSE = FF
# 4069 byte boot
# D8 = Erase EEPROM, D0 = Save EEPROM
debug2560_w5200_isp: HFUSE = D0
# 2.7V brownout
debug2560_w5200_isp: EFUSE = FD
debug2560_w5200_isp: isp

# Debug build for Arduino Mega2560
debug2560_w5500: TARGET = debug2560_w5500
debug2560_w5500: MCU_TARGET = atmega2560
debug2560_w5500: CFLAGS += '-D__WIZ_W5500__' '-DDEBUG_MAIN=1' '-DDEBUG_NET=1' '-DDEBUG_TFTP=1' '-DBAUD_RATE=115200'
debug2560_w5500: AVR_FREQ = 16000000L
debug2560_w5500: LDSECTIONS  = -Wl,--section-start=.text=0x3e000
debug2560_w5500: $(PROGRAM)_debug2560_w5500.hex
debug2560_w5500: $(PROGRAM)_debug2560_w5500.lst
debug2560_w5500_isp: debug2560_w5500
debug2560_w5500_isp: TARGET = debug2560_w5500
debug2560_w5500_isp: MCU_TARGET = atmega2560
# Low power xtal (16MHz) 16KCK/14CK+65ms
debug2560_w5500_isp: LFUSE = FF
# 4069 byte boot
# D8 = Erase EEPROM, D0 = Save EEPROM
debug2560_w5500_isp: HFUSE = D0
# 2.7V brownout
debug2560_w5500_isp: EFUSE = FD
debug2560_w5500_isp: isp

# Debug build for Atmega1284
debug1284_w5100: TARGET = debug1284_w5100
debug1284_w5100: MCU_TARGET = atmega1284p
debug1284_w5100: CFLAGS += '-D__WIZ_W5100__' '-DDEBUG_MAIN=1' '-DDEBUG_NET=1' '-DDEBUG_TFTP=1' '-DBAUD_RATE=115200'
debug1284_w5100: AVR_FREQ = 16000000L
debug1284_w5100: LDSECTIONS  = -Wl,--section-start=.text=0x1e000
debug1284_w5100: $(PROGRAM)_debug1284_w5100.hex
debug1284_w5100: $(PROGRAM)_debug1284_w5100.lst
debug1284_w5100_isp: debug1284_w5100
debug1284_w5100_isp: TARGET = debug1284_w5100
debug1284_w5100_isp: MCU_TARGET = atmega1284p
# External power xtal (16MHz) 16KCK/14CK+65ms
debug1284_w5100_isp: LFUSE = FF
# 4096 word boot
# D8 = Erase EEPROM, D0 = Save EEPROM
debug1284_w5100_isp: HFUSE = D0
# 2.7V brownout
debug1284_w5100_isp: EFUSE = FD
debug1284_w5100_isp: isp

# Debug build for Atmega1284
debug1284_w5200: TARGET = debug1284_w5200
debug1284_w5200: MCU_TARGET = atmega1284p
debug1284_w5200: CFLAGS += '-D__WIZ_W5200__' '-DDEBUG_MAIN=1' '-DDEBUG_NET=1' '-DDEBUG_TFTP=1' '-DDEBUG_VALD=1' '-DBAUD_RATE=115200'
debug1284_w5200: AVR_FREQ = 16000000L
debug1284_w5200: LDSECTIONS  = -Wl,--section-start=.text=0x1e000
debug1284_w5200: $(PROGRAM)_debug1284_w5200.hex
debug1284_w5200: $(PROGRAM)_debug1284_w5200.lst
debug1284_w5200_isp: debug1284_w5200
debug1284_w5200_isp: TARGET = debug1284_w5200
debug1284_w5200_isp: MCU_TARGET = atmega1284p
# External power xtal (16MHz) 16KCK/14CK+65ms
debug1284_w5200_isp: LFUSE = FF
# 4096 word boot
# D8 = Erase EEPROM, D0 = Save EEPROM
debug1284_w5200_isp: HFUSE = D0
# 2.7V brownout
debug1284_w5200_isp: EFUSE = FD
debug1284_w5200_isp: isp

# Debug build for Atmega1284
debug1284_w5500: TARGET = debug1284_w5500
debug1284_w5500: MCU_TARGET = atmega1284p
debug1284_w5500: CFLAGS += '-D__WIZ_W5500__' '-DDEBUG_MAIN=1' '-DDEBUG_NET=1' '-DDEBUG_TFTP=1' '-DDEBUG_VALD=1' '-DBAUD_RATE=115200'
debug1284_w5500: AVR_FREQ = 16000000L
debug1284_w5500: LDSECTIONS  = -Wl,--section-start=.text=0x1e000
debug1284_w5500: $(PROGRAM)_debug1284_w5500.hex
debug1284_w5500: $(PROGRAM)_debug1284_w5500.lst
debug1284_w5500_isp: debug1284_w5500
debug1284_w5500_isp: TARGET = debug1284_w5500
debug1284_w5500_isp: MCU_TARGET = atmega1284p
# External power xtal (16MHz) 16KCK/14CK+65ms
debug1284_w5500_isp: LFUSE = FF
# 4096 word boot
# D8 = Erase EEPROM, D0 = Save EEPROM
debug1284_w5500_isp: HFUSE = D0
# 2.7V brownout
debug1284_w5500_isp: EFUSE = FD
debug1284_w5500_isp: isp


# Atmega328 used on Arduino UNO and Arduino Ethernet
atmega328_w5100: EXT_OBJ = optiboot.o
atmega328_w5100: TARGET = atmega328_w5100
atmega328_w5100: MCU_TARGET = atmega328p
atmega328_w5100: CFLAGS += '-D__WIZ_W5100__' '-DBAUD_RATE=115200'
atmega328_w5100: AVR_FREQ = 16000000L
atmega328_w5100: LDSECTIONS  = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
atmega328_w5100: $(PROGRAM)_atmega328_w5100.hex
atmega328_w5100: $(PROGRAM)_atmega328_w5100.lst
atmega328_w5100_isp: atmega328_w5100
atmega328_w5100_isp: TARGET = atmega328_w5100
atmega328_w5100_isp: MCU_TARGET = atmega328p
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega328_w5100_isp: LFUSE = FF
# 4096 byte boot, SPIEN
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega328_w5100_isp: HFUSE = D0
# 2.7V brownout
atmega328_w5100_isp: EFUSE = 05
atmega328_w5100_isp: isp

# Atmega328 used on Arduino UNO and Arduino Ethernet
atmega328_w5200: EXT_OBJ = optiboot.o
atmega328_w5200: TARGET = atmega328_w5200
atmega328_w5200: MCU_TARGET = atmega328p
atmega328_w5200: CFLAGS += '-D__WIZ_W5200__' '-DBAUD_RATE=115200'
atmega328_w5200: AVR_FREQ = 16000000L
atmega328_w5200: LDSECTIONS  = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
atmega328_w5200: $(PROGRAM)_atmega328_w5200.hex
atmega328_w5200: $(PROGRAM)_atmega328_w5200.lst
atmega328_w5200_isp: atmega328_w5200
atmega328_w5200_isp: TARGET = atmega328_w5200
atmega328_w5200_isp: MCU_TARGET = atmega328p
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega328_w5200_isp: LFUSE = FF
# 4096 byte boot, SPIEN
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega328_w5200_isp: HFUSE = D0
# 2.7V brownout
atmega328_w5200_isp: EFUSE = 05
atmega328_w5200_isp: isp

# Atmega328 used on Arduino UNO and Arduino Ethernet
atmega328_w5500: EXT_OBJ = optiboot.o
atmega328_w5500: TARGET = atmega328_w5500
atmega328_w5500: MCU_TARGET = atmega328p
atmega328_w5500: CFLAGS += '-D__WIZ_W5500__' '-DBAUD_RATE=115200'
atmega328_w5500: AVR_FREQ = 16000000L
atmega328_w5500: LDSECTIONS  = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
atmega328_w5500: $(PROGRAM)_atmega328_w5500.hex
atmega328_w5500: $(PROGRAM)_atmega328_w5500.lst
atmega328_w5500_isp: atmega328_w5500
atmega328_w5500_isp: TARGET = atmega328_w5500
atmega328_w5500_isp: MCU_TARGET = atmega328p
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega328_w5500_isp: LFUSE = FF
# 4096 byte boot, SPIEN
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega328_w5500_isp: HFUSE = D0
# 2.7V brownout
atmega328_w5500_isp: EFUSE = 05
atmega328_w5500_isp: isp

atmega328_ethernet: TARGET = atmega328_ethernet
atmega328_ethernet: MCU_TARGET = atmega328p
atmega328_ethernet: CFLAGS += '-D__WIZ_W5100__' '-DARDUINO_ETHERNET' '-DBAUD_RATE=115200'
atmega328_ethernet: AVR_FREQ = 16000000L
atmega328_ethernet: LDSECTIONS  = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
atmega328_ethernet: $(PROGRAM)_atmega328_ethernet.hex
atmega328_ethernet: $(PROGRAM)_atmega328_ethernet.lst
atmega328_ethernet_isp: atmega328_ethernet
atmega328_ethernet_isp: TARGET = atmega328_ethernet
atmega328_ethernet_isp: MCU_TARGET = atmega328p
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega328_ethernet_isp: LFUSE = FF
# 4096 byte boot, SPIEN
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega328_ethernet_isp: HFUSE = D0
# 2.7V brownout
atmega328_ethernet_isp: EFUSE = 05
atmega328_ethernet_isp: isp

atmega32u4_w5100: EXT_OBJ = optiboot.o
atmega32u4_w5100: TARGET = atmega32u4_w5100
atmega32u4_w5100: MCU_TARGET = atmega32u4
atmega32u4_w5100: CFLAGS += '-D__WIZ_W5100__' '-DBAUD_RATE=115200'
atmega32u4_w5100: AVR_FREQ = 16000000L
atmega32u4_w5100: LDSECTIONS  = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
atmega32u4_w5100: $(PROGRAM)_atmega32u4_w5100.hex
atmega32u4_w5100: $(PROGRAM)_atmega32u4_w5100.lst
atmega32u4_w5100_isp: atmega32u4_w5100
atmega32u4_w5100_isp: TARGET = atmega32u4_w5100
atmega32u4_w5100_isp: MCU_TARGET = atmega32u4
# Fuse settings from 32u4 Caterina bootloader
atmega32u4_w5100_isp: LFUSE = FF
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega32u4_w5100_isp: HFUSE = D0
atmega32u4_w5100_isp: EFUSE = CB
atmega32u4_w5100_isp: isp

atmega32u4_w5200: EXT_OBJ = optiboot.o
atmega32u4_w5200: TARGET = atmega32u4_w5200
atmega32u4_w5200: MCU_TARGET = atmega32u4
atmega32u4_w5200: CFLAGS += '-D__WIZ_W5200__' '-DBAUD_RATE=115200'
atmega32u4_w5200: AVR_FREQ = 16000000L
atmega32u4_w5200: LDSECTIONS  = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
atmega32u4_w5200: $(PROGRAM)_atmega32u4_w5200.hex
atmega32u4_w5200: $(PROGRAM)_atmega32u4_w5200.lst
atmega32u4_w5200_isp: atmega32u4_w5200
atmega32u4_w5200_isp: TARGET = atmega32u4_w5200
atmega32u4_w5200_isp: MCU_TARGET = atmega32u4
# Fuse settings from 32u4 Caterina bootloader
atmega32u4_w5200_isp: LFUSE = FF
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega32u4_w5200_isp: HFUSE = D0
atmega32u4_w5200_isp: EFUSE = CB
atmega32u4_w5200_isp: isp

atmega32u4_w5500: EXT_OBJ = optiboot.o
atmega32u4_w5500: TARGET = atmega32u4_w5500
atmega32u4_w5500: MCU_TARGET = atmega32u4
atmega32u4_w5500: CFLAGS += '-D__WIZ_W5500__' '-DBAUD_RATE=115200'
atmega32u4_w5500: AVR_FREQ = 16000000L
atmega32u4_w5500: LDSECTIONS  = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
atmega32u4_w5500: $(PROGRAM)_atmega32u4_w5500.hex
atmega32u4_w5500: $(PROGRAM)_atmega32u4_w5500.lst
atmega32u4_w5500_isp: atmega32u4_w5500
atmega32u4_w5500_isp: TARGET = atmega32u4_w5500
atmega32u4_w5500_isp: MCU_TARGET = atmega32u4
# Fuse settings from 32u4 Caterina bootloader
atmega32u4_w5500_isp: LFUSE = FF
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega32u4_w5500_isp: HFUSE = D0
atmega32u4_w5500_isp: EFUSE = CB
atmega32u4_w5500_isp: isp


# Mega has a minimum boot size of 1024 bytes
atmega2560_w5100: TARGET = atmega2560_w5100
atmega2560_w5100: MCU_TARGET = atmega2560
atmega2560_w5100: CFLAGS += '-D__WIZ_W5100__' '-DBAUD_RATE=115200'
atmega2560_w5100: AVR_FREQ = 16000000L
atmega2560_w5100: LDSECTIONS  = -Wl,--section-start=.text=0x3e000
atmega2560_w5100: $(PROGRAM)_atmega2560_w5100.hex
atmega2560_w5100: $(PROGRAM)_atmega2560_w5100.lst
atmega2560_w5100_isp: atmega2560_w5100
atmega2560_w5100_isp: TARGET = atmega2560_w5100
atmega2560_w5100_isp: MCU_TARGET = atmega2560
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega2560_w5100_isp: LFUSE = FF
# 4069 byte boot
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega2560_w5100_isp: HFUSE = D0
# 2.7V brownout
atmega2560_w5100_isp: EFUSE = FD
atmega2560_w5100_isp: isp


# Mega has a minimum boot size of 1024 bytes
atmega2560_w5200: TARGET = atmega2560_w5200
atmega2560_w5200: MCU_TARGET = atmega2560
atmega2560_w5200: CFLAGS += '-D__WIZ_W5200__' '-DBAUD_RATE=115200'
atmega2560_w5200: AVR_FREQ = 16000000L
atmega2560_w5200: LDSECTIONS  = -Wl,--section-start=.text=0x3e000
atmega2560_w5200: $(PROGRAM)_atmega2560_w5200.hex
atmega2560_w5200: $(PROGRAM)_atmega2560_w5200.lst
atmega2560_w5200_isp: atmega2560_w5200
atmega2560_w5200_isp: TARGET = atmega2560_w5200
atmega2560_w5200_isp: MCU_TARGET = atmega2560
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega2560_w5200_isp: LFUSE = FF
# 4069 byte boot
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega2560_w5200_isp: HFUSE = D0
# 2.7V brownout
atmega2560_w5200_isp: EFUSE = FD
atmega2560_w5200_isp: isp


# Mega has a minimum boot size of 1024 bytes
atmega2560_w5500: TARGET = atmega2560_w5500
atmega2560_w5500: MCU_TARGET = atmega2560
atmega2560_w5500: CFLAGS += '-D__WIZ_W5500__' '-DBAUD_RATE=115200'
atmega2560_w5500: AVR_FREQ = 16000000L
atmega2560_w5500: LDSECTIONS  = -Wl,--section-start=.text=0x3e000
atmega2560_w5500: $(PROGRAM)_atmega2560_w5500.hex
atmega2560_w5500: $(PROGRAM)_atmega2560_w5500.lst
atmega2560_w5500_isp: atmega2560_w5500
atmega2560_w5500_isp: TARGET = atmega2560_w5500
atmega2560_w5500_isp: MCU_TARGET = atmega2560
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega2560_w5500_isp: LFUSE = FF
# 4069 byte boot
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega2560_w5500_isp: HFUSE = D0
# 2.7V brownout
atmega2560_w5500_isp: EFUSE = FD
atmega2560_w5500_isp: isp


# Atmega1284
atmega1284_w5100: TARGET = atmega1284_w5100
atmega1284_w5100: MCU_TARGET = atmega1284p
atmega1284_w5100: CFLAGS += '-D__WIZ_W5100__' '-DBAUD_RATE=115200'
atmega1284_w5100: AVR_FREQ = 16000000L
atmega1284_w5100: LDSECTIONS  = -Wl,--section-start=.text=0x1e000
atmega1284_w5100: $(PROGRAM)_atmega1284_w5100.hex
atmega1284_w5100: $(PROGRAM)_atmega1284_w5100.lst
atmega1284_w5100_isp: atmega1284_w5100
atmega1284_w5100_isp: TARGET = atmega1284_w5100
atmega1284_w5100_isp: MCU_TARGET = atmega1284p
# External power xtal (16MHz) 16KCK/14CK+65ms
atmega1284_w5100_isp: LFUSE = FF
# 4096 word boot
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega1284_w5100_isp: HFUSE = D0
# 2.7V brownout
atmega1284_w5100_isp: EFUSE = FD
atmega1284_w5100_isp: isp


atmega1284_w5200: TARGET = atmega1284_w5200
atmega1284_w5200: MCU_TARGET = atmega1284p
atmega1284_w5200: CFLAGS += '-D__WIZ_W5200__' '-DBAUD_RATE=115200'
atmega1284_w5200: AVR_FREQ = 16000000L
atmega1284_w5200: LDSECTIONS  = -Wl,--section-start=.text=0x1e000
atmega1284_w5200: $(PROGRAM)_atmega1284_w5200.hex
atmega1284_w5200: $(PROGRAM)_atmega1284_w5200.lst
atmega1284_w5200_isp: atmega1284_w5200
atmega1284_w5200_isp: TARGET = atmega1284_w5200
atmega1284_w5200_isp: MCU_TARGET = atmega1284p
# External power xtal (16MHz) 16KCK/14CK+65ms
atmega1284_w5200_isp: LFUSE = FF
# 4096 word boot
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega1284_w5200_isp: HFUSE = D0
# 2.7V brownout
atmega1284_w5200_isp: EFUSE = FD
atmega1284_w5200_isp: isp


atmega1284_w5500: TARGET = atmega1284_w5500
atmega1284_w5500: MCU_TARGET = atmega1284p
atmega1284_w5500: CFLAGS += '-D__WIZ_W5500__' '-DBAUD_RATE=115200'
atmega1284_w5500: AVR_FREQ = 16000000L
atmega1284_w5500: LDSECTIONS  = -Wl,--section-start=.text=0x1e000
atmega1284_w5500: $(PROGRAM)_atmega1284_w5500.hex
atmega1284_w5500: $(PROGRAM)_atmega1284_w5500.lst
atmega1284_w5500_isp: atmega1284_w5500
atmega1284_w5500_isp: TARGET = atmega1284_w5500
atmega1284_w5500_isp: MCU_TARGET = atmega1284p
# External power xtal (16MHz) 16KCK/14CK+65ms
atmega1284_w5500_isp: LFUSE = FF
# 4096 word boot
# D8 = Erase EEPROM, D0 = Save EEPROM
atmega1284_w5500_isp: HFUSE = D0
# 2.7V brownout
atmega1284_w5500_isp: EFUSE = FD
atmega1284_w5500_isp: isp

#
# Generic build targets
#
isp: $(TARGET)
	$(ISPFUSES)
	$(ISPFLASH)

isp-stk500: $(PROGRAM)_$(TARGET).hex
	$(STK500-1)
	$(STK500-2)

%.elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(SIZE) $@

clean:
	rm -rf *.o *.i *.s *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex *~

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O binary $< $@
