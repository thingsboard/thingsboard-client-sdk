/*
  ThingsBoard.h - Library API for sending data to the ThingsBoard.
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef ThingsBoard_h
#define ThingsBoard_h

#include <PubSubClient.h>
#include <ArduinoJson.h>
#if ARDUINOJSON_VERSION_MAJOR == 6
#include "ArduinoJson/Polyfills/type_traits.hpp"
#else 
#include "ArduinoJson/TypeTraits/IsIntegral.hpp"
#endif

// Forward declaration
class ThingsBoard;

// Telemetry record class, allows to store different data using common interface.
class Telemetry {
  friend class ThingsBoard;
public:
  inline Telemetry()
    :m_type(TYPE_NONE), m_key(NULL), m_value() { }

  // Constructs telemetry record from integer value.
  // EnableIf trick is required to overcome ambiguous float/integer conversion
#if ARDUINOJSON_VERSION_MAJOR == 6
  template<
      typename T,
      typename = ARDUINOJSON_NAMESPACE::enable_if<ARDUINOJSON_NAMESPACE::is_integral<T>::value>
  >
#else 
  template<
      typename T,
      typename = ArduinoJson::Internals::EnableIf<ArduinoJson::Internals::IsIntegral<T>::value>
  >
#endif
  inline Telemetry(const char *key, T val)
  :m_type(TYPE_INT), m_key(key), m_value()   { m_value.integer = val; }

  // Constructs telemetry record from boolean value.
  inline Telemetry(const char *key, bool val)
  :m_type(TYPE_BOOL), m_key(key), m_value()  { m_value.boolean = val; }

  // Constructs telemetry record from float value.
  inline Telemetry(const char *key, float val)
  :m_type(TYPE_REAL), m_key(key), m_value()  { m_value.real = val; }

  // Constructs telemetry record from string value.
  inline Telemetry(const char *key, const char *val)
  :m_type(TYPE_STR), m_key(key), m_value()   { m_value.str = val; }

private:
  // Data container
  union data {
    const char  *str;
    bool        boolean;
    int         integer;
    float       real;
  };

  // Data type inside a container
  enum dataType {
    TYPE_NONE,
    TYPE_BOOL,
    TYPE_INT,
    TYPE_REAL,
    TYPE_STR,
  };

  dataType     m_type;  // Data type flag
  const char   *m_key;  // Data key
  data         m_value; // Data value

  // Serializes key-value pair in a generic way.
  bool serializeKeyval(JsonVariant &jsonObj) const;
};

// Convenient aliases

typedef Telemetry Attribute;
typedef Telemetry RPC_Response;
// JSON object is used to communicate RPC parameters to the client
typedef JsonVariant RPC_Data;

// RPC callback wrapper
class RPC_Callback {
  friend class ThingsBoard;
public:

  // RPC callback signature
  typedef RPC_Response (*processFn)(const RPC_Data &data);

  // Constructs empty callback
  inline RPC_Callback()
    :m_name(), m_cb(NULL)                {  }

  // Constructs callback that will be fired upon a RPC request arrival with 
  // given method name
  inline RPC_Callback(const char *methodName, processFn cb)
    :m_name(methodName), m_cb(cb)        {  }

private:
  const char  *m_name;    // Method name
  processFn   m_cb;       // Callback to call
};

// ThingsBoard client class
class ThingsBoard
{
public:
  // Initializes ThingsBoard class with network client.
  inline ThingsBoard(Client &client) :m_client(client) { }

  // Destroys ThingsBoard class with network client.
  inline ~ThingsBoard() { }

  // Connects to the specified ThingsBoard server and port.
  // Access token is used to authenticate a client.
  // Returns true on success, false otherwise.
  bool connect(const char *host, const char *access_token, int port = 1883);

  // Disconnects from ThingsBoard. Returns true on success.
  inline void disconnect() {
    m_client.disconnect();
  }

  // Returns true if connected, false otherwise.
  inline bool connected() {
    return m_client.connected();
  }

  // Executes an event loop for PubSub client.
  inline void loop() {
    m_client.loop();
  }

  //------------------------------------------------------------------------------------------------
  // Telemetry API

  // Sends integer telemetry data to the ThingsBoard, returns true on success.
  inline bool sendTelemetryInt(const char *key, int value) {
    return sendKeyval(key, value);
  }

  // Sends boolean telemetry data to the ThingsBoard, returns true on success.
  inline bool sendTelemetryBool(const char *key, bool value) {
    return sendKeyval(key, value);
  }

  // Sends float telemetry data to the ThingsBoard, returns true on success.
  inline bool sendTelemetryFloat(const char *key, float value) {
    return sendKeyval(key, value);
  }

  // Sends string telemetry data to the ThingsBoard, returns true on success.
  inline bool sendTelemetryString(const char *key, const char *value) {
    return sendKeyval(key, value);
  }

  // Sends aggregated telemetry to the ThingsBoard.
  inline bool sendTelemetry(const Telemetry *data, size_t data_count) {
    return sendDataArray(data, data_count);
  }

  // Sends custom JSON telemetry string to the ThingsBoard.
  inline bool sendTelemetryJson(const char *json) {
    return m_client.publish("v1/devices/me/telemetry", json);
  }

  //------------------------------------------------------------------------------------------------
  // Attribute API

  // Sends integer attribute with given name and value.
  inline bool sendAttributeInt(const char *attrName, int value) {
    return sendKeyval(attrName, value, false);
  }

  // Sends boolean attribute with given name and value.
  inline bool sendAttributeBool(const char *attrName, bool value) {
    return sendKeyval(attrName, value, false);
  }

  // Sends float attribute with given name and value.
  inline bool sendAttributeFloat(const char *attrName, float value) {
    return sendKeyval(attrName, value, false);
  }

  // Sends string attribute with given name and value.
  inline bool sendAttributeString(const char *attrName, const char *value) {
    return sendKeyval(attrName, value, false);
  }

  // Sends aggregated attributes to the ThingsBoard.
  inline bool sendAttributes(const Attribute *data, size_t data_count) {
    return sendDataArray(data, data_count, false);
  }

  // Sends custom JSON with attributes to the ThingsBoard.
  inline bool sendAttributeJSON(const char *json) {
    return m_client.publish("v1/devices/me/attributes", json);
  }

  //------------------------------------------------------------------------------------------------
  // Server-side RPC API

  // Subscribes multiple RPC callbacks with given size
  bool RPC_Subscribe(const RPC_Callback *callbacks, size_t callbacks_size);

  inline bool RPC_Unsubscribe() {
    ThingsBoard::m_subscribedInstance = NULL;
    return m_client.unsubscribe("v1/devices/me/rpc/request/+");
  }

private:
  // Sends single key-value in a generic way.
  template<typename T>
  bool sendKeyval(const char *key, T value, bool telemetry = true) {
    Telemetry t(key, value);

    char payload[64];
    {
      Telemetry t(key, value);
#if ARDUINOJSON_VERSION_MAJOR == 6
      StaticJsonDocument<64> jsonBuffer;
      JsonVariant object = jsonBuffer.to<JsonVariant>();
#else
      StaticJsonBuffer<64> jsonBuffer;
      JsonObject &obj = jsonBuffer.createObject();
      JsonVariant object(obj);
#endif
      t.serializeKeyval(object);
#if ARDUINOJSON_VERSION_MAJOR == 6
      serializeJson(object, payload, sizeof(payload));
#else
      object.printTo(payload, sizeof(payload));
#endif
    }
    return telemetry ? sendTelemetryJson(payload) : sendAttributeJSON(payload);
  }

  // Processes RPC message
  void process_message(char* topic, uint8_t* payload, unsigned int length);

  // Sends array of attributes or telemetry to ThingsBoard
  bool sendDataArray(const Telemetry *data, size_t data_count, bool telemetry = true);

  PubSubClient m_client;              // PubSub MQTT client instance.
  RPC_Callback m_rpcCallbacks[8];     // RPC callbacks array

  // PubSub client cannot call a method when message arrives on subscribed topic.
  // Only free-standing function is allowed.
  // To be able to forward event to an instance, rather than to a function, this pointer exists.
  static ThingsBoard *m_subscribedInstance;

  // The callback for when a PUBLISH message is received from the server.
  static void on_message(char* topic, uint8_t* payload, unsigned int length);
};

#endif // ThingsBoard_h
