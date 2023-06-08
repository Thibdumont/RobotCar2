#ifndef SERIAL_COM_MANAGER_H
#define SERIAL_COM_MANAGER_H

#include <ArduinoJson.h>
#include "TimeManager.h"
#include "CarControlManager.h"
#include "ServoManager.h"
#include "RadarManager.h"
#include "VoltageManager.h"

#define SYSTEM_DATA_SEND_INTERVAL 100
#define ESP_DATA_MAX_RECEIVE_INTERVAL 500

class SerialComManager
{
public:
    SerialComManager(TimeManager *, CarControlManager *, ServoManager *, VoltageManager *, RadarManager *);
    void receiveSerialData();
    void sendSerialData();

private:
    TimeManager *timeManager;
    CarControlManager *carControlManager;
    ServoManager *servoManager;
    VoltageManager *voltageManager;
    RadarManager *radarManager;
    unsigned long lastSendTime;
    unsigned long lastReceiveTime;
    unsigned long commandCounter;
};

#endif