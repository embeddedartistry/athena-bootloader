# Converting Binaries for TFTP Upload

**Table of Contents:**

1. [Finding the HEX file with Arduino-built Programs](#finding-the-hex-file-with-arduino-built-programs)
1. [Windows Conversion](#windows-conversion)
2. [Linux and OS X Conversion](#linux-and-wsl-conversion)
3. [OS X Conversion](#os-x-conversion)

## Finding the HEX File with Arduino-built Programs

First, build your sketch inside _Arduino IDE_ using the __Verify__ button. Next, without exiting the _Arduino IDE_, you need to navigate to the temporary directory where your project was built. On all platforms, you can find out the temporary build folder by checking `Show verbose output during compilation` in _Arduino IDE_'s preferences menu. The path for the temporary build output directory will be included at the end of the compilation output.

For example:

```
/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-size -A /var/folders/0h/_66m4dqj3p1b3j1xqg10r08w0000gn/T/arduino_build_678002/WebServer.ino.elf
Sketch uses 12774 bytes (4%) of program storage space. Maximum is 258048 bytes.
Global variables use 797 bytes (9%) of dynamic memory, leaving 7395 bytes for local variables. Maximum is 8192 bytes.
```

Look for a line mentioning a file in `.elf` or `.hex`. In the example above, we can see `/var/folders/0h/_66m4dqj3p1b3j1xqg10r08w0000gn/T/arduino_build_678002/WebServer.ino.elf`. The temporary folder the build output is stored in is `/var/folders/0h/_66m4dqj3p1b3j1xqg10r08w0000gn/T/arduino_build_678002/`.

If we look at the contents of the folder, we can see there is a corresponding `.hex` file:

```
$ ls /var/folders/0h/_66m4dqj3p1b3j1xqg10r08w0000gn/T/arduino_build_678002/
WebServer.ino.eep                 core
WebServer.ino.elf                 includes.cache
WebServer.ino.hex                 libraries
WebServer.ino.with_bootloader.hex preproc
build.options.json                sketch
```

We'll convert that to a binary using `avr-objcopy`

## Windows Conversion

On Windows, the temporary directory is often something like:

```
C:\Documents and Settings\Administrator\Local Settings\Temp\build5571819468326525374.tmp\Blink.cpp.hex
```

To convert the file to a binary, use the following `avr-objcopy` pattern:

```
"C:\Program Files\Arduino\hardware\tools\avr\bin\avr-objcopy.exe" -I ihex "C:\path\to\your\program.hex" -O binary program.bin
```

Using the example above, we would say:

```
"C:\Program Files\Arduino\hardware\tools\avr\bin\avr-objcopy.exe" -I ihex
"C:\Documents and Settings\Administrator\Local Settings\Temp\build5571819468326525374.tmp\Blink.cpp.hex"
-O binary Blink.cpp.bin
```

The output will be placed int he current directory.

## Linux and WSL Conversion

For Linux, the file is likely to be in your `/tmp` directory. The form of `avr-objcopy` to use is:

```
avr-objcopy -I ihex /path/to/sketch.hex -O binary sketch.bin
```

## OS X Conversion

For OS X, the file location is likely within `/var/folders/`. The form of `avr-objcopy` to use is:

```
avr-objcopy -I ihex /path/to/sketch.hex -O binary sketch.bin
```
