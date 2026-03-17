#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <WiFi.h>
#include "time.h"

const char* ssid       = "YOUR_WIFI_SSID";
const char* password   = "YOUR_WIFI_PASS";

//const char* ntpServer = "pool.ntp.org";
const char* ntpServer = "time3.google.com";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;



TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

#define TFT_BLACK 0x0000 // black

void setup(void) {
  tft.init();
  tft.setRotation(1);  // 0–3; try different values if display is upside down
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(20, 0);
  tft.println("# Clock #");
  
  Serial.begin(115200);
  tft.setCursor(20, 40);

  tft.setTextSize(1);

  Serial.printf("Connecting to %s ", ssid);
  tft.println("Connecting to wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      tft.println(".");
  }
  Serial.println(" CONNECTED");
  tft.println(" CONNECTED ");

  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //printLocalTime();

  sleep(5);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
 
}

void loop() {

delay(1000);
printLocalTime();
//printTime();
}


void printTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  tft.fillScreen(TFT_BLACK);

  tft.setTextDatum(MC_DATUM);
  
  tft.drawString(
    String(timeinfo.tm_hour) + ":" +
    String(timeinfo.tm_min) + ":" +
    String(timeinfo.tm_sec),
    60,//tft.width() / 2,
    60,//tft.height() / 2,
    16
  );
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){

    tft.setCursor(0, 0, 2);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(1);
    tft.println("Failed to obtain time");

    setup();

    return;
  }
  
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 2);
  //tft.setTextColor(TFT_WHITE,TFT_BLACK);  
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(1);
  tft.println(&timeinfo, "%A");

  tft.setCursor(0, 20, 2);
  //tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);  
  tft.setTextSize(2);
  tft.println(&timeinfo, "%h %d %Y");

  tft.setCursor(0, 50, 4);
  //tft.setTextColor(TFT_WHITE,TFT_BLACK); 
  tft.setTextColor(TFT_CYAN, TFT_BLACK); 
  tft.setTextSize(4);
  tft.println(&timeinfo, "%I : %M");

  tft.fillRect(170, 8, 120, 40, TFT_PINK);

  tft.setCursor(180, 10, 2);
  //tft.setTextColor(TFT_WHITE,TFT_BLACK); 
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); 
  tft.setTextSize(2);
  tft.println(&timeinfo, " %S %p ");

  
}

  