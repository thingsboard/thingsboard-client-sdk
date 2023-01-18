#include "ThingsBoard.h"

#include <ESP8266WiFi.h>


#define WIFI_AP             "YOUR_WIFI_SSID"
#define WIFI_PASSWORD       "YOUR_WIFI_PASSWORD"

#define THINGSBOARD_SERVER  "thingsboard.cloud"
#define THINGSBOARD_PORT    1883

const char* provisionDeviceKey = "YOUR_PROVISION_DEVICE_KEY";
const char* provisionDeviceSecret = "YOUR_PROVISION_DEVICE_SECRET";
const char* deviceName = "YOUR_DEVICE_NAME";

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard client provision instance
ThingsBoard tb_provision(espClient);

// Initialize Thingsboard instance
ThingsBoard tb(espClient);

// the Wifi radio's status
int status = WL_IDLE_STATUS;

unsigned long previous_processing_time;

// Statuses for provisioning
bool provisionRequestSent = false;
volatile bool provisionResponseProcessed = false;

// Struct for client connecting after provisioning
struct Credentials {
  String client_id;
  String username;
  String password;
};

Credentials credentials;

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
  previous_processing_time = millis();
}

void processProvisionResponse(const Provision_Data &data) {
  Serial.println("Received device provision response");
  int jsonSize = measureJson(data) + 1;
  char buffer[jsonSize];
  serializeJson(data, buffer, jsonSize);
  Serial.println(buffer);
  if (strncmp(data["status"], "SUCCESS", strlen("SUCCESS")) != 0) {
    Serial.print("Provision response contains the error: ");
    Serial.println(data["errorMsg"].as<String>());
    provisionResponseProcessed = true;
    return;
  }
  if (strncmp(data["credentialsType"], "ACCESS_TOKEN", strlen("ACCESS_TOKEN")) == 0) {
    credentials.client_id = "";
    credentials.username = data["credentialsValue"].as<String>();
    credentials.password = "";
  }
  if (strncmp(data["credentialsType"], "MQTT_BASIC", strlen("MQTT_BASIC")) == 0) {
    JsonObject credentials_value = data["credentialsValue"].as<JsonObject>();
    credentials.client_id = credentials_value["clientId"].as<String>();
    credentials.username = credentials_value["userName"].as<String>();
    credentials.password = credentials_value["password"].as<String>();
  }
  if (tb_provision.connected()) {
    tb_provision.disconnect();
  }
  provisionResponseProcessed = true;
}

const Provision_Callback provisionCallback = processProvisionResponse;

void loop() {
  if (millis() - previous_processing_time >= 1000) {
    previous_processing_time = millis();
    if (WiFi.status() != WL_CONNECTED) {
      reconnect();
    }

    if (!provisionRequestSent) {
      if (!tb_provision.connected()) {
        // Connect to the ThingsBoard
        Serial.print("Connecting to: ");
        Serial.print(THINGSBOARD_SERVER);
        if (!tb_provision.connect(THINGSBOARD_SERVER, "provision", THINGSBOARD_PORT)) {
          Serial.println("Failed to connect");
          return;
        }
        if (tb_provision.Provision_Subscribe(provisionCallback)) {
          if (tb_provision.sendProvisionRequest(deviceName, provisionDeviceKey, provisionDeviceSecret)) {
            provisionRequestSent = true;
            Serial.println("Provision request was sent!");
          }
        }
      }

    } else if (provisionResponseProcessed) {
      if (!tb.connected()) {
        // Connect to the ThingsBoard
        Serial.print("Connecting to: ");
        Serial.println(THINGSBOARD_SERVER);
        if (!tb.connect(THINGSBOARD_SERVER, credentials.username.c_str(), THINGSBOARD_PORT, credentials.client_id.c_str(), credentials.password.c_str())) {
          Serial.println("Failed to connect");
          Serial.println(credentials.client_id.c_str());
          Serial.println(credentials.username.c_str());
          Serial.println(credentials.password.c_str());
          return;
        } else {
          Serial.println("Connected!");
        }
      } else {
        Serial.println("Sending telemetry...");
        tb.sendTelemetryInt("temperature", 22);
        tb.sendTelemetryFloat("humidity", 42.5);
      }
    }

    if (!provisionResponseProcessed) {
      tb_provision.loop();
    } else {
      tb.loop();
    }
  }
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
