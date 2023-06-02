#include <Arduino.h>
#include "LEDManager.h"
#include "VoltageManager.h"
#include "MotorManager.h"
#include "ServoManager.h"

unsigned long currentTime = 0;
VoltageManager *voltageManager;
LEDManager *ledManager;
MotorManager *motorManager;
ServoManager *servoManager;

void setup()
{

  Serial.begin(9600);
  voltageManager = new VoltageManager();
  ledManager = new LEDManager();
  motorManager = new MotorManager();
  servoManager = new ServoManager();
  // motorManager->testMotor();
}

void loop()
{
  currentTime = millis();

  voltageManager->updateVoltage(currentTime);
  ledManager->updateLED(currentTime, voltageManager->getVoltage());

  // servoManager->testServo(currentTime);

  servoManager->updateServo(currentTime);
}
