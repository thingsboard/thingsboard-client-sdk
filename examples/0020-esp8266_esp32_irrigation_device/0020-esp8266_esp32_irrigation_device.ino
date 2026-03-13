#include <Arduino.h>
#include <Arduino_MQTT_Client.h>
#include <WiFi.h>
#include <ThingsBoard.h>
#include <Server_Side_RPC.h>
#include <array>

constexpr char WIFI_SSID[] = "YOUR_WIFI_SSID";
constexpr char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";

constexpr char TOKEN[] = "YOUR_DEVICE_TOKEN";
constexpr char THINGSBOARD_SERVER[] = "YOUR_THINGSBOARD_SERVER";
constexpr uint16_t THINGSBOARD_PORT = 1883U;

constexpr uint32_t SERIAL_DEBUG_BAUD = 9600U;
constexpr uint32_t MEASURE_PERIOD_MS = 1000U;
constexpr uint32_t TELEMETRY_PERIOD_MS = 1000U;
constexpr uint32_t MAX_MESSAGE_SIZE = 2048U;

static const int AOUT_PIN = 36;

static bool default_mode = true;
static bool forced_pump_on = false;

constexpr const char RPC_SET_PUMP_STATUS[] = "setPumpStatus";
constexpr const char RPC_SET_AUTO_MODE[]   = "setAutoMode";
constexpr uint8_t MAX_RPC_SUBSCRIPTIONS = 5U;
constexpr uint8_t MAX_RPC_RESPONSE = 10U;

Server_Side_RPC<MAX_RPC_SUBSCRIPTIONS, MAX_RPC_RESPONSE> rpc;
const std::array<IAPI_Implementation*, 1U> apis = { &rpc };

static bool subscribed = false;

static const int LED_RED_PIN = 25;
static const int LED_GREEN_PIN = 33;
static const int LED_BLUE_PIN = 32;
static const int RELAY_PIN = 27;

static const int AIR_ATTEMPTS = 5;
static const int WATER_ATTEMPTS = 5;

static const uint32_t DELAY_BETWEEN_READINGS_MS = 2000;
static const uint32_t AIR_CALIBRATION_MS = 10000;
static const uint32_t WATER_CALIBRATION_MS = 10000;

static const int DRY_TO_NORMAL = 35;
static const int NORMAL_TO_WET = 70;
static const int HYSTERESIS = 5;

static const int BRIGHT_MAX = 255;
static const int BRIGHT_MIN = 10;

WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoardSized<8, 20> tb(mqttClient, MAX_MESSAGE_SIZE);

enum SoilState { DRY, NORMAL, WET };
static int dry_enter = 0, dry_leave = 0, wet_enter = 0, wet_leave = 0;

void processSetPumpStatus(const JsonVariantConst &data, JsonDocument &response);
void processSetAutoMode(const JsonVariantConst &data, JsonDocument &response);

static void connectWiFi();
static void connectToThingsBoard();
static const char* stateToString(SoilState s);

static void setRGB(int r, int g, int b);
static void pinsReset();
static int mapPercentsToBrightness(int x, int inMin, int inMax, int outMin, int outMax);
static void setLedForState(SoilState s, int percents);

static int median(int *arr, int n);
static int convertMoistureToPercent(int value, int airValue, int waterValue);
static int samplesReading(int count, const char *label);
static void makeCalibration(int &airValue, int &waterValue);

static void sendTelemetry(int raw, int percents, SoilState state,
                          int airValue, int waterValue, bool calibrated, int pumpOn);

const std::array<RPC_Callback, MAX_RPC_SUBSCRIPTIONS> callbacks = {
  RPC_Callback{ RPC_SET_PUMP_STATUS, processSetPumpStatus },
  RPC_Callback{ RPC_SET_AUTO_MODE,   processSetAutoMode }
};

static void connectWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

static bool jsonToBool(const JsonVariantConst &data) {
  if (data.is<bool>()) return data.as<bool>();
  if (data.is<int>())  return data.as<int>() != 0;

  if (data.is<JsonObjectConst>()) {
    JsonObjectConst obj = data.as<JsonObjectConst>();
    if (obj.containsKey("enabled")) return obj["enabled"].as<bool>();
    if (obj.containsKey("pumpOn"))  return obj["pumpOn"].as<bool>();
    if (obj.containsKey("status"))  return obj["status"].as<bool>();
  }
  return false;
}

void processSetPumpStatus(const JsonVariantConst &data, JsonDocument &response) {
  if (default_mode) {
    Serial.println("[RPC] setPumpStatus ignored (AUTO mode)");
    response["auto_mode"] = 1;
    response["pump_on"] = 0;
    return;
  }

  forced_pump_on = jsonToBool(data);

  response["auto_mode"] = 0;
  response["pump_on"] = forced_pump_on ? 1 : 0;

  Serial.print("[RPC] setPumpStatus => ");
  Serial.println(forced_pump_on ? "ON" : "OFF");
}


void processSetAutoMode(const JsonVariantConst &data, JsonDocument &response) {
  default_mode = jsonToBool(data);
  if (default_mode) {
    forced_pump_on = false;
  }

  response["auto_mode"] = default_mode ? 1 : 0;
  response["pump_on"] = forced_pump_on ? 1 : 0;

  Serial.print("[RPC] setAutoMode => ");
}

static void connectToThingsBoard() {
  if (!tb.connected()) {
    Serial.print("Connecting to ThingsBoard: ");
    Serial.println(THINGSBOARD_SERVER);

    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println("Failed to connect to ThingsBoard!");
      return;
    }

    Serial.println("Connected to ThingsBoard successfully!");
    subscribed = false;
  }
}

static const char* stateToString(SoilState s) {
  switch (s) {
    case DRY: return "DRY";
    case WET: return "WET";
    default:  return "NORMAL";
  }
}

static void setRGB(int r, int g, int b) {
  analogWrite(LED_RED_PIN,   255 - r);
  analogWrite(LED_GREEN_PIN, 255 - g);
  analogWrite(LED_BLUE_PIN,  255 - b);
}

static void pinsReset() {
  setRGB(0, 0, 0);
}

static int mapPercentsToBrightness(int x, int inMin, int inMax, int outMin, int outMax) {
  if (inMax == inMin) return outMin;

  if (x < inMin) x = inMin;
  if (x > inMax) x = inMax;

  long numerator = (long)(x - inMin) * (outMax - outMin);
  long denominator = (inMax - inMin);
  return (int)(outMin + numerator / denominator);
}

static void setLedForState(SoilState s, int percents) {
  int red = 0, green = 0, blue = 0;

  if (s == DRY) {
    red = mapPercentsToBrightness(percents, 0, DRY_TO_NORMAL, BRIGHT_MAX, BRIGHT_MIN);
  } else if (s == WET) {
    blue = mapPercentsToBrightness(percents, NORMAL_TO_WET, 100, BRIGHT_MIN, BRIGHT_MAX);
  } else {
    const int mid = (DRY_TO_NORMAL + NORMAL_TO_WET) / 2;
    const int brightnessRange = abs(percents - mid);
    const int maxBrightnessRange = max(mid - DRY_TO_NORMAL, NORMAL_TO_WET - mid);
    green = mapPercentsToBrightness(brightnessRange, 0, maxBrightnessRange, BRIGHT_MAX, BRIGHT_MIN);
  }

  setRGB(red, green, blue);
}

static int median(int *arr, int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - 1 - i; j++) {
      if (arr[j] > arr[j + 1]) {
        int tmp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = tmp;
      }
    }
  }
  return arr[n / 2];
}

static int convertMoistureToPercent(int value, int airValue, int waterValue) {
  const int divider = airValue - waterValue;
  if (divider == 0) {
    Serial.println("[WARN] AIR and WATER calibration values are the same.");
    return 0;
  }

  const float pct = (float)(airValue - value) * 100.0f / (float)divider;
  if (pct < 0) return 0;
  if (pct > 100) return 100;
  return (int)pct;
}

static int samplesReading(int count, const char *label) {
  Serial.print("Taking ");
  Serial.print(count);
  Serial.print(" readings for ");
  Serial.println(label);

  int values[10];
  const int cap = (int)(sizeof(values) / sizeof(values[0]));
  if (count > cap) count = cap;

  int got = 0;
  for (int i = 0; i < count; i++) {
    const int v = analogRead(AOUT_PIN);
    values[got++] = v;

    Serial.print("Reading ");
    Serial.print(i + 1);
    Serial.print("/");
    Serial.print(count);
    Serial.print(": ");
    Serial.println(v);

    delay(DELAY_BETWEEN_READINGS_MS);
  }

  return median(values, got);
}

static void makeCalibration(int &airValue, int &waterValue) {
  Serial.print("Put sensor in AIR. Starting in ");
  Serial.print(DELAY_BETWEEN_READINGS_MS / 1000.0f);
  Serial.println(" seconds...");
  delay(DELAY_BETWEEN_READINGS_MS);

  airValue = samplesReading(AIR_ATTEMPTS, "AIR");
  setRGB(255, 255, 255);
  Serial.print("Put sensor in WATER (up to the white line). Starting in ");
  Serial.print(WATER_CALIBRATION_MS / 1000.0f);
  Serial.println(" seconds...");
  delay(WATER_CALIBRATION_MS);

  waterValue = samplesReading(WATER_ATTEMPTS, "WATER");

  Serial.print("Calibration complete! AIR=");
  Serial.print(airValue);
  Serial.print(" WATER=");
  Serial.println(waterValue);

  if ((float)airValue * 0.8f <= (float)waterValue) {
    Serial.println("[WARN] Calibration looks wrong: AIR not > WATER enough.");
    airValue = -1;
    waterValue = -1;
    pinsReset();
  }
}

static void sendTelemetry(int raw, int percents, SoilState state,
                          int airValue, int waterValue, bool calibrated, int pumpOn) {
  static uint32_t lastSend = 0;
  const uint32_t now = millis();
  if (now - lastSend < TELEMETRY_PERIOD_MS) return;
  lastSend = now;

  if (!tb.connected()) return;

  tb.sendTelemetryData("soil_raw", raw);
  tb.sendTelemetryData("soil_percents", percents);
  tb.sendTelemetryData("soil_state", stateToString(state));

  tb.sendTelemetryData("pump_on", pumpOn ? 1 : 0);
  tb.sendTelemetryData("auto_mode", default_mode ? 1 : 0);

  tb.sendTelemetryData("calibrated", calibrated ? 1 : 0);
}
static int controlRelay(SoilState state, int pumpOn) {
  if (state == DRY) {
    Serial.print("The soil moisture is DRY => activate pump");
    digitalWrite(RELAY_PIN, HIGH);
    return 1;
  } else {
    Serial.print("The soil moisture is WET => deactivate the pump");
    digitalWrite(RELAY_PIN, LOW);
    return 0;
  }
}


void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);

  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  pinsReset();
  analogReadResolution(12);

  connectWiFi();
  connectToThingsBoard();
  tb.setBufferSize(2048, 2048);
  tb.Subscribe_API_Implementations(apis.cbegin(), apis.cend());

  tb.sendTelemetryData("soil_raw", 0);
  tb.sendTelemetryData("soil_percents", 0);
  tb.sendTelemetryData("soil_state", "NORMAL");
  tb.sendTelemetryData("calibrated", 0);
  tb.sendTelemetryData("pump_on", 0);

  Serial.println("Soil moisture starting...");
}

void loop() {
  if (tb.connected()) tb.loop();

  if (tb.connected() && !subscribed) {
    Serial.println("Subscribing for RPC...");
    if (!rpc.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
      Serial.println("Failed to subscribe for RPC");
    } else {
      subscribed = true;
      Serial.println("RPC subscribed");
    }
  }

  static bool calibrated = false;
  static int airValue = 0;
  static int waterValue = 0;
  static SoilState state = NORMAL;
  static int pumpOn = 0;

  if (!calibrated) {
    while (true) {
      makeCalibration(airValue, waterValue);
      tb.sendTelemetryData("calibrated", calibrated ? 1 : 0);
      if (airValue > 0 && waterValue > 0) break;

      Serial.println("Calibration error. Retrying in 10 seconds...");
      delay(10000);
    }

    dry_enter = DRY_TO_NORMAL - HYSTERESIS;
    dry_leave = DRY_TO_NORMAL + HYSTERESIS;
    wet_enter = NORMAL_TO_WET + HYSTERESIS;
    wet_leave = NORMAL_TO_WET - HYSTERESIS;

    state = NORMAL;
    setLedForState(state, (DRY_TO_NORMAL + NORMAL_TO_WET) / 2);

    calibrated = true;
  }

  static uint32_t lastMeasure = 0;
  const uint32_t now = millis();
  if (now - lastMeasure < MEASURE_PERIOD_MS) return;
  lastMeasure = now;

  const int raw = analogRead(AOUT_PIN);
  const int pct = convertMoistureToPercent(raw, airValue, waterValue);

  if (state == DRY && pct >= dry_leave) state = NORMAL;
  else if (state == WET && pct <= wet_leave) state = NORMAL;
  else if (state == NORMAL) {
    if (pct <= dry_enter) state = DRY;
    else if (pct >= wet_enter) state = WET;
  }

  if (default_mode) {
    pumpOn = controlRelay(state, pumpOn);
  } else {
    digitalWrite(RELAY_PIN, forced_pump_on ? HIGH : LOW);
    pumpOn = forced_pump_on ? 1 : 0;
  }

  setLedForState(state, pct);

  Serial.print("Moisture: ");
  Serial.print(pct);
  Serial.print("% (raw: ");
  Serial.print(raw);
  Serial.print(") | state: ");
  Serial.println(stateToString(state));

  sendTelemetry(raw, pct, state, airValue, waterValue, calibrated, pumpOn);

  delay(1000);
}
