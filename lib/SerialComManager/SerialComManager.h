#ifndef SERIAL_COM_MANAGER_H
#define SERIAL_COM_MANAGER_H

#include <ArduinoJson.h>
#include "TimeManager.h"
#include "MotorManager.h"
#include "ServoManager.h"
#include "RadarManager.h"
#include "VoltageManager.h"
#include "InboundData.h"

#define SYSTEM_DATA_SEND_INTERVAL 500
#define ESP_DATA_MAX_RECEIVE_INTERVAL 500

class SerialComManager
{
public:
    SerialComManager(TimeManager *, MotorManager *, ServoManager *, VoltageManager *, RadarManager *);
    void receiveSerialData();
    void sendSerialData();

private:
    TimeManager *timeManager;
    MotorManager *motorManager;
    ServoManager *servoManager;
    VoltageManager *voltageManager;
    RadarManager *radarManager;
    unsigned long lastSystemDataSendTime;
    unsigned long lastEspDataReceiveTime;
    InboundData inboundData;
};

#endif