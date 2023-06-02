#include "ServoManager.h"

ServoManager::ServoManager()
{
    angle = 90;
    servo.attach(PIN_Servo_z, 500, 2400); // 500: 0 degree  2400: 180 degree
    servo.attach(PIN_Servo_z);
    servo.write(angle); // sets the servo position according to the 90（middle）
    delay(500);
    servo.detach();
}

void ServoManager::applyRotation(uint8_t newAngle, unsigned long currentTime)
{
    if (angle != newAngle)
    {
        servoTurnStartTime = currentTime;
        turnDuration = (abs(newAngle - angle) * 3) + 10;
        angle = newAngle;

        servo.attach(PIN_Servo_z);
        servo.write(angle);
    }
}

bool ServoManager::isServoTurnOver(unsigned long currentTime)
{
    return currentTime - servoTurnStartTime > turnDuration;
}

void ServoManager::updateServo(unsigned long currentTime)
{
    if (isServoTurnOver(currentTime)) // On attend que le servo ait terminé sa rotation avant de traiter l'état suivant
    {
        servo.detach();
    }
}

void ServoManager::testServo(unsigned long currentTime)
{
    applyRotation(0, currentTime);
}
