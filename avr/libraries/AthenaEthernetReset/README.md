# Ethernet Reset Server Library for Athena Bootloader

This library is meant for Arduino application using the Athena bootloader. The library will create a server which can be used to remotely reset the device, as well as to remotely enter the bootloader mode so a sketch can be uploaded via TFTP.

## Files

The source files for this library are:

* [`AthenaEthernetReset.cpp`](AthenaEthernetReset.cpp)
* [`AthenaEthernetReset.h`](AthenaEthernetReset.h)

An example sketch is provided and can be found at [`examples/ResetServer/ResetServer.ino`](examples/ResetServer/ResetServer.ino). The sketch shows you how the Ethernet reset server should be included in your application.

## Usage

This library creates a web server on the Arduino that can be used for resetting the device remotely. The server will run on the port specified in the constructor.

```
/* Create the reset server at the specified port.*/
AthenaEthernetReset reset(8080);
```

 The server can be used to remotely reset a device or remotely enter programming mode. This is done through a GET request to specific URLs:

```
http://{ip}:{port}/{reset_path}/reset
http://{ip}:{port}/{reset_path}/reprogram
```

The `reset` URL will perform a normal reset of the program. This endpoint does not provide a reprogramming opportunity.

The `reprogram` URL will invalidate the existing sketch so the bootloader won't time out. The bootloader will wait until a new image is uploaded.

You can go to the URL in a browser, or you can use curl:

```
$ curl 10.0.1.199:8080/athena/reprogram
```

Instead of a password, a `{reset_path}`` is used. If the path is correct, you can access the commands. This reset path matches the "password" that is configured by the AthenaEEPROM library. You should override the default password, but do not let your team's password be revealed in any public-facing repositories.

In the `setup` function, you need to call `.begin()` on the reset server. The Arduino/Ethernet Shield and the server are configured using the settings already stored in the EEPROM. This means that you *MUST* update the network settings on your Arduino with the "WriteNetworkSettings" sketch. 

The AthenaEthernetReset server should be started before any other servers are started.

```
void setup()
{
    Serial.begin(115200);
    while(!Serial) delay(10); 
    Serial.println("Starting Ethernet Reset Server Example");
  
    // Initializes the Ethernet shield + server
    reset.begin();
}
```

In your `loop()` function, you need to add a call to the `.check()` function:


```
void loop()
{
    // Check for reset or reprogram requests
    reset.check();
}
```

