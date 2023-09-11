# Subscribing to shared attributes changes

## Devices
| Supported Devices |
|-------------------|
|  ESP32            |
|  ESP8266          |

## Framework

Arduino

## ThingsBoard API
[Shared Attributes](https://thingsboard.io/docs/user-guide/attributes/#shared-attributes)

## Feature
Allows subscribing to the given shared attributes,
meaning a callback will be called as soon as one of them has been changed on the cloud.
Optionally no keys can be passed, this functions as a sort of whitelist and allows every single
shared attribute update to call the given callback as soon as it changes its value.
