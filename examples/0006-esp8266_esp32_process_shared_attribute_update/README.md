# Subscribing to shared attributes changes

## Devices
| Supported Devcies |
|-------------------|
|  ESP32            |
|  ESP8266          |

## ThingsBoard API
[Shared Attributes](https://thingsboard.io/docs/user-guide/attributes/#shared-attributes)

## Feature
Allows to subscribe to the given shared attributes,
meaning a callback will be called as soon as one of them has been changed on the cloud.
Optionally no keys can be passed, this functions as a sort of whitelist and allows every single
shared attribute update to call the given callback as soon as it changes it's value.
