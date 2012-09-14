#include "EthernetReset.h"

void EthernetReset::stdResponce(char* msg)
{
	_client.println("HTTP/1.1 200 OK");
	_client.println("Content-Type: text/html");
	_client.println("Connnection: close");
	_client.println();
	_client.println("<!DOCTYPE HTML>");
	_client.println("<html>");
	_client.println(msg);
	_client.println("</html>");
}

void EthernetReset::watchdogReset()
{
	stop();
	wdt_disable();
	wdt_enable(WDTO_2S);
	while(1);
}

void EthernetReset::stop(void)
{
	delay(10);
	_client.stop();
}

void EthernetReset::begin()
{
	if(NetEEPROM.sigIsSet()) {
		Ethernet.begin(NetEEPROM.readMAC(), NetEEPROM.readIP(), NetEEPROM.readGW(),
					   NetEEPROM.readGW(), NetEEPROM.readSN());

		_server->begin();
	}
}

void EthernetReset::check()
{
	/* 25 is the the maximum command lenth plus
	 * the standart GET and HTTP prefix and postfix */
	char http_req[_path_len + 25];
	_client = _server->available();
	if(_client) {
		DBG(Serial.println("new reset client");)
		while(_client.connected()) {
			if(_client.available()) {
				char c;
				char* url = http_req;
				while(( c = _client.read()) != '\n'){
					*url = c;
					url++;
				}
				DBG(*url = '\0';)
				url = http_req + 5;
				_client.flush();
				DBG(Serial.println(url);)
				if(!strncmp(url, _path, _path_len)) {
					url += (_path_len + 1);
					if(!strncmp(url, "reset", 5)) {
						stdResponce("Arduino will be doing a normal reset in 2 seconds");
						watchdogReset();
					} else if(!strncmp(url,"reprogram", 9)) {
						stdResponce("Arduino will reset for reprogramming in 2 seconds");
						NetEEPROM.writeImgBad();
						watchdogReset();
					} else stdResponce("Wrong command");
				} else stdResponce("Wrong path");
				break;
			}
		}
		stop();
		DBG(Serial.println("reset client disonnected");)
	}
}

