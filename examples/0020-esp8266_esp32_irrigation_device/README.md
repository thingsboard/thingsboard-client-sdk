# ESP32-Based Irrigation device

## Devices
| Supported Devices |
|-------------------|
|  ESP32            |
|  ESP8266          |

## Framework

Arduino

## ThingsBoard API
[Telemetry](https://thingsboard.io/docs/user-guide/telemetry/)
[Server-side Remote Procedure Call](https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc)

## Feature
After the device connects to ThingsBoard, it begins sending telemetry - values that change over time and are useful for monitoring with history. 
In this project, telemetry includes the raw soil sensor reading, soil moisture percentage, soil state, and pump status. 
Because telemetry is stored over time, it can be displayed on ThingsBoard using charts and time-series widgets.
The device also supports server-side RPC methods, which can be called remotely from the ThingsBoard platform.
When such a method is triggered from the cloud, the ESP32 receives the corresponding message and executes the callback function assigned to that RPC method. 
This makes it possible to control the device remotely, for example by switching between AUTO and MANUAL modes or turning the pump on and off. 
If needed, the callback can also return a response to the cloud with the updated state values.
