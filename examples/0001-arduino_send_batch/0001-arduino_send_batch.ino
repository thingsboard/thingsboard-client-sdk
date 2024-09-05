// This sketch demonstrates connecting and sending telemetry and attributes
// it batch using ThingsBoard SDK
//
// Hardware:
//  - Arduino Uno
//  - ESP8266 connected to Arduino Uno

#include <ThingsBoard.h>
#include <Arduino_MQTT_Client.h>
#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <SoftwareSerial.h>


constexpr char WIFI_SSID[] = "YOUR_WIFI_SSID";
constexpr char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
constexpr char TOKEN[] = "YOUR_DEVICE_ACCESS_TOKEN";

// Thingsboard we want to establish a connection too
constexpr char THINGSBOARD_SERVER[] = "demo.thingsboard.io";

// MQTT port used to communicate with the server, 1883 is the default unencrypted MQTT port,
// whereas 8883 would be the default encrypted SSL MQTT port
constexpr uint16_t THINGSBOARD_PORT = 1883U;

// Maximum size packets will ever be sent or received by the underlying MQTT client,
// if the size is to small messages might not be sent or received messages will be discarded
constexpr uint16_t MAX_MESSAGE_SIZE = 128U;

// Baud rate for the debugging serial connection
// If the Serial output is mangled, ensure to change the monitor speed accordingly to this variable
constexpr uint32_t SERIAL_DEBUG_BAUD = 9600U;
constexpr uint32_t SERIAL_ESP8266_DEBUG_BAUD = 9600U;

constexpr char CONNECTING_MSG[] = "Connecting to: (%s) with token (%s)";
constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char HUMIDITY_KEY[] = "humidity";
constexpr char DEVICE_TYPE_KEY[] = "device_type";
constexpr char ACTIVE_KEY[] = "active";
constexpr char SENSOR_VALUE[] = "sensor";


// Serial driver for ESP
SoftwareSerial soft(2U, 3U); // RX, TX
// Initialize the Ethernet client object
WiFiEspClient espClient;
// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);
// Initialize ThingsBoard instance with the maximum needed buffer size
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);


/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {
  Serial.println("Connecting to AP ...");
  // Attempting to establish a connection to the given WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    // Delay 500ms until a connection has been successfully established
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
#if ENCRYPTED
  espClient.setCACert(ROOT_CERT);
#endif
}

/// @brief Reconnects the WiFi uses InitWiFi if the connection has been removed
/// @return Returns true as soon as a connection has been established again
bool reconnect() {
  // Check to ensure we aren't connected yet
  const uint8_t status = WiFi.status();
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
  // initialize serial for ESP module
  soft.begin(SERIAL_ESP8266_DEBUG_BAUD);
  delay(1000);

  // initialize ESP module
  WiFi.init(&soft);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  InitWiFi();
}

void loop() {
  delay(1000);

  if (!reconnect()) {
    return;
  }

  if (!tb.connected()) {
    // Reconnect to the ThingsBoard server,
    // if a connection was disrupted or has not yet been established
    char message[Helper::detectSize(CONNECTING_MSG, THINGSBOARD_SERVER, TOKEN)];
    snprintf(message, sizeof(message), CONNECTING_MSG, THINGSBOARD_SERVER, TOKEN);
    Serial.println(message);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  Serial.println("Sending telemetry data...");

  constexpr size_t TELEMETRY_SIZE = 2U;
  Telemetry data[TELEMETRY_SIZE] = {
    { TEMPERATURE_KEY, 42.2 },
    { HUMIDITY_KEY,    80 },
  };

  /* For C++98 compiler, shipped with Arduino IDE version 1.6.6 or less:

  Telemetry data[TELEMETRY_SIZE] = {
    Telemetry( TEMPERATURE_KEY, 42.2 ),
    Telemetry( HUMIDITY_KEY,    80 ),
  };

  */

  // Uploads new telemetry to ThingsBoard using MQTT. 
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api 
  // for more details
  Telemetry* begin = data;
  Telemetry* end = data + TELEMETRY_SIZE;
  tb.sendTelemetry<TELEMETRY_SIZE>(begin, end);

  Serial.println("Sending attributes data...");

  constexpr size_t ATTRIBUTES_SIZE = 2U;
  Attribute attributes[ATTRIBUTES_SIZE] = {
    { DEVICE_TYPE_KEY,  SENSOR_VALUE },
    { ACTIVE_KEY,       true     },
  };

  /* For C++98 compiler, shipped with Arduino IDE version 1.6.6 or less:

  Attribute attributes[ATTRIBUTES_SIZE] = {
    Attribute( DEVICE_TYPE_KEY,  SENSOR_VALUE ),
    Attribute( ACTIVE_KEY,       true     ),
  };

  */

  // Publish attribute update to ThingsBoard using MQTT. 
  // See https://thingsboard.io/docs/reference/mqtt-api/#publish-attribute-update-to-the-server 
  // for more details
  begin = attributes;
  end = attributes + ATTRIBUTES_SIZE;
  tb.sendAttributes<ATTRIBUTES_SIZE>(begin, end);

  tb.loop();
}
