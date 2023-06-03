#include "SerialComManager.h"

SerialComManager::SerialComManager(
    TimeManager *timeManager,
    MotorManager *motorManager,
    ServoManager *servoManager,
    VoltageManager *voltageManager,
    RadarManager *radarManager)
{
    this->timeManager = timeManager;
    this->motorManager = motorManager;
    this->servoManager = servoManager;
    this->voltageManager = voltageManager;
    this->radarManager = radarManager;
    lastSystemDataSendTime = 0;
    lastEspDataReceiveTime = 0;
}

void SerialComManager::receiveSerialData()
{
    static String serialPortData = "";
    uint8_t c = "";

    // Si on a eu aucune donnée de l'ESP depuis un moment, on stoppe la voiture par sécurité
    if (timeManager->getLoopTime() - lastEspDataReceiveTime > ESP_DATA_MAX_RECEIVE_INTERVAL)
    {
        motorManager->stop();
        servoManager->applyRotation(90);
    }

    if (Serial.available() > 0)
    {
        while (c != '}' && Serial.available() > 0)
        {
            c = Serial.read();
            serialPortData += (char)c;
        }
    }
    if (c == '}') // Data frame tail check
    {
        StaticJsonDocument<200> json;
        deserializeJson(json, serialPortData);
        serialPortData = "";

        if (json.containsKey("commandCounter"))
        {
            inboundData.commandCounter = (unsigned long)json["commandCounter"];
        }

        if (json.containsKey("directionX"))
        {
            inboundData.turnForce = (float)json["directionX"];
        }

        if (json.containsKey("speedThrottle"))
        {
            inboundData.speedThrottle = (float)json["speedThrottle"];
        }

        if (json.containsKey("servoAngle"))
        {
            inboundData.servoAngle = (int)json["servoAngle"];
            servoManager->applyRotation(inboundData.servoAngle);
        }

        if (json.containsKey("speed"))
        {
            inboundData.maxSpeed = (int)json["speed"];
        }

        lastEspDataReceiveTime = millis();
    }
}

void SerialComManager::sendSerialData()
{
    if (timeManager->getLoopTime() - lastSystemDataSendTime > SYSTEM_DATA_SEND_INTERVAL)
    {
        StaticJsonDocument<200> json;
        json["heartbeat"] = millis();
        json["commandCounter"] = inboundData.commandCounter;
        json["speed"] = inboundData.maxSpeed;
        json["servoAngle"] = inboundData.servoAngle;
        json["distance"] = radarManager->getDistance();
        json["batteryVoltage"] = voltageManager->getVoltage();
        serializeJson(json, Serial);
        lastSystemDataSendTime = timeManager->getLoopTime();
    }
}
