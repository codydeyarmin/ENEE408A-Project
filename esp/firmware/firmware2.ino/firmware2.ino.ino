//#include <dummy.h>

/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include <Esp.h>

const char* ssid     = "ENEE408A Drone Team";
const char* password = "dr0n3sAr3R3@llyC00l!";

const char* host = "192.168.1.3";
const char* streamId   = "....................";
const char* privateKey = "....................";

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

    // TIMEOUT LOOP
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 10000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            //ESP.restart();
            return;
        }
    }
}
