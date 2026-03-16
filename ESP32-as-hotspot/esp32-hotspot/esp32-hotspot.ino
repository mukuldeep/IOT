#include <WiFi.h>

const char* ssid = "ESP32_Hotspot";
const char* password = "12345678";

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {

  switch(event) {

    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
      Serial.print("Device Connected: ");
      Serial.print(info.wifi_ap_staconnected.mac[0], HEX); Serial.print(":");
      Serial.print(info.wifi_ap_staconnected.mac[1], HEX); Serial.print(":");
      Serial.print(info.wifi_ap_staconnected.mac[2], HEX); Serial.print(":");
      Serial.print(info.wifi_ap_staconnected.mac[3], HEX); Serial.print(":");
      Serial.print(info.wifi_ap_staconnected.mac[4], HEX); Serial.print(":");
      Serial.println(info.wifi_ap_staconnected.mac[5], HEX);

      Serial.print("AID: ");
      Serial.println(info.wifi_ap_staconnected.aid);
      break;

    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
      Serial.print("Device Disconnected: ");
      Serial.print(info.wifi_ap_stadisconnected.mac[0], HEX); Serial.print(":");
      Serial.print(info.wifi_ap_stadisconnected.mac[1], HEX); Serial.print(":");
      Serial.print(info.wifi_ap_stadisconnected.mac[2], HEX); Serial.print(":");
      Serial.print(info.wifi_ap_stadisconnected.mac[3], HEX); Serial.print(":");
      Serial.print(info.wifi_ap_stadisconnected.mac[4], HEX); Serial.print(":");
      Serial.println(info.wifi_ap_stadisconnected.mac[5], HEX);

      break;
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.onEvent(WiFiEvent);

  WiFi.softAP(ssid, password);

  Serial.println("Hotspot started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
}