#include <WiFi.h>
#include "time_manager.hpp"

void TimeManager::syncTime()
{
    if(!WiFi.isConnected()){
        Serial.println("Unable to NTP because the WiFi is not connected");
        return;
    }
    const uint32_t JST = 3600 * 9;
    configTime(JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");

    struct tm tm;
    getLocalTime(&tm);
    Serial.printf("getLocalTime %04d.%02d.%02d\n", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday);

    rtc_date_t rtc_date;
    rtc_time_t rtc_time;

    rtc_date.year = tm.tm_year + 1900;
    rtc_date.mon = tm.tm_mon + 1;
    rtc_date.day = tm.tm_mday;
    Serial.printf("setDate %04d.%02d.%02d\n", rtc_date.year, rtc_date.mon, rtc_date.day);
    M5.RTC.setDate(&rtc_date);

    rtc_time.hour = tm.tm_hour;
    rtc_time.min = tm.tm_min;
    rtc_time.sec = tm.tm_sec;
    Serial.printf("setTime %02d.%02d.%02d\n", rtc_time.hour, rtc_time.min, rtc_time.sec);
    M5.RTC.setTime(&rtc_time);
    delay(1000);
}

String TimeManager::getDate(void)
{
    rtc_date_t rtc_date;
    M5.RTC.getDate(&rtc_date);

#if 0
    char c_year[4];
    snprintf(c_year, sizeof(c_year), "%04d", rtc_date.year);

    char c_month[2];
    snprintf(c_month, sizeof(c_month), "%02d", rtc_date.mon);

    char c_day[2];
    snprintf(c_day, sizeof(c_day), "%02d", rtc_date.day);
#endif

    char c_date[100];
    snprintf(c_date, sizeof(c_date), "%04d.%02d.%02d", rtc_date.year,
                    rtc_date.mon, rtc_date.day);
    String date(c_date);
    Serial.printf("%s\n", date.c_str());
    return date;
}

void TimeManager::setWakeupTime(int8_t hour, int8_t min)
{
    if(hour >= 0 && hour < 24){
        wakeup_hour = hour;
    }else{
        Serial.printf("wakeup set hour out of range %d\n", hour);
    }

    if(min >= 0 && min < 60){
        wakeup_min = min;
    }else{
        Serial.printf("wakeup set min out of range %d\n", min);
    }

    if(min >= 0 && min < 60){
        wakeup_min = min;
    }
}

int8_t TimeManager::getHour(void)
{
    rtc_time_t rtc_time;
    M5.RTC.getTime(&rtc_time);

    return rtc_time.hour;
}

int8_t TimeManager::getMin(void)
{
    rtc_time_t rtc_time;
    M5.RTC.getTime(&rtc_time);

    return rtc_time.min;
}

rtc_time_t TimeManager::getWakeupTime(void)
{
    rtc_time_t rtc_time;
    rtc_time.hour = wakeup_hour;
    rtc_time.min = wakeup_min;
    Serial.printf("getWakeupTime %02d:%02d\n", rtc_time.hour, rtc_time.min);
    return rtc_time;
}