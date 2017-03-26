#include "../lib/RadioHead/RH_RF69.h"
#include <Arduino.h>

#define LOBSTER_FREQUENCY 434.200

RH_RF69 radio(10, 2);

struct
{
    uint8_t lat_o;
    uint8_t lon_o;
    uint16_t temp;
    uint16_t pres;
    uint16_t hmdt;
    float time;
    float lat;
    float lon;
    float alt;
    float batlvl;
    float velocity;
    int16_t acc[3];
    int16_t gyro[3];
    int16_t mag[3];
    int8_t rssi;
} data;

void setup()
{
    Serial.begin(9600);
    while(!Serial)
        ;
    if (!radio.init())
        Serial.println("Radio initialization failed");
    if (!radio.setFrequency(LOBSTER_FREQUENCY))
        Serial.println("Radio initialization failed");
    radio.setTxPower(14, true);
    radio.setEncryptionKey(NULL);
}

void loop()
{
    if (radio.available())
    {
        uint8_t len = sizeof(data);
        if (radio.recv((uint8_t*)&data, &len))
        {
            data.rssi = radio.rssiRead();
            for(uint8_t i = 0; i < len; i++)
                Serial.write(((uint8_t*)&data)[i]);
        }
        else { Serial.println("Receive failed"); }
    }
}