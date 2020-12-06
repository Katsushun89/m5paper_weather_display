#pragma once

class TimeSync
{
public:
    TimeSync(){};
    ~TimeSync() = default;
    void syncTime();
    String getDate(void);
};