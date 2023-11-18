#ifndef CAR_CONTROL_MANAGER_H
#define CAR_CONTROL_MANAGER_H

#include "MotorManager.h"
#include "RadarManager.h"
#include <Arduino.h>

#define TURN_DEAD_ZONE 0.05
#define MOTOR_THROTTLE_DEADZONE 0.01
#define SAFE_STOP_DISTANCE 12

class CarControlManager
{
public:
    CarControlManager(MotorManager *, RadarManager *);
    uint16_t getMaxSpeed();
    void setMaxSpeed(uint16_t);
    void setSpeedThrottle(float);
    void setTurnFactor(float);
    float getTurnFactor();
    void setAutoSpeedFactor(float);
    float getAutoSpeedFactor();
    void setAutoSpeedMode(uint8_t);
    uint8_t getAutoSpeedmode();
    void setBoost(uint8_t);
    void setDirectionX(float);
    void stop();
    boolean isGoingForwardSafe();
    void setSafeStopDistance(uint16_t);
    uint16_t getSafeStopDistance();
    void applyMotorDirectionXAndThrottle();

private:
    MotorManager *motorManager;
    RadarManager *radarManager;
    float directionX;
    float speedThrottle;
    float turnFactor;
    float autoSpeedFactor;
    uint8_t autoSpeedMode;
    uint8_t boost;
    uint16_t maxSpeed;
    uint16_t safeStopDistance;
    uint16_t getSpeed(MotorSide motorDirection, uint16_t baseSpeed, float speedThrottle, float directionX);
    uint16_t getDirectionSpeedModifier(uint16_t baseSpeed, float speedThrottle, float directionX);
    MotorDirection getDirection(MotorSide motorDirection, float speedThrottle, float directionX);
    uint16_t computeMaxSpeed(boolean isGoingBackward);
};

#endif