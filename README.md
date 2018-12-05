# Arduino ThingsBoard SDK

[![Build Status](https://travis-ci.org/thingsboard/ThingsBoard-Arduino-MQTT-SDK.svg?branch=master)](https://travis-ci.org/thingsboard/ThingsBoard-Arduino-MQTT-SDK)

This library provides access to ThingsBoard platform over MQTT protocol.

## Examples

The SDK comes with a number of example sketches. See File > Examples > ThingsBoard within the Arduino application.
Please review the complete guide for ESP32 Pico Kit GPIO control and DHT22 sensor monitoring available [here](  https://thingsboard.io/docs/samples/esp32/gpio-control-pico-kit-dht22-sensor/).

## Installation

ThingsBoard SDK can be installed directly from the Arduino Library manager.
Following dependencies must be installed, too:

 - [MQTT PubSub Client](https://github.com/knolleary/pubsubclient) - for interacting with MQTT.
 - [ArduinoJSON](https://github.com/bblanchon/ArduinoJson) - for dealing with JSON files.

## Supported ThingsBoard Features

 - [Telemetry data upload](https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api)
 - [Device attribute publish](https://thingsboard.io/docs/reference/mqtt-api/#publish-attribute-update-to-the-server)
 - [Server-side RPC](https://thingsboard.io/docs/reference/mqtt-api/#server-side-rpc)

## License

This code is released under the MIT License.
