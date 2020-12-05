#define LGFX_M5PAPER
#include <M5EPD.h>
#include <LovyanGFX.hpp>
#include <map>
#include "src/weather_forecast.hpp"

LGFX gfx;
LGFX_Sprite temp_sp(&gfx);
LGFX_Sprite humi_sp(&gfx);
LGFX_Sprite rain_sp(&gfx);

WeatherForecast weather_forecast;

int w;
int h;

//weather
int now_weather = 0;
std::map<int, String> weather_icon_file_map;

void setupWeatherIcon(void)
{
  weather_icon_file_map[CLOUDY] = "/cloudy.jpeg";
  weather_icon_file_map[SUNNY] = "/sunny.jpeg";
  weather_icon_file_map[RAINY] = "/rainly.jpeg";
  weather_icon_file_map[SUNNY_AND_CLOUDY] = "/sunnyandcloudy.jpeg";
  weather_icon_file_map[RAINY_AND_CLOUDY] = "/rainyandcloudy.jpeg";
}

void drawDate(const char c[])
{
  gfx.startWrite();
  gfx.setCursor(10, 10);
  gfx.println(c);
  gfx.endWrite();
  gfx.display();
}

void setup(void)
{
  setupWeatherIcon();

  //M5.begin();
  M5.begin(true, true, true, true, false, false);//custmized

  M5.SHT30.Begin();
  M5.RTC.begin();

  gfx.init();
  gfx.setRotation(1);

  w = gfx.width();
  h = gfx.height();
  int rectwidth = std::min(w, h) / 4;

  gfx.setBrightness(50);
  gfx.setEpdMode(epd_mode_t::epd_quality);  // 高品質更新、白黒反転が一瞬起きる

  //gfx.setFont(&fonts::lgfxJapanGothic_40);
  gfx.setFont(&fonts::Font8);
  gfx.setTextColor(TFT_BLACK, TFT_WHITE);
  gfx.setTextSize(0.85);

  drawDate("12.01 12:34");

  temp_sp.setColorDepth(4);
  temp_sp.createSprite(120, 80);
  temp_sp.setFont(&fonts::Font8);

  humi_sp.setColorDepth(4);
  humi_sp.createSprite(120, 80);
  humi_sp.setFont(&fonts::Font8);

  rain_sp.setColorDepth(4);
  rain_sp.createSprite(530, 180);
  rain_sp.setFont(&fonts::lgfxJapanGothic_40);

  delay(1000);

  drawThermometerIcon();
  drawHumidityIcon();
  drawTempAndHumid();

  if(weather_forecast.downloadWeatherForecast()){
    drawWeather();
    drawRainFallChance();
  }
}

void drawWeather(void)
{
  int weather_enum = weather_forecast.getWeatherEnum();
  gfx.startWrite();
  gfx.drawJpgFile(SD, weather_icon_file_map[weather_enum].c_str(), 40, 100); 
  gfx.endWrite();
  gfx.display();
}

void drawThermometerIcon(void)
{
  gfx.startWrite();
  gfx.drawJpgFile(SD, "/thermometer.jpg", 470, 35); 
  gfx.endWrite();
  gfx.display();
}

void drawHumidityIcon(void)
{
  gfx.startWrite();
  gfx.drawJpgFile(SD, "/humidity.jpg", 710, 35); 
  gfx.endWrite();
  gfx.display();
}

void drawTempAndHumid(void)
{
  M5.SHT30.UpdateData();
  float temp = M5.SHT30.GetTemperature();
  float humi = M5.SHT30.GetRelHumidity();
  Serial.printf("Temperatura: %d*C  Humedad: %d%%\r\n", (int)temp, (int)humi);
  temp_sp.clear(TFT_WHITE);
  temp_sp.setTextColor(TFT_BLACK);
  temp_sp.drawNumber((int)temp, 0, 0);
  temp_sp.pushSprite(570, 40);

  humi_sp.clear(TFT_WHITE);
  humi_sp.setTextColor(TFT_BLACK);
  humi_sp.drawNumber((int)humi, 0, 0);
  humi_sp.pushSprite(570+250, 40);
}

void drawRainFallChance(void)
{
  rain_sp.clear(TFT_WHITE);
  rain_sp.setTextColor(TFT_BLACK);
  
  String rfc00_06 = weather_forecast.getRainFallChance00_06() + "%";
  String rfc06_12 = weather_forecast.getRainFallChance06_12() + "%";
  String rfc12_18 = weather_forecast.getRainFallChance12_18() + "%";
  String rfc18_24 = weather_forecast.getRainFallChance18_24() + "%";

  rain_sp.setTextSize(0.8);
  rain_sp.drawString("00-06", 120*0, 0);
  rain_sp.drawString("06-12", 120*1, 0);
  rain_sp.drawString("12-18", 120*2, 0);
  rain_sp.drawString("18-24", 120*3, 0);
  rain_sp.setTextSize(1.3);
  rain_sp.drawString(rfc00_06.c_str(), 120*0, 50);
  rain_sp.drawString(rfc06_12.c_str(), 120*1, 50);
  rain_sp.drawString(rfc12_18.c_str(), 120*2, 50);
  rain_sp.drawString(rfc18_24.c_str(), 120*3, 50);
  rain_sp.pushSprite(470, 160);
}

void loop(void)
{
  if(M5.BtnP.wasPressed()){
    M5.shutdown(5);
  }

  M5.update();
}
