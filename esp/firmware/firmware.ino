/*
 *  firmware.ino
 *  
 *  Firmware flashed onto an ESP32 for communication between 
 *  UART on STM and a PC
 */

#include <WiFi.h>
#include <Esp.h>

const char* ssid     = "ENEE408A Drone Team";
const char* password = "dr0n3sAr3R3@llyC00l!";
const char* host = "192.168.1.2";
//byte flushSeq[4] = {0x34, 0x30, 0x38, 0x41}; // 408A

WiFiClient client;
const int connPort = 40810;
String relayStr;

void setup() {
    Serial.begin(74880);
    delay(10);

    // We start by connecting to a WiFi network
    Serial.print("\nConnecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("\nWiFi connected. IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    delay(500);
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    if (!client.connect(host, connPort)) {
        Serial.println("connection failed");
        return;
    } else {
        Serial.println("connection established");
    }

    // Keep a connection until it goes stale
    while(keepCommunicating());   
    Serial.println("loop end"); 
}

int keepCommunicating() {
    //Serial.println("ARDUINO ----> Server");
    
    // Reading from UART, sending to TCP 40810
    if (Serial.available() > 0) {
      char c = Serial.read();
      client.write(c);
    }

    /*
    relayStr = "";
    while (Serial.available() > 0){
        relayStr += (char) Serial.read();
        if (relayStr.length() > 4) {
            Serial.println(relayStr.substring(relayStr.length() - 4));
        }

        if (relayStr.length() > 4 && relayStr.substring(relayStr.length() - 4) == "408A") {
          break;
        }
    }
    
    if ((relayStr.length() > 4) && relayStr.substring(relayStr.length() - 4) == "408A") { //relayStr[relayStr.length() - 1] == '\n' && relayStr[relayStr.length() - 2] == '\r') {
        Serial.println(relayStr);
        client.print(relayStr);
    }
    */

    //Serial.println("Server ----> ARDUINO");
    // Reading from TCP 40810, sending to UART
    if (client.available() > 0) {
      char c = client.read();
      Serial.write(c);
    }

    /*
    relayStr = "";
    while (client.available() > 0) { 
       relayStr += (char) client.read();
    }
    if (relayStr.length() > 0) {
        Serial.println(relayStr);
    }
    */


    //Serial.println("... timeout loop ...");
    // TIMEOUT LOOP (10 second timeout)
    //return 1;
    unsigned long timeout = millis();
    while (!client.connected()) {
        if (millis() - timeout > 500) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            //ESP.restart();
            return 0;
        }
    }

    return 1;
}
