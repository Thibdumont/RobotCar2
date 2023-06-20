#ifndef CAR_CONTROL_MANAGER_H
#define CAR_CONTROL_MANAGER_H

#include "MotorManager.h"
#include "RadarManager.h"
#include <Arduino.h>

#define TURN_DEAD_ZONE 0.05
#define MOTOR_THROTTLE_DEADZONE 0.01

class CarControlManager
{
public:
    CarControlManager(MotorManager *, RadarManager *);
    int getMaxSpeed();
    void setMaxSpeed(int);
    void setSpeedThrottle(float);
    void setDirectionX(float);
    void stop();
    bool isGoingForwardSafe();
    void applyMotorDirectionXAndThrottle();

private:
    MotorManager *motorManager;
    RadarManager *radarManager;
    float directionX;
    float speedThrottle;
    int maxSpeed;
    uint8_t getSpeed(MotorSide motorDirection, uint8_t baseSpeed, float speedThrottle, float directionX);
    MotorDirection getDirection(MotorSide motorDirection, float speedThrottle, float directionX);
};

#endif