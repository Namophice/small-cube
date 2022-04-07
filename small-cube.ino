#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "./pic/pic.h"
#include "./char/char.h"

TFT_eSPI tft = TFT_eSPI();

const char *ssid = "Tenda_AE4BB0";
const char *password = "0123456789qwerdf";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.aliyun.com");

void setup() {
  // 初始化
  tft.init();
  tft.writecommand(0x36);
  tft.writedata(0x20);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10, 1);
  tft.print("Loading...");
  
  // 休眠2s
  delay(2000);

  // 连接Wifi
  wifiConnect();

  // 同步时间
  timeClient.begin();
  // 设置时区
  timeClient.setTimeOffset(28800);
}

int requestIntervalCache = 6000;
int picIndex = 0;
void loop() {
  requestIntervalCache = requestIntervalCache + 1;
  if (requestIntervalCache > 6000) {
    // 时间更新
    timeClient.update();
    requestIntervalCache = 0;
  }

  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int year = ptm -> tm_year + 1900;
  int month = ptm -> tm_mon + 1;
  int monthDay = ptm -> tm_mday;
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  tft.setCursor(15, 20, 1);
  tft.setTextSize(7);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  if (hours < 10) {
    tft.print("0");
  }
  tft.print(hours);

  if (picIndex < 5) {
    tft.print(":");
  } else {
    tft.print(" ");
  }
  if (minutes < 10) {
    tft.print("0");
  }
  tft.print(minutes);

  tft.setTextSize(2);
  tft.setCursor(15, 100, 1);
  tft.print(year);
  tft.print("/");
  if (month < 10) {
    tft.print("0");
  }
  tft.print(month);
  tft.print("/");
  if (monthDay < 10) {
    tft.print("0");
  }
  tft.print(monthDay);
  
  tft.setTextSize(3);
  tft.setCursor(185, 95, 1);
  if (seconds < 10) {
    tft.print("0");
  }
  tft.print(seconds);

  tft.drawLine(0, 135, 240, 135, TFT_WHITE);

  tft.setSwapBytes(true);
  tft.pushImage(0, 135, 100, 100, pics[picIndex]);
  picIndex = picIndex + 1;
  if (picIndex >= 10) {
    picIndex = 0;
  }

  showCostomChar(130, 140, "猪", TFT_WHITE);
  showCostomChar(175, 140, "猪", TFT_WHITE);
  showCostomChar(130, 190, "专", TFT_WHITE);
  showCostomChar(175, 190, "属", TFT_WHITE);

  // 休眠1s
  delay(100);
}

// 连接wifi
void wifiConnect() {
  WiFi.begin(ssid, password);
  int wifi_wait = 0;
  int temp = 0;
  tft.fillScreen(TFT_WHITE);
  while (temp < 7 | WiFi.status() != WL_CONNECTED ) {
    delay(700);
    
    wifi_wait = wifi_wait + 1;
    temp = temp + 1;

    tft.setCursor(10, 10, 1);
    tft.print("Wifi connecting");

    if (wifi_wait == 1) {
      tft.print(".  ");
    }
    if (wifi_wait == 2) {
      tft.print(".. ");
    }
    if (wifi_wait == 3) {
      tft.print("...");
      wifi_wait = 0;
    }
  }

  tft.setCursor(10, 10, 1);
  tft.println("Wifi connected     ");
  
  // 休眠2s
  delay(2000);
  
  tft.fillScreen(TFT_BLACK);
}

void showCostomChar(int32_t x, int32_t y, const char c[3], uint32_t color) { 
  for (int k = 0; k < 3; k++)
    if (hanzi[k].Index[0] == c[0] && hanzi[k].Index[1] == c[1] && hanzi[k].Index[2] == c[2]) { tft.drawBitmap(x, y, hanzi[k].hz_Id, hanzi[k].hz_width, 40, color);
    }
}
