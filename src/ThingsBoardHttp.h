/*
  ThingsBoardHttp.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef ThingsBoard_Http_h
#define ThingsBoard_Http_h

// Library includes.
#if !defined(ESP8266) && !defined(ESP32)
#include <ArduinoHttpClient.h>
#else
#include <HTTPClient.h>
#endif

// Local includes.
#include "ThingsBoardDefaultLogger.h"
#include "Telemetry.h"
#include "Constants.h"

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
// HTTP topics.
constexpr char HTTP_TELEMETRY_TOPIC[] PROGMEM = "/api/v1/%s/telemetry";
constexpr char HTTP_ATTRIBUTES_TOPIC[] PROGMEM = "/api/v1/%s/attributes";
constexpr char HTTP_RPC_TOPIC[] PROGMEM = "/api/v1/%s/rpc?timeout=%u";
constexpr char HTTP_POST_PATH[] PROGMEM = "application/json";

// Log messages.
constexpr char POST[] PROGMEM = "POST";
constexpr char GET[] PROGMEM = "GET";
constexpr char HTTP_FAILED[] PROGMEM = "(%s) failed HTTP response (%d)";

// ThingsBoard HTTP client class
template<size_t MaxFieldsAmt = Default_Fields_Amt,
         typename Logger = ThingsBoardDefaultLogger>
class ThingsBoardHttpSized
{
  public:
    // Initializes ThingsBoardHttpSized class with network client.
#if !defined(ESP8266) && !defined(ESP32)
    inline ThingsBoardHttpSized(Client &client, const char *access_token,
                                const char *host, const uint16_t& port = 80U)
      : m_client(client, host, port)
#else
    inline ThingsBoardHttpSized(WiFiClient &client, const char *access_token,
                                const char *host, const uint16_t& port = 80U)
      : m_client()
      , m_wifiClient(client)
      , m_secure(port == 433U)
#endif
      , m_host(host)
      , m_port(port)
      , m_token(access_token)
    {
#if defined(ESP8266) || defined(ESP32)
      m_client.addHeader("Content-Type", HTTP_POST_PATH);
      m_client.begin(client, m_host, m_port, "/", m_secure);
#else
      m_client.connect(m_host, m_port)
#endif
    }

    // Destroys ThingsBoardHttpSized class with network client.
    inline ~ThingsBoardHttpSized() {
      // Nothing to do.
    }

    // Loop handles get requests to subscribed topics,
    // the timeout in milliseconds defined how long we have to respond.
    inline void loop(const uint32_t& timeoutMs) {
      char path[detectSize(HTTP_RPC_TOPIC, m_token, timeoutMs)];
      snprintf_P(path, sizeof(path), HTTP_RPC_TOPIC, m_token, timeoutMs);

      JsonObject data;
      if (!getMessage(path, data)) {
        return;
      }
      serializeJsonPretty(data, Serial);
    }

    //----------------------------------------------------------------------------
    // Telemetry API

    //----------------------------------------------------------------------------
    // Telemetry API

    // Sends integer telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryInt(const char *key, int value) {
      return sendKeyValue(key, value);
    }

    // Sends boolean telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryBool(const char *key, bool value) {
      return sendKeyValue(key, value);
    }

    // Sends float telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryFloat(const char *key, float value) {
      return sendKeyValue(key, value);
    }

    // Sends string telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryString(const char *key, const char *value) {
      return sendKeyValue(key, value);
    }

    // Sends aggregated telemetry to the ThingsBoard.
    inline const bool sendTelemetry(const Telemetry *data, size_t data_count) {
      return sendDataArray(data, data_count);
    }

    // Sends custom JSON telemetry string to the ThingsBoard, using HTTP.
    inline const bool sendTelemetryJson(const char *json) {
      if (json == nullptr || m_token == nullptr) {
        return false;
      }

      char path[detectSize(HTTP_TELEMETRY_TOPIC, m_token)];
      snprintf_P(path, sizeof(path), HTTP_TELEMETRY_TOPIC, m_token);
      return postMessage(path, json);
    }

    // Sends custom JSON telemetry JsonObject to the ThingsBoard.
    inline const bool sendTelemetryJson(const JsonObject& jsonObject, const uint32_t& jsonSize) {
      const uint32_t jsonObjectSize = jsonObject.size();
      if (MaxFieldsAmt < jsonObjectSize) {
        char message[detectSize(TOO_MANY_JSON_FIELDS, jsonObjectSize, MaxFieldsAmt)];
        snprintf_P(message, sizeof(message), TOO_MANY_JSON_FIELDS, jsonObjectSize, MaxFieldsAmt);
        Logger::log(message);
        return false;
      }
      char json[jsonSize];
      serializeJson(jsonObject, json, jsonSize);
      return sendTelemetryJson(json);
    }

    // Sends custom JSON telemetry JsonVariant to the ThingsBoard.
    inline const bool sendTelemetryJson(const JsonVariant& jsonVariant, const uint32_t& jsonSize) {
      const uint32_t jsonVariantSize = jsonVariant.size();
      if (MaxFieldsAmt < jsonVariantSize) {
        char message[detectSize(TOO_MANY_JSON_FIELDS, jsonVariantSize, MaxFieldsAmt)];
        snprintf_P(message, sizeof(message), TOO_MANY_JSON_FIELDS, jsonVariantSize, MaxFieldsAmt);
        Logger::log(message);
        return false;
      }
      char json[jsonSize];
      serializeJson(jsonVariant, json, jsonSize);
      return sendTelemetryJson(json);
    }

    //----------------------------------------------------------------------------
    // Attribute API

    // Sends integer attribute with given name and value.
    inline const bool sendAttributeInt(const char *attrName, int value) {
      return sendKeyValue(attrName, value, false);
    }

    // Sends boolean attribute with given name and value.
    inline const bool sendAttributeBool(const char *attrName, bool value) {
      return sendKeyValue(attrName, value, false);
    }

    // Sends float attribute with given name and value.
    inline const bool sendAttributeFloat(const char *attrName, float value) {
      return sendKeyValue(attrName, value, false);
    }

    // Sends string attribute with given name and value.
    inline const bool sendAttributeString(const char *attrName, const char *value) {
      return sendKeyValue(attrName, value, false);
    }

    // Sends aggregated attributes to the ThingsBoard.
    inline const bool sendAttributes(const Attribute *data, size_t data_count) {
      return sendDataArray(data, data_count, false);
    }

    // Sends custom JSON with attributes to the ThingsBoard, using HTTP.
    inline const bool sendAttributeJSON(const char *json) {
      if (json == nullptr || m_token == nullptr) {
        return false;
      }

      char path[detectSize(HTTP_ATTRIBUTES_TOPIC, m_token)];
      snprintf_P(path, sizeof(path), HTTP_ATTRIBUTES_TOPIC, m_token);
      return postMessage(path, json);
    }

    // Sends custom JsonObject with attributes to the ThingsBoard.
    inline const bool sendAttributeJSON(const JsonObject& jsonObject, const uint32_t& jsonSize) {
      const uint32_t jsonObjectSize = jsonObject.size();
      if (MaxFieldsAmt < jsonObjectSize) {
        char message[detectSize(TOO_MANY_JSON_FIELDS, jsonObjectSize, MaxFieldsAmt)];
        snprintf_P(message, sizeof(message), TOO_MANY_JSON_FIELDS, jsonObjectSize, MaxFieldsAmt);
        Logger::log(message);
        return false;
      }
      char json[jsonSize];
      serializeJson(jsonObject, json, jsonSize);
      return sendAttributeJSON(json);
    }

    // Sends custom JsonVariant with attributes to the ThingsBoard.
    inline const bool sendAttributeJSON(const JsonVariant& jsonVariant, const uint32_t& jsonSize) {
      const uint32_t jsonVariantSize = jsonVariant.size();
      if (MaxFieldsAmt < jsonVariantSize) {
        char message[detectSize(TOO_MANY_JSON_FIELDS, jsonVariantSize, MaxFieldsAmt)];
        snprintf_P(message, sizeof(message), TOO_MANY_JSON_FIELDS, jsonVariantSize, MaxFieldsAmt);
        Logger::log(message);
        return false;
      }
      char json[jsonSize];
      serializeJson(jsonVariant, json, jsonSize);
      return sendAttributeJSON(json);
    }

  private:
    // Establishes a connection over HTTP or HTTPS, on the given api path.
    inline const bool connect(const char* path) {
      if (path == nullptr) {
        return false;
      }
#if !defined(ESP8266) || defined(ESP32)
      m_client.setURL(path);
#endif

      if (m_client.connected()) {
        return true;
      }

#if !defined(ESP8266) && !defined(ESP32)
      if (!m_client.connect(m_host, m_port)) {
#else
      if (!m_client.begin(m_wifiClient, m_host, m_port, path, m_secure)) {
#endif
        Logger::log(CONNECT_FAILED);
        return false;
      }
      return true;
    }

    // Clears any remaining memory of the previous conenction,
    // but keeps the TCP connection open so it can be reused.
    inline void clearConnection(void) {
#if !defined(ESP8266) && !defined(ESP32)
      m_client.stop();
#else
      m_client.end();
#endif
    }

    // Posts the given json data on the given api path.
    inline const bool postMessage(const char* path, const char* json) {
      if (!connect(path)) {
        return false;
      }

      bool result = true;

#if !defined(ESP8266) && !defined(ESP32)
      const bool success = m_client.post(path, HTTP_POST_PATH, json);
      const int status = m_client.responseStatusCode();
      if (!success || status != HTTP_SUCCESS) {
#else
      const int status = m_client.POST(json);
      if (status != HTTP_CODE_OK) {
#endif
        char message[detectSize(HTTP_FAILED, POST, status)];
        snprintf_P(message, sizeof(message), HTTP_FAILED, POST, status);
        Logger::log(message);
        result = false;
      }

      clearConnection();
      return result;
    }

    // Sends a get request to the given api path and copies the response into the string object
    // will be kept the safe if the get request wasn't successfull.
    inline const bool getMessage(const char* path, JsonObject& response) {
      if (!connect(path)) {
        return false;
      }

      bool result = true;
      DeserializationError error(DeserializationError::Code::Ok);
      const char* payload = nullptr;
      static StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      jsonBuffer.clear();

#if !defined(ESP8266) && !defined(ESP32)
      const bool success = m_client.get(path);
      const int status = m_client.responseStatusCode();
      if (!success || status != HTTP_SUCCESS) {
#else
      const int status = m_client.GET();
      if (status != HTTP_CODE_OK) {
#endif
        char message[detectSize(HTTP_FAILED, GET, status)];
        snprintf_P(message, sizeof(message), HTTP_FAILED, GET, status);
        Logger::log(message);
        result = false;
        goto cleanup;
      }

#if !defined(ESP8266) && !defined(ESP32)
      payload = m_client.responseBody().c_str();
#else
      payload = m_client.getString().c_str();
#endif
      error = deserializeJson(jsonBuffer, payload);

      if (error) {
        char message[detectSize(UNABLE_TO_DE_SERIALIZE_JSON, error.c_str())];
        snprintf_P(message, sizeof(message), UNABLE_TO_DE_SERIALIZE_JSON, error.c_str());
        Logger::log(message);
        result = false;
        goto cleanup;
      }

      response = jsonBuffer.template as<JsonObject>();

      cleanup:
      clearConnection();
      return result;
    }

    // Returns the length in chars needed for a given value with the given argument string to be displayed completly.
    inline const uint8_t detectSize(const char* msg, ...) const {
      va_list args;
      va_start(args, msg);
      // Result is what would have been written if the passed buffer would have been large enough not counting null character,
      // or if an error occured while creating the string a negative number is returned instead. TO ensure this will not crash the system
      // when creating an array with negative size we assert beforehand with a clear error message.
      const int32_t result = JSON_STRING_SIZE(vsnprintf_P(nullptr, 0U, msg, args));
      assert(result >= 0);
      va_end(args);
      return result;
    }

    // Sends array of attributes or telemetry to ThingsBoard
    inline const bool sendDataArray(const Telemetry *data, size_t data_count, bool telemetry = true) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      JsonVariant object = jsonBuffer.template to<JsonVariant>();

      for (size_t i = 0; i < data_count; ++i) {
        if (!data[i].SerializeKeyValue(object)) {
          Logger::log(UNABLE_TO_SERIALIZE);
          return false;
        }
      }

      return telemetry ? sendTelemetryJson(object, JSON_STRING_SIZE(measureJson(object))) : sendAttributeJSON(object, JSON_STRING_SIZE(measureJson(object)));
    }

    // Sends single key-value in a generic way.
    template<typename T>
    inline const bool sendKeyValue(const char *key, T value, bool telemetry = true) {
      Telemetry t(key, value);
      if (t.IsEmpty()) {
        // Message is ignored and not sent at all.
        return false;
      }

      StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonBuffer;
      JsonVariant object = jsonBuffer.template to<JsonVariant>();
      if (!t.SerializeKeyValue(object)) {
        Logger::log(UNABLE_TO_SERIALIZE);
        return false;
      }

      return telemetry ? sendTelemetryJson(object, JSON_STRING_SIZE(measureJson(object))) : sendAttributeJSON(object, JSON_STRING_SIZE(measureJson(object)));
    }

#if !defined(ESP8266) && !defined(ESP32)
    HttpClient m_client;
#else
    HTTPClient m_client;
    WiFiClient& m_wifiClient;
    const bool m_secure;
#endif
    const char *m_host;
    const uint16_t m_port;
    const char *m_token;
};

using ThingsBoardHttp = ThingsBoardHttpSized<>;

#endif // ThingsBoard_Http_h
