// This sketch demonstrates connecting and receiving RPC calls from 
// ThingsBoard using ThingsBoard SDK.
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

// Serial driver for ESP
SoftwareSerial soft(2, 3); // RX, TX
// Initialize the Ethernet client object
WiFiEspClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);

void setup() {
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
}

// Processes function for RPC call "example_set_temperature"
// RPC_Data is a JSON variant, that can be queried using operator[]
// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
RPC_Response processTemperatureChange(const RPC_Data &data)
{
  Serial.println("Received the set temperature RPC method");

  // Process data

  float example_temperature = data["temp"];

  Serial.print("Example temperature: ");
  Serial.println(example_temperature);

  // Just an response example
  return RPC_Response("example_response", 42);
}

// Processes function for RPC call "example_set_switch"
// RPC_Data is a JSON variant, that can be queried using operator[]
// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
RPC_Response processSwitchChange(const RPC_Data &data)
{
  Serial.println("Received the set switch method");

  // Process data

  bool switch_state = data["switch"];

  Serial.print("Example switch state: ");
  Serial.println(switch_state);

  // Just an response example
  return RPC_Response("example_response", 22.02);
}

const size_t callbacks_size = 2;
RPC_Callback callbacks[callbacks_size] = {
  { "example_set_temperature",    processTemperatureChange },
  { "example_set_switch",         processSwitchChange }
};

bool subscribed = false;

void loop() {
  delay(100);

  if (WiFi.status() == WL_IDLE_STATUS) {
    // Still connecting
    return;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to AP ...");
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    // Connect to WPA/WPA2 network
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    return;
  }

  if (!tb.connected()) {
    subscribed = false;

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

  if (!subscribed) {
    Serial.println("Subscribing for RPC...");

    // Perform a subscription. All consequent data processing will happen in
    // processTemperatureChange() and processSwitchChange() functions,
    // as denoted by callbacks[] array.
    if (!tb.RPC_Subscribe(callbacks, callbacks_size)) {
      Serial.println("Failed to subscribe for RPC");
      return;
    }

    Serial.println("Subscribe done");
    subscribed = true;
  }

  Serial.println("Waiting for data...");
  tb.loop();
}
