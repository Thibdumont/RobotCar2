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

void setup()
{

  Serial.begin(115200);
  timeManager = new TimeManager();
  voltageManager = new VoltageManager(timeManager);
  ledManager = new LEDManager(timeManager, voltageManager);
  arduinoShieldButtonManager = new ArduinoShieldButtonManager(ledManager);
  radarManager = new RadarManager();
  motorManager = new MotorManager();
  carControlManager = new CarControlManager(motorManager, radarManager);
  servoManager = new ServoManager(timeManager);
  infraRedCaptorManager = new InfraRedCaptorManager();
  serialComManager = new SerialComManager(timeManager, carControlManager, servoManager, voltageManager, radarManager);
  mpuManager = new MpuManager();
}

void loop()
{
  timeManager->updateLoopTime();
  voltageManager->updateVoltage();
  // ledManager->updateLED();
  servoManager->updateServo();
  serialComManager->receiveSerialData();
  serialComManager->sendSerialData();

  // mpuManager->readMpuValues();

  // timeManager->displayLoopPerformanceStats();
}
