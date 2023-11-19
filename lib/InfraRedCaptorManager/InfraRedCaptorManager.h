#ifndef INFRA_RED_CAPTOR_MANAGER_H
#define INFRA_RED_CAPTOR_MANAGER_H

#include <Arduino.h>

#define PIN_ITR20001xxxL A2
#define PIN_ITR20001xxxM A1
#define PIN_ITR20001xxxR A0

#define IR_GROUNDED_THRESHOLD 600 // Max value to consider the captor on ground

class InfraRedCaptorManager
{
public:
    InfraRedCaptorManager();
    float getLeftCaptorValue();
    float getMiddleCaptorValue();
    float getRightCaptorValue();
    uint8_t isOnGround();
    void testInfraRedCaptor();

private:
};

#endif
