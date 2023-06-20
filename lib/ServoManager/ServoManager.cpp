#include "ServoManager.h"

ServoManager::ServoManager(TimeManager *timeManager)
{
    this->timeManager = timeManager;
    angle = 90;
    servo.attach(PIN_Servo_z, 500, 2400); // 500: 0 degree  2400: 180 degree
    servo.attach(PIN_Servo_z);
    servo.write(angle); // sets the servo position according to the 90（middle）
    delay(500);
    servo.detach();
}

void ServoManager::applyRotation(uint8_t newAngle)
{
    if (angle != newAngle)
    {
        servoTurnStartTime = timeManager->getLoopTime();
        // turnDuration = (abs(newAngle - angle) * 3) + 15;
        turnDuration = DELAY_BEFORE_SERVO_DETACH;
        angle = newAngle;

        servo.attach(PIN_Servo_z);
        servo.write(angle);
    }
}

bool ServoManager::isServoTurnOver()
{
    return timeManager->getLoopTime() - servoTurnStartTime > turnDuration;
}

void ServoManager::updateServo()
{
    if (isServoTurnOver()) // On attend que le servo ait terminé sa rotation avant de traiter l'état suivant
    {
        servo.detach();
    }
}

uint8_t ServoManager::getAngle()
{
    return angle;
}

void ServoManager::testServo()
{
    applyRotation(0);
}
