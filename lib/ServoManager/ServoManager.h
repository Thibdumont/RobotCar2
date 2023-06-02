
#include <Servo.h>
#include <Arduino.h>

#define PIN_Servo_z 10

class ServoManager
{
public:
    ServoManager();
    void applyRotation(uint8_t, unsigned long);
    bool isServoTurnOver(unsigned long);
    void updateServo(unsigned long);
    void testServo(unsigned long);

private:
    Servo servo;
    uint8_t angle;
    unsigned long servoTurnStartTime;
    unsigned long turnDuration;
};
