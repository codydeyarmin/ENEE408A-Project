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
const char* host = "192.168.1.4";
//byte flushSeq[4] = {0x34, 0x30, 0x38, 0x41}; // 408A

WiFiClient client;
const int connPort = 40812;
String relayStr;

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
    Serial.begin(74880);
    delay(10);
    
    flashBlue(500, 1);

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
    flashBlue(200, 5);
}

void loop() {
    delay(500);
    flashBlue(100, 3);
    //delay(50);
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    if (!client.connect(host, connPort)) {
        Serial.println("connection failed");
        digitalWrite(2,LOW);
    } else {
        Serial.println("connection established");
        digitalWrite(2,HIGH);
        // Keep a connection until it goes stale
        while(keepCommunicating());   
        Serial.println("loop end"); 
    }
}

int keepCommunicating() {
    //Serial.println("ARDUINO ----> Server");
    
    // Reading from UART, sending to TCP 40810
    if (Serial.available() > 0) {
      char c = Serial.read();
      client.write(c);
    }

    //Serial.println("Server ----> ARDUINO");
    // Reading from TCP 40810, sending to UART
    if (client.available() > 0) {
      char c = client.read();
      Serial.write(c);
    }

    // TIMEOUT LOOP (10 second timeout)
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
