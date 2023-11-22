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
#include "MpuManager.h"
#include "ArduinoShieldButtonManager.h"
#include "RobotStateManager.h"

TimeManager *timeManager;
VoltageManager *voltageManager;
LEDManager *ledManager;
ArduinoShieldButtonManager *arduinoShieldButtonManager;
MotorManager *motorManager;
CarControlManager *carControlManager;
ServoManager *servoManager;
RadarManager *radarManager;
InfraRedCaptorManager *infraRedCaptorManager;
SerialComManager *serialComManager;
MpuManager *mpuManager;
RobotStateManager *robotStateManager;

void setup()
{
  Serial.begin(115200);
  timeManager = new TimeManager();
  voltageManager = new VoltageManager();
  ledManager = new LEDManager(timeManager, voltageManager);
  arduinoShieldButtonManager = new ArduinoShieldButtonManager(ledManager);
  radarManager = new RadarManager();
  motorManager = new MotorManager();
  carControlManager = new CarControlManager(motorManager, radarManager, timeManager);
  servoManager = new ServoManager(timeManager);
  infraRedCaptorManager = new InfraRedCaptorManager();
  robotStateManager = new RobotStateManager(carControlManager, servoManager);
  mpuManager = new MpuManager();
  serialComManager = new SerialComManager(timeManager, carControlManager, servoManager, voltageManager, radarManager, arduinoShieldButtonManager, infraRedCaptorManager, robotStateManager);
}

void loop()
{
  timeManager->updateLoopTime();
  serialComManager->receiveSerialData();
  serialComManager->sendSerialData();
  carControlManager->updateCarMotion();
  arduinoShieldButtonManager->detectPress();

  // mpuManager->readMpuValues();

  // timeManager->displayLoopPerformanceStats();
}
