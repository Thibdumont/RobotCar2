#include "CarControlManager.h"

CarControlManager::CarControlManager(MotorManager *motorManager, RadarManager *radarManager)
{
    this->motorManager = motorManager;
    this->radarManager = radarManager;

    maxSpeed = MOTOR_MAX_SPEED;
    safeStopDistance = SAFE_STOP_DISTANCE;
    speedThrottle = 0;
    directionX = 0;
    boost = false;
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
    return radarManager->getDistance() > safeStopDistance;
}

void CarControlManager::setSafeStopDistance(int safeStopDistance)
{
    this->safeStopDistance = safeStopDistance;
}

int CarControlManager::getSafeStopDistance()
{
    return this->safeStopDistance;
}

void CarControlManager::setDirectionX(float directionX)
{
    this->directionX = directionX;
}

void CarControlManager::setSpeedThrottle(float speedThrottle)
{
    this->speedThrottle = speedThrottle;
}

void CarControlManager::setBoost(bool boost)
{
    this->boost = boost;
}

void CarControlManager::applyMotorDirectionXAndThrottle()
{
    // Si pas d'input, on stoppe les moteurs
    if ((speedThrottle > -MOTOR_THROTTLE_DEADZONE && speedThrottle < MOTOR_THROTTLE_DEADZONE) && (directionX > -TURN_DEAD_ZONE && directionX < TURN_DEAD_ZONE))
    {
        motorManager->stop();
        return;
    }

    int _maxSpeed = this->boost ? MOTOR_MAX_SPEED : this->maxSpeed;
    MotorDirection leftDirection;
    MotorDirection rightDirection;

    uint16_t leftSpeed = (uint16_t)((float)_maxSpeed * abs(speedThrottle));
    uint16_t rightSpeed = (uint16_t)((float)_maxSpeed * abs(speedThrottle));

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

uint16_t CarControlManager::getSpeed(MotorSide motorSide, uint16_t baseSpeed, float speedThrottle, float directionX)
{
    if (speedThrottle < -MOTOR_THROTTLE_DEADZONE || speedThrottle > MOTOR_THROTTLE_DEADZONE)
    {
        if (motorSide == MotorSide::LEFT)
        {
            if (directionX < TURN_DEAD_ZONE)
            {
                return baseSpeed - this->getDirectionSpeedModifier(baseSpeed, speedThrottle, directionX);
            }
            else
            {
                return baseSpeed + this->getDirectionSpeedModifier(baseSpeed, speedThrottle, directionX);
            }
        }
        else if (motorSide == MotorSide::RIGHT)
        {
            if (directionX < TURN_DEAD_ZONE)
            {
                return baseSpeed + this->getDirectionSpeedModifier(baseSpeed, speedThrottle, directionX);
            }
            else
            {
                return baseSpeed - this->getDirectionSpeedModifier(baseSpeed, speedThrottle, directionX);
            }
        }
    }
    else
    {
        return maxSpeed * abs(directionX);
    }

    return 0;
}

uint16_t CarControlManager::getDirectionSpeedModifier(uint16_t baseSpeed, float speedThrottle, float directionX)
{
    return (baseSpeed * abs(directionX) * (1 - abs(speedThrottle / 2)));
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
