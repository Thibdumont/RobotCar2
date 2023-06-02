#include <Arduino.h>
#include "LEDManager.h"
#include "VoltageManager.h"

unsigned long currentTime = 0;
VoltageManager *voltageManager = new VoltageManager();
LEDManager *ledManager = new LEDManager();

// put function declarations here:

void setup()
{
  Serial.begin(9600);
  voltageManager = new VoltageManager();
  ledManager = new LEDManager();
}

void loop()
{
  currentTime = millis();

  voltageManager->updateVoltage(currentTime);
  ledManager->updateLED(currentTime, voltageManager->getVoltage());
}
