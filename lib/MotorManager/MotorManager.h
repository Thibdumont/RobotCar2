#ifndef MOTOR_MANAGER_H
#define MOTOR_MANAGER_H

#include <Servo.h>
#include <Arduino.h>

#define MOTOR_MAX_SPEED 250
#define MOTOR_MIN_SPEED 50
#define TURN_DEAD_ZONE 0.1
#define MOTOR_THROTTLE_DEADZONE 0.01

#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8
#define PIN_Motor_AIN_1 7
#define PIN_Motor_STBY 3

enum MotorDirection
{
    FORWARD,
    BACKWARD
};

enum MotorSide
{
    LEFT,
    RIGHT
};

class MotorManager
{
public:
    MotorManager();
    void applyRotation(MotorDirection, uint8_t, MotorDirection, uint8_t);
    void stop();
    void testMotor();
    int getMaxSpeed();
    void setMaxSpeed(int);
    void applyMotorTurnAndThrottle(float turnForce, float motorThrottle);

private:
    float turnForce;
    float motorThrottle;
    int maxSpeed;
    uint8_t getSpeed(MotorSide motorDirection, uint8_t baseSpeed, float motorThrottle, float turnForce);
    MotorDirection getDirection(MotorSide motorDirection, float motorThrottle, float turnForce);
};

#endif