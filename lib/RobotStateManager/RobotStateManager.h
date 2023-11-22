#ifndef ROBOT_STATE_MANAGER_H
#define ROBOT_STATE_MANAGER_H

#include <Arduino.h>
#include "CarControlManager.h"
#include "ServoManager.h"

class RobotStateManager
{
public:
    RobotStateManager(CarControlManager *, ServoManager *);
    boolean isIdle();

private:
    CarControlManager *carControlManager;
    ServoManager *servoManager;
};

#endif
