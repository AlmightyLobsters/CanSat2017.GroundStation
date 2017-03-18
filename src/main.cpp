#include <RFM69.h>

#define FREQUENCY RF69_433MHZ
#define LFREQUENCY 434200000
#define NODEID 100
#define NETWORKID 42
#define PROMISCUOUS true

RFM69 radio(10, 2, true, digitalPinToInterrupt(2));

template<typename T>
	bool TrySuccess(T, T, char* = (char*)"");

void setup() {
  Serial.begin(9600);
	Serial.println("Starting");
	delay(1000);
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
  radio.setHighPower(true);
  radio.setFrequency(LFREQUENCY);
	radio.promiscuous(PROMISCUOUS);
	Serial.println("Listening at 433 Mhz");
}

byte ackCount=0;
uint32_t packetCount = 0;
void loop() {
  if(radio.receiveDone()) {
    Serial.print("#[");
    Serial.print(++packetCount);
    Serial.print(']');
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
    if (PROMISCUOUS)
    {
      Serial.print("to [");Serial.print(radio.TARGETID, DEC);Serial.print("] ");
    }
    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]); //TODO deserialize data or add a parser to js
    Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");

    if (radio.ACKRequested())
    {
      byte theNodeID = radio.SENDERID;
      radio.sendACK();
      Serial.print(" - ACK sent.");

      // When a node requests an ACK, respond to the ACK
      // and also send a packet requesting an ACK (every 3rd one only)
      // This way both TX/RX NODE functions are tested on 1 end at the GATEWAY
      if (ackCount++%3==0)
      {
        Serial.print(" Pinging node ");
        Serial.print(theNodeID);
        Serial.print(" - ACK...");
        delay(3); //need this when sending right after reception .. ?
        if (radio.sendWithRetry(theNodeID, "ACK TEST", 8, 0))  // 0 = only 1 attempt, no retries
          Serial.print("ok!");
        else Serial.print("nothing");
      }
    }
    Serial.println();
  }
  delay(500);
}

template<typename T>
	bool TrySuccess(T result, T expect, char* action)
	{
		if (result == expect)
			printf("Success %s\n", action);
		else
			printf("Error %s. Error: %d\n", action, result);
		return result == expect;
	}
