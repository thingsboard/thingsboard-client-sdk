#include "ThingsBoard.h"

#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include "SoftwareSerial.h"

#define WIFI_AP             "YOUR_AP_NAME"
#define WIFI_PASSWORD       "YOUR_AP_PASSWORD"
#define TOKEN               "YOUR_ACCESS_TOKEN"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

// Serial driver for ESP
SoftwareSerial soft(2, 3); // RX, TX
// Initialize the Ethernet client object
WiFiEspClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);

void setup() {
  uint8_t wifiStatus;

  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  soft.begin(9600);

  // initialize ESP module
  WiFi.init(&soft);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network
  while (wifiStatus != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    // Connect to WPA/WPA2 network
    wifiStatus = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    delay(500);
  }

  Serial.println("Connected to AP");
}

void loop() {
  if (!tb.connected()) {
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
    }
  }

  Serial.println("Send data...");

  tb.sendFloat("temperature", 100.0);
  tb.sendFloat("humidity", 101.1);

  tb.loop();

  delay(1000);
}
