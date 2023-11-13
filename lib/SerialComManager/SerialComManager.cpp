#include "SerialComManager.h"

SerialComManager::SerialComManager(
    TimeManager *timeManager,
    CarControlManager *carControlManager,
    ServoManager *servoManager,
    VoltageManager *voltageManager,
    RadarManager *radarManager,
    ArduinoShieldButtonManager *arduinoShieldButtonManager,
    LEDManager *ledManager)
{
    this->timeManager = timeManager;
    this->carControlManager = carControlManager;
    this->servoManager = servoManager;
    this->voltageManager = voltageManager;
    this->radarManager = radarManager;
    this->arduinoShieldButtonManager = arduinoShieldButtonManager;
    this->ledManager = ledManager;
    handshakeRequest = false;
    heartbeat = 0;
    lastSendTime = 0;
    lastReceiveTime = 0;
}

void SerialComManager::receiveSerialData()
{
    static String serialPortData = "";
    uint16_t c = "";

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
        StaticJsonDocument<400> json;
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
            bool boost = (uint8_t)json["boost"];
            boost ? ledManager->changeColor(255, 0, 0) : ledManager->changeColor(255, 255, 0);
            carControlManager->setBoost((uint8_t)json["boost"]);
            carControlManager->applyMotorDirectionXAndThrottle();
        }

        if (json.containsKey("headPosition"))
        {
            servoManager->applyRotation((int)json["headPosition"]);
        }

        if (json.containsKey("servoSpeed"))
        {
            servoManager->setServoSpeed((int)json["servoSpeed"]);
        }

        if (json.containsKey("maxSpeed"))
        {
            carControlManager->setMaxSpeed((int)json["maxSpeed"]);
        }

        if (json.containsKey("safeStopDistance"))
        {
            carControlManager->setSafeStopDistance((int)json["safeStopDistance"]);
        }

        lastReceiveTime = millis();
    }
}

void SerialComManager::sendSerialData()
{
    if (timeManager->getLoopTime() - lastSendTime > SYSTEM_DATA_SEND_INTERVAL)
    {
        StaticJsonDocument<400> json;
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
        if (servoManager->getServoSpeed() != servoSpeed || handshakeRequest)
        {
            servoSpeed = servoManager->getServoSpeed();
            json["servoSpeed"] = servoSpeed;
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
        if (carControlManager->getSafeStopDistance() != safeStopDistance || handshakeRequest)
        {
            safeStopDistance = carControlManager->getSafeStopDistance();
            json["safeStopDistance"] = safeStopDistance;
        }
        serializeJson(json, Serial);
        handshakeRequest = false; // Handshake occurs only once
        lastSendTime = timeManager->getLoopTime();
    }
}
