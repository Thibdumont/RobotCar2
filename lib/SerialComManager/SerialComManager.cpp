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
    lastSendTime = 0;
    lastReceiveTime = 0;
}

void SerialComManager::receiveSerialData()
{
    static String serialPortData = "";
    uint8_t c = "";

    // Si on a eu aucune donnée de l'ESP depuis un moment, on stoppe la voiture par sécurité
    if (timeManager->getLoopTime() - lastReceiveTime > ESP_DATA_MAX_RECEIVE_INTERVAL)
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
            commandCounter = (unsigned long)json["commandCounter"];
        }

        if (json.containsKey("directionX") && json.containsKey("speedThrottle"))
        {
            motorManager->applyMotorTurnAndThrottle((float)json["directionX"], (float)json["speedThrottle"]);
        }

        if (json.containsKey("servoAngle"))
        {
            servoManager->applyRotation((int)json["servoAngle"]);
        }

        if (json.containsKey("speed"))
        {
            motorManager->setMaxSpeed((int)json["speed"]);
        }

        lastReceiveTime = millis();
    }
}

void SerialComManager::sendSerialData()
{
    if (timeManager->getLoopTime() - lastSendTime > SYSTEM_DATA_SEND_INTERVAL)
    {
        StaticJsonDocument<200> json;
        json["heartbeat"] = millis();
        json["commandCounter"] = commandCounter;
        json["speed"] = motorManager->getMaxSpeed();
        json["servoAngle"] = servoManager->getAngle();
        json["distance"] = radarManager->getDistance();
        json["batteryVoltage"] = voltageManager->getVoltage();
        serializeJson(json, Serial);
        lastSendTime = timeManager->getLoopTime();
    }
}
