// This sketch demonstrates connecting and sending telemetry
// using ThingsBoard SDK and GSM modem, such as SIM900
//
// Hardware:
//  - Arduino Uno
//  - SIM900 Arduino shield connected to Arduino Uno

// Select your modem:
#define TINY_GSM_MODEM_SIM800
// #define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_ESP8266

#include <TinyGsmClient.h>
#include <SoftwareSerial.h>
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>


// Your GPRS credentials
// Leave empty, if missing user or pass
constexpr char APN[] = "internet";
constexpr char USER[] = "";
constexpr char PASS[] = "";

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
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

constexpr char CONNECTING_MSG[] = "Connecting to: (%s) with token (%s)";
constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char HUMIDITY_KEY[] = "humidity";


// Serial port for GSM shield
SoftwareSerial serialGsm(7U, 8U); // RX, TX pins for communicating with modem

// Initialize GSM modem
TinyGsm modem(serialGsm);

// Initialize GSM client
TinyGsmClient client(modem);

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(client);

// Initialize ThingsBoard instance with the maximum needed buffer size
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

// Set to true, if modem is connected
bool modemConnected = false;


void setup() {
  // If analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

  // Set console baud rate
  Serial.begin(SERIAL_DEBUG_BAUD);

  // Set GSM module baud rate
  serialGsm.begin(115200);
  delay(3000);

  // Lower baud rate of the modem.
  // This is highly practical for Uno board, since SoftwareSerial there
  // works too slow to receive a modem data.
  serialGsm.write("AT+IPR=9600\r\n");
  serialGsm.end();
  serialGsm.begin(9600);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();

  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");
}

void loop() {
  delay(1000);

  if (!modemConnected) {
    Serial.print("Waiting for network...");
    if (!modem.waitForNetwork()) {
        Serial.println(" fail");
        delay(10000);
        return;
    }
    Serial.println(" OK");

    Serial.print("Connecting to ");
    Serial.print(APN);
    if (!modem.gprsConnect(APN, USER, PASS)) {
        Serial.println(" fail");
        delay(10000);
        return;
    }

    modemConnected = true;
    Serial.println(" OK");
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

  // Uploads new telemetry to ThingsBoard using HTTP.
  // See https://thingsboard.io/docs/reference/http-api/#telemetry-upload-api
  // for more details
  Serial.println("Sending temperature data...");
  tb.sendTelemetryData(TEMPERATURE_KEY, random(10, 31));

  Serial.println("Sending humidity data...");
  tb.sendTelemetryData(HUMIDITY_KEY, random(40, 90));

  tb.loop();
}
