// This sketch demonstrates connecting and sending telemetry and attributes
// it batch using ThingsBoard SDK
//
// Hardware:
//  - Arduino Uno
//  - ESP8266 connected to Arduino Uno

#include <ThingsBoard.h>
#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <SoftwareSerial.h>


#if THINGSBOARD_ENABLE_PROGMEM
constexpr char WIFI_SSID[] PROGMEM = "YOUR_WIFI_SSID";
constexpr char WIFI_PASSWORD[] PROGMEM = "YOUR_WIFI_PASSWORD";
#else
constexpr char WIFI_SSID[] = "YOUR_WIFI_SSID";
constexpr char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";
#endif

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char TOKEN[] PROGMEM = "YOUR_DEVICE_ACCESS_TOKEN";
#else
constexpr char TOKEN[] = "YOUR_DEVICE_ACCESS_TOKEN";
#endif

// Thingsboard we want to establish a connection too
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char THINGSBOARD_SERVER[] PROGMEM = "demo.thingsboard.io";
#else
constexpr char THINGSBOARD_SERVER[] = "demo.thingsboard.io";
#endif

// MQTT port used to communicate with the server, 1883 is the default unencrypted MQTT port,
// whereas 8883 would be the default encrypted SSL MQTT port
#if THINGSBOARD_ENABLE_PROGMEM
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883U;
#else
constexpr uint16_t THINGSBOARD_PORT = 1883U;
#endif

// Maximum size packets will ever be sent or received by the underlying MQTT client,
// if the size is to small messages might not be sent or received messages will be discarded
#if THINGSBOARD_ENABLE_PROGMEM
constexpr uint32_t MAX_MESSAGE_SIZE PROGMEM = 128U;
#else
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
#endif

// Baud rate for the debugging serial connection
// If the Serial output is mangled, ensure to change the monitor speed accordingly to this variable
#if THINGSBOARD_ENABLE_PROGMEM
constexpr uint32_t SERIAL_DEBUG_BAUD PROGMEM = 9600U;
constexpr uint32_t SERIAL_ESP8266_DEBUG_BAUD PROGMEM = 9600U;
#else
constexpr uint32_t SERIAL_DEBUG_BAUD = 9600U;
constexpr uint32_t SERIAL_ESP8266_DEBUG_BAUD = 9600U;
#endif

#if THINGSBOARD_ENABLE_PROGMEM
constexpr char CONNECTING_MSG[] PROGMEM = "Connecting to: (%s) with token (%s)";
constexpr char TEMPERATURE_KEY[] PROGMEM = "temperature";
constexpr char HUMIDITY_KEY[] PROGMEM = "humidity";
constexpr char DEVICE_TYPE_KEY[] PROGMEM = "device_type";
constexpr char ACTIVE_KEY[] PROGMEM = "active";
constexpr char SENSOR_VALUE[] PROGMEM = "sensor";
#else
constexpr char CONNECTING_MSG[] = "Connecting to: (%s) with token (%s)";
constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char HUMIDITY_KEY[] = "humidity";
constexpr char DEVICE_TYPE_KEY[] = "device_type";
constexpr char ACTIVE_KEY[] = "active";
constexpr char SENSOR_VALUE[] = "sensor";
#endif


// Serial driver for ESP
SoftwareSerial soft(2U, 3U); // RX, TX
// Initialize the Ethernet client object
WiFiEspClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);


/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {
#if THINGSBOARD_ENABLE_PROGMEM
  Serial.println(F("Connecting to AP ..."));
#else
  Serial.println("Connecting to AP ...");
#endif
  // Attempting to establish a connection to the given WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    // Delay 500ms until a connection has been succesfully established
    delay(500);
#if THINGSBOARD_ENABLE_PROGMEM
    Serial.print(F("."));
#else
    Serial.print(".");
#endif
  }
#if THINGSBOARD_ENABLE_PROGMEM
  Serial.println(F("Connected to AP"));
#else
  Serial.println("Connected to AP");
#endif
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
#if THINGSBOARD_ENABLE_PROGMEM
    Serial.println(F("WiFi shield not present"));
#else
    Serial.println("WiFi shield not present");
#endif
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
    char message[ThingsBoard::detectSize(CONNECTING_MSG, THINGSBOARD_SERVER, TOKEN)];
    snprintf_P(message, sizeof(message), CONNECTING_MSG, THINGSBOARD_SERVER, TOKEN);
    Serial.println(message);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
#if THINGSBOARD_ENABLE_PROGMEM
      Serial.println(F("Failed to connect"));
#else
      Serial.println("Failed to connect");
#endif
      return;
    }
  }

#if THINGSBOARD_ENABLE_PROGMEM
  Serial.println(F("Sending telemetry data..."));
#else
  Serial.println("Sending telemetry data...");
#endif

  const uint8_t data_items = 2U;
  Telemetry data[data_items] = {
    { TEMPERATURE_KEY, 42.2 },
    { HUMIDITY_KEY,    80 },
  };

  /* For C++98 compiler, shipped with Arduino IDE version 1.6.6 or less:

  Telemetry data[data_items] = {
    Telemetry( TEMPERATURE_KEY, 42.2 ),
    Telemetry( HUMIDITY_KEY,    80 ),
  };

  */

  // Uploads new telemetry to ThingsBoard using MQTT. 
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api 
  // for more details
  tb.sendTelemetry(data, data_items);

#if THINGSBOARD_ENABLE_PROGMEM
  Serial.println(F("Sending attributes data..."));
#else
  Serial.println("Sending attributes data...");
#endif

  const int attribute_items = 2;
  Attribute attributes[attribute_items] = {
    { DEVICE_TYPE_KEY,  SENSOR_VALUE },
    { ACTIVE_KEY,       true     },
  };

  /* For C++98 compiler, shipped with Arduino IDE version 1.6.6 or less:

  Attribute attributes[data_items] = {
    Attribute( DEVICE_TYPE_KEY,  SENSOR_VALUE ),
    Attribute( ACTIVE_KEY,       true     ),
  };

  */

  // Publish attribute update to ThingsBoard using MQTT. 
  // See https://thingsboard.io/docs/reference/mqtt-api/#publish-attribute-update-to-the-server 
  // for more details
  tb.sendAttributes(attributes, attribute_items);

  tb.loop();
}
