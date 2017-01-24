#include "../lib/RFM69/RFM69.h"

#define LOBSTER_FREQ 434200000
#define NODEID 97
#define NETWORKID 8
#define ENCRYPT_KEY "AlmightyLobsters"

RFM69 radio(10, 2, true, digitalPinToInterrupt(2));

char buffer[50];

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Serial initialized");
  Serial.println("Proceeding to initialize radio");
  while(!radio.initialize(FREQUENCY, NODEID, NETWORKID))
    Serial.println("Error iniitalizing radio");
  Serial.println("Radio initialized");
  radio.setHighPower();
  radio.encrypt(ENCRYPT_KEY);
  radio.setFrequency(LOBSTER_FREQ);
  Serial.println("Listening");
}

void loop() {
  if (radio.receiveDone())
  {
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");

    for(byte i = 0; i < radio.DATALEN; i++)
      Serial.print(radio.DATA[i]);

    Serial.print(" [RX_RSSI:");Serial.print(radio.readRSSI());Serial.println("]");
  }
}
