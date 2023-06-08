#ifndef GYRO_MANAGER_H
#define GYRO_MANAGER_H

#include <Arduino.h>
#include <Wire.h>

// Manage MPU chip readings : gyro and accelerometer
class MpuManager
{
public:
    MpuManager();
    void readMpuValues();
    void calculate_IMU_error();

private:
    int MPU = 0x68;
    float AccX, AccY, AccZ;
    float GyroX, GyroY, GyroZ;
    float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
    float roll, pitch, yaw;
    float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
    float elapsedTime, currentTime, previousTime;
    int c = 0;
};

#endif