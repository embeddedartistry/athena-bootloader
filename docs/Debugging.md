## Debugging Guide

If you cannot connect to the bootloader over TFTP, here are suggested debugging steps:

* [ ] Check your network configuration:
    * [ ] Is the network configured properly so you can talk to your device?
    * [ ] Is the IP address assigned to your device what you're expecting?
    * [ ] Does the device IP address match the programmed settings?
* [ ] Ping IP of the bootloader when you're in bootloader mode to make sure you get a response
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
