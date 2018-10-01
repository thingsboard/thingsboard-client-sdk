/*
  ThingsBoard.cpp - Library for sending data to the ThingsBoard.
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
  https://thingsboard.io/
*/

#include "ThingsBoard.h"

ThingsBoard::ThingsBoard(Client &client)
    :m_client(client) {

    }

bool ThingsBoard::connect(const String &host, const String &access_token, int port) {
  if (!host || !access_token) {
    return false;
  }
  m_client.setServer(host.c_str(), port);
  return m_client.connect("ThingsBoardDevice", access_token.c_str(), NULL);
}

bool ThingsBoard::connected() {
  return m_client.connected();
}

bool ThingsBoard::sendInt(const String &key, int value) {
  return send_keyval(key, value);
}

bool ThingsBoard::sendFloat(const String &key, float value) {
  return send_keyval(key, value);
}

bool ThingsBoard::sendString(const String &key, const String &value) {
  return send_keyval(key, value, true);
}

bool ThingsBoard::sendJson(const String &json) {
  return m_client.publish("v1/devices/me/telemetry", json.c_str());
}

void ThingsBoard::loop() {
  m_client.loop();
}

/*----------------------------------------------------------------------------*/

template<typename T>
bool ThingsBoard::send_keyval(const String &key, T value, bool quote_value) {
    String payload = "{";
    payload += "\""; payload += key; payload += "\":";
    if (quote_value) {
      payload += "\"";
    }
    payload += value;
    if (quote_value) {
      payload += "\"";
    }
    payload += "}";
    return m_client.publish("v1/devices/me/telemetry", payload.c_str());
}
