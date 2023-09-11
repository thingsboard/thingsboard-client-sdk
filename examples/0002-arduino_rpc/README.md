# Calling methods from the cloud on this device (Server-side RPC)

## Devices
| Supported Devices |
|-------------------|
|  Arduino Uno      |
|  ESP8266          |

## Framework

Arduino

## ThingsBoard API
[Server-side Remote Procedure Call](https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc)

## Feature
Creating RPC methods that can be called from the cloud and will result in us receiving a message,
that then calls the corresponding callback on the device, which can optionally return a response that includes certain values
