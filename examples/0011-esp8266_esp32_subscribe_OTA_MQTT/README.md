# Passive OTA firmware update (not immediate)

## Devices
| Supported Devcies |
|-------------------|
|  ESP32            |
|  ESP8266          |

## ThingsBoard API
[OTA Firmware updates](https://thingsboard.io/docs/user-guide/ota-updates/)
[Shared Attributes](https://thingsboard.io/docs/user-guide/attributes/#shared-attributes)

## Feature
Subscribing to changes of the firmware shared attributes, which then start an OTA firmware update,
as soon as they have been updated and include the same title,
but a different version number then the one that was passed initally
