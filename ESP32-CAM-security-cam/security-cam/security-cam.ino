#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems

#define FLASH_GPIO_NUM 4

// Pin definition for AI-THINKER ESP32-CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

int pictureCount = 0;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector
  Serial.begin(115200);

  //LED PIN
  pinMode(FLASH_GPIO_NUM, OUTPUT);
  digitalWrite(FLASH_GPIO_NUM, LOW);

  // Camera Configuration
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Image Quality Settings
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA; // 640x480 (Better for speed/storage)
    //config.frame_size = FRAMESIZE_HD; // 1280x720 wide
    //config.frame_size = FRAMESIZE_UXGA; // 1600x1200 (Best Quality)
    config.jpeg_quality = 3;          // 0-63, lower is higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Init SD Card
  if(!SD_MMC.begin("/sdcard", true)){
    Serial.println("SD Card Mount Failed");
    return;
  }
}

void loop() {

  //Turn Flash ON
  digitalWrite(FLASH_GPIO_NUM, HIGH);
  delay(150); // Give the sensor a moment to adjust to the light

  for(int i = 0; i < 2; i++) {
    camera_fb_t * dummy_fb = esp_camera_fb_get();
    esp_camera_fb_return(dummy_fb); 
    delay(200); // Give the hardware time to react
  }

  // 1. Take Picture
  camera_fb_t * fb = esp_camera_fb_get();

  // Turn Flash OFF immediately to save power/heat
  delay(100);
  digitalWrite(FLASH_GPIO_NUM, LOW);
  delay(10);

  if(!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // 2. Define File Path
  String path = "/pic_" + String(pictureCount) + ".jpg";
  fs::FS &fs = SD_MMC;
  
  // 3. Save to SD
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file in writing mode");
  } else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved: %s\n", path.c_str());
    pictureCount++;
  }
  file.close();

  // 4. Return frame buffer to be reused
  esp_camera_fb_return(fb);

  // 5. Wait 2 second
  delay(2000);
}