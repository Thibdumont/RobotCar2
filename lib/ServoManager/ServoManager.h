#ifndef SERVO_MANAGER_H
#define SERVO_MANAGER_H

#include <Arduino.h>
#define SUPPRESS_HPP_WARNING
#include <ServoEasing.h>
#include "TimeManager.h"

#define PIN_Servo_z 10
#define SERVO_DEFAULT_SPEED 120

class ServoManager
{
public:
    ServoManager(TimeManager *);
    void applyRotation(uint8_t);
    uint8_t getAngle();
    void setServoSpeed(uint8_t);
    uint8_t getServoSpeed();
    void testServo();

private:
    TimeManager *timeManager;
    ServoEasing servo;
    uint8_t angle;
    uint8_t servoSpeed;
};

#endif