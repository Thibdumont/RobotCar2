#include "SerialComManager.h"

SerialComManager::SerialComManager(
    TimeManager *timeManager,
    CarControlManager *carControlManager,
    ServoManager *servoManager,
    VoltageManager *voltageManager,
    RadarManager *radarManager,
    ArduinoShieldButtonManager *arduinoShieldButtonManager,
    InfraRedCaptorManager *infraRedCaptorManager)
{
    this->timeManager = timeManager;
    this->carControlManager = carControlManager;
    this->servoManager = servoManager;
    this->voltageManager = voltageManager;
    this->radarManager = radarManager;
    this->arduinoShieldButtonManager = arduinoShieldButtonManager;
    this->infraRedCaptorManager = infraRedCaptorManager;

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

    // Motor
    if (json.containsKey("maxSpeed"))
    {
        carControlManager->setMaxSpeed((uint16_t)json["maxSpeed"]);
    }

    if (json.containsKey("directionX"))
    {
        carControlManager->setDirectionX((float)json["directionX"]);
        carControlManager->applyCarMotion();
    }

    if (json.containsKey("speedThrottle"))
    {
        carControlManager->setSpeedThrottle((float)json["speedThrottle"]);
        carControlManager->applyCarMotion();
    }

    if (json.containsKey("safeStopDistance"))
    {
        carControlManager->setSafeStopDistance((uint16_t)json["safeStopDistance"]);
    }

    if (json.containsKey("boost"))
    {
        carControlManager->setBoost((uint8_t)json["boost"]);
        carControlManager->applyCarMotion();
    }

    if (json.containsKey("turnFactor"))
    {
        carControlManager->setTurnFactor((float)json["turnFactor"]);
    }

    if (json.containsKey("autoSpeedFactor"))
    {
        carControlManager->setAutoSpeedFactor((float)json["autoSpeedFactor"]);
    }

    if (json.containsKey("autoSpeedMode"))
    {
        carControlManager->setAutoSpeedMode((uint8_t)json["autoSpeedMode"]);
    }

    // Servo
    if (json.containsKey("headPosition"))
    {
        servoManager->applyRotation((uint8_t)json["headPosition"]);
    }

    if (json.containsKey("servoSpeed"))
    {
        servoManager->setServoSpeed((uint8_t)json["servoSpeed"]);
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
        // Motor
        if (carControlManager->getMaxSpeed() != maxSpeed || syncRequestReceived)
        {
            json["maxSpeed"] = maxSpeed = carControlManager->getMaxSpeed();
        }
        if (carControlManager->getSafeStopDistance() != safeStopDistance || syncRequestReceived)
        {
            json["safeStopDistance"] = safeStopDistance = carControlManager->getSafeStopDistance();
        }
        if (carControlManager->getTurnFactor() != turnFactor || syncRequestReceived)
        {
            json["turnFactor"] = turnFactor = carControlManager->getTurnFactor();
        }
        if (carControlManager->getAutoSpeedFactor() != autoSpeedFactor || syncRequestReceived)
        {
            json["autoSpeedFactor"] = autoSpeedFactor = carControlManager->getAutoSpeedFactor();
        }
        if (carControlManager->getAutoSpeedmode() != autoSpeedMode || syncRequestReceived)
        {
            json["autoSpeedMode"] = autoSpeedMode = carControlManager->getAutoSpeedmode();
        }
        // Servo
        if (servoManager->getAngle() != servoAngle || syncRequestReceived)
        {
            json["servoAngle"] = servoAngle = servoManager->getAngle();
        }
        if (servoManager->getServoSpeed() != servoSpeed || syncRequestReceived)
        {
            json["servoSpeed"] = servoSpeed = servoManager->getServoSpeed();
        }
        // Radar
        if (radarManager->getDistance() != radarDistance || syncRequestReceived)
        {
            json["radarDistance"] = radarDistance = radarManager->getDistance();
        }
        // IR captor
        if (infraRedCaptorManager->isOnGround() != onGround || syncRequestReceived)
        {
            json["onGround"] = onGround = infraRedCaptorManager->isOnGround();
        }
        // Battery
        if (voltageManager->getVoltage() != batteryVoltage || syncRequestReceived)
        {
            json["batteryVoltage"] = batteryVoltage = voltageManager->getVoltage();
        }
        // Wifi
        if (arduinoShieldButtonManager->getWifiSoftApMode() != wifiSoftApMode || syncRequestReceived)
        {
            json["wifiSoftApMode"] = wifiSoftApMode = arduinoShieldButtonManager->getWifiSoftApMode();
        }
        // Debug
        if (timeManager->getLoopAverageDuration() != unoLoopDuration || syncRequestReceived)
        {
            json["unoLoopDuration"] = unoLoopDuration = timeManager->getLoopAverageDuration();
        }
        serializeJson(json, Serial);
        syncRequestReceived = false; // Sync request occurs only once
        lastSendTime = timeManager->getLoopTime();
    }
}
