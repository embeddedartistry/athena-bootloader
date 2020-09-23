## Debugging Guide

If you cannot connect to the bootloader over TFTP, here are suggested debugging steps:

* [ ] Check your network configuration:
    * [ ] Is the network configured properly so you can talk to your device?
    * [ ] Is the IP address assigned to your device what you're expecting?
    * [ ] Does the device IP address match the programmed settings?
    * [ ] Do you need to disable firewalls on your router or your computer?
        * For example, port 46969 is usually blocked by the Windows firewall
* [ ] Ping IP of the bootloader when you're in bootloader mode to make sure you get a response
* [ ] Check the bootloader using a debug version. Does the TFTP server initialize correctly? If not, check:
    * [ ] Do you need to specify a custom CS pin for your board in the EEPROM?
    * [ ] Do you need to enable reset behavior for the Ethernet chip by specifying a reset pin in the EEPROM?
* [ ] Load a Web server example sketch (such as Ethernet2->WebServer) and configure the sketch to use the same IP settings. Confirm that you can talk to the device over HTTP
    * If you can't connect to the device, check your network settings
* [ ] Load the AthenaEthernetReset/ResetServer example sketch and make sure you can connect to the reset server and issue a device reset
    * [ ] If you can't connect, ping the IP and see if you get a response. 
        * If there is no response, check your network settings
    * [ ] If you can connect to Reset server, issue an HTTP request to trigger reset mode.
* [ ] Having succeeded in connecting to the ResetServer, issue a reprogram command. Then, ping the bootloader to make sure you get a response
* [ ] Attempt TFTP update

In most cases, the problem is related to the network configuration. The device IP on the network does not have the programmed value. You will want to update the network settings with the AthenaEEPROM library example sketch.

To see what values are being used by the bootloader, you can program the debug variant of the bootloader. MAC and IP information will be printed out during the boot process.

## Timeout with "Sent WRQ"

If you see this timeout error pattern, TFTP is not receiving a response from that device. Do not trust the tftp "Connected to" status response. You likely have a network configuration problem. Follow the debugging steps above.

```
$ tftp 10.0.1.199
tftp> status
Connected to 10.0.1.199.
Mode: netascii Verbose: off Tracing: off
Rexmt-interval: 5 seconds, Max-timeout: 25 seconds
tftp> trace
Packet tracing on.
tftp> verbose
Verbose mode on.
tftp> mode octet
mode set to octet
tftp> put Blink2x.bin
putting Blink2x.bin to 10.0.1.199:Blink2x.bin [octet]
sent WRQ <file=Blink2x.bin, mode=octet>
sent WRQ <file=Blink2x.bin, mode=octet>
sent WRQ <file=Blink2x.bin, mode=octet>
sent WRQ <file=Blink2x.bin, mode=octet>
sent WRQ <file=Blink2x.bin, mode=octet>
Transfer timed out.
```

## WRQ message is received by device, but transfer fails

If the WRQ message is received by the bootloader, but the transfer still fails, you likely have port 46969 (or whatever the data port is) blocked on your router or computer. You can see this behavior by noticing no data is received after the "Changed to port" message occurs.

```
Tftp: Processing packet of size 0x0038
Tftp: Set up return address
Tftp: This is block 0x704C with opcode 0x0002 and data length 0x002C
Tftp: Write request
Tftp: Init socket to port 0xB779
Tftp: Init socket done
Tftp: Changed to port 0xB779
Tftp: Sent ACK
Tftp: Response sent
```

## Socket Close or Socket Open Errors

Errors like the following indicate problems communicating with the Ethernet chip. Check your CS pin definition (you may need to specify a custom pin using the EEPROM). Also check to see if you need to enable bootloader reset behavior by specifying a reset pin in the EEPROM.

```
Tftp: Init socket to port 0x0045
Tftp: Socket close error
Tftp: TFTP server init done
```
