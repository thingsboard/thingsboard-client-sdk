// This sketch demonstrates connecting and sending telemetry 
// using ThingsBoard SDK
//
// Hardware:
//  - Arduino Uno
//  - ESP8266 connected to Arduino Uno

#include "ThingsBoard.h"

#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include "SoftwareSerial.h"

#define WIFI_AP             "YOUR_WIFI_AP"
#define WIFI_PASSWORD       "YOUR_WIFI_PASSWORD"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/ 
// to understand how to obtain an access token
#define TOKEN               "YOUR_ACCESS_TOKEN"
#define THINGSBOARD_SERVER  "thingsboard.cloud"

// Baud rate for serial debug
#define SERIAL_DEBUG_BAUD   9600
// Baud rate for communicating with ESP chip
#define SERIAL_ESP8266_BAUD 9600

// Serial driver for ESP
SoftwareSerial soft(2, 3); // RX, TX
// Initialize the Ethernet client object
WiFiEspClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;                    

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  // initialize serial for ESP module
  soft.begin(SERIAL_ESP8266_BAUD);

  // initialize ESP module
  WiFi.init(&soft);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  Serial.println("Connected to AP");
}

void loop() {
  delay(1000);

  if (status != WL_CONNECTED) {
    Serial.println("Connecting to AP ...");
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    return;
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
  }

  Serial.println("Sending data...");

  // Uploads new telemetry to ThingsBoard using MQTT. 
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api 
  // for more details

  tb.sendTelemetryInt("temperature", 22);
  tb.sendTelemetryFloat("humidity", 42.5);

  tb.loop();
}
