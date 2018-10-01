/*
  ThingsBoard.h - Library API for sending data to the ThingsBoard.
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef ThingsBoard_h
#define ThingsBoard_h

#include <PubSubClient.h>

class ThingsBoard
{
public:
  ThingsBoard(Client &client);

  bool connect(const String &host, const String &access_token, int port = 1883);

  bool connected();

  bool sendInt(const String &key, int value);

  bool sendFloat(const String &key, float value);

  bool sendString(const String &key, const String &value);

  bool sendJson(const String &json);

  void loop();

private:
  template<typename T>
  bool send_keyval(const String &key, T value);

  PubSubClient m_client;
};

#endif // ThingsBoard_h
