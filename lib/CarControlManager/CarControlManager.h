#ifndef CAR_CONTROL_MANAGER_H
#define CAR_CONTROL_MANAGER_H

#include "MotorManager.h"
#include "RadarManager.h"
#include <Arduino.h>

#define MOTOR_MAX_SPEED 250
#define TURN_DEAD_ZONE 0.05
#define MOTOR_THROTTLE_DEADZONE 0.01

class CarControlManager
{
public:
    CarControlManager(MotorManager *, RadarManager *);
    int getMaxSpeed();
    void setMaxSpeed(int);
    void stop();
    bool isGoingForwardSafe();
    void applyMotorTurnAndThrottle(float turnForce, float motorThrottle);

private:
    MotorManager *motorManager;
    RadarManager *radarManager;
    float turnForce;
    float motorThrottle;
    int maxSpeed;
    uint8_t getSpeed(MotorSide motorDirection, uint8_t baseSpeed, float motorThrottle, float turnForce);
    MotorDirection getDirection(MotorSide motorDirection, float motorThrottle, float turnForce);
};

#endif