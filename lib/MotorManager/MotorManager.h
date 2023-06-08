#ifndef MOTOR_MANAGER_H
#define MOTOR_MANAGER_H

#include <Servo.h>
#include <Arduino.h>

#define MOTOR_MAX_SPEED 250

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

private:
};

#endif