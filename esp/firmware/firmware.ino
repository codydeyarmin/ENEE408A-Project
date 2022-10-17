#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// WiFi network name
#define ROOM 1116
#if ROOM == 1116
    #define WIFI_NETWORK "VisionSystem1116-2.4"
#elif ROOM == 1215
    #define WIFI_NETWORK "VisionSystem1215-2.4"
#endif

WiFiUDP client;
unsigned char seq = 0;

const char* VS_ADDRESS = "192.168.1.2";
const int VS_PORT = 7755;
const byte FLUSH_SEQUENCE[] = {0xFF, 0xFE, 0xFD, 0xFC};

void setup() {  
  // Begin serial communication with Arduino
  Serial.begin(9600);

  // Connect to Vision System network
  WiFi.begin(WIFI_NETWORK, NULL);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    ESP.restart();
  }

  // Begin listening for responses on port 7755
  client.begin(7755);
  
  // Open a packet for writing
  beginPacket();
}

void loop() {
  static int opcode, pos;
  static unsigned long lastWrite;
  static bool dataWritten;
  
  // Accept incoming data
  if (Serial.available()) {
    char c = Serial.read();
    if (c == FLUSH_SEQUENCE[pos]) {
      pos++;
    } else {
      // If this is the first byte of a payload, store it as the opcode
      if (!dataWritten) {
        opcode = c;
      }
      // If `c` does not continue the flush sequence,
      // write back the captured bytes and reset flush
      for (int i = 0; i < pos; i++) {
        client.write(FLUSH_SEQUENCE[i]);
      }
      pos = 0;
      // Write incoming byte
      client.write(c);
      dataWritten = true;
    }
    // Update time of last byte received for timeout
    lastWrite = millis();
  }
  
  // If the flush sequence is fully written, send data
  if (pos == 4) {
    unsigned char expectedSeq = endPacket();
    pos = 0;
    
    // If the opcode sent requires a response, start listening
    if (opcode == 0 || opcode == 2 || opcode == 4 || opcode == 6) {
      byte buffer[128];
      int packetSize;
      unsigned long start = millis();
      while (millis() - start < 100) {
        if ((packetSize = client.parsePacket())) {
          client.read(buffer, packetSize);
          if (buffer[0] == expectedSeq) {
            Serial.write(buffer + 1, packetSize - 1);
            break;
          }
        }
      }
    }
    
    dataWritten = false;
    beginPacket();
  }
  
  // Clear the write buffer after timeout
  if (dataWritten && millis() - lastWrite > 20) {
    ESP.restart();
  }
}

void beginPacket() {
  client.beginPacket(VS_ADDRESS, VS_PORT);
  client.write(seq);
}

unsigned char endPacket() {
  client.endPacket();
  return seq++;
}

