// This sketch demonstrates connecting and sending telemetry and attributes
// it batch using ThingsBoard SDK
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

void setup() {
  uint8_t wifiStatus;

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
  delay(1000);

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect, retrying ...");
      return;
    }
  }

  Serial.println("Sending telemetry data...");

  const int data_items = 2;
  Telemetry data[data_items] = {
    { "temperature", 42.2 },
    { "humidity",    80 },
  };

  /* For C++98 compiler, shipped with Arduino IDE version 1.6.6 or less:

  Telemetry data[data_items] = {
    Telemetry( "temperature", 42.2 ),
    Telemetry( "humidity",    80 ),
  };

  */

  // Uploads new telemetry to ThingsBoard using MQTT. 
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api 
  // for more details
  tb.sendTelemetry(data, data_items);

  Serial.println("Sending attributes data...");

  const int attribute_items = 2;
  Attribute attributes[attribute_items] = {
    { "device_type",  "sensor" },
    { "active",       true     },
  };

  /* For C++98 compiler, shipped with Arduino IDE version 1.6.6 or less:

  Attribute attributes[data_items] = {
    Attribute( "device_type",  "sensor" ),
    Attribute( "active",       true     ),
  };

  */

  // Publish attribute update to ThingsBoard using MQTT. 
  // See https://thingsboard.io/docs/reference/mqtt-api/#publish-attribute-update-to-the-server 
  // for more details
  tb.sendAttributes(attributes, attribute_items);
  tb.loop();
}
