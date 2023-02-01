# Sending telemetry / attribute data

## Devices
| Supported Devcies |
|-------------------|
|  ESP32            |
|  ESP8266          |

## ThingsBoard API
[Telemetry](https://thingsboard.io/docs/user-guide/telemetry/)
[Attributes](https://thingsboard.io/docs/user-guide/attributes/)

## Feature
Allows to upload telemetry values to the cloud, as well as attributes.
Telemetry values keep track of their previous values meaning we can draw graphs with them.
Meant for values wich change over time and where a history might be useful (temperature, humidity, ...)
Where as attributes are meant for data, which does not require a history and is more seldomly updated (version, settings, ...)
