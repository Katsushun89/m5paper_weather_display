#pragma once

class TimeManager
{
public:
    TimeManager(){};
    ~TimeManager() = default;
    void syncTime();
    String getDate(void);
};