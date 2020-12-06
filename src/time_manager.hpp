#pragma once

#include <M5EPD.h>
class TimeManager
{
public:
    TimeManager(){};
    ~TimeManager() = default;
    void syncTime();
    String getDate(void);
    int8_t getHour(void);
    int8_t getMin(void);
    void setWakeupTime(int8_t hour, int8_t min);
    rtc_time_t getWakeupTime(void);
private:
    int8_t wakeup_hour;
    int8_t wakeup_min;
};