#define LGFX_M5PAPER
#include <M5EPD.h>
#include <LovyanGFX.hpp>
#include <map>

LGFX gfx;
LGFX_Sprite sp(&gfx);

int w;
int h;

enum {
  CLOUDY = 0,
  SUNNY,
  RAINY,
  SNOW,
};

int now_weather = 0;

std::map<int, String> weather_icon_file_map;

void setupWeatherIcon(void)
{
  weather_icon_file_map[CLOUDY] = "/cloudy.jpeg";
  weather_icon_file_map[SUNNY] = "/sunny.jpeg";
  weather_icon_file_map[RAINY] = "/rainly.jpeg";
  weather_icon_file_map[SNOW] = "/snow.jpeg";
}

void printText(const char c[])
{
  gfx.startWrite();
  gfx.println(c);
  gfx.endWrite();
  gfx.display();

}
void setup(void)
{
  setupWeatherIcon();

  M5.begin();

  delay(1000);

  gfx.init();

  gfx.setRotation(1);

  w = gfx.width();
  h = gfx.height();
  int rectwidth = std::min(w, h) / 4;

  gfx.setBrightness(50);

  gfx.setEpdMode(epd_mode_t::epd_quality);  // 高品質更新、白黒反転が一瞬起きる

  gfx.setFont(&fonts::lgfxJapanGothic_40);
  gfx.setTextColor(TFT_BLACK, TFT_WHITE);
  gfx.setTextSize(2);

  printText("2020/12/01 12:34");

  delay(1000);
}

void updateWeatherIcon(void)
{
  gfx.startWrite();
  gfx.drawJpgFile(SD, weather_icon_file_map[now_weather].c_str(), 40, 100); 
  gfx.endWrite();
  gfx.display();
}

void decWeather()
{
  now_weather--;
  if(now_weather < 0) now_weather = weather_icon_file_map.size() - 1;
  Serial.printf("now_weather %d\n", now_weather);
  updateWeatherIcon();
}

void incWeather()
{
  now_weather++;
  if(now_weather >= weather_icon_file_map.size()) now_weather = 0;
  Serial.printf("now_weather %d\n", now_weather);
  updateWeatherIcon();
}

void loop(void)
{
  if( M5.BtnL.wasPressed()) decWeather();
  //if( M5.BtnP.wasPressed()) reset();
  if( M5.BtnR.wasPressed()) incWeather();
  M5.update();
}
