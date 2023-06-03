#ifndef VOLTAGE_MANAGER_H
#define VOLTAGE_MANAGER_H

#include <Arduino.h>
#include "TimeManager.h"
#define VOL_MEASURE_PIN A3

class VoltageManager
{
public:
    VoltageManager(TimeManager *);
    void updateVoltage();
    float getVoltage();

private:
    TimeManager *timeManager;
    float voltage;
    unsigned long lastVoltageMeasure;
};

#endif