# Claiming devices as a cloud user

## Devices
| Supported Devices |
|-------------------|
|  ESP32            |
|  ESP8266          |

## Framework

Arduino

## ThingsBoard API
[Claiming devices](https://thingsboard.io/docs/user-guide/claiming-devices/)

## Feature
Allows any given user on the cloud to assign the given device as their own (claim),
as long as they enter the given device name and secret key in the given amount of time
Optionally a secret key can be passed or be left empty (cloud will allow any user to claim the device for the given amount of time)
