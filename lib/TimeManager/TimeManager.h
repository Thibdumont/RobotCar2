#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <ArduinoJson.h>

class TimeManager
{
public:
    TimeManager();
    unsigned long getLoopTime();
    void updateLoopTime();
    float getLoopAverageDuration();
    void displayLoopPerformanceStats();

private:
    unsigned long startingTime;
    unsigned long previousLoopTime;
    unsigned long loopTime;
    unsigned long loopMaxDuration;
    unsigned long loopMinDuration;
    unsigned long loopCount;
};

#endif
