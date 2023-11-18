#include "VoltageManager.h"

VoltageManager::VoltageManager()
{
    pinMode(VOL_MEASURE_PIN, INPUT);
}

float VoltageManager::getVoltage()
{
    float voltage = (analogRead(VOL_MEASURE_PIN) * 5) * ((10 + 1.5) / 1.5) / 1024; // Read voltage value
    return voltage + (voltage * 0.08);
}
