#ifndef SERIAL_COM_MANAGER_H
#define SERIAL_COM_MANAGER_H

#include <ArduinoJson.h>
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
    SerialComManager(MotorManager *, ServoManager *, VoltageManager *, RadarManager *);
    void receiveSerialData(unsigned long);
    void sendSerialData(unsigned long);

private:
    unsigned long lastSystemDataSendTime;
    unsigned long lastEspDataReceiveTime;
    InboundData inboundData;
    MotorManager *motorManager;
    ServoManager *servoManager;
    VoltageManager *voltageManager;
    RadarManager *radarManager;
};

#endif