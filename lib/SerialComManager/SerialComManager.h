#ifndef SERIAL_COM_MANAGER_H
#define SERIAL_COM_MANAGER_H

#include <ArduinoJson.h>
#include "TimeManager.h"
#include "CarControlManager.h"
#include "ServoManager.h"
#include "RadarManager.h"
#include "VoltageManager.h"
#include "ArduinoShieldButtonManager.h"
#include "LEDManager.h"

#define SYSTEM_DATA_SEND_INTERVAL 100
#define ESP_DATA_MAX_RECEIVE_INTERVAL 500

class SerialComManager
{
public:
    SerialComManager(TimeManager *, CarControlManager *, ServoManager *, VoltageManager *, RadarManager *, ArduinoShieldButtonManager *, LEDManager *);
    void receiveSerialData();
    void sendSerialData();

private:
    TimeManager *timeManager;
    CarControlManager *carControlManager;
    ServoManager *servoManager;
    VoltageManager *voltageManager;
    RadarManager *radarManager;
    ArduinoShieldButtonManager *arduinoShieldButtonManager;
    LEDManager *ledManager;
    boolean handshakeRequest;
    unsigned long lastSendTime;
    unsigned long lastReceiveTime;
    unsigned long heartbeat;
    int maxSpeed;
    int safeStopDistance;
    uint8_t servoAngle;
    uint8_t servoSpeed;
    uint16_t radarDistance;
    float unoLoopDuration;
    float batteryVoltage;
    boolean wifiSoftApMode;
};

#endif