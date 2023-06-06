#ifndef SERVO_MANAGER_H
#define SERVO_MANAGER_H

#include <Servo.h>
#include <Arduino.h>
#include "TimeManager.h"

#define PIN_Servo_z 10

class ServoManager
{
public:
    ServoManager(TimeManager *);
    void applyRotation(uint8_t);
    bool isServoTurnOver();
    void updateServo();
    uint8_t getAngle();
    void testServo();

private:
    TimeManager *timeManager;
    Servo servo;
    uint8_t angle;
    unsigned long servoTurnStartTime;
    unsigned long turnDuration;
};

#endif