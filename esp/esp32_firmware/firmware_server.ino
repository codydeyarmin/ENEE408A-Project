/*
 *  firmware_server.ino
 *  
 *  Firmware flashed onto an ESP32 for communication between 
 *  UART on STM and a PC
 */

#include <WiFi.h>
#include <Esp.h>

const char* ssid     = "ENEE408A Drone Team";
const char* password = "dr0n3sAr3R3@llyC00l!";
//const char* ssid     = "ENEE408A ESP32 AP";
//const char* password = "ESP32dr0n3sAr3R3@llyC00l!";
//const char* host = "192.168.1.4";
//byte flushSeq[4] = {0x34, 0x30, 0x38, 0x41}; // 408A
WiFiServer wifiServer(40810);

//WiFiClient client;
//const int connPort = 40812;
//String relayStr;

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 24);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

void flashBlue(int ms, int num) {
    for (int i=0; i < num; i++) {
        digitalWrite(2,HIGH);
        delay(ms);
        digitalWrite(2,LOW);
        delay(ms);  
    }
}

void setup() {
    pinMode(2,OUTPUT);
    Serial.begin(250000);
    //Serial.begin(115200);
    delay(10);
    
    flashBlue(500, 1);

    //WiFi.softAP(ssid, password);
    //IPAddress IP = WiFi.softAPIP();
  
    // We start by connecting to a WiFi network
    //Serial.print("\nConnecting to ");
    //Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        //Serial.print(".");
    }

    //Serial.println("");
    //Serial.print("\nWiFi connected. IP address: ");
    //Serial.println(WiFi.localIP());
    
    wifiServer.begin();
    flashBlue(200, 5);
}

void loop() {
    delay(500);
    flashBlue(100, 3);
    //delay(50);
    //Serial.print("connecting to ");
    //Serial.println(host);

    WiFiClient client = wifiServer.available();
    if (client) {
      digitalWrite(2,HIGH);
      while (client.connected()) {
          // Reading from UART, sending to TCP 40810
          if (Serial.available() > 0) {
              char c = Serial.read();
              client.write(c);
              //Serial.print("(STM -> Unity)");
              //Serial.println((byte) c);
          }
    
          //Serial.println("Server ----> ARDUINO");
          // Reading from TCP 40810, sending to UART
          if (client.available() > 0) {
              char c = client.read();
              Serial.write(c);
              //Serial.print("(Unity -> STM)");
              //Serial.println((byte) c);
          }
      }
 
      client.stop();
      digitalWrite(2,LOW);
      //Serial.println("Client disconnected");
  }
}
