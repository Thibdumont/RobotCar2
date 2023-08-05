#include "MotorManager.h"

MotorManager::MotorManager()
{
    pinMode(PIN_Motor_PWMA, OUTPUT);
    pinMode(PIN_Motor_PWMB, OUTPUT);
    pinMode(PIN_Motor_AIN_1, OUTPUT);
    pinMode(PIN_Motor_BIN_1, OUTPUT);
    pinMode(PIN_Motor_STBY, OUTPUT);
}

void MotorManager::applyRotation(MotorDirection leftDirection, uint16_t leftSpeed, MotorDirection rightDirection, uint16_t rightSpeed)
{
    digitalWrite(PIN_Motor_STBY, HIGH);

    if (leftSpeed > MOTOR_MAX_SPEED)
    {
        leftSpeed = MOTOR_MAX_SPEED;
    }
    if (rightSpeed > MOTOR_MAX_SPEED)
    {
        rightSpeed = MOTOR_MAX_SPEED;
    }

    switch (leftDirection)
    {
    case MotorDirection::FORWARD:
        digitalWrite(PIN_Motor_BIN_1, HIGH);
        analogWrite(PIN_Motor_PWMB, leftSpeed);
        break;
    case MotorDirection::BACKWARD:
        digitalWrite(PIN_Motor_BIN_1, LOW);
        analogWrite(PIN_Motor_PWMB, leftSpeed);
        break;
    default:
        break;
    }

    switch (rightDirection)
    {
    case MotorDirection::FORWARD:
        digitalWrite(PIN_Motor_AIN_1, HIGH);
        analogWrite(PIN_Motor_PWMA, rightSpeed);
        break;
    case MotorDirection::BACKWARD:
        digitalWrite(PIN_Motor_AIN_1, LOW);
        analogWrite(PIN_Motor_PWMA, rightSpeed);
        break;
    default:
        break;
    }
}

void MotorManager::stop()
{
    analogWrite(PIN_Motor_PWMB, 0);
    digitalWrite(PIN_Motor_STBY, LOW);
    analogWrite(PIN_Motor_PWMA, 0);
    digitalWrite(PIN_Motor_STBY, LOW);
}

void MotorManager::testMotor()
{
    applyRotation(MotorDirection::FORWARD, 100, MotorDirection::BACKWARD, 100);
    delay(1000);
    applyRotation(MotorDirection::BACKWARD, 100, MotorDirection::FORWARD, 100);
    delay(1000);
    stop();
}
