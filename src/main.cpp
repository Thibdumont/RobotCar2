#include <Arduino.h>
#include "LED.h"
#include "VoltageManager.h"

unsigned long currentTime = 0;
VoltageManager *voltageManager = new VoltageManager();

// put function declarations here:

void setup()
{
  Serial.begin(9600);
  voltageManager = new VoltageManager();
  initLED();
}

void loop()
{
  currentTime = millis();

  handleLED(currentTime, voltageManager->getVoltage());
  voltageManager->updateVoltage(currentTime);
}
