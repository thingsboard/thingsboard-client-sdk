# ThingsBoard Client SDK Examples

This directory contains example projects demonstrating the usage of the ThingsBoard Client SDK across different platforms and development boards. Depending on your target platform and the specific ThingsBoard feature you want to explore, you can find corresponding example projects organized in folders.

## Directory Structure and Usage

Each sub-folder represents a specific example that demonstrates one or more features of ThingsBoard over different platforms like Arduino, ESP8266/ESP32 with Arduino, and ESP32 with ESP-IDF. Below you can find guidelines on which folder to choose based on the feature and platform:

### Feature Support Over MQTT and HTTP(S)

#### Over `MQTT`:

All possible features are implemented over `MQTT`:

- [Telemetry data upload](https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api)
- [Device attribute publish](https://thingsboard.io/docs/reference/mqtt-api/#publish-attribute-update-to-the-server)
- [Server-side RPC](https://thingsboard.io/docs/reference/mqtt-api/#server-side-rpc)
- [Client-side RPC](https://thingsboard.io/docs/reference/mqtt-api/#client-side-rpc)
- [Request attribute values](https://thingsboard.io/docs/reference/mqtt-api/#request-attribute-values-from-the-server)
- [Attribute update subscription](https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server)
- [Device provisioning](https://thingsboard.io/docs/reference/mqtt-api/#device-provisioning)
- [Device claiming](https://thingsboard.io/docs/reference/mqtt-api/#claiming-devices)
- [Firmware OTA update](https://thingsboard.io/docs/reference/mqtt-api/#firmware-api)

#### Over `HTTP(S)`:

The remaining features have to be implemented by hand with the `sendGetRequest` or `sendPostRequest` method. See the [ThingsBoard Documentation](https://thingsboard.io/docs/reference/http-api) on how these features could be implemented.

- [Telemetry data upload](https://thingsboard.io/docs/reference/http-api/#telemetry-upload-api)
- [Device attribute publish](https://thingsboard.io/docs/reference/http-api/#publish-attribute-update-to-the-server)

### Folder Prefix Guide

- `arduino`: Examples using Arduino platform on Arduino development boards.
- `esp8266_esp32`: Examples using the Arduino platform on the ESP8266 or ESP32 development boards.
- `espressif_esp32`: Examples using the ESP-IDF platform on the ESP32 development board.

### Examples Overview

| Folder Name                                       | Description                                                      | Platform                          |
|---------------------------------------------------|------------------------------------------------------------------|-----------------------------------|
| `0000-arduino_send_telemetry`                     | Send telemetry data from an Arduino board.                       | Arduino                           |
| `0001-arduino_send_batch`                         | Send batch telemetry data from an Arduino board.                 | Arduino                           |
| `0002-arduino_rpc`                                | Handle RPC commands on Arduino.                                  | Arduino                           |
| `0003-esp8266_esp32_send_data`                    | Send data from an ESP8266 or ESP32 board using Arduino platform. | ESP8266/ESP32 (Arduino)           |
| `0004-arduino-sim900_send_telemetry`              | Send telemetry using Arduino with SIM900 module.                 | Arduino (SIM900)                  |
| `0005-arduino-sim900_send_telemetry_http`         | Send telemetry over HTTP using Arduino with SIM900.              | Arduino (SIM900)                  |
| `0006-esp8266_esp32_process_shared_attribute_update` | Process shared attribute updates on ESP8266/ESP32.            | ESP8266/ESP32 (Arduino)           |
| `0007-esp8266_esp32_claim_device`                 | Device claiming on ESP8266/ESP32.                                | ESP8266/ESP32 (Arduino)           |
| `0008-esp8266_esp32_provision_device`             | Device provisioning on ESP8266/ESP32.                            | ESP8266/ESP32 (Arduino)           |
| `0009-esp8266_esp32_process_OTA_MQTT`             | Handle OTA updates via MQTT on ESP8266/ESP32.                    | ESP8266/ESP32 (Arduino)           |
| `0010-esp8266_esp32_rpc`                          | Handle RPC on ESP8266/ESP32 using Arduino platform.              | ESP8266/ESP32 (Arduino)           |
| `0011-esp8266_esp32_subscribe_OTA_MQTT`           | Subscribe to OTA updates via MQTT on ESP8266/ESP32.              | ESP8266/ESP32 (Arduino)           |
| `0012-esp8266_esp32_request_shared_attribute`     | Request shared attributes on ESP8266/ESP32.                      | ESP8266/ESP32 (Arduino)           |
| `0013-esp8266_esp32_request_rpc`                  | Request execution of RPC on ESP8266/ESP32.                       | ESP8266/ESP32 (Arduino)           |
| `0014-espressif_esp32_send_data`                  | Send data from ESP32 using ESP-IDF platform.                     | ESP32 (ESP-IDF)                   |
| `0015-espressif_esp32_process_OTA_MQTT`           | Process OTA updates via MQTT on ESP32 using ESP-IDF.             | ESP32 (ESP-IDF)                   |
| `0016-espressif_esp32_rpc`                        | Handle RPC on ESP32 using ESP-IDF.                               | ESP32 (ESP-IDF)                   |
| `0017-espressif_esp32_process_shared_attribute_update` | Process shared attribute updates on ESP32 using ESP-IDF.    | ESP32 (ESP-IDF)                   |
| `0018-espressif_esp32_provision_device`           | Device provisioning on ESP32 using ESP-IDF.                      | ESP32 (ESP-IDF)                   |

Each folder contains a `README.md` file with more information about the example. Please refer to the specific `README.md` in each folder for more detailed guidance.
