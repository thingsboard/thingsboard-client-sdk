# Active OTA firmware update (immediate)

## Devices
| Supported Devcies |
|-------------------|
|  ESP32            |
|  ESP8266          |

## ThingsBoard API
[OTA Firmware updates](https://thingsboard.io/docs/user-guide/ota-updates/)
[Shared Attributes](https://thingsboard.io/docs/user-guide/attributes/#shared-attributes)

## Feature
Requests the current firmware shared attributes which then start an OTA firmware update immediately,
if the received shared attributes include the same title but a different version number then the one that was passed initally
