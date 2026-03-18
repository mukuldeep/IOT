#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SD_MMC.h"

// Replace with your network credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
String folder = "/public_html";

AsyncWebServer server(80);

// Helper to map file extensions to Browser Content-Types
String getMimeType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js"))  return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

void setup() {
  Serial.begin(115200);

  // 1. Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 2. Initialize SD Card (1-bit mode is more stable for ESP32-CAM)
  if(!SD_MMC.begin("/sdcard", true)){ 
    Serial.println("SD Card Mount Failed!");
    return;
  }
  Serial.println("SD Card Initialized.");

  // 3. Define Route: Serve Static Files from SD
  server.onNotFound([](AsyncWebServerRequest *request) {
    String path = request->url();
    
    // Default to index.html if root is requested
    if (path == "/") path = "/index.html";

    path = folder+path;
    Serial.printf("Requesting: %s\n", path.c_str());

    if (SD_MMC.exists(path)) {
      // The library handles streaming the file asynchronously
      request->send(SD_MMC, path, getMimeType(path));
    } else {
      request->send(404, "text/plain", "404: File Not Found on SD Card");
    }
  });

  // 4. Start Server
  server.begin();
}

void loop() {
  // Empty! AsyncWebServer runs in the background.
  // You can put your Camera capture logic here without lag.
}