#include "SerialComManager.h"

SerialComManager::SerialComManager(
    TimeManager *timeManager,
    CarControlManager *carControlManager,
    ServoManager *servoManager,
    VoltageManager *voltageManager,
    RadarManager *radarManager,
    ArduinoShieldButtonManager *arduinoShieldButtonManager)
{
    this->timeManager = timeManager;
    this->carControlManager = carControlManager;
    this->servoManager = servoManager;
    this->voltageManager = voltageManager;
    this->radarManager = radarManager;
    this->arduinoShieldButtonManager = arduinoShieldButtonManager;
    handshakeRequest = false;
    heartbeat = 0;
    lastSendTime = 0;
    lastReceiveTime = 0;
}

void SerialComManager::receiveSerialData()
{
    static String serialPortData = "";
    uint8_t c = "";

    // If we didn't receive some data from ESP for a certain amount of time, we stop the car to be safe
    if (timeManager->getLoopTime() - lastReceiveTime > ESP_DATA_MAX_RECEIVE_INTERVAL)
    {
        carControlManager->stop();
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
        StaticJsonDocument<300> json;
        deserializeJson(json, serialPortData);
        serialPortData = "";

        if (json.containsKey("handshake"))
        {
            handshakeRequest = true;
        }

        if (json.containsKey("directionX"))
        {
            carControlManager->setDirectionX((float)json["directionX"]);
            carControlManager->applyMotorDirectionXAndThrottle();
        }

        if (json.containsKey("speedThrottle"))
        {
            carControlManager->setSpeedThrottle((float)json["speedThrottle"]);
            carControlManager->applyMotorDirectionXAndThrottle();
        }

        if (json.containsKey("boost"))
        {
            carControlManager->setBoost(json["boost"].as<String>().equals("true"));
            carControlManager->applyMotorDirectionXAndThrottle();
        }

        if (json.containsKey("headPosition"))
        {
            servoManager->applyRotation((int)json["headPosition"]);
        }

        if (json.containsKey("maxSpeed"))
        {
            carControlManager->setMaxSpeed((int)json["maxSpeed"]);
        }

        lastReceiveTime = millis();
    }
}

void SerialComManager::sendSerialData()
{
    if (timeManager->getLoopTime() - lastSendTime > SYSTEM_DATA_SEND_INTERVAL)
    {
        StaticJsonDocument<300> json;
        json["heartbeat"] = heartbeat++;
        if (carControlManager->getMaxSpeed() != maxSpeed || handshakeRequest)
        {
            maxSpeed = carControlManager->getMaxSpeed();
            json["maxSpeed"] = maxSpeed;
        }
        if (servoManager->getAngle() != servoAngle || handshakeRequest)
        {
            servoAngle = servoManager->getAngle();
            json["servoAngle"] = servoAngle;
        }
        if (radarManager->getDistance() != radarDistance || handshakeRequest)
        {
            radarDistance = radarManager->getDistance();
            json["radarDistance"] = radarDistance;
        }
        if (timeManager->getLoopAverageDuration() != unoLoopDuration || handshakeRequest)
        {
            unoLoopDuration = timeManager->getLoopAverageDuration();
            json["unoLoopDuration"] = unoLoopDuration;
        }
        if (voltageManager->getVoltage() != batteryVoltage || handshakeRequest)
        {
            batteryVoltage = voltageManager->getVoltage();
            json["batteryVoltage"] = batteryVoltage;
        }
        if (arduinoShieldButtonManager->getWifiSoftApMode() != wifiSoftApMode || handshakeRequest)
        {
            wifiSoftApMode = arduinoShieldButtonManager->getWifiSoftApMode();
            json["wifiSoftApMode"] = wifiSoftApMode;
        }
        serializeJson(json, Serial);
        handshakeRequest = false; // Handshake occurs only once
        lastSendTime = timeManager->getLoopTime();
    }
}
