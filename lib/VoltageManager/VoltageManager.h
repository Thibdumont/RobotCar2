#ifndef VOLTAGE_MANAGER_H
#define VOLTAGE_MANAGER_H

#include <Arduino.h>
#define VOL_MEASURE_PIN A3

class VoltageManager
{
public:
    VoltageManager();
    float getVoltage();

private:
};

#endif