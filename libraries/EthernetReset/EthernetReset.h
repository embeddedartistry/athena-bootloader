#ifndef EthernetReset_h
#define EthernetReset_h

#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <string.h>
#include <Arduino.h>
#include <NetEEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetServer.h>
#include <EthernetClient.h>

#define P(name)   static const prog_uchar name[] PROGMEM
//#define DEBUG
#ifdef DEBUG
	#define DBG(c) c
#else
	#define DBG(c)
#endif


class EthernetReset
{
	private:
		EthernetServer* _server;
		EthernetClient _client;
		void stdResponce(char* msg);
		void watchdogReset();
		void stop(void);
		char* _path;
		size_t _path_len;

	public:
		//EthernetReset(String);
		EthernetReset(char* path, word port = NetEEPROM.readPort()) {
			_path = path;
			_path_len = strlen(_path);
			_server = new EthernetServer(port);
		}

		void begin(void);
		void check(void);
};

#endif
