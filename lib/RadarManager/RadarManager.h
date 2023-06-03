#ifndef RADAR_MANAGER_H
#define RADAR_MANAGER_H

#include <Servo.h>
#include <Arduino.h>

#define TRIG_PIN 13      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 12      // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

class RadarManager
{
public:
    RadarManager();
    uint16_t getDistance();
    void testRadar();

private:
};

#endif