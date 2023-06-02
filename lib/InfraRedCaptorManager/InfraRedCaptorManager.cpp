#include "InfraRedCaptorManager.h"

InfraRedCaptorManager::InfraRedCaptorManager()
{
    pinMode(PIN_ITR20001xxxL, INPUT);
    pinMode(PIN_ITR20001xxxM, INPUT);
    pinMode(PIN_ITR20001xxxR, INPUT);
}

float InfraRedCaptorManager::getLeftCaptorValue()
{
    return analogRead(PIN_ITR20001xxxL);
}

float InfraRedCaptorManager::getMiddleCaptorValue()
{
    return analogRead(PIN_ITR20001xxxL);
}

float InfraRedCaptorManager::getRightCaptorValue()
{
    return analogRead(PIN_ITR20001xxxL);
}

bool InfraRedCaptorManager::isOnGround()
{
    if (getLeftCaptorValue() > IR_GROUNDED_THRESHOLD || getMiddleCaptorValue() > IR_GROUNDED_THRESHOLD || getRightCaptorValue() > IR_GROUNDED_THRESHOLD)
    {
        return false;
    }
    return true;
}

void InfraRedCaptorManager::testInfraRedCaptor()
{
    if (!isOnGround())
    {
        Serial.println("Car is airborne !");
    }
}