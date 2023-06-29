#include "VoltageManager.h"

VoltageManager::VoltageManager(TimeManager *timeManager)
{
    this->timeManager = timeManager;
    voltage = 0;
    lastVoltageMeasure = 0;
    pinMode(VOL_MEASURE_PIN, INPUT);
}

void VoltageManager::updateVoltage()
{
    if (timeManager->getLoopTime() - lastVoltageMeasure > VOLTAGE_MEASURE_INTERVAL)
    {
        lastVoltageMeasure = timeManager->getLoopTime();
        voltage = (analogRead(VOL_MEASURE_PIN) * 5) * ((10 + 1.5) / 1.5) / 1024; // Read voltage value
        voltage = voltage + (voltage * 0.08);
    }
}

float VoltageManager::getVoltage()
{
    return voltage;
}
