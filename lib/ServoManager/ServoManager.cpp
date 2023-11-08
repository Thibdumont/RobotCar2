#include "ServoManager.h"
#include <ServoEasing.hpp>

ServoManager::ServoManager(TimeManager *timeManager)
{
    this->timeManager = timeManager;
    servo.attach(PIN_Servo_z, 500, 2400); // 500: 0 degree  2400: 180 degree
    servo.attach(PIN_Servo_z);
    servo.setEasingType(EASE_LINEAR);
    servo.easeTo(90, SERVO_SPEED); // set the servo position in the middle
}

void ServoManager::applyRotation(uint8_t newAngle)
{
    if (angle != newAngle)
    {
        angle = newAngle;
        servo.startEaseTo(angle, SERVO_SPEED, START_UPDATE_BY_INTERRUPT);
    }
}

uint8_t ServoManager::getAngle()
{
    return angle;
}

void ServoManager::testServo()
{
    servo.startEaseTo(angle, 0, START_UPDATE_BY_INTERRUPT);
}
