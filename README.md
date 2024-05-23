# Client SDK to connect with ThingsBoard IoT Platform from various IoT devices (Arduino, Espressif, etc.)

[![MIT license](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://lbesson.mit-license.org/)
[![ESP32](https://img.shields.io/badge/ESP-32-green.svg?style=flat-square)](https://www.espressif.com/en/products/socs/esp32)
[![ESP8266](https://img.shields.io/badge/ESP-8266-blue.svg?style=flat-square)](https://www.espressif.com/en/products/socs/esp8266)
[![GitHub release](https://img.shields.io/github/release/thingsboard/thingsboard-client-sdk/all.svg?style=flat-square)](https://github.com/thingsboard/thingsboard-client-sdk/releases/)
[![GitHub downloads](https://img.shields.io/github/downloads/thingsboard/thingsboard-client-sdk/all.svg?style=flat-square)](https://github.com/thingsboard/thingsboard-client-sdk/releases/)
[![Arduino actions status](https://github.com/thingsboard/thingsboard-client-sdk/actions/workflows/arduino-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-client-sdk/actions/workflows/arduino-compile.yml)
[![Espressif IDF v4.4 actions status](https://github.com/thingsboard/thingsboard-client-sdk/actions/workflows/espidf-compile-v4.4.yml/badge.svg)](https://github.com/thingsboard/thingsboard-client-sdk/actions/workflows/espidf-compile-v4.4.yml)
[![Espressif IDF v5.1 actions status](https://github.com/thingsboard/thingsboard-client-sdk/actions/workflows/espidf-compile-v5.1.yml/badge.svg)](https://github.com/thingsboard/thingsboard-client-sdk/actions/workflows/espidf-compile-v5.1.yml)
[![ESP32 actions status](https://github.com/thingsboard/thingsboard-client-sdk/actions/workflows/esp32-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-client-sdk/actions/workflows/esp32-compile.yml)
[![ESP8266 actions status](https://github.com/thingsboard/thingsboard-client-sdk/actions/workflows/esp8266-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-client-sdk/actions/workflows/esp8266-compile.yml)
![GitHub stars](https://img.shields.io/github/stars/thingsboard/thingsboard-client-sdk?style=social)

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
git submodule add https://github.com/thingsboard/thingsboard-client-sdk.git components/ThingsBoard
```

#### Arduino IDE

To add an external library, we simply have to open `Tools` -> `Manage Libraries` and then search for `ThingsBoard` then press the `install` button for the wanted version. See [how to install library on Arduino IDE](https://arduinogetstarted.com/faq/how-to-install-library-on-arduino-ide) for more detailed information and some troubleshooting if the aforementioned method does not work.


## Dependencies

Following dependencies are installed automatically or must be installed, too:

**Installed automatically:**
 - [ArduinoJSON](https://github.com/bblanchon/ArduinoJson) — needed for dealing with the `JSON` payload that is sent to and received by `ThingsBoard`

**Needs to be installed manually:**
 - [MQTT PubSub Client](https://github.com/thingsboard/pubsubclient) — for interacting with `MQTT`, when using the `Arduino_MQTT_Client` instance as an argument to `ThingsBoard`.
 - [Arduino Http Client](https://github.com/arduino-libraries/ArduinoHttpClient) — for interacting with `HTTP/S` when using the `Arduino_HTTP_Client` instance as an argument to `ThingsBoardHttp`.
 - [MbedTLS Library](https://github.com/Seeed-Studio/Seeed_Arduino_mbedtls) — needed to create hashes for the OTA update (`ESP8266` only, already included in `ESP32` base firmware).
 - [WiFiEsp Client](https://github.com/bportaluri/WiFiEsp) — needed when using a `Arduino Uno` with a `ESP8266`.
 - [StreamUtils](https://github.com/bblanchon/StreamUtils) — needed when sending arbitrary amount of payload even if the buffer size is too small to hold that complete payload is wanted, aforementioned feature is automatically enabled if the library is installed

## Supported ThingsBoard Features

Example implementations for all base features, mentioned above, can be found in the `examples` folder. See the `README.md` in each example folder, to see which boards are supported and which functionality the example shows.

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

The remaining features have to be implemented by hand with the `sendGetRequest` or `sendPostRequest` method. See the [ThingsBoard Documentation](https://thingsboard.io/docs/reference/http-api) on how these features could be implemented. This is not done directly in the library, because most features require constant polling, whether an event occurred or not, this would cause massive overhead if it is done for all possible features and therefore not recommended.

 - [Telemetry data upload](https://thingsboard.io/docs/reference/http-api/#telemetry-upload-api)
 - [Device attribute publish](https://thingsboard.io/docs/reference/http-api/#publish-attribute-update-to-the-server)

## Troubleshooting

This troubleshooting guide contains common issues that are well known and can occur if the library is used wrongly. Ensure to read this section before creating a new `GitHub Issue`.

### Enabling internal debug messages

If the device is causing problems that are not already described in more detail below, it might be useful to enable internal debug messages, which will allow the library to print more information about sent and received messages as well as internal processes. This is disabled per default to decrease the amount of logs and memory for the log strings on the flash.

```cpp
// If not set the value is 0 per default, meaning it will only print internal error messages,
// set to 1 to also print debugging messages which might help to discern the exact place where a method fails
#define THINGSBOARD_ENABLE_DEBUG 1
#include <ThingsBoard.h>
```

### No PROGMEM support causing crashes

If the device is crashing with a `Exception` especially `Exception (3)`, more specifically `LoadStoreError` or `LoadStoreErrorCause` this might be caused because all constant variables are per default in flash memory to decrease the overall memory footprint of the library. This can cause crashes if the underlying used libraries or the board itself don't support `PROGMEM`, to mitigate that add a `#define THINGSBOARD_ENABLE_PROGMEM 0` before including the ThingsBoard header file. This will simply remove all constants from the flash memory region and should therefore resolve any incompatibilities.

```cpp
// If not set the value is 1 per default if the pgmspace include exists,
// set to 0 if the board has problems with PROGMEM variables and does not seem to work correctly
#define THINGSBOARD_ENABLE_PROGMEM 0
#include <ThingsBoard.h>
```

### Not enough space for JSON serialization

The buffer size for the serialized JSON is fixed to 64 bytes. The SDK will not send data if the size of it is bigger than the configured internal buffer size. Respective logs in the `"Serial Monitor"` window will indicate the condition:

```
[TB] Buffer size (64) to small for the given payloads size (83), increase with setBufferSize accordingly or set THINGSBOARD_ENABLE_STREAM_UTILS to 1 before including ThingsBoard
```

If that's the case, the buffer size for serialization should be increased. To do so, `setBufferSize()` method can be used or the `bufferSize` passed to the constructor can be increased as illustrated below:

```cpp
// Initialize underlying client, used to establish a connection
WiFiClient espClient;

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);

// The SDK setup with 64 bytes for JSON payload
// ThingsBoard tb(mqttClient);

// The SDK setup with 128 bytes for JSON payload
ThingsBoard tb(mqttClient, 128);

void setup() {
  // Increase internal buffer size after inital creation.
  tb.setBufferSize(128);
}
```

Alternatively, it is possible to enable the mentioned `THINGSBOARD_ENABLE_STREAM_UTILS` option, which sends messages that are bigger than the given buffer size with a method that skips the internal buffer, be aware tough this only works for sent messages. The internal buffer size still has to be big enough to receive the biggest possible message received by the client that is sent by the server.

For that the only thing that needs to be done is to install the required `StreamUtils` library, see the [Dependencies](https://github.com/thingsboard/thingsboard-client-sdk?tab=readme-ov-file#dependencies) section.

### Dynamic ThingsBoard usage

All internal methods call attempt to utilize the stack as far as possible and completely minimize heap usage, that is the reason why there are places in the library where template arguments are required. If that memory being on the heap is not an issue, it is possible to remove the need to enter those template arguments altogether. Simply enable the `THINGSBOARD_ENABLE_DYNAMIC` option like shown below.

```cpp
// If not set the value is 0 per default,
// set to 1 if the MaxFieldsAmount template argument should be automatically deduced instead
#define THINGSBOARD_ENABLE_DYNAMIC 1
#include <ThingsBoard.h>
```

### Too much data fields must be serialized

The received `JSON` payload, as well as the `sendAttributes` and `sendTelemetry` methods, use the [`StaticJsonDocument`](https://arduinojson.org/v6/api/staticjsondocument/) this requires the `MaxFieldsAmount` template argument to be passed in the constructor template list. The default value is 8, if more than that are sent, the `"Serial Monitor"` window will get a respective log showing an error:

```
[TB] Unable to serialize key-value json
[TB] Too many JSON fields passed (26), increase MaxFieldsAmount (8) accordingly
```

Additionally, the [`StaticJsonDocument`](https://arduinojson.org/v6/api/staticjsondocument/) is also used to deserialize the received payload for every kind of response received by the server, besides the `OTA` binary data.
This means that if the `MaxFieldsAmount` template argument is smaller than the amount of requested client or shared attributes, the `"Serial Monitor"` window will get a respective log showing an error:

```
[TB] Unable to de-serialize received json data with error (DeserializationError::NoMemory)
```

To fix the issue we simply have to increase the template argument to the required amount.

```cpp
// Initialize underlying client, used to establish a connection
WiFiClient espClient;

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);

// The SDK setup with 64 bytes for JSON payload and 8 fields for JSON object
// ThingsBoard tb(mqttClient);

// The SDK setup with 128 bytes for JSON payload and 32 fields for JSON object
ThingsBoardSized<32> tb(mqttClient, 128);
```

Alternatively to remove the need for the `MaxFieldsAmount` template argument in the constructor template list and to ensure the size the buffer should have is always enough to hold sent or received messages, see the [Dynamic ThingsBoard section](https://github.com/thingsboard/thingsboard-client-sdk?tab=readme-ov-file#dynamic-thingsboard-usage) section. This makes the library use the [`DynamicJsonDocument`](https://arduinojson.org/v6/api/dynamicjsondocument/) instead of the default [`StaticJsonDocument`](https://arduinojson.org/v6/api/staticjsondocument/). Be aware though as this copies sent or received payloads onto the heap.

### Too many subscriptions

The possible event subscription classes that are passed to internal methods, use arrays which reside on the stack those require the `MaxSubscribtions` template argument to be passed in the constructor template list. The default value is 2, if the method call attempts to subscribe more than that many events in total, the `"Serial Monitor"` window will get a respective log showing an error:

```
[TB] Too many server-side RPC subscriptions, increase MaxSubscribtions or unsubscribe
```

Important is that both server-side RPC and request attribute values are temporary, meaning once the request has been received it is deleted, and it is therefore possible to subscribe another event again. However, all other subscriptions like client-side RPC or attribute update subscription are permanent meaning once the event has been subscribed we can only unsubscribe all events to make more room.

Additionally, every aforementioned type of request has its own array meaning one type of event subscription (client-side RPC) does not affect the possible amount for another event subscription (attribute update subscription). Therefore, the only thing that needs to be done is to increase the size accordingly.

```cpp
// Initialize underlying client, used to establish a connection
WiFiClient espClient;

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);

// The SDK setup with 64 bytes for JSON payload, 8 fields for JSON object and 2 maximum subscriptions of every possible type
// ThingsBoard tb(mqttClient);

// The SDK setup with 128 bytes for JSON payload, 32 fields for JSON object and 8 maximum subscriptions of every possible type
ThingsBoardSized<32, 8> tb(mqttClient, 128);
```

Alternatively, to remove the need for the `MaxSubscribtions` template argument in the constructor template list, see the [Dynamic ThingsBoard section](https://github.com/thingsboard/thingsboard-client-sdk?tab=readme-ov-file#dynamic-thingsboard-usage) section. This will replace the internal implementation with a growing vector instead, meaning all the subscribed callback data will reside on the heap instead.

### Too many attributes

The possible attribute values that are passed to the `Shared_Attribute_Callback` or `Attribute_Request_Callback`, use arrays which reside on the stack those require the `MaxAttributes` template argument to be passed in the constructor template list. The default value is 5, if we attempt to subscribe or request more attributes than that, the `"Serial Monitor"` window will get a respective log showing a crash:

```
Assertion `m_size < Capacity' failed.
```

Important is that the minimum size used has to be 5 if the OTA update is used, this is the case, because internally the OTA update process requests or subscribes to updates of 5 attributes. If the amount is decreased below those 5 then the OTA update process can not be started correctly and will not work anymore.

Additionally, the size passed in the template list of the `Shared_Attribute_Callback` or `Attribute_Request_Callback` class, should be the same as the `ThingsBoardSized` class template list. If it isn't, it will not be possible to call the internal methods.

Therefore, the only thing that needs to be done is to increase the size accordingly.

```cpp
// Initialize underlying client, used to establish a connection
WiFiClient espClient;

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);

// The SDK setup with 64 bytes for JSON payload, 32 fields for JSON object, 8 maximum subscriptions of every possible type and 5 possible attribute values that can be passed to Shared_Attribute_Callback or Attribute_Request_Callback
// ThingsBoard tb(mqttClient);

// The SDK setup with 128 bytes for JSON payload, 32 fields for JSON object, 8 maximum subscriptions of every possible type and 6 possible attribute values that can be passed to Shared_Attribute_Callback or Attribute_Request_Callback
ThingsBoardSized<32, 8, 6> tb(mqttClient, 128);
```

Alternatively, to remove the need for the `MaxAttributes` template argument in the constructor template list, see the [Dynamic ThingsBoard section](https://github.com/thingsboard/thingsboard-client-sdk?tab=readme-ov-file#dynamic-thingsboard-usage) section. This will replace the internal implementation with a growing vector instead, meaning all the subscribed attribute data will reside on the heap instead.

### Server-side RPC response overflowed

The possible response in subscribed `RPC_Callback` methods, use the [`StaticJsonDocument`](https://arduinojson.org/v6/api/staticjsondocument/) this requires the `MaxRPC` template argument to be passed in the constructor template list. The default value is 0, if we attempt to return more key-value pairs in the `JSON` that, the `"Serial Monitor"` window will get a respective log showing an error:

```
[TB] Server-side RPC response overflowed, increase MaxRPC (0)
```

The default size is only 0, because if a callback only uses the [`JsonDocument::set()`](https://arduinojson.org/v6/api/jsondocument/set/) method, it does not require additional memory. This is only the case if we attempt to add key-value pairs to the [`JsonDocument`](https://arduinojson.org/v6/api/jsondocument/). Therefore, the only thing that needs to be done is to increase the size accordingly.

```cpp
// Initialize underlying client, used to establish a connection
WiFiClient espClient;

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);

// The SDK setup with 64 bytes for JSON payload, 32 fields for JSON object, 8 maximum subscriptions of every possible type, 5 possible attribute values that can be passed to Shared_Attribute_Callback or Attribute_Request_Callback and 0 possible key-value pairs that can be passed as a response from a server-side RPC call
// ThingsBoard tb(mqttClient);

// The SDK setup with 128 bytes for JSON payload, 32 fields for JSON object, 8 maximum subscriptions of every possible type, 6 possible attribute values that can be passed to Shared_Attribute_Callback or Attribute_Request_Callback and 2 possible key-value pairs that can be passed as a response from a server-side RPC call
ThingsBoardSized<32, 8, 6, 5> tb(mqttClient, 128);
```

Alternatively, to remove the need for the `MaxRPC` template argument in the constructor template list, see the [Dynamic ThingsBoard section](https://github.com/thingsboard/thingsboard-client-sdk?tab=readme-ov-file#dynamic-thingsboard-usage) section. This will instead expect an additional parameter `responseSize` in the `RPC_Callback` constructor argument list, which shows the internal size the [`JsonDocument`](https://arduinojson.org/v6/api/jsondocument/) needs to have to contain the response. Use `JSON_OBJECT_SIZE()` and pass the amount of key value pair to calculate the estimated size. See https://arduinojson.org/v6/assistant/ for more information.

## Tips and Tricks

### Custom Updater Instance

When using the `ThingsBoard` class instance, the class used to flash the binary data onto the device is not hard coded,
but instead the `OTA_Update_Callback` class expects an optional argument, the `IUpdater` implementation.

Thanks to it being an interface it allows an arbitrary implementation,
meaning as long as the device can flash binary data and supports the C++ STL it supports OTA updates, with the `ThingsBoard` library.

Currently, implemented in the library itself are the `Arduino_ESP32_Updater`, which is used for flashing the binary data when using a `ESP32` and `Arduino`, the `Arduino_ESP8266_Updater` which is used with the `ESP8266` and `Arduino` and the `Espressif_Updater` which is used with the `ESP32` and the `Espressif IDF` tool chain.

If another device wants to be supported, a custom interface implementation needs to be created.
For that a `class` needs to inherit the `IUpdater` interface and `override` the needed methods shown below:

```cpp
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

Once that has been done it can simply be passed instead of the `Espressif_Updater`, `Arduino_ESP8266_Client` or the `Arduino_ESP32_Client` instance.

```cpp
// Initalize the Updater client instance used to flash binary to flash memory
Custom_Updater updater;

const OTA_Update_Callback callback(&progressCallback, &updatedCallback, CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION, &updater, FIRMWARE_FAILURE_RETRIES, FIRMWARE_PACKET_SIZE);
```

### Custom HTTP Instance

When using the `ThingsBoardHttp` class instance, the protocol used to send the data to the HTTP broker is not hard coded,
but instead the `ThingsBoardHttp` class expects the argument to a `IHTTP_Client` implementation.

Thanks to it being an interface it allows an arbitrary implementation,
meaning the underlying HTTP client can be whatever the user decides, so it can for example be used to support platforms using `Arduino` or even `Espressif IDF`.

Currently, implemented in the library itself is the `Arduino_HTTP_Client`, which is simply a wrapper around the [`ArduinoHttpClient`](https://github.com/arduino-libraries/ArduinoHttpClient), see [dependencies](https://github.com/arduino-libraries/ArduinoHttpClient?tab=readme-ov-file#dependencies) for whether the board you are using is supported or not.

If another device wants to be supported, a custom interface implementation needs to be created.
For that a `class` needs to inherit the `IHTTP_Client` interface and `override` the needed methods shown below:

```cpp
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

Once that has been done it can simply be passed instead of the `Arduino_HTTP_Client` instance.

```cpp
// Initialize underlying client, used to establish a connection
WiFiClient espClient;

// Initalize the Http client instance
Custom_HTTP_Client httpClient(espClient, THINGSBOARD_SERVER, THINGSBOARD_PORT);

// The SDK setup with 8 fields for JSON object
ThingsBoardHttp tb(httpClient, TOKEN, THINGSBOARD_SERVER, THINGSBOARD_PORT);
```

### Custom MQTT Instance

When using the `ThingsBoard` class instance, the protocol used to send the data to the MQTT broker is not hard coded,
but instead the `ThingsBoard` class expects the argument to a `IMQTT_Client` implementation.

Thanks to it being an interface it allows an arbitrary implementation,
meaning the underlying MQTT client can be whatever the user decides, so it can for example be used to support platforms using `Arduino` or even `Espressif IDF`.

Currently, implemented in the library itself is the `Arduino_MQTT_Client`, which is simply a wrapper around the [`PubSubClient`](https://github.com/thingsboard/pubsubclient), see [compatible Hardware](https://github.com/thingsboard/pubsubclient?tab=readme-ov-file#compatible-hardware) for whether the board you are using is supported or not, useful when using `Arduino`. As well as the `Espressif_MQTT_Client`, which is a simple wrapper around the [`esp-mqtt`](https://github.com/espressif/esp-mqtt), useful when using `Espressif IDF` with a `ESP32`.

If another device wants to be supported, a custom interface implementation needs to be created.
For that a `class` needs to inherit the `IMQTT_Client` interface and `override` the needed methods shown below:

```cpp
#include <IMQTT_Client.h>

class Custom_MQTT_Client : public IMQTT_Client {
  public:
    void set_data_callback(data_function cb) override {
        // Nothing to do
    }

    void set_connect_callback(connect_function cb) override {
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

Once that has been done it can simply be passed instead of the `Arduino_MQTT_Client` or the `Espressif_MQTT_Client` instance.

```cpp
// Initialize underlying client, used to establish a connection
WiFiClient espClient;

// Initalize the Mqtt client instance
Custom_MQTT_Client mqttClient(espClient);

// The SDK setup with 64 bytes for JSON payload
ThingsBoard tb(mqttClient);
```

### Custom Logger Instance

When using the `ThingsBoard` class instance, the class used to print internal warning messages is not hard coded, but instead the `ThingsBoard` class expects the template argument to a `Logger` implementation. See the [Enabling internal debug messages](https://github.com/thingsboard/thingsboard-client-sdk?tab=readme-ov-file#enabling-internal-debug-messages) section if the logger should also receive debug messages.

Thanks to it being a template parameter it allows an arbitrary implementation,
meaning the underlying Logger client can be whatever the user decides, so it can for example be used to print the messages onto a serial card instead of the serial console.

Currently, implemented in the library itself is the `DefaultLogger`, which is simply a wrapper around a `printf` call. If the functionality wants to be extended, a custom implementation needs to be created.
For that a `class` needs to fulfill the contract and implement the needed methods shown below:

```cpp
class CustomLogger {
  public:
    template<typename ...Args>
    static int printfln(char const * const format, Args const &... args) {
        return 0;
    }

    static int println(char const * const message) {
        return 0;
    }
};
```

Once that has been done it can simply be passed as the last template parameter.

```cpp
// Initialize underlying client, used to establish a connection
WiFiClient espClient;

// Initalize the Mqtt client instance
Arduino_MQTT_Client mqttClient(espClient);

// The SDK setup with 64 bytes for JSON payload, 32 fields for JSON object, 8 maximum subscriptions of every possible type, 5 possible attribute values that can be passed to Shared_Attribute_Callback or Attribute_Request_Callback and 0 possible key-value pairs that can be passed as a response from a server-side RPC call and the default logging instance (DefaultLogger)
// ThingsBoard tb(mqttClient);

// The SDK setup with 128 bytes for JSON payload, 32 fields for JSON object, 8 maximum subscriptions of every possible type, 6 possible attribute values that can be passed to Shared_Attribute_Callback or Attribute_Request_Callback, 2 possible key-value pairs that can be passed as a response from a server-side RPC call and a custom logging instance (CustomLogger)
ThingsBoardSized<32, 8, 6, 5, CustomLogger> tb(mqttClient, 128);
```

## Have a question or proposal?

You are welcome in our [issues](https://github.com/thingsboard/thingsboard-client-sdk/issues) and [Q&A forum](https://groups.google.com/forum/#!forum/thingsboard).

## License

This code is released under the MIT License.
