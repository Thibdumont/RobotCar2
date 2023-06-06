#include "MotorManager.h"

MotorManager::MotorManager()
{
    maxSpeed = 255;

    pinMode(PIN_Motor_PWMA, OUTPUT);
    pinMode(PIN_Motor_PWMB, OUTPUT);
    pinMode(PIN_Motor_AIN_1, OUTPUT);
    pinMode(PIN_Motor_BIN_1, OUTPUT);
    pinMode(PIN_Motor_STBY, OUTPUT);
}

void MotorManager::applyRotation(MotorDirection leftDirection, uint8_t leftSpeed, MotorDirection rightDirection, uint8_t rightSpeed)
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

int MotorManager::getMaxSpeed()
{
    return maxSpeed;
}

void MotorManager::setMaxSpeed(int maxSpeed)
{
    this->maxSpeed = maxSpeed;
    if (maxSpeed > MOTOR_MAX_SPEED)
    {
        this->maxSpeed = MOTOR_MAX_SPEED;
    }
}

void MotorManager::applyMotorTurnAndThrottle(float turnForce, float motorThrottle)
{
    this->turnForce = turnForce;
    this->motorThrottle = motorThrottle;

    // Si pas d'input, on stoppe les moteurs
    if ((motorThrottle > -MOTOR_THROTTLE_DEADZONE && motorThrottle < MOTOR_THROTTLE_DEADZONE) && (turnForce > -TURN_DEAD_ZONE && turnForce < TURN_DEAD_ZONE))
    {
        stop();
        return;
    }

    MotorDirection leftDirection;
    MotorDirection rightDirection;

    uint8_t leftSpeed = (uint8_t)((float)maxSpeed * abs(motorThrottle));
    uint8_t rightSpeed = (uint8_t)((float)maxSpeed * abs(motorThrottle));

    leftSpeed = getSpeed(MotorSide::LEFT, leftSpeed, motorThrottle, turnForce);
    leftDirection = getDirection(MotorSide::LEFT, motorThrottle, turnForce);
    rightSpeed = getSpeed(MotorSide::RIGHT, rightSpeed, motorThrottle, turnForce);
    rightDirection = getDirection(MotorSide::RIGHT, motorThrottle, turnForce);

    applyRotation(leftDirection, leftSpeed, rightDirection, rightSpeed);
}

uint8_t MotorManager::getSpeed(MotorSide motorSide, uint8_t baseSpeed, float motorThrottle, float turnForce)
{
    if (motorThrottle < -MOTOR_THROTTLE_DEADZONE || motorThrottle > MOTOR_THROTTLE_DEADZONE)
    {
        if (motorSide == MotorSide::LEFT)
        {
            if (turnForce < TURN_DEAD_ZONE)
            {
                return baseSpeed - (baseSpeed * abs(turnForce) * (1 - abs(motorThrottle / 2)));
            }
            else
            {
                return baseSpeed;
            }
        }
        else if (motorSide == MotorSide::RIGHT)
        {
            if (turnForce < TURN_DEAD_ZONE)
            {
                return baseSpeed;
            }
            else
            {
                return baseSpeed - (baseSpeed * abs(turnForce) * (1 - abs(motorThrottle / 2)));
            }
        }
    }
    else
    {
        return maxSpeed * abs(turnForce);
    }
}

MotorDirection MotorManager::getDirection(MotorSide motorSide, float motorThrottle, float turnForce)
{
    if (motorThrottle < -MOTOR_THROTTLE_DEADZONE)
    {
        return MotorDirection::BACKWARD;
    }
    else if (motorThrottle > MOTOR_THROTTLE_DEADZONE)
    {
        return MotorDirection::FORWARD;
    }
    else
    {
        if (motorSide == MotorSide::LEFT)
        {
            if (turnForce < TURN_DEAD_ZONE)
            {
                return MotorDirection::BACKWARD;
            }
            else
            {
                return MotorDirection::FORWARD;
            }
        }
        else
        {
            if (turnForce < TURN_DEAD_ZONE)
            {
                return MotorDirection::FORWARD;
            }
            else
            {
                return MotorDirection::BACKWARD;
            }
        }
    }
}
