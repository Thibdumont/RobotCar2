#include "CarControlManager.h"

CarControlManager::CarControlManager(MotorManager *motorManager, RadarManager *radarManager)
{
    this->motorManager = motorManager;
    this->radarManager = radarManager;

    maxSpeed = MOTOR_MAX_SPEED;
}

int CarControlManager::getMaxSpeed()
{
    return maxSpeed;
}

void CarControlManager::setMaxSpeed(int maxSpeed)
{
    this->maxSpeed = maxSpeed;
    if (maxSpeed > MOTOR_MAX_SPEED)
    {
        this->maxSpeed = MOTOR_MAX_SPEED;
    }
}

void CarControlManager::stop()
{
    motorManager->stop();
}

bool CarControlManager::isGoingForwardSafe()
{
    return radarManager->getDistance() > 8;
}

void CarControlManager::applyMotorTurnAndThrottle(float turnForce, float motorThrottle)
{
    this->turnForce = turnForce;
    this->motorThrottle = motorThrottle;

    // Si pas d'input, on stoppe les moteurs
    if ((motorThrottle > -MOTOR_THROTTLE_DEADZONE && motorThrottle < MOTOR_THROTTLE_DEADZONE) && (turnForce > -TURN_DEAD_ZONE && turnForce < TURN_DEAD_ZONE))
    {
        motorManager->stop();
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

    if (isGoingForwardSafe() || leftDirection == MotorDirection::BACKWARD || rightDirection == MotorDirection::BACKWARD)
    {
        motorManager->applyRotation(leftDirection, leftSpeed, rightDirection, rightSpeed);
    }
    else
    {
        stop();
    }
}

uint8_t CarControlManager::getSpeed(MotorSide motorSide, uint8_t baseSpeed, float motorThrottle, float turnForce)
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

    return 0;
}

MotorDirection CarControlManager::getDirection(MotorSide motorSide, float motorThrottle, float turnForce)
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
