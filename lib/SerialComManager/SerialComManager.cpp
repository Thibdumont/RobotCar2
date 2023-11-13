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

    syncRequestReceived = false;
    syncRequestSent = false;
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
        processCommands(serialPortData);

        // After receiving data from Esp for first time, we ask for a sync request
        handleEspSyncRequest();

        lastReceiveTime = millis();
        serialPortData = "";
    }
}

void SerialComManager::processCommands(String serialPortData)
{
    StaticJsonDocument<400> json;
    deserializeJson(json, serialPortData);
    if (json.containsKey("syncRequest"))
    {
        syncRequestReceived = true;
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
        (uint8_t) json["boost"] == 1 ? ledManager->changeColor(255, 0, 0) : ledManager->changeColor(255, 0, 255);
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
}

void SerialComManager::handleEspSyncRequest()
{
    if (!syncRequestSent)
    {
        StaticJsonDocument<20> json;
        json["syncRequest"] = true;
        serializeJson(json, Serial);
        syncRequestSent = true;
    }
}

/**
 * Only send data when it has changed or if sync request received
 */
void SerialComManager::sendSerialData()
{
    if (timeManager->getLoopTime() - lastSendTime > SYSTEM_DATA_SEND_INTERVAL)
    {
        StaticJsonDocument<400> json;
        json["heartbeat"] = heartbeat++;
        if (syncRequestReceived)
        {
            json["syncReply"] = 1;
        }
        if (carControlManager->getMaxSpeed() != maxSpeed || syncRequestReceived)
        {
            maxSpeed = carControlManager->getMaxSpeed();
            json["maxSpeed"] = maxSpeed;
        }
        if (servoManager->getAngle() != servoAngle || syncRequestReceived)
        {
            servoAngle = servoManager->getAngle();
            json["servoAngle"] = servoAngle;
        }
        if (servoManager->getServoSpeed() != servoSpeed || syncRequestReceived)
        {
            servoSpeed = servoManager->getServoSpeed();
            json["servoSpeed"] = servoSpeed;
        }
        if (radarManager->getDistance() != radarDistance || syncRequestReceived)
        {
            radarDistance = radarManager->getDistance();
            json["radarDistance"] = radarDistance;
        }
        if (timeManager->getLoopAverageDuration() != unoLoopDuration || syncRequestReceived)
        {
            unoLoopDuration = timeManager->getLoopAverageDuration();
            json["unoLoopDuration"] = unoLoopDuration;
        }
        if (voltageManager->getVoltage() != batteryVoltage || syncRequestReceived)
        {
            batteryVoltage = voltageManager->getVoltage();
            json["batteryVoltage"] = batteryVoltage;
        }
        if (arduinoShieldButtonManager->getWifiSoftApMode() != wifiSoftApMode || syncRequestReceived)
        {
            wifiSoftApMode = arduinoShieldButtonManager->getWifiSoftApMode();
            json["wifiSoftApMode"] = wifiSoftApMode;
        }
        if (carControlManager->getSafeStopDistance() != safeStopDistance || syncRequestReceived)
        {
            safeStopDistance = carControlManager->getSafeStopDistance();
            json["safeStopDistance"] = safeStopDistance;
        }
        serializeJson(json, Serial);
        syncRequestReceived = false; // Sync request occurs only once
        lastSendTime = timeManager->getLoopTime();
    }
}
