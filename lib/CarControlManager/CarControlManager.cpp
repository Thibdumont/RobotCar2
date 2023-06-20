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

void CarControlManager::setDirectionX(float directionX)
{
    this->directionX = directionX;
}
void CarControlManager::setSpeedThrottle(float speedThrottle)
{
    this->speedThrottle = speedThrottle;
}

void CarControlManager::applyMotorDirectionXAndThrottle()
{
    // Si pas d'input, on stoppe les moteurs
    if ((speedThrottle > -MOTOR_THROTTLE_DEADZONE && speedThrottle < MOTOR_THROTTLE_DEADZONE) && (directionX > -TURN_DEAD_ZONE && directionX < TURN_DEAD_ZONE))
    {
        motorManager->stop();
        return;
    }

    MotorDirection leftDirection;
    MotorDirection rightDirection;

    uint8_t leftSpeed = (uint8_t)((float)maxSpeed * abs(speedThrottle));
    uint8_t rightSpeed = (uint8_t)((float)maxSpeed * abs(speedThrottle));

    leftSpeed = getSpeed(MotorSide::LEFT, leftSpeed, speedThrottle, directionX);
    leftDirection = getDirection(MotorSide::LEFT, speedThrottle, directionX);
    rightSpeed = getSpeed(MotorSide::RIGHT, rightSpeed, speedThrottle, directionX);
    rightDirection = getDirection(MotorSide::RIGHT, speedThrottle, directionX);

    if (isGoingForwardSafe() || leftDirection == MotorDirection::BACKWARD || rightDirection == MotorDirection::BACKWARD)
    {
        motorManager->applyRotation(leftDirection, leftSpeed, rightDirection, rightSpeed);
    }
    else
    {
        stop();
    }
}

uint8_t CarControlManager::getSpeed(MotorSide motorSide, uint8_t baseSpeed, float speedThrottle, float directionX)
{
    if (speedThrottle < -MOTOR_THROTTLE_DEADZONE || speedThrottle > MOTOR_THROTTLE_DEADZONE)
    {
        if (motorSide == MotorSide::LEFT)
        {
            if (directionX < TURN_DEAD_ZONE)
            {
                return baseSpeed - (baseSpeed * abs(directionX) * (1 - abs(speedThrottle / 2)));
            }
            else
            {
                return baseSpeed;
            }
        }
        else if (motorSide == MotorSide::RIGHT)
        {
            if (directionX < TURN_DEAD_ZONE)
            {
                return baseSpeed;
            }
            else
            {
                return baseSpeed - (baseSpeed * abs(directionX) * (1 - abs(speedThrottle / 2)));
            }
        }
    }
    else
    {
        return maxSpeed * abs(directionX);
    }

    return 0;
}

MotorDirection CarControlManager::getDirection(MotorSide motorSide, float speedThrottle, float directionX)
{
    if (speedThrottle < -MOTOR_THROTTLE_DEADZONE)
    {
        return MotorDirection::BACKWARD;
    }
    else if (speedThrottle > MOTOR_THROTTLE_DEADZONE)
    {
        return MotorDirection::FORWARD;
    }
    else
    {
        if (motorSide == MotorSide::LEFT)
        {
            if (directionX < TURN_DEAD_ZONE)
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
            if (directionX < TURN_DEAD_ZONE)
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
