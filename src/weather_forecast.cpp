#include <WiFi.h>
#include <HTTPClient.h>
#include "weather_forecast.hpp"

WeatherForecast::WeatherForecast(void)
{
    this->endpoint = "https://www.drk7.jp/weather/json/14.js";
    this->region = "東部";
}

String WeatherForecast::createJson(String json_string)
{
    json_string.replace("drk7jpweather.callback(","");
    return json_string.substring(0, json_string.length() - 2);
}

bool WeatherForecast::getWeatherForecast(DynamicJsonDocument &doc)
{
    Serial.println("getWeatherForecast");//debug
    bool ret = true;
    if((WiFi.status() == WL_CONNECTED)){
        HTTPClient http;
        http.begin(this->endpoint.c_str());
        int http_code = http.GET();
        if (http_code > 0) {
            String json_string = createJson(http.getString());
            deserializeJson(doc, json_string);
        } else {
            Serial.println("Error on HTTP request");
            ret = false;
        }
        http.end();
    }
    return ret;
}

bool WeatherForecast::downloadWeatherForecast(void)
{
    Serial.println("downloadWeatherForecast");//debug
    if(!WiFi.isConnected()){
        this->is_downloaded_weather = false;
        return false;
    }

    DynamicJsonDocument weather_info(20000);

    if(!getWeatherForecast(weather_info)){
        this->is_downloaded_weather = false;
        return false;
    }

    String today_weather = weather_info["pref"]["area"][this->region.c_str()]["info"][0];

    DynamicJsonDocument today_weather_info(20000);
    deserializeJson(today_weather_info, today_weather);    

    String w = today_weather_info["weather"];
    this->weather = w;

    String max_temp = today_weather_info["temperature"]["range"][0]["content"];
    this->max_temperature = max_temp;

    String min_temp = today_weather_info["temperature"]["range"][1]["content"];
    this->min_temperature = min_temp;

    String rain_0 = today_weather_info["rainfallchance"]["period"][0]["content"];
    this->rain_fall_chance_00_06 = rain_0;

    String rain_1 = today_weather_info["rainfallchance"]["period"][1]["content"];
    this->rain_fall_chance_06_12 = rain_1;

    String rain_2 = today_weather_info["rainfallchance"]["period"][2]["content"];
    this->rain_fall_chance_12_18 = rain_2;

    String rain_3 = today_weather_info["rainfallchance"]["period"][3]["content"];
    this->rain_fall_chance_18_24 = rain_3;

    this->is_downloaded_weather = true;

    return true;
}

int WeatherForecast::getWeatherEnum(void)
{
    String w = getWeather();
    if(weather.indexOf("雨") != -1){
        if(weather.indexOf("くもり") != -1){
            return RAINY_AND_CLOUDY;
        }else{
            return RAINY;
        }
    }else if(weather.indexOf("晴") != -1){
        if(weather.indexOf("くもり") != -1){
            return SUNNY_AND_CLOUDY;
        }else{
            return SUNNY;
        }
    }else if(weather.indexOf("雪") != -1){
        return SNOW;
    }else if(weather.indexOf("くもり") != -1){
        return CLOUDY;
    }
    return WEATHER_NOT_SET;
}
