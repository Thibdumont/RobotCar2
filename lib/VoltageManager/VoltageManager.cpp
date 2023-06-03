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
    if (timeManager->getLoopTime() - lastVoltageMeasure > 1000)
    {
        lastVoltageMeasure = timeManager->getLoopTime();
        voltage = (analogRead(VOL_MEASURE_PIN) * 5) * ((10 + 1.5) / 1.5) / 1024; // Read voltage value
                                                                                 // float voltage = (analogRead(VOL_MEASURE_PIN) * 0.0375);
        voltage = voltage + (voltage * 0.08);
        // Serial.print(" Voltage : ");
        // Serial.println(voltage, 3);
    }
}

float VoltageManager::getVoltage()
{
    return voltage;
}
