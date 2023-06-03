#include <Arduino.h>
#include "TimeManager.h"
#include "LEDManager.h"
#include "VoltageManager.h"
#include "MotorManager.h"
#include "ServoManager.h"
#include "RadarManager.h"
#include "InfraRedCaptorManager.h"
#include "SerialComManager.h"

TimeManager *timeManager;
VoltageManager *voltageManager;
LEDManager *ledManager;
MotorManager *motorManager;
ServoManager *servoManager;
RadarManager *radarManager;
InfraRedCaptorManager *infraRedCaptorManager;
SerialComManager *serialComManager;

void setup()
{

  Serial.begin(9600);
  timeManager = new TimeManager();
  voltageManager = new VoltageManager(timeManager);
  ledManager = new LEDManager(timeManager, voltageManager);
  motorManager = new MotorManager();
  servoManager = new ServoManager(timeManager);
  radarManager = new RadarManager();
  infraRedCaptorManager = new InfraRedCaptorManager();
  serialComManager = new SerialComManager(timeManager, motorManager, servoManager, voltageManager, radarManager);
}

void loop()
{
  timeManager->updateLoopTime();
  voltageManager->updateVoltage();
  ledManager->updateLED();
  servoManager->updateServo();
  serialComManager->receiveSerialData();
  serialComManager->sendSerialData();

  // timeManager->displayLoopPerformanceStats();
}
