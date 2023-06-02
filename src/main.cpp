#include <Arduino.h>
#include "LEDManager.h"
#include "VoltageManager.h"
#include "MotorManager.h"

unsigned long currentTime = 0;
VoltageManager *voltageManager = new VoltageManager();
LEDManager *ledManager = new LEDManager();
MotorManager *motorManager = new MotorManager();

// put function declarations here:

void setup()
{
  Serial.begin(9600);
  voltageManager = new VoltageManager();
  ledManager = new LEDManager();
  motorManager = new MotorManager();
  motorManager->testMotor();
}

void loop()
{
  currentTime = millis();

  voltageManager->updateVoltage(currentTime);
  ledManager->updateLED(currentTime, voltageManager->getVoltage());
}
