# Client SDK to connect with ThingsBoard IoT Platform from various IoT devices (Arduino, Espressif, etc.)

[![MIT license](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://lbesson.mit-license.org/)
[![ESP32](https://img.shields.io/badge/ESP-32-green.svg?style=flat-square)](https://www.espressif.com/en/products/socs/esp32)
[![ESP8266](https://img.shields.io/badge/ESP-8266-blue.svg?style=flat-square)](https://www.espressif.com/en/products/socs/esp8266)
[![GitHub release](https://img.shields.io/github/release/thingsboard/thingsboard-arduino-sdk/all.svg?style=flat-square)](https://github.com/thingsboard/thingsboard-arduino-sdk/releases/)
[![GitHub downloads](https://img.shields.io/github/downloads/thingsboard/thingsboard-arduino-sdk/all.svg?style=flat-square)](https://github.com/thingsboard/thingsboard-arduino-sdk/releases/)
[![Arduino actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/arduino-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/arduino-compile.yml)
[![Espressif IDF v4.4 actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/espidf-compile-v4.4.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/espidf-compile-v4.4.yml)
[![Espressif IDF v5.1 actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/espidf-compile-v5.1.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/espidf-compile-v5.1.yml)
[![ESP32 actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp32-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp32-compile.yml)
[![ESP8266 actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp8266-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp8266-compile.yml)
![GitHub stars](https://img.shields.io/github/stars/thingsboard/thingsboard-arduino-sdk?style=social)

This library provides access to the ThingsBoard platform over the `MQTT` or `HTTP(S)` protocols.

## Examples

The SDK comes with a number of example sketches. See **Files --> Examples --> ThingsBoard** within the Arduino application.
Please review the complete guide for `ESP32` Pico Kit `GPIO` control and `DHT22` sensor monitoring available [here](https://thingsboard.io/docs/samples/esp32/gpio-control-pico-kit-dht22-sensor/).

## Supported Frameworks

`ThingsBoardArduinoSDK` does not directly depend on any specific `MQTT Client` or `HTTP Client` implementation, instead any implementation of the `IMQTT_Client` or `IHTTP Client` can be used. Because there are no further dependencies on `Arduino`, besides the client that communicates it allows us to use this library with `Arduino`, when using the `Arduino_MQTT_Client` or with `Espressif IDF` when using the `Espressif_MQTT_Client`.

Example usage for `Espressif` can be found in the `examples/0014-espressif_esp32_send_data` folder, all other code portions can be implemented the same way only initialization of the needed dependencies is slightly different. Meaning internal call to `ThingsBoard` works the same on both `Espressif` and `Arduino`.

This is also the case, because the only always used dependency that is remaining, is [`ArduinoJson`](https://arduinojson.org/), which despite its name does not require any `Arduino` component.

## Installation

This project can be built with either [PlatformIO](https://platformio.org/), [`ESP IDF Extension`](https://www.espressif.com/) or [Arduino IDE](https://www.arduino.cc/en/software).

The project can be found in the [PlatformIO Registry](https://registry.platformio.org/libraries/thingsboard/ThingsBoard), [ESP Component registry](https://components.espressif.com/components/thingsboard/thingsboard) or the [Arduino libraries](https://www.arduino.cc/reference/en/libraries/thingsboard/).

A description on how to include the library in you project can be found below for each of the aforementioned possible methods of integrating the project.

#### PlatformIO

To add an external library, the most important portion is the [`lib_deps`](https://docs.platformio.org/en/latest/projectconf/sections/env/options/library/lib_deps.html) specification, simply add `thingsboard/ThingsBoard`.
There are multiple ways to define the version that should be fetched, but the most basic is simply getting the last released version, with the aforementioned line, to learn more see [Package Specifications](https://docs.platformio.org/en/latest/core/userguide/pkg/cmd_install.html#package-specifications).

```
lib_deps=
    thingsboard/ThingsBoard
```

#### ESP IDF Extension

To add an external library, what needs to be done differs between versions. If an [ESP-IDF](https://github.com/espressif/esp-idf) version after and including `v3.2.0` is used
then the project can simply be added over the [Component Manager](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-component-manager.html).

To do that we can simply call `idf.py add-dependency <DEPENDENCY>`, with the name of the dependency as an argument. Similar to `PlatformIO` there are a multiple way to define the version that should be fetched, but the method below is the most basic to simply get the last released version, to learn more see [Using Component Manager with a Project](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-component-manager.html#using-with-a-project).

```
idf.py add-dependency "thingsboard/ThingsBoard"
```

If an [ESP-IDF](https://github.com/espressif/esp-idf) version prior to `v3.2.0` is used then the component has to be added as a `git submodule`.
Meaning the repository has to first be a `git` project, if that is not the case already simply install `git` and call `git init` in the folder containing your project.

Similar to the other call there are a multiple way to define the version that should be fetched, but the method below is the most basic to simply get the last released version, to learn more see [Git Submodule Help page](https://git-scm.com/docs/git-submodule).

```
git submodule add https://github.com/thingsboard/thingsboard-arduino-sdk.git components/ThingsBoard
```

#### Arduino IDE

To add an external library, we simply have to open `Tools` -> `Manage Libraries` and then search for `ThingsBoard` then press the `install` button for the wanted version. See [how to install library on Arduino IDE](https://arduinogetstarted.com/faq/how-to-install-library-on-arduino-ide) for more detailed information and some troubleshooting if the aforementioned method does not work.


## Dependencies

Following dependencies are installed automatically or must be installed, too:

**Installed automatically:**
 - [ArduinoJSON](https://github.com/bblanchon/ArduinoJson) — needed for dealing with the `JSON` payload that is sent to and received by `ThingsBoard`
*Installed automatically, but you can remove them if you don't need them*:
 - [MQTT PubSub Client](https://github.com/thingsboard/pubsubclient) — for interacting with `MQTT`, when using the `Arduino_MQTT_Client` instance as an argument to `ThingsBoard`.
 - [Arduino Http Client](https://github.com/arduino-libraries/ArduinoHttpClient) — for interacting with `HTTP/S` when using the `Arduino_HTTP_Client` instance as an argument to `ThingsBoardHttp`.
**Needs to be installed manually:**
 - [MbedTLS Library](https://github.com/Seeed-Studio/Seeed_Arduino_mbedtls) — needed to create hashes for the OTA update (`ESP8266` only, already included in `ESP32` base firmware).
 - [WiFiEsp Client](https://github.com/bportaluri/WiFiEsp) — needed when using a `Arduino Uno` with a `ESP8266`.
 - [StreamUtils](https://github.com/bblanchon/StreamUtils) — needed if `#define THINGSBOARD_ENABLE_STREAM_UTILS 1` is set, it allows sending arbitrary amount of payload even if the buffer size is too small to hold that complete payload

## Supported ThingsBoard Features

### Over `MQTT`:

All possible features are implemented over `MQTT`:

 - [Telemetry data upload](https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api)
 - [Device attribute publish](https://thingsboard.io/docs/reference/mqtt-api/#publish-attribute-update-to-the-server)
 - [Server-side RPC](https://thingsboard.io/docs/reference/mqtt-api/#server-side-rpc)
 - [Client-side RPC](https://thingsboard.io/docs/reference/mqtt-api/#client-side-rpc)
 - [Request attribute values](https://thingsboard.io/docs/reference/mqtt-api/#request-attribute-values-from-the-server)
 - [Attribute update subscription](https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server)
 - [Device provisioning](https://thingsboard.io/docs/reference/mqtt-api/#device-provisioning)
 - [Device claiming](https://thingsboard.io/docs/reference/mqtt-api/#claiming-devices)
 - [Firmware OTA update](https://thingsboard.io/docs/reference/mqtt-api/#firmware-api)

### Over `HTTP(S)`:

The remaining features have to be implemented by hand with the `sendGetRequest` or `sendPostRequest` method. See the [ThingsBoard Documentation](https://thingsboard.io/docs/reference/http-api) on how these features could be implemented.

 - [Telemetry data upload](https://thingsboard.io/docs/reference/http-api/#telemetry-upload-api)
 - [Device attribute publish](https://thingsboard.io/docs/reference/http-api/#publish-attribute-update-to-the-server)

Example implementations for all base features, mentioned above, can be found in the `examples` folder. See the according `README.md`, to see which boards are supported and which functionality the example shows.

## Troubleshooting

This troubleshooting guide contains common issues that are well known and can occur if the library is used wrongly. Ensure to read this section before creating a new `GitHub Issue`.

### No PROGMEM support causing crashes

If the device is crashing with an `Exception` especially `Exception (3)`, more specifically `LoadStoreError` or `LoadStoreErrorCause` this might be because, all constant variables are per default in flash memory to decrease the memory footprint of the library, if the libraries used or the board itself don't support `PROGMEM`. This can cause crashes to mitigate that add a `#define THINGSBOARD_ENABLE_PROGMEM 0` before including the ThingsBoard header file.

```c++
// If not set otherwise the value is 1 per default if the pgmspace include exists,
// set to 0 if the board has problems with PROGMEM variables and does not seem to work correctly.
#define THINGSBOARD_ENABLE_PROGMEM 0
#include <ThingsBoard.h>
```

### Dynamic ThingsBoard usage

The received `JSON` payload, as well as the `sendAttributes` and `sendTelemetry` methods, use the [`StaticJsonDocument`](https://arduinojson.org/v6/api/staticjsondocument/) by default, this furthermore requires the `MaxFieldsAmt` template argument passed in the constructor. To set the size the buffer should have, where bigger messages will cause not sent/received key-value pairs or failed de-/serialization.

To remove the need for the `MaxFieldsAmt` template argument in the constructor and ensure the size the buffer should have is always enough to hold the sent or received messages, instead `#define THINGSBOARD_ENABLE_DYNAMIC 1` can be set before including the ThingsBoard header file. This makes the library use the [`DynamicJsonDocument`](https://arduinojson.org/v6/api/dynamicjsondocument/) instead of the default [`StaticJsonDocument`](https://arduinojson.org/v6/api/staticjsondocument/).

```c++
// If not set otherwise the value is 0 per default,
// set to 1 if the MaxFieldsAmt template argument should not be required.
#define THINGSBOARD_ENABLE_DYNAMIC 1
#include <ThingsBoard.h>
```

### Not enough space for JSON serialization

The buffer size for the serialized JSON is fixed to 64 bytes. The SDK will not send data if the size of it is bigger than the size originally passed in the constructor as a template argument (`PayLoadSize`). Respective logs in the `"Serial Monitor"` window will indicate the condition:

```
[TB] Buffer size (64) to small for the given payloads size (83), increase with setBufferSize accordingly or set THINGSBOARD_ENABLE_STREAM_UTILS to 1 before including ThingsBoard
```

If that's the case, the buffer size for serialization should be increased. To do so, `setBufferSize()` method can be used or the `bufferSize` passed to the constructor can be increased as illustrated below:

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

Alternatively, it is possible to enable the mentioned `THINGSBOARD_ENABLE_STREAM_UTILS` option, which sends messages that are bigger than the given buffer size with a method that skips the internal buffer, be aware tough this only works for sent messages. The internal buffer size still has to be big enough to receive the biggest possible message received by the client that is sent by the server.

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

    bool publish(const char *topic, const uint8_t *payload, const size_t& length) override {
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

    bool begin_publish(const char *topic, const size_t& length) override {
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

Your class must have the method `log` with the same prototype as in the example. It will be called if the library needs to print any log messages.

After that, you can use it in place of the regular `ThingsBoard` class. **Note that the serialized JSON buffer size must be specified explicitly, as described [here](#too-much-data-fields-must-be-serialized).**

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

You are welcome in our [issues](https://github.com/thingsboard/thingsboard-arduino-sdk/issues) and [Q&A forum](https://groups.google.com/forum/#!forum/thingsboard).

## License

This code is released under the MIT License.
