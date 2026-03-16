#include <WiFi.h>
#include "esp_wifi.h"
#include "lwip/lwip_napt.h"

#define NAPT 1
#define NAPT_PORT 10

const char* sta_ssid = "YOUR_ROUTER_WIFI";
const char* sta_pass = "YOUR_ROUTER_PASSWORD";

const char* ap_ssid = "ESP32_Hotspot";
const char* ap_pass = "12345678";

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
    switch(event)
    {
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.print("Device Connected: ");
            for(int i=0;i<6;i++){
                Serial.print(info.wifi_ap_staconnected.mac[i], HEX);
                if(i<5) Serial.print(":");
            }
            Serial.println();
            break;

        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.print("Device Disconnected: ");
            for(int i=0;i<6;i++){
                Serial.print(info.wifi_ap_stadisconnected.mac[i], HEX);
                if(i<5) Serial.print(":");
            }
            Serial.println();
            break;
    }
}

void setup()
{
    Serial.begin(115200);

    WiFi.onEvent(WiFiEvent);

    // STA + AP mode
    WiFi.mode(WIFI_AP_STA);

    // connect to router
    WiFi.begin(sta_ssid, sta_pass);

    Serial.print("Connecting to router");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected to router");
    Serial.print("Router IP: ");
    Serial.println(WiFi.localIP());

    // start hotspot
    WiFi.softAP(ap_ssid, ap_pass);

    Serial.println("Hotspot started");
    Serial.print("Hotspot IP: ");
    Serial.println(WiFi.softAPIP());


#if NAPT
    ip_napt_init(NAPT, NAPT_PORT);
    ip_napt_enable_no(SOFTAP_IF, 1);
#endif
}

void loop()
{
}