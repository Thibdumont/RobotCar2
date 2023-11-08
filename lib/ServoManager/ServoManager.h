#ifndef SERVO_MANAGER_H
#define SERVO_MANAGER_H

#include <Arduino.h>
#include <ServoEasing.h>
#include "TimeManager.h"

#define PIN_Servo_z 10
#define SERVO_SPEED 120

class ServoManager
{
public:
    ServoManager(TimeManager *);
    void applyRotation(uint8_t);
    uint8_t getAngle();
    void testServo();

private:
    TimeManager *timeManager;
    ServoEasing servo;
    uint8_t angle;
};

#endif