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
  //Serial.println("idk trying to receive");
  //delay(500);
  receiving = "";
  byte test[256];
  int idx = 0;

//  while (1) {
//    
//  }
  while (mSerial.available() > 0) {
    char c = (char) mSerial.read();
    //Serial.print((char)(((byte) c) * 5 / 3.3));
    Serial.print((char)(((byte) c) & 127));
    //Serial.print(" ");
    //test[idx++] = (byte) c;
    //receiving += c;
    
    //Serial.print((byte)c);
    //Serial.print(" ");
  }

  //delay(500);
  //Serial.println("check length");
  //delay(500);
//  if (receiving.length() > 0) {
//    Serial.print("Received bytes: ");
//    Serial.println(receiving); 
//  }
  
  delay(10);
  //Serial.print((receiving != "connection established"));
  return (receiving != "connection established\r\n");
}

// SEND
void sendData() {
  MsgPack::Packer packer;
  generateTelemetry();
  //int first = 1024;
  packer.serialize(aX, aY, aZ, gX, gY, gZ);
  
  Serial.print("Sending telemetry data: ");
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
  for(int i=0; i < packer.size(); i++) {
    Serial.print((int) packer.data()[i], HEX);
    Serial.print(" ");
  }
  //Serial.write((char *) packer.data());
  Serial.println(" ");
  
  
  //delay(500);
  sending = "";
  //mSerial->write("Hello from Arduino Uno!!");
  mSerial.write((char *)packer.data());
  mSerial.write((char *) flushSeq);
}

void setup() {
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  
  // put your setup code here, to run once:
  Serial.begin(38400);
  mSerial.begin(38400); 
  Serial.println("begin");
  //delay(500);
  //while (receiveData());
  //initial = true;
  //Serial.println("!!! received connection established !!!");
  delay(1000);
}

void loop() {
  //sendData();
  receiveData();
  //delay(5000);
  //Serial.println("end loop");
}
