#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <ThingsBoard.h>


constexpr char WIFI_SSID[] PROGMEM = "YOUR_WIFI_SSID";
constexpr char WIFI_PASSWORD[] PROGMEM = "YOUR_WIFI_PASSWORD";

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
constexpr char TOKEN[] PROGMEM = "YOUR_DEVICE_ACCESS_TOKEN";

// Thingsboard we want to establish a connection too
constexpr char THINGSBOARD_SERVER[] PROGMEM = "demo.thingsboard.io";
// MQTT port used to communicate with the server, 1883 is the default unencrypted MQTT port,
// whereas 8883 would be the default encrypted SSL MQTT port
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883U;

// Maximum size packets will ever be sent or received by the underlying MQTT client,
// if the size is to small messages might not be sent or received messages will be discarded
constexpr uint32_t MQTT_MAX_MESSAGE_SIZE PROGMEM = 128U;

// Baud rate for the debugging serial connection
constexpr uint32_t SERIAL_DEBUG_BAUD PROGMEM = 115200U;

// Possible character options used to generate a password if none is provided.
constexpr char PASSWORD_OPTIONS[] PROGMEM = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// See https://thingsboard.io/docs/user-guide/claiming-devices/
// to know how to claim a device once the request has been sent to Thingsboard cloud
constexpr uint32_t CLAIMING_REQUEST_DURATION_MS PROGMEM = (3U * 60U * 1000U);
// Optionally keep the claiming request secret key empty,
// and a random password will be generated for the claiming request instead.
std::string claimingRequestSecretKey = "";


// Initialize underlying client, used to establish a connection
WiFiClient espClient;
// Initialize ThingsBoard instance with the maximum needed buffer size
ThingsBoardSized<MQTT_MAX_MESSAGE_SIZE> tb(espClient);

// Statuses for claiming
bool claimingRequestSent = false;


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

/// @brief Updated callback that will be called as soon as the firmware update finishes
/// @param success Either true (update succesfull) or false (update failed)
void updatedCallback(const bool& success) {
  if (success) {
    Serial.println("Done, Reboot now");
#if defined(ESP8266)
    ESP.restart();
#elif defined(ESP32)
    esp_restart();
#endif
    return;
  }
  Serial.println("Downloading firmware failed");
}

/// @brief Generates a random password from a defined set of predefined options
/// @param length Length of the password that should be generated
/// @return The generated password.
const std::string generateRandomPassword(const uint8_t& length = 8U) {
  std::string password = "";
  for (int i = 0; i < length; i++) {
    password.append(1U, PASSWORD_OPTIONS[random(sizeof(PASSWORD_OPTIONS))]);
  }
  return password;
}

void setup() {
  // If analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  delay(1000);
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
    Serial.printf("Connecting to: (%s) with token (%s)\n", THINGSBOARD_SERVER, TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  if (!claimingRequestSent) {
    // Check if passed claimingRequestSecretKey was empty,
    // and if it was generate a random password and use that one instead
    if (claimingRequestSecretKey.empty()) {
      claimingRequestSecretKey = generateRandomPassword();
    }
    Serial.printf("Sending claiming request with password (%s) being (%u) characters long and a timeout of (%u)ms", claimingRequestSecretKey.c_str(), claimingRequestSecretKey.length(), CLAIMING_REQUEST_DURATION_MS);
    claimingRequestSent = tb.sendClaimingRequest(claimingRequestSecretKey.c_str(), CLAIMING_REQUEST_DURATION_MS);
  }

  tb.loop();
}
