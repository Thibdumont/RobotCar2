#include "RadarManager.h"

RadarManager::RadarManager()
{
    pinMode(ECHO_PIN, INPUT); // Ultrasonic module initialization
    pinMode(TRIG_PIN, OUTPUT);
}

uint16_t RadarManager::getDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    return ((unsigned int)pulseIn(ECHO_PIN, HIGH) / 58);
}

void RadarManager::testRadar()
{
    Serial.print("Radar : ");
    Serial.println(getDistance());
}
