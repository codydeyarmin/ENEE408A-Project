#include "Arduino.h"
#include "SoftwareSerial.h"
#include "ArduinoJson.h"
#include "MsgPack.h"  

int tx = 11, rx = 10;
SoftwareSerial mSerial(rx, tx); //connects to wifi module

int aX, aY, aZ, gX, gY, gZ;
float temp;

int dX, dY, dZ;
String stuff;

String receiving, sending;
bool initial = false;
byte flushSeq[4] = {0x34, 0x30, 0x38, 0x41}; // 408A

// Generate random values to send to server as test
void generateTelemetry() {
  aX = random(-1024, 1024);
  aY = random(-1024, 1024);
  aZ = random(-1024, 1024);
  gX = random(-1024, 1024);
  gY = random(-1024, 1024);
  gZ = random(-1024, 1024);
  temp = (float) (random(0, 1024) / 5);
}

// RECEIVE
int receiveData() {
  Serial.println("idk trying to receive");
  delay(500);
  receiving = "";
  byte test[256];
  int idx = 0;
  
  while (mSerial.available() > 0) {
    char c = (char) mSerial.read();
    test[idx++] = (byte) c;
    receiving += c;
    
    //Serial.print((byte)c);
    //Serial.print(" ");
  }

  delay(500);
  Serial.println("check length");
  delay(500);
  if (receiving.length() > 0) {
    Serial.print("Received bytes: ");
    Serial.println(receiving); 

    if (initial) {
      MsgPack::Unpacker unpacker;
      unpacker.feed(test, idx);
      unpacker.deserialize(dX, dY, dZ, stuff);
      
      Serial.print("Received data: ");
      Serial.print(dX);
      Serial.print(", ");
      Serial.print(dY);
      Serial.print(", ");
      Serial.print(dZ);
      Serial.print(", ");
      Serial.println(stuff);
    }
  }
  
  delay(500);
  //Serial.print((receiving != "connection established"));
  return (receiving != "connection established\r\n");
}

// SEND
void sendData() {
  MsgPack::Packer packer;
  generateTelemetry();
  //int first = 1024;
  packer.serialize(aX, aY, aZ, gX, gY, gZ);
  
  Serial.println("Sending telemetry data: 256, ");
  Serial.print(aX);
  Serial.print(", ");
  Serial.print(aY);
  Serial.print(", ");
  Serial.print(aZ);
  Serial.print(", ");
  Serial.print(gX);
  Serial.print(", ");
  Serial.print(gY);
  Serial.print(", ");
  Serial.print(gZ);
  Serial.print(" ---- ");
  Serial.write((char *) packer.data());
  Serial.println(" ");
  
  
  delay(500);
  sending = "";
  //mSerial->write("Hello from Arduino Uno!!");
  mSerial.write((char *)packer.data());
  mSerial.write((char *) flushSeq);
}

void setup() {
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  mSerial.begin(9600); 
  Serial.println("begin");
  //delay(500);
  //while (receiveData());
  //initial = true;
  //Serial.println("!!! received connection established !!!");
  delay(1000);
}

void loop() {
  sendData();
  receiveData();
  Serial.println("end loop");
}
