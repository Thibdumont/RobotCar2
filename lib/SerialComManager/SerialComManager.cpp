#include "SerialComManager.h"

SerialComManager::SerialComManager(
    MotorManager *motorManager,
    ServoManager *servoManager,
    VoltageManager *voltageManager,
    RadarManager *radarManager)
{
    this->motorManager = motorManager;
    this->servoManager = servoManager;
    this->voltageManager = voltageManager;
    this->radarManager = radarManager;
    lastSystemDataSendTime = 0;
    lastEspDataReceiveTime = 0;
}

void SerialComManager::receiveSerialData(unsigned long currentTime)
{
    static String serialPortData = "";
    uint8_t c = "";

    // Si on a eu aucune donnée de l'ESP depuis un moment, on stoppe la voiture par sécurité
    if (currentTime - lastEspDataReceiveTime > ESP_DATA_MAX_RECEIVE_INTERVAL)
    {
        motorManager->stop();
        servoManager->applyRotation(90, currentTime);
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
            servoManager->applyRotation(inboundData.servoAngle, currentTime);
        }

        if (json.containsKey("speed"))
        {
            inboundData.maxSpeed = (int)json["speed"];
        }

        lastEspDataReceiveTime = millis();
        Serial.print("turnForce : ");
        Serial.println((float)json["directionX"]);
    }
}

void SerialComManager::sendSerialData(unsigned long currentTime)
{
    if (currentTime - lastSystemDataSendTime > SYSTEM_DATA_SEND_INTERVAL)
    {
        StaticJsonDocument<200> json;
        json["heartbeat"] = millis();
        json["commandCounter"] = inboundData.commandCounter;
        json["speed"] = inboundData.maxSpeed;
        json["servoAngle"] = inboundData.servoAngle;
        json["distance"] = radarManager->getDistance();
        json["batteryVoltage"] = voltageManager->getVoltage();
        // serializeJson(json, Serial);
        lastSystemDataSendTime = currentTime;
    }
}
