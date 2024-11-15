# Sending telemetry / attribute data

## Devices
| Supported Devices |
|-------------------|
|  ESP32            |
|  ESP8266          |

## Framework

Arduino

## ThingsBoard API
[Telemetry](https://thingsboard.io/docs/user-guide/telemetry/)
[Attributes](https://thingsboard.io/docs/user-guide/attributes/)

## Feature
Allows uploading attributes values to the cloud.
Telemetry values keep track of their previous values meaning we can draw graphs with them.
Meant for values which change over time and where a history might be useful (temperature, humidity, ...)
Whereas attributes are meant for data, which does not require a history and is more seldom updated (version, settings, ...)
