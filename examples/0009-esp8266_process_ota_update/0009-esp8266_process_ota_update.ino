#include "ThingsBoard.h"

#include <ESP8266WiFi.h>


#define WIFI_AP             "ThingsBoardTN"
#define WIFI_PASSWORD       "4Friends123!"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN               "U1hUxTpuRH9YUfIDTd1u"
#define THINGSBOARD_SERVER  "192.168.0.2"
#define UPDATES_PORT 8080

#define CURRENT_FIRMWARE_TITLE "blink"
#define CURRENT_FIRMWARE_VERSION "0"


// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

volatile bool start_update = false;
String target_firmware_title;
String target_firmware_version;

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
}

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
    JsonObject fw_info = tb.Request_Firmware_Info();
    if (fw_info) {
      Serial.println("FW info received!");
      target_firmware_title = fw_info["fw_title"].as<String>();
      target_firmware_version = fw_info["fw_version"].as<String>();
      if (target_firmware_title != String(CURRENT_FIRMWARE_TITLE) || target_firmware_version != String(CURRENT_FIRMWARE_VERSION)) {
        Serial.println("Received firmware is different from the current one.");
        start_update = true;
      } else {
        Serial.println("Received firmware is the same like the current one.");
      }
    }
  }

  if (start_update) {
    Serial.println("Starting update...");
    tb.update(THINGSBOARD_SERVER, UPDATES_PORT, target_firmware_title.c_str(), target_firmware_version.c_str());
  }

  tb.loop();
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
