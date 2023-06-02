#include <Arduino.h>
#include "LEDManager.h"
#include "VoltageManager.h"
#include "MotorManager.h"
#include "ServoManager.h"
#include "RadarManager.h"

unsigned long currentTime = 0;
VoltageManager *voltageManager;
LEDManager *ledManager;
MotorManager *motorManager;
ServoManager *servoManager;
RadarManager *radarManager;

void setup()
{

  Serial.begin(9600);
  voltageManager = new VoltageManager();
  ledManager = new LEDManager();
  motorManager = new MotorManager();
  servoManager = new ServoManager();
  radarManager = new RadarManager();
}

void loop()
{
  currentTime = millis();

  voltageManager->updateVoltage(currentTime);
  ledManager->updateLED(currentTime, voltageManager->getVoltage());
  servoManager->updateServo(currentTime);
}
