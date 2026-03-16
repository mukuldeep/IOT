#include <WiFi.h>

const char* ssid = "ESP32_Hotspot";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println("Hotspot started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
}