# Arduino ThingsBoard SDK

[![MIT license](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://lbesson.mit-license.org/)
[![ESP32](https://img.shields.io/badge/ESP-32-green.svg?style=flat-square)](https://www.espressif.com/en/products/socs/esp32)
[![ESP8266](https://img.shields.io/badge/ESP-8266-blue.svg?style=flat-square)](https://www.espressif.com/en/products/socs/esp8266)
[![GitHub release](https://img.shields.io/github/release/thingsboard/thingsboard-arduino-sdk/all.svg?style=flat-square)](https://github.com/thingsboard/thingsboard-arduino-sdk/releases/)
[![GitHub downloads](https://img.shields.io/github/downloads/thingsboard/thingsboard-arduino-sdk/all.svg?style=flat-square)](https://github.com/thingsboard/thingsboard-arduino-sdk/releases/)
[![Arduino actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/arduino-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/arduino-compile.yml)
[![ESP32 actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp32-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp32-compile.yml)
[![ESP8266 actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp8266-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp8266-compile.yml)
![GitHub stars](https://img.shields.io/github/stars/thingsboard/thingsboard-arduino-sdk?style=social)

This library provides access to ThingsBoard platform over the `MQTT` protocol or alternatively over `HTTP/S`.

## Examples

The SDK comes with a number of example sketches. See **Files --> Examples --> ThingsBoard** within the Arduino application.
Please review the complete guide for `ESP32` Pico Kit `GPIO` control and `DHT22` sensor monitoring available [here](https://thingsboard.io/docs/samples/esp32/gpio-control-pico-kit-dht22-sensor/).

## Installation

ThingsBoard SDK can be installed directly from the [Arduino Library manager](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries) or [PlattformIO](https://registry.platformio.org/).
Following dependencies are installed automatically or must be installed, too:

**Installed automatically:**
 - [ArduinoJSON](https://github.com/bblanchon/ArduinoJson) — for dealing with `JSON` files.


**Needs to be installed manually:**
 - [MQTT PubSub Client](https://github.com/thingsboard/pubsubclient) — for interacting with `MQTT`, when using the `Arduino_MQTT_Client` instance as an argument to `ThingsBoard`.
 - [Arduino Http Client](https://github.com/arduino-libraries/ArduinoHttpClient) — for interacting with `HTTP/S` when using the `Arduino_HTTP_Client` instance as an argument to `ThingsBoardHttp`.
 - [MbedTLS Library](https://github.com/Seeed-Studio/Seeed_Arduino_mbedtls) — needed to create hashes for the OTA update (`ESP8266` only, already included in `ESP32` base firmware).
 - [WiFiEsp Client](https://github.com/bportaluri/WiFiEsp) — needed when using a `Arduino Uno` in combination with a `ESP8266`.

## Supported ThingsBoard Features

### Over `MQTT`:

All possible features are implemented over `MQTT`

 - [Telemetry data upload](https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api)
 - [Device attribute publish](https://thingsboard.io/docs/reference/mqtt-api/#publish-attribute-update-to-the-server)
 - [Server-side RPC](https://thingsboard.io/docs/reference/mqtt-api/#server-side-rpc)
 - [Client-side RPC](https://thingsboard.io/docs/reference/mqtt-api/#client-side-rpc)
 - [Attribute update subscription](https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server)
 - [Device provisioning](https://thingsboard.io/docs/reference/mqtt-api/#device-provisioning)
 - [Device claiming](https://thingsboard.io/docs/reference/mqtt-api/#claiming-devices)
 - [Firmware OTA update](https://thingsboard.io/docs/reference/mqtt-api/#firmware-api)

Over `HTTP\S`:

Remaining features have to be implemented by hand with the `sendGetRequest` or `sendPostRequest` method, see the [ThingsBoard Documentation](https://thingsboard.io/docs/reference/http-api) on how these features could be implemented.

 - [Telemetry data upload](https://thingsboard.io/docs/reference/http-api/#telemetry-upload-api)
 - [Device attribute publish](https://thingsboard.io/docs/reference/http-api/#publish-attribute-update-to-the-server)

Example implementations for all base features, mentioned above, can be found in the `examples` folder. See the according `README.md`, to see which boards are supported and which functionality the example shows.

## Troubleshooting

### No PROGMEM support causing crashes

If the device is crashing with an `Exception` especially `Exception (3)`, more specifically `LoadStoreError` or `LoadStoreErrorCause` this might be because, all constant variables are per default in flash memory to decrease the memory footprint of the library, if the libraries used or the board itself don't support `PROGMEM`. This can cause crashes to mitigate that simply add a `#define THINGSBOARD_ENABLE_PROGMEM 0` before including the ThingsBoard header file.

```c++
// If not set otherwise the value is 1 per default if the pgmspace include exists,
// set to 0 if the board has problems with PROGMEM variables and does not seem to work correctly.
#define THINGSBOARD_ENABLE_PROGMEM 0
#include <ThingsBoard.h>
```

### Dynamic ThingsBoard usage

The received `JSON` payload, as well as the `sendAttributes` and `sendTelemetry` methods use the [`StaticJsonDocument`](https://arduinojson.org/v6/api/staticjsondocument/) by default, this furthermore requires the `MaxFieldsAmt` template argument passed in the constructor. To set the size the buffer should have, where bigger messages will cause not sent / received key-value pairs or failed de-/serialization.

To remove the need for the `MaxFieldsAmt` template argument in the constructor and ensure the size the buffer should have is always enough to hold the sent or received messages, instead `#define THINGSBOARD_ENABLE_DYNAMIC 1` can be set before including the ThingsBoard header file. This makes the library use the [`DynamicJsonDocument`](https://arduinojson.org/v6/api/dynamicjsondocument/) instead of the default [`StaticJsonDocument`](https://arduinojson.org/v6/api/staticjsondocument/).

```c++
// If not set otherwise the value is 0 per default,
// set to 1 if the MaxFieldsAmt template argument should not be required.
#define THINGSBOARD_ENABLE_DYNAMIC 1
#include <ThingsBoard.h>
```

### Not enough space for JSON serialization

The buffer size for the serialized JSON is fixed to 64 bytes. The SDK will not send data, if the size of it is bigger than the size originally passed in the constructor as a template argument (`PayLoadSize`). Respective logs in the `"Serial Monitor"` window will indicate the condition:

```
[TB] Buffer size (64) to small for the given payloads size (83), increase with setBufferSize accordingly or set THINGSBOARD_ENABLE_STREAM_UTILS to 1 before including ThingsBoard
```

If that's a case, the buffer size for serialization should be increased. To do so, `setBufferSize()` method can be used or alternatively the `bufferSize` passed to the constructor can be increased as illustrated below:

```cpp
// For the sake of example
WiFiEspClient espClient;

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);

// The SDK setup with 64 bytes for JSON buffer
// ThingsBoard tb(mqttClient);

// The SDK setup with 128 bytes for JSON buffer
ThingsBoard tb(mqttClient, 128);

void setup() {
  // Increase internal buffer size after inital creation.
  tb.setBufferSize(128);
}
```

Alternatively it is possible to enable the mentioned `THINGSBOARD_ENABLE_STREAM_UTILS` option, which sends messages that are bigger than the given buffer size with a method that skips the internal buffer, be aware tough this only works for sent messages. The internal buffer size still has to be big enough to receive the biggest possible message received by the client that is sent by the server.

```cpp
// Enable skipping usage of the buffer for sends that are bigger than the internal buffer size
#define THINGSBOARD_ENABLE_STREAM_UTILS 1

// For the sake of example
WiFiEspClient espClient;

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);

// The SDK setup with 64 bytes for JSON buffer
ThingsBoard tb(mqttClient);
```

### Too much data fields must be serialized

A buffer allocated internally by `ArduinoJson` library is fixed and is capable for processing not more than 8 fields. If you are trying to send more than that, you will get a respective log showing an error in the `"Serial Monitor"` window:

```
[TB] Too many JSON fields passed (26), increase MaxFieldsAmt (8) accordingly
```

The solution is to use `ThingsBoardSized` class instead of `ThingsBoard`. **Note that the serialized JSON buffer size must be specified explicitly, as described [here](#not-enough-space-for-json-serialization)**. See **Dynamic ThingsBoard Usage** above if the usage of `MaxFieldsAmt`, should be replaced with automatic detection of the needed size.

```cpp
// For the sake of example
WiFiEspClient espClient;

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);

// The SDK setup with 8 fields for JSON object
// ThingsBoard tb(mqttClient);

// The SDK setup with 128 bytes for JSON payload and 32 fields for JSON object.
ThingsBoardSized<32> tb(mqttClient, 128);
```

## Tips and Tricks

### Custom Updater Instance

When using the `ThingsBoard` class instance, the class used to flash the binary data onto the device is not hard coded,
but instead the `OTA_Update_Callback` class expects an optional argument, the `IUpdater` implementation.

Thanks to it being a `interface` it allows an arbitrary implementation,
meaning as long as the device can flash binary data and supports the C++ STL it supports OTA updates, with the `ThingsBoard` library.

Currently, implemented in the library itself are the `ESP32_Updater`, which is used for flashing the binary data when using a `ESP32` and the `ESP8266_Updater` which is used with the `ESP8266`.
If another device wants to be supported, a custom interface implementation needs to be created. For that a `class` that inherits the `IUpdater` interface needs to be created and `override` the needed methods.

```c++
#include <IUpdater.h>

class Custom_Updater : public IUpdater {
  public:
    bool begin(const size_t& firmware_size) override {
        return true;
    }
  
    size_t write(uint8_t* payload, const size_t& total_bytes) override {
        return total_bytes;
    }
  
    void reset() override {
        // Nothing to do
    }
  
    bool end() override {
        return true;
    }
};
```

### Custom HTTP Instance

When using the `ThingsBoardHttp` class instance, the protocol used to send the data to the HTTP broker is not hard coded,
but instead the `ThingsBoardHttp` class expects the argument to a `IHTTP_Client` implementation.

Thanks to it being a `interface` it allows an arbitrary implementation,
meaning the underlying HTTP client can be whatever the user decides, so it can for example be used to support platforms using `Arduino` or even `Espressif IDF`.

Currently, implemented in the library itself is the `Arduino_HTTP_Client`, which is simply a wrapper around the [`ArduinoHttpClient`](https://github.com/arduino-libraries/ArduinoHttpClient), see [dependencies](https://github.com/arduino-libraries/ArduinoHttpClient?tab=readme-ov-file#dependencies) for whether the board you are using is supported or not. If another device wants to be supported, a custom interface implementation needs to be created.
For that a `class` that inherits the `IHTTP_Client` interface needs to be created and `override` the needed methods.


```c++
#include <IHTTP_Client.h>

class Custom_HTTP_Client : public IHTTP_Client {
  public:
    void set_keep_alive(const bool& keep_alive) override {
        // Nothing to do
    }

    int connect(const char *host, const uint16_t& port) override {
        return 0;
    }

    void stop() override {
        // Nothing to do
    }

    int post(const char *url_path, const char *content_type, const char *request_body) override {
        return 0;
    }

    int get_response_status_code() override {
        return 200;
    }

    int get(const char *url_path) override{
        return 0;
    }

    String get_response_body() override{
        return String();
    }
};
```

### Custom MQTT Instance

When using the `ThingsBoard` class instance, the protocol used to send the data to the MQTT broker is not hard coded,
but instead the `ThingsBoard` class expects the argument to a `IMQTT_Client` implementation.

Thanks to it being a `interface` it allows an arbitrary implementation,
meaning the underlying MQTT client can be whatever the user decides, so it can for example be used to support platforms using `Arduino` or even `Espressif IDF`.

Currently, implemented in the library itself is the `Arduino_MQTT_Client`, which is simply a wrapper around the [`TBPubSubClient`](https://github.com/thingsboard/pubsubclient), see [compatible Hardware](https://github.com/thingsboard/pubsubclient?tab=readme-ov-file#compatible-hardware) for whether the board you are using is supported or not. If another device wants to be supported, a custom interface implementation needs to be created.
For that a `class` that inherits the `IMQTT_Client` interface needs to be created and `override` the needed methods.

```c++
#include <IMQTT_Client.h>

class Custom_MQTT_Client : public IMQTT_Client {
  public:
    void set_callback(function cb) override {
        // Nothing to do
    }

    bool set_buffer_size(const uint16_t& buffer_size) override{
        return true;
    }

    uint16_t get_buffer_size() override  {
        return 0U;
    }

    void set_server(const char *domain, const uint16_t& port) override {
        // Nothing to do
    }

    bool connect(const char *client_id, const char *user_name, const char *password) override {
        return true;
    }

    void disconnect() override {
        // Nothing to do
    }

    bool loop() override {
        return true;
    }

    bool publish(const char *topic, const uint8_t *payload, const uint32_t& length) override {
        return true;
    }

    bool subscribe(const char *topic) override {
        return true;
    }

    bool unsubscribe(const char *topic) override {
        return true;
    }

    bool connected() override {
        return true;
    }

#if THINGSBOARD_ENABLE_STREAM_UTILS

    bool begin_publish(const char *topic, const uint32_t& length) override {
        return true;
    }

    bool end_publish() override {
        return true;
    }

    //----------------------------------------------------------------------------
    // Print interface
    //----------------------------------------------------------------------------

    size_t write(uint8_t payload_byte) override {
        return 1U;
    }

    size_t write(const uint8_t *buffer, size_t size) override {
        return size;
    }

#endif // THINGSBOARD_ENABLE_STREAM_UTILS
};
```

### Custom Logger Instance

To use your own logger you have to create a class and pass it as second template parameter `Logger` to your `ThingsBoardSized` class instance.

**For example:**

```cpp
class CustomLogger {
public:
  static void log(const char *error) {
    Serial.print("[Custom Logger] ");
    Serial.println(error);
  }
};
```

Your class must have method `log` with the same prototype as in the example. It will be called, if the library needs to print any log messages.

After that, you can use it in place of regular `ThingsBoard` class. **Note that the serialized JSON buffer size must be specified explicitly, as described [here](#too-much-data-fields-must-be-serialized).**

```cpp
// For the sake of example
WiFiEspClient espClient;

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);

// The SDK setup with 8 fields for JSON object
// ThingsBoard tb(mqttClient);

// The SDK setup with 128 bytes for JSON payload and 32 fields for JSON object.
ThingsBoardSized<32, CustomLogger> tb(mqttClient, 128);
```

## Have a question or proposal?

You are welcomed in our [issues](https://github.com/thingsboard/thingsboard-arduino-sdk/issues) and [Q&A forum](https://groups.google.com/forum/#!forum/thingsboard).

## License

This code is released under the MIT License.
