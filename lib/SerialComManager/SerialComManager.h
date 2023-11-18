#ifndef SERIAL_COM_MANAGER_H
#define SERIAL_COM_MANAGER_H

#include <ArduinoJson.h>
#include "TimeManager.h"
#include "CarControlManager.h"
#include "ServoManager.h"
#include "RadarManager.h"
#include "VoltageManager.h"
#include "ArduinoShieldButtonManager.h"
#include "InfraRedCaptorManager.h"

#define SYSTEM_DATA_SEND_INTERVAL 100
#define ESP_DATA_MAX_RECEIVE_INTERVAL 500

class SerialComManager
{
public:
    SerialComManager(TimeManager *, CarControlManager *, ServoManager *, VoltageManager *, RadarManager *, ArduinoShieldButtonManager *, InfraRedCaptorManager *);
    void receiveSerialData();
    void sendSerialData();

private:
    TimeManager *timeManager;
    CarControlManager *carControlManager;
    ServoManager *servoManager;
    VoltageManager *voltageManager;
    RadarManager *radarManager;
    ArduinoShieldButtonManager *arduinoShieldButtonManager;
    InfraRedCaptorManager *infraRedCaptorManager;
    boolean syncRequestReceived;
    boolean syncRequestSent;
    unsigned long lastSendTime;
    unsigned long lastReceiveTime;
    unsigned long heartbeat;
    // Motor
    uint16_t maxSpeed;
    uint16_t safeStopDistance;
    float turnFactor;
    float autoSpeedFactor;
    uint8_t autoSpeedMode;
    // Servo
    uint8_t servoAngle;
    uint8_t servoSpeed;
    // Radar
    uint16_t radarDistance;
    // IR captor
    uint8_t onGround;
    // Battery
    float batteryVoltage;
    // Wifi
    uint8_t wifiSoftApMode;
    // Debug
    float unoLoopDuration;
    void processCommands(String serialPortData);
    void handleEspSyncRequest();
};

#endif