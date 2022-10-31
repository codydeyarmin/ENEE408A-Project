#include "Arduino.h"
#include "SoftwareSerial.h"
#include "ArduinoJson.h"

int tx = 11;
int rx = 10;
SoftwareSerial* mSerial = new SoftwareSerial(rx, tx); //connects to wifi module
//DynamicJsonDocument doc(1024);
String receiving, sending;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  
  mSerial->begin(57600); 
  delay(5000);
}

int receive() {
  receiving = "";
  while (mSerial->available()) {
    char c = (char) mSerial->read();
    receiving += c;
    //Serial.print((byte)c);
    //Serial.print(" ");
  }
  Serial.println("");
  if (receiving.length() > 0) {
    Serial.print("Received: ");
    Serial.println(receiving);  
  }

  //Serial.print((receiving != "connection established"));
  return (receiving != "connection established\r\n");
}

void sendTo() {
  Serial.println("Sending message to TCP server from Arduino");
  sending = "";
  mSerial->println("Hello from Arduino Uno!!");
}

void loop() {
  // put your main code here, to run repeatedly:

  while (receive());
  delay(1000);
  Serial.println("!!! received connection established !!!");
  sendTo();
  delay(1000);
  receive();
  
  delay(300000);
}
