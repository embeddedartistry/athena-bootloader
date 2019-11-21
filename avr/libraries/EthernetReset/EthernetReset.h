#ifndef EthernetReset_h
#define EthernetReset_h

#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <string.h>
#include <Arduino.h>
#include <NewEEPROM.h>
#include <NetEEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetServer.h>
#include <EthernetClient.h>

#ifndef ETHERNETRESET_DEBUG
#define ETHERNETRESET_DEBUG 0
#endif

#if ETHERNETRESET_DEBUG > 0
	#define ETHERNET_DEBUG(c) c
#else
	#define ETHERNET_DEBUG(c)
#endif

class EthernetReset
{
	private:
		EthernetServer* _server;
		EthernetClient _client;
		char _path[20];

		void stdResponse(const char* msg);
		void watchdogReset();
		void stop(void);

	public:
		EthernetReset(int port);
		~EthernetReset() = default;

		void begin(void);
		void check(void);
};

#endif
