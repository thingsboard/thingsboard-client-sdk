
# Arduino ThingsBoard SDK

[![Build Status](https://travis-ci.org/thingsboard/ThingsBoard-Arduino-MQTT-SDK.svg?branch=master)](https://travis-ci.org/thingsboard/ThingsBoard-Arduino-MQTT-SDK)

This library provides access to ThingsBoard platform over MQTT protocol.

## Examples

The SDK comes with a number of example sketches. See File > Examples > ThingsBoard within the Arduino application.
Please review the complete guide for ESP32 Pico Kit GPIO control and DHT22 sensor monitoring available [here](https://thingsboard.io/docs/samples/esp32/gpio-control-pico-kit-dht22-sensor/).

## Installation

ThingsBoard SDK can be installed directly from the Arduino Library manager.
Following dependencies must be installed, too:

 - [MQTT PubSub Client](https://github.com/knolleary/pubsubclient) — for interacting with MQTT.
 - [ArduinoJSON](https://github.com/bblanchon/ArduinoJson) — for dealing with JSON files.
 - [Arduino Http Client](https://github.com/arduino-libraries/ArduinoHttpClient) — for interacting with ThingsBoard using HTTP.

## Supported ThingsBoard Features

 - [Telemetry data upload](https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api)
 - [Device attribute publish](https://thingsboard.io/docs/reference/mqtt-api/#publish-attribute-update-to-the-server)
 - [Server-side RPC](https://thingsboard.io/docs/reference/mqtt-api/#server-side-rpc)

## Troubleshooting

### Not enough space for JSON serialization

The buffer size for the serialized JSON is fixed to 64 bytes. The SDK will reject a data, if there is more data to be sent. Respective logs in the "Serial Monitor" window will indicate the condition:

```
[TB] too small buffer for JSON data
```

If that's a case, the buffer size for serialization should be increased. To do so, `ThingsBoardSized` class can be used in place of `ThingsBoard` as illustrated below:

```cpp
// For the sake of example
WiFiEspClient espClient;

// The SDK setup with 64 bytes for JSON buffer
// ThingsBoard tb(espClient);

// The SDK setup with 128 bytes for JSON buffer
ThingsBoardSized<128> tb(espClient);
```

### Too much data fields must be serialized

A buffer allocated internally by ArduinoJson library is fixed and is capable for processing not more than 8 fields. If you are trying to send more than that, you will get an error in the "Serial Monitor" window of the Arduino IDE:

```
[TB] too much JSON fields passed
```

The solution is to use `ThingsBoardSized` class instead of `ThingsBoard`. **Note that the serialized JSON buffer size must be specified explicitly, as described [here](#not-enough-space-for-json-serialization).**

```cpp
// For the sake of example
WiFiEspClient espClient;

// The SDK setup with 8 fields for JSON object
// ThingsBoard tb(espClient);

// The SDK setup with 128 bytes for JSON payload and 32 fields for JSON object.
ThingsBoardSized<128, 32> tb(espClient);
```

## Tips and Tricks
To use your own logger you have to create a class and pass it as third parameter Logger to your `ThingsBoardSized` class instance.

For example:

```cpp
class CustomLogger {
public:
  static void log(const char *error) {
    Serial.print("[Custom Logger] ");
    Serial.println(error);
  }
};
```
Your class must have method `log` with the same prototype as in the example. It will be called if some error occurs.

After that, you can use it in place of regular `ThingsBoard` class. **Note that the serialized JSON buffer size must be specified explicitly, as described [here](#too-much-data-fields-must-be-serialized).**

```cpp
// For the sake of example
WiFiEspClient espClient;

// The SDK setup with 8 fields for JSON object
// ThingsBoard tb(espClient);

// The SDK setup with 128 bytes for JSON payload and 32 fields for JSON object.
ThingsBoardSized<128, 32, CustomLogger> tb(espClient);
```

## Have a question or proposal?

You are welcomed in our [issues](https://github.com/thingsboard/ThingsBoard-Arduino-MQTT-SDK/issues) and [Q&A forum](https://groups.google.com/forum/#!forum/thingsboard).

## License

This code is released under the MIT License.
