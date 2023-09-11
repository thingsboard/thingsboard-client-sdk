# Calling methods on the cloud on this device (Client-side RPC)

## Devices
| Supported Devices |
|-------------------|
|  ESP32            |
|  ESP8266          |

## Framework

Arduino

## ThingsBoard API
[Client-side Remote Procedure Call](https://thingsboard.io/docs/user-guide/rpc/#client-side-rpc)

## Feature
Call RPC method that has been created on the cloud and will result in us receiving an optional response,
that then calls the corresponding callback on the device, if there is no response instead we will receive a timeout after a few seconds,
see [Rule chain](https://thingsboard.io/docs/user-guide/rpc/#processing-the-client-side-rpc-by-the-platform) on how to create methods on the cloud
