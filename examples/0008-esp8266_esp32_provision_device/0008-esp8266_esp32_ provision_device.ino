#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <ThingsBoard.h>


constexpr char WIFI_SSID[] PROGMEM = "YOUR_WIFI_SSID";
constexpr char WIFI_PASSWORD[] PROGMEM = "YOUR_WIFI_PASSWORD";

// Thingsboard we want to establish a connection too
constexpr char THINGSBOARD_SERVER[] PROGMEM = "demo.thingsboard.io";
// MQTT port used to communicate with the server, 1883 is the default unencrypted MQTT port,
// whereas 8883 would be the default encrypted SSL MQTT port
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883U;

// Maximum size packets will ever be sent or received by the underlying MQTT client,
// if the size is to small messages might not be sent or received messages will be discarded
constexpr uint32_t MQTT_MAX_MESSAGE_SIZE PROGMEM = 256U;

// Baud rate for the debugging serial connection
constexpr uint32_t SERIAL_DEBUG_BAUD PROGMEM = 115200U;

// See https://thingsboard.io/docs/user-guide/device-provisioning/
// to understand how to create a device profile to be able to provision a device
constexpr char PROVISION_DEVICE_KEY[] PROGMEM = "YOUR_PROVISION_DEVICE_KEY";
constexpr char PROVISION_DEVICE_SECRET[] PROGMEM = "YOUR_PROVISION_DEVICE_SECRET";
// Optionally keep the device name empty and the WiFi mac address of the integrated
// wifi chip on ESP32 or ESP8266 will be used as the name instead
// Ensuring your device name is unique, even when reusing this code for multiple devices
constexpr char DEVICE_NAME[] PROGMEM = "";


// Initialize underlying client, used to establish a connection
WiFiClient espClient;
// Initialize ThingsBoard instance with the maximum needed buffer size
ThingsBoardSized<MQTT_MAX_MESSAGE_SIZE> tb(espClient);

uint32_t previous_processing_time = 0U;

// Statuses for provisioning
bool provisionRequestSent = false;
bool provisionResponseProcessed = false;

// Struct for client connecting after provisioning
struct Credentials {
  std::string client_id;
  std::string username;
  std::string password;
};
Credentials credentials;


/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {
  Serial.println("Connecting to AP ...");
  // Attempting to establish a connection to the given WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    // Delay 500ms until a connection has been succesfully established
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

/// @brief Reconnects the WiFi uses InitWiFi if the connection has been removed
/// @return Returns true as soon as a connection has been established again
const bool reconnect() {
  // Check to ensure we aren't connected yet
  const wl_status_t status = WiFi.status();
  if (status == WL_CONNECTED) {
    return true;
  }

  // If we aren't establish a new connection to the given WiFi network
  InitWiFi();
  return true;
}

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  InitWiFi();
  previous_processing_time = millis();
}

void processProvisionResponse(const Provision_Data &data) {
  int jsonSize = JSON_STRING_SIZE(measureJson(data));
  char buffer[jsonSize];
  serializeJson(data, buffer, jsonSize);
  Serial.printf("Received device provision response (%s)\n", buffer);

  if (strncmp(data["status"], "SUCCESS", strlen("SUCCESS")) != 0) {
    Serial.printf("Provision response contains the error: (%s)\n", data["errorMsg"].as<const char*>());
    return;
  }

  if (strncmp(data["credentialsType"], "ACCESS_TOKEN", strlen("ACCESS_TOKEN")) == 0) {
    credentials.client_id = "";
    credentials.username = data["credentialsValue"].as<std::string>();
    credentials.password = "";
  }
  else if (strncmp(data["credentialsType"], "MQTT_BASIC", strlen("MQTT_BASIC")) == 0) {
    auto credentials_value = data["credentialsValue"].as<JsonObjectConst>();
    credentials.client_id = credentials_value["clientId"].as<std::string>();
    credentials.username = credentials_value["userName"].as<std::string>();
    credentials.password = credentials_value["password"].as<std::string>();
  }
  else {
    Serial.printf("Unexpected provision credentialsType: (%s)\n", data["credentialsType"].as<const char*>());
    return;
  }

  // Unsubscribe from the callback, has to be done after reading because it seems to
  // churn up the letters of the first key making it not readable anymore.
  tb.Provision_Unsubscribe();

  // Disconnect from the cloud client connected to the provision account, because it is no longer needed the device has been provisioned
  // and we can reconnect to the cloud with the newly generated credentials.
  if (tb.connected()) {
    tb.disconnect();
  }
  provisionResponseProcessed = true;
}

const Provision_Callback provisionCallback(&processProvisionResponse);

void loop() {
  if (!reconnect()) {
    return;
  }
  else if (millis() - previous_processing_time < 1000) {
    return;
  }

  previous_processing_time = millis();

  if (!provisionRequestSent) {
    if (!tb.connected()) {
      // Connect to the ThingsBoard server as a client wanting to provision a new device
      Serial.printf("Connecting to: (%s)\n", THINGSBOARD_SERVER);
      if (!tb.connect(THINGSBOARD_SERVER, "provision", THINGSBOARD_PORT)) {
        Serial.println("Failed to connect");
        return;
      }
      Serial.println("Sending provisioning request");
      provisionRequestSent = tb.Provision_Subscribe(provisionCallback) &&
                             tb.sendProvisionRequest((DEVICE_NAME != NULL) && (DEVICE_NAME[0] == '\0') ? WiFi.macAddress().c_str() : DEVICE_NAME, PROVISION_DEVICE_KEY, PROVISION_DEVICE_SECRET);
      }
    }
  } else if (provisionResponseProcessed) {
    if (!tb.connected()) {
      // Connect to the ThingsBoard server, as the provisioned client
      Serial.printf("Connecting to: (%s)\n", THINGSBOARD_SERVER);
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

  tb.loop();
}
