#include <Arduino.h>
#include "TimeManager.h"
#include "LEDManager.h"
#include "VoltageManager.h"
#include "CarControlManager.h"
#include "MotorManager.h"
#include "ServoManager.h"
#include "RadarManager.h"
#include "InfraRedCaptorManager.h"
#include "SerialComManager.h"

TimeManager *timeManager;
VoltageManager *voltageManager;
LEDManager *ledManager;
MotorManager *motorManager;
CarControlManager *carControlManager;
ServoManager *servoManager;
RadarManager *radarManager;
InfraRedCaptorManager *infraRedCaptorManager;
SerialComManager *serialComManager;

void setup()
{

  Serial.begin(115200);
  timeManager = new TimeManager();
  voltageManager = new VoltageManager(timeManager);
  ledManager = new LEDManager(timeManager, voltageManager);
  radarManager = new RadarManager();
  motorManager = new MotorManager();
  carControlManager = new CarControlManager(motorManager, radarManager);
  servoManager = new ServoManager(timeManager);
  infraRedCaptorManager = new InfraRedCaptorManager();
  serialComManager = new SerialComManager(timeManager, carControlManager, servoManager, voltageManager, radarManager);
}

void loop()
{
  timeManager->updateLoopTime();
  voltageManager->updateVoltage();
  ledManager->updateLED();
  servoManager->updateServo();
  serialComManager->receiveSerialData();
  serialComManager->sendSerialData();
  // carControlManager->preventCrash();

  // timeManager->displayLoopPerformanceStats();
}
