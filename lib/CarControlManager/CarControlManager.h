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
    int getMaxSpeed();
    void setMaxSpeed(int);
    void setSpeedThrottle(float);
    void setBoost(bool);
    void setDirectionX(float);
    void stop();
    bool isGoingForwardSafe();
    void setSafeStopDistance(int);
    int getSafeStopDistance();
    void applyMotorDirectionXAndThrottle();

private:
    MotorManager *motorManager;
    RadarManager *radarManager;
    float directionX;
    float speedThrottle;
    bool boost;
    int maxSpeed;
    int safeStopDistance;
    uint16_t getSpeed(MotorSide motorDirection, uint16_t baseSpeed, float speedThrottle, float directionX);
    uint16_t getDirectionSpeedModifier(uint16_t baseSpeed, float speedThrottle, float directionX);
    MotorDirection getDirection(MotorSide motorDirection, float speedThrottle, float directionX);
};

#endif