# m5paper_weather_display
Show the weather & condition(temperature, humidity)

## Requirement

- Arduino IDE (Version: 1.8.15)
- Arduino Json (Version: 6.18.2)
- LovyanGFX (Version: 0.4.1)
- M5EPD (Version: 0.1.1)

## Usage 

1. Write the image files to the SD card & insert the SD card

1. Modify the Wi-Fi configuration in `config.h`

1. Modify the area configuration in `weather_forecast.cpp` :
```c
WeatherForecast::WeatherForecast(void)
{
    this->endpoint = "https://www.drk7.jp/weather/json/14.js";
    this->region = "東部";
}
```
4. Build the project & upload the application to your board
