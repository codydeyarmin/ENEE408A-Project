#include <ESP8266WiFi.h>
//#include <WiFiUdp.h>
#include "WebSocketClient.h"

// WiFi network name
#define WIFI_NETWORK "Gary's Hotspot"
#define CLIENT_ADDRESS "192.168.1.2"
#define PORT 40810

//WiFiUDP client;
WiFiClient client;
unsigned char seq = 0;
char buff[100];
byte buff_index = 0;

//const char* WIFI_ADDRESS = CLIENT_ADDRESS;
//const int VS_PORT = PORT;
const byte FLUSH_SEQUENCE[] = {0xFF, 0xFE, 0xFD, 0xFC}; // what is this exactly??

void setup() {  
	// Begin serial communication with Arduino
	Serial.begin(9600);

	// Connect to WiFi Network
	WiFi.begin(WIFI_NETWORK, NULL);
	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		ESP.restart();
	}

	// Begin listening for responses on port 7755
	client.connect(CLIENT_ADDRESS, PORT);
}

void loop() {
	// Read in data from STM32L4A6
	while (Serial.available() > 0) {
	buff[buff_index++] = Serial.read();
		if(!client.connected())
			ESP.restart();
	  
		if (buff[buff_index-1] == '\n') {
			client.write(buff, buff_index);
			buff_index = 0; 
        }
	}
	
	// Read everything from TCP40810 and print them to Serial
	while (client.available()) {
		char ch = static_cast<char>(client.read());
		Serial.write(ch);
	}	
}


