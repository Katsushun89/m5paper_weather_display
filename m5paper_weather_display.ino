#define LGFX_M5PAPER
#include <M5EPD.h>
#include <LovyanGFX.hpp>
#include <map>

LGFX gfx;
LGFX_Sprite temp_sp(&gfx);
LGFX_Sprite humi_sp(&gfx);

int w;
int h;

//weather
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

  delay(1000);

  drawThermometerIcon();
  drawHumidityIcon();
  drawTempAndHumid();
}

void updateWeatherIcon(void)
{
  gfx.startWrite();
  gfx.drawJpgFile(SD, weather_icon_file_map[now_weather].c_str(), 40, 100); 
  gfx.endWrite();
  gfx.display();
}

void drawThermometerIcon(void)
{
  gfx.startWrite();
  gfx.drawJpgFile(SD, "/thermometer.jpg", 470, 45); 
  gfx.endWrite();
  gfx.display();
}

void drawHumidityIcon(void)
{
  gfx.startWrite();
  gfx.drawJpgFile(SD, "/humidity.jpg", 710, 45); 
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

void drawTempAndHumid(void)
{
  M5.SHT30.UpdateData();
  float temp = M5.SHT30.GetTemperature();
  float humi = M5.SHT30.GetRelHumidity();
  Serial.printf("Temperatura: %d*C  Humedad: %d%%\r\n", (int)temp, (int)humi);
  temp_sp.clear(TFT_WHITE);
  temp_sp.setTextColor(TFT_BLACK);
  temp_sp.drawNumber((int)temp, 0, 0);
  temp_sp.pushSprite(570, 50);

  humi_sp.clear(TFT_WHITE);
  humi_sp.setTextColor(TFT_BLACK);
  humi_sp.drawNumber((int)humi, 0, 0);
  humi_sp.pushSprite(570+250, 50);
}

void loop(void)
{
  if(M5.BtnL.wasPressed()) decWeather();
  if(M5.BtnR.wasPressed()) incWeather();
  if(M5.BtnP.wasPressed()){
    M5.shutdown(5);
  }

  M5.update();
}
