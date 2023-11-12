#include "ServoManager.h"
#include <ServoEasing.hpp>

ServoManager::ServoManager()
{
    this->servoSpeed = SERVO_DEFAULT_SPEED;
    servo.attach(PIN_Servo_z, 500, 2400); // 500: 0 degree  2400: 180 degree
    servo.attach(PIN_Servo_z);
    servo.setEasingType(EASE_LINEAR);
    servo.easeTo(90, servoSpeed); // set the servo position in the middle
}

void ServoManager::applyRotation(uint8_t newAngle)
{
    if (angle != newAngle)
    {
        angle = newAngle;
        servo.startEaseTo(angle, servoSpeed, START_UPDATE_BY_INTERRUPT);
    }
}

uint8_t ServoManager::getAngle()
{
    return angle;
}

void ServoManager::setServoSpeed(uint8_t servoSpeed)
{
    this->servoSpeed = servoSpeed;
}

uint8_t ServoManager::getServoSpeed()
{
    return servoSpeed;
}

void ServoManager::testServo()
{
    servo.startEaseTo(angle, servoSpeed, START_UPDATE_BY_INTERRUPT);
}
