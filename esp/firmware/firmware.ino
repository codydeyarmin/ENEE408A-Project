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
const char* host = "192.168.1.3";

WiFiClient client;
const int connPort = 40810;
String relayStr;

void setup() {
    Serial.begin(57600);
    delay(10);

    // We start by connecting to a WiFi network
    Serial.print("\nConnecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        //WiFi.begin(ssid, password);
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
}

void keepCommunicating() {
    // Reading from UART, sending to TCP 40810
    relayStr = "";
    while (Serial.available() > 0)
        relayStr += (char) Serial.read();
    if (relayStr.length() > 0)
        client.println(relayStr);

    // Reading from TCP 40810, sending to UART
    relayStr = "";
    while (client.available())
        relayStr += (char) client.read();
    if (relayStr.length() > 0)
        Serial.println(relayStr);

    // TIMEOUT LOOP (10 second timeout)
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 10000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            //ESP.restart();
            return 0;
        }
    }

    return 1;
}
