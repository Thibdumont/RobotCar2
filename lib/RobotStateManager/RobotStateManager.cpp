#include "RobotStateManager.h"

RobotStateManager::RobotStateManager(CarControlManager *carControlManager, ServoManager *servoManager)
{
    this->carControlManager = carControlManager;
    this->servoManager = servoManager;
}

boolean RobotStateManager::isIdle()
{
    return this->carControlManager->isIdle() && this->servoManager->isIdle();
}
