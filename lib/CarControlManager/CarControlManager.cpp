#include "CarControlManager.h"

CarControlManager::CarControlManager(MotorManager *motorManager, RadarManager *radarManager, TimeManager *timeManager)
{
    this->motorManager = motorManager;
    this->radarManager = radarManager;
    this->timeManager = timeManager;

    maxSpeed = MOTOR_MAX_SPEED;
    safeStopDistance = SAFE_STOP_DISTANCE;
    speedThrottle = 0;
    directionX = 0;
    turnFactor = 0;
    autoSpeedFactor = 1;
    autoSpeedMode = 0;
    boost = 0;
}

uint16_t CarControlManager::getMaxSpeed()
{
    return maxSpeed;
}

void CarControlManager::setMaxSpeed(uint16_t maxSpeed)
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

boolean CarControlManager::isGoingForwardSafe()
{
    return radarManager->getDistance() > safeStopDistance;
}

void CarControlManager::setSafeStopDistance(uint16_t safeStopDistance)
{
    this->safeStopDistance = safeStopDistance;
}

uint16_t CarControlManager::getSafeStopDistance()
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

void CarControlManager::setTurnFactor(float turnFactor)
{
    this->turnFactor = turnFactor;
}

float CarControlManager::getTurnFactor()
{
    return turnFactor;
}

void CarControlManager::setAutoSpeedFactor(float autoSpeedFactor)
{
    this->autoSpeedFactor = autoSpeedFactor;
}

float CarControlManager::getAutoSpeedFactor()
{
    return autoSpeedFactor;
}

void CarControlManager::setAutoSpeedMode(uint8_t autoSpeedMode)
{
    this->autoSpeedMode = autoSpeedMode;
}

uint8_t CarControlManager::getAutoSpeedmode()
{
    return autoSpeedMode;
}

void CarControlManager::setBoost(uint8_t boost)
{
    this->boost = boost;
}

void CarControlManager::applyCarMotion()
{
    // If no input, stop the car
    if ((speedThrottle > -MOTOR_THROTTLE_DEADZONE && speedThrottle < MOTOR_THROTTLE_DEADZONE) && (directionX > -TURN_DEAD_ZONE && directionX < TURN_DEAD_ZONE))
    {
        motorManager->stop();
        return;
    }

    MotorDirection leftDirection = getDirection(MotorSide::LEFT, speedThrottle, directionX);
    MotorDirection rightDirection = getDirection(MotorSide::RIGHT, speedThrottle, directionX);

    uint16_t _maxSpeed = computeMaxSpeed(leftDirection == MotorDirection::BACKWARD || rightDirection == MotorDirection::BACKWARD);

    uint16_t leftSpeed = (uint16_t)((float)_maxSpeed * abs(speedThrottle));
    uint16_t rightSpeed = (uint16_t)((float)_maxSpeed * abs(speedThrottle));

    leftSpeed = getSpeed(MotorSide::LEFT, leftSpeed, speedThrottle, directionX);
    rightSpeed = getSpeed(MotorSide::RIGHT, rightSpeed, speedThrottle, directionX);

    if (isGoingForwardSafe() || leftDirection == MotorDirection::BACKWARD || rightDirection == MotorDirection::BACKWARD)
    {
        motorManager->applyRotation(leftDirection, leftSpeed, rightDirection, rightSpeed);
    }
    else
    {
        stop();
    }
    lastCarMotionUpdate = timeManager->getLoopTime();
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
    return (baseSpeed * abs(directionX) * (1 - abs(speedThrottle / this->turnFactor)));
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

uint16_t CarControlManager::computeMaxSpeed(boolean isGoingBackward)
{
    if (this->boost == 1)
    {
        return MOTOR_MAX_SPEED;
    }
    if (isGoingBackward)
    {
        return this->maxSpeed;
    }
    if (this->autoSpeedMode == 1)
    {
        return computeAutoSpeed();
    }
    return this->maxSpeed;
}

void CarControlManager::updateCarMotion()
{
    if (timeManager->getLoopTime() - lastCarMotionUpdate > CAR_MOTION_UPDATE_INTERVAL)
    {
        applyCarMotion();
    }
}

uint16_t CarControlManager::computeAutoSpeed()
{
    uint16_t distance = this->radarManager->getDistance();
    // Round up the distance to the upper ten and multiply with the auto speed factor
    return min(MOTOR_MAX_SPEED, max(MOTOR_MIN_SPEED, (uint16_t)((distance + (10 - (distance % 10))) * this->autoSpeedFactor)));
}