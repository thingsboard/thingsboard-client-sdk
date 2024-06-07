# Sending telemetry / attribute data

## Devices
| Supported Devices |
|-------------------|
|  ESP32            |

## Framework

Espressif IDF

## ThingsBoard API
[Server-side Remote Procedure Call](https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc)

## Feature
Creating RPC methods that can be called from the cloud and will result in us receiving a message,
that then calls the corresponding callback on the device, which can optionally return a response that includes certain values
