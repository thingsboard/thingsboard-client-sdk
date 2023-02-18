
# Arduino ThingsBoard SDK

[![MIT license](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://lbesson.mit-license.org/)
[![ESP32](https://img.shields.io/badge/ESP-32-green.svg?style=flat-square)](https://www.espressif.com/en/products/socs/esp32)
[![ESP8266](https://img.shields.io/badge/ESP-8266-blue.svg?style=flat-square)](https://www.espressif.com/en/products/socs/esp8266)
[![GitHub release](https://img.shields.io/github/release/thingsboard/thingsboard-arduino-sdk/all.svg?style=flat-square)](https://github.com/thingsboard/thingsboard-arduino-sdk/releases/)
[![GitHub downloads](https://img.shields.io/github/downloads/thingsboard/thingsboard-arduino-sdk/all.svg?style=flat-square)](https://github.com/thingsboard/thingsboard-arduino-sdk/releases/)
[![Actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/arduino-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/arduino-compile.yml)
[![Actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp32-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp32-compile.yml)
[![Actions status](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp8266-compile.yml/badge.svg)](https://github.com/thingsboard/thingsboard-arduino-sdk/actions/workflows/esp8266-compile.yml)
![GitHub stars](https://img.shields.io/github/stars/thingsboard/thingsboard-arduino-sdk?style=social)

This library provides access to ThingsBoard platform over MQTT protocol.

## Examples

The SDK comes with a number of example sketches. See **Files --> Examples --> ThingsBoard** within the Arduino application.
Please review the complete guide for `ESP32` Pico Kit GPIO control and `DHT22` sensor monitoring available [here](https://thingsboard.io/docs/samples/esp32/gpio-control-pico-kit-dht22-sensor/).

## Installation

ThingsBoard SDK can be installed directly from the Arduino Library manager.
Following dependencies are installed automatically or must be installed, too:

**Installed automatically:**
 - [MQTT PubSub Client](https://github.com/knolleary/pubsubclient) — for interacting with MQTT.
 - [ArduinoJSON](https://github.com/bblanchon/ArduinoJson) — for dealing with JSON files.
 - [Arduino Http Client](https://github.com/arduino-libraries/ArduinoHttpClient) — for interacting with ThingsBoard using HTTP.

**Needs to be installed manually:**
 - [MbedTLS Library](https://github.com/Seeed-Studio/Seeed_Arduino_mbedtls) — needed to create hashes for the OTA update (ESP8266 only, already included in ESP32 base firmware).
 - [WiFiEsp Client](https://github.com/bportaluri/WiFiEsp) — needed when using an `Arduino Uno` in combiantion with an `ESP8266`.

## Supported ThingsBoard Features

 - [Telemetry data upload](https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api)
 - [Device attribute publish](https://thingsboard.io/docs/reference/mqtt-api/#publish-attribute-update-to-the-server)
 - [Server-side RPC](https://thingsboard.io/docs/reference/mqtt-api/#server-side-rpc)
 - [Client-side RPC](https://thingsboard.io/docs/reference/mqtt-api/#client-side-rpc)
 - [Attribute update subscription](https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server)
 - [Device provisioning](https://thingsboard.io/docs/reference/mqtt-api/#device-provisioning)
 - [Device claiming](https://thingsboard.io/docs/reference/mqtt-api/#claiming-devices)
 - [Firmware OTA update](https://thingsboard.io/docs/reference/mqtt-api/#firmware-api)

Example implementations for all features can be found in the `examples` folder. See the according `README.md`, to see which boards are supported and which functionality the example shows.

## Troubleshooting

### PubSubClient causing problems with non ESP boards

For boards that do support the C++ Standard Library (STL), but aren't either the `ESP32` or the `ESP8266` the PubSubClient does not support `std::function` usage.
Meaning the usage of the C++ STL with the ThingsBoard Arduino SDK has to be disabled, luckily this can be done pretty easily simply add a `#define THINGSBOARD_ENABLE_STL 0` before including the ThingsBoard header file.

```c++
// If not set otherwise the value is 0 or 1 depending on if the given device does support the needed C++ STL functionalities.
// Set to 0 if the board does support the C++ STL, but is neither en ESP32 nor an ESP8266, see https://github.com/knolleary/pubsubclient/pull/993 for more information about the issue
#define THINGSBOARD_ENABLE_STL 0
#include <ThingsBoard.h>
```

### No PROGMEM support causing crashes

All constant variables are per default in flash memory to decrease the memory footprint of the library, if the libraries used or the board itself don't support `PROGMEM`. This can cause crashes to mitigate that simply add a `#define THINGSBOARD_ENABLE_PROGMEM 0` before including the ThingsBoard header file.

```c++
// If not set otherwise the value is 1 per default if the ARDUINO plattform is used,
// set to 0 if the board has problems with PROGMEM variables and does not seem to work correctly.
#define THINGSBOARD_ENABLE_PROGMEM 0
#include <ThingsBoard.h>
```

### Not enough space for JSON serialization

The buffer size for the serialized JSON is fixed to 64 bytes. The SDK will not send data, if the size of it is bigger than the size originally passed in the constructor as a template argument (`PayLoadSize`). Respective logs in the `"Serial Monitor"` window will indicate the condition:

```
[TB] Buffer size (64) to small for the given payloads size (83), increase with setBufferSize accordingly
```

If that's a case, the buffer size for serialization should be increased. To do so, `setBufferSize()` method can be used or alternatively the `bufferSize` passed to the constructor can be increased as illustrated below:

```cpp
// For the sake of example
WiFiEspClient espClient;

// The SDK setup with 64 bytes for JSON buffer
// ThingsBoard tb(espClient);

// The SDK setup with 128 bytes for JSON buffer
ThingsBoard tb(espClient, 128);

void setup() {
  // Increase internal buffer size after inital creation.
  tb.setBufferSize(128);
}
```

### Too much data fields must be serialized

A buffer allocated internally by `ArduinoJson` library is fixed and is capable for processing not more than 8 fields. If you are trying to send more than that, you will get a respective log showing an error in the `"Serial Monitor"` window:

```
[TB] Too many JSON fields passed (26), increase MaxFieldsAmt (8) accordingly
```

The solution is to use `ThingsBoardSized` class instead of `ThingsBoard`. **Note that the serialized JSON buffer size must be specified explicitly, as described [here](#not-enough-space-for-json-serialization).**

```cpp
// For the sake of example
WiFiEspClient espClient;

// The SDK setup with 8 fields for JSON object
// ThingsBoard tb(espClient);

// The SDK setup with 128 bytes for JSON payload and 32 fields for JSON object.
ThingsBoardSized<32> tb(espClient, 128);
```

## Tips and Tricks

To use your own logger you have to create a class and pass it as third parameter `Logger` to your `ThingsBoardSized` class instance.

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

// The SDK setup with 8 fields for JSON object
// ThingsBoard tb(espClient);

// The SDK setup with 128 bytes for JSON payload and 32 fields for JSON object.
ThingsBoardSized<32, CustomLogger> tb(espClient, 128);
```

## Have a question or proposal?

You are welcomed in our [issues](https://github.com/thingsboard/thingsboard-arduino-sdk/issues) and [Q&A forum](https://groups.google.com/forum/#!forum/thingsboard).

## License

This code is released under the MIT License.
