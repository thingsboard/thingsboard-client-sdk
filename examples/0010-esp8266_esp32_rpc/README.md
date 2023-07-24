# Calling methods from the cloud on this device (Server-side RPC)

## Devices
| Supported Devices |
|-------------------|
|  ESP32            |
|  ESP8266          |

## ThingsBoard API
[Server-side Remote Procedure Call](https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc)

## Feature
Creating RPC methods that can be called from the cloud and will result in us receiving a message,
that then calls the corresponding callback on the device, which can optionally return a response that includes certain values

## Remarks
Custom Updater implementation can be used to flash any firmware device as long as it supports the STL base functionality.
Simply create an own `IUpdater` implementation similar to `ESP32_Updater` or `ESP8266_Updater`,
[Thinger IO Arduino library](https://github.com/thinger-io/Arduino-Library) can be used as inspiration on how to write the firmware to [`WiFiStorage for Arduino Nano`](https://github.com/thinger-io/Arduino-Library/blob/master/src/ThingerWiFiNINAOTA.h), [`File Storage for Arduino Nano`](https://github.com/thinger-io/Arduino-Library/blob/master/src/ThingerMbedOTA.h) or [`Second Stage Bootloader (SNB) for Arduino MKRNB`](https://github.com/thinger-io/Arduino-Library/blob/master/src/ThingerMKRNBOTA.h)
