# Detecting and reacting to connection state

## Devices
| Supported Devices |
|-------------------|
|  ESP32            |

## Framework

Espressif IDF

## ThingsBoard API
-

## Feature
Allows detecting and react to changes in the connection state. Especially useful when using the `Espressif_MQTT_Client`, because both the `disconnect` and `connect` method are non-blocking.
This means that directly calling `connect` and then attempting to send data to ThingsBoard will cause the calls to fail, because the connection has not been established yet.

To fix this we can either read the current state of the device, so whether it is still connecting or already connected. Alternatively, we can subscribe a callback that will automatically inform once the connection state changes, so we are also informed if the device is now connected.
