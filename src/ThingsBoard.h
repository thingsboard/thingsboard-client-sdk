/*
  ThingsBoard.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef ThingsBoard_h
#define ThingsBoard_h

#if !defined(ESP8266) && !defined(ESP32)
#include <ArduinoHttpClient.h>
#endif

#if defined(ESP8266)
#include <Updater.h>
#elif defined(ESP32)
#include <Update.h>
#endif

#include <PubSubClient.h>
#include <ArduinoJson.h>

// Local includes.
#include "ThingsBoardDefaultLogger.h"

#define Default_Payload 64
#define Default_Fields_Amt 8

class ThingsBoardDefaultLogger;

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
#if !defined(ESP8266) && !defined(ESP32)
const char HTTP_TELEMETRY_TOPIC[] = PSTR("/api/v1/%s/telemetry");
const char HTTP_ATTRIBUTES_TOPIC[] = PSTR("/api/v1/%s/attributes");
const char HTTP_POST_PATH[] = PSTR("application/json");
#endif

// Publish data topics.
const char ATTRIBUTE_TOPIC[] = PSTR("v1/devices/me/attributes");
const char TELEMETRY_TOPIC[] = PSTR("v1/devices/me/telemetry");

// RPC topics.
const char RPC_SUBSCRIBE_TOPIC[] = PSTR("v1/devices/me/rpc/request/+");
const char RPC_TOPIC[] = PSTR("v1/devices/me/rpc");

// Firmware topics.
const char FIRMWARE_RESPONSE_TOPIC[] = PSTR("v2/fw/response");

// Shared attribute topics.
const char ATTRIBUTE_REQUEST_TOPIC[] = PSTR("v1/devices/me/attributes/request/%u");
const char ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC[] = PSTR("v1/devices/me/attributes/response/+");
const char ATTRIBUTE_RESPONSE_TOPIC[] = PSTR("v1/devices/me/attributes/response");

// Provision topics.
const char PROV_RESPONSE_TOPIC[] = PSTR("/provision/response");

// Default login data.
const char PROV_ACCESS_TOKEN[] = PSTR("provision");
const char DEFAULT_CLIENT_ID[] = PSTR("TbDev");

// Shared attribute request keys.
const char SHARED_KEYS[] = PSTR("sharedKeys");
const char SHARED_KEY[] = PSTR("shared");

// RPC data keys.
const char RPC_METHOD_KEY[] = PSTR("method");
const char RPC_PARAMS_KEY[] = PSTR("params");
const char RPC_REQUEST_KEY[] = PSTR("request");
const char RPC_RESPONSE_KEY[] = PSTR("response");

// Log messages.
const char INVALID_BUFFER_SIZE[] = PSTR("PayloadSize (%u) to small for the given payloads size (%u)");
const char MAX_RPC_EXCEEDED[] = PSTR("Too many rpc subscriptions, increase MaxFieldsAmt or unsubscribe");
const char MAX_SHARED_ATT_UPDATE_EXCEEDED[] = PSTR("Too many shared attribute update callback subscriptions, increase MaxFieldsAmt or unsubscribe");
const char MAX_SHARED_ATT_REQUEST_EXCEEDED[] = PSTR("Too many shared attribute request callback subscriptions, increase MaxFieldsAmt");
const char NUMBER_PRINTF[] = PSTR("%u");
const char COMMA = PSTR(',');
const char NO_KEYS_TO_REQUEST[] = PSTR("No keys to request were given");
const char REQUEST_ATT[] = PSTR("Requesting shared attributes transformed from (%s) into json (%s)");
const char UNABLE_TO_SERIALIZE[] = PSTR("Unable to serialize data");
const char UNABLE_TO_DE_SERIALIZE_RPC[] = PSTR("Unable to de-serialize RPC");
const char UNABLE_TO_DE_SERIALIZE_ATT_REQUEST[] = PSTR("Unable to de-serialize shared attribute request");
const char UNABLE_TO_DE_SERIALIZE_ATT_UPDATE[] = PSTR("Unable to de-serialize shared attribute update");
const char RECEIVED_RPC_LOG_MESSAGE[] = PSTR("Received RPC:");
const char RPC_METHOD_NULL[] = PSTR("RPC method is NULL");
const char RPC_CB_NULL[] = PSTR("RPC callback or subscribed rpc method name is NULL");
const char NO_RPC_PARAMS_PASSED[] = PSTR("No parameters passed with RPC, passing null JSON");
const char CALLING_RPC[] = PSTR("Calling RPC:");
const char RECEIVED_ATT_UPDATE[] = PSTR("Received shared attribute update");
const char NOT_FOUND_ATT_UPDATE[] = PSTR("Shared attribute update key not found");
const char ATT_CB_ID[] = PSTR("Shared attribute update callback id: (%u)");
const char ATT_CB_IS_NULL[] = PSTR("Shared attribute update callback is NULL");
const char ATT_CB_NO_KEYS[] = PSTR("No keys subscribed. Calling subscribed callback for any updated attributes (assumed to be subscribed to every possible key)");
const char ATT_IS_NULL[] = PSTR("Subscribed shared attribute update key is NULL");
const char ATT_IN_ARRAY[] = PSTR("Shared attribute update key: (%s) is subscribed");
const char ATT_NO_CHANGE[] = PSTR("No keys that we subscribed too were changed, skipping callback");
const char CALLING_ATT_CB[] = PSTR("Calling subscribed callback for updated shared attribute (%s)");
const char RECEIVED_ATT[] = PSTR("Received shared attribute request");
const char ATT_KEY_NOT_FOUND[] = PSTR("Shared attribute key not found");
const char ATT_REQUEST_CB_IS_NULL[] = PSTR("Shared attribute request callback is NULL");
const char CALLING_REQUEST_ATT_CB[] = PSTR("Calling subscribed callback for response id (%u)");
const char TOO_MANY_JSON_FIELDS[] = PSTR("Too many JSON fields passed (%u), increase MaxFieldsAmt (%u) accordingly");
const char CB_ON_MESSAGE[] = PSTR("Callback on_message from topic: (%s)");
const char CONNECT_FAILED[] = PSTR("Connecting to server failed");

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
// Claim topics.
const char CLAIM_TOPIC[] = PSTR("v1/devices/me/claim");

// Provision topics.
const char PROV_REQUEST_TOPIC[] = PSTR("/provision/request");

// Claim data keys.
const char SECRET_KEY[] = PSTR("secretKey");
const char DURATION_KEY[] = PSTR("durationMs");
const char DEVICE_NAME_KEY[] = PSTR("deviceName");
const char PROV_DEVICE_KEY[] = PSTR("provisionDeviceKey");
const char PROV_DEVICE_SECRET_KEY[] = PSTR("provisionDeviceSecret");

// Provision data keys.
const char PROV_STATUS_KEY[] = PSTR("status");
const char PROV_CRED_TYPE_KEY[] = PSTR("credentialsType");
const char SUCCESS[] = PSTR("SUCCESS");
const char PROV_CRED_TYPE_VALUE[] = PSTR("X509_CERTIFICATE");

// Log messages.
const char PROV_REQUEST[] = PSTR("Provision request:");
const char UNABLE_TO_DE_SERIALIZE_PROV_RESPONSE[] = PSTR("Unable to de-serialize provision response");
const char PROV_RESPONSE[] = PSTR("Process provisioning response");
const char RECEIVED_PROV_RESPONSE[] = PSTR("Received provision response");
const char X509_NOT_SUPPORTED[] = PSTR("Provision response contains X509_CERTIFICATE credentials, this is not supported yet");

#if !defined(ARDUINO_AVR_MEGA)
// Firmware topics.
const char FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC[] = PSTR("v2/fw/response/#");
const char FIRMWARE_REQUEST_TOPIC[] = PSTR("v2/fw/request/0/chunk/%u");

// Firmware data keys.
const char CURR_FW_TITLE_KEY[] = PSTR("current_fw_title");
const char CURR_FW_VER_KEY[] = PSTR("current_fw_version");
const char CURR_FW_STATE_KEY[] = PSTR("current_fw_state");
const char FW_VER_KEY[] = PSTR("fw_version");
const char FW_TITLE_KEY[] = PSTR("fw_title");
const char FW_CHKS_KEY[] = PSTR("fw_checksum");
const char FW_CHKS_ALGO_KEY[] = PSTR("fw_checksum_algorithm");
const char FW_SIZE_KEY[] = PSTR("fw_size");
const char FW_CHECKSUM_VALUE[] = PSTR("MD5");
const char FW_STATE_CHECKING[] = PSTR("CHECKING FIRMWARE");
const char FW_STATE_NO_FW[] = PSTR("NO FIRMWARE FOUND");
const char FW_STATE_UP_TO_DATE[] = PSTR("UP TO DATE");
const char FW_STATE_INVALID_CHKS[] = PSTR("CHKS IS NOT MD5");
const char FW_STATE_DOWNLOADING[] = PSTR("DOWNLOADING");
const char FW_STATE_FAILED[] = PSTR("FAILED");
const char FW_STATE_UPDATE_ERROR[] = PSTR("UPDATE ERROR");
const char FW_STATE_CHKS_ERROR[] = PSTR("CHECKSUM ERROR");

// Log messages.
const char NO_FW[] = PSTR("No new firmware assigned on the given device");
const char FW_UP_TO_DATE[] = PSTR("Firmware is already up to date");
const char FW_NOT_FOR_US[] = PSTR("Firmware is not for us (title is different)");
const char FW_CHKS_ALGO_NOT_SUPPORTED[] = PSTR("Checksum algorithm is not supported, please use MD5 only");
const char PAGE_BREAK[] = PSTR("=================================");
const char NEW_FW[] = PSTR("A new Firmware is available:");
const char FROM_TOO[] = PSTR("(%s) => (%s)");
const char DOWNLOADING_FW[] = PSTR("Attempting to download over MQTT...");
const char NOT_ENOUGH_RAM[] = PSTR("Not enough RAM");
const char SLASH = PSTR('/');
const char UNABLE_TO_WRITE[] = PSTR("Unable to write firmware");
const char UNABLE_TO_DOWNLOAD[] = PSTR("Unable to download firmware");
const char FW_CHUNK[] = PSTR("Receive chunk (%i), with size (%u) bytes");
const char ERROR_UPDATE_BEGIN[] = PSTR("Error during Update.begin");
const char MD5_ACTUAL[] = PSTR("MD5 actual checksum: (%s)");
const char MD5_EXPECTED[] = PSTR("MD5 expected checksum: (%s)");
const char CHKS_VER_FAILED[] = PSTR("Checksum verification failed");
const char CHKS_VER_SUCCESS[] = PSTR("Checksum is the same as expected");
const char FW_UPDATE_SUCCESS[] = PSTR("Update success");
#endif
#endif

// Telemetry record class, allows to store different data using common interface.
class Telemetry {
    template<size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
    friend class ThingsBoardSized;
#ifndef ESP8266
    template<size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
    friend class ThingsBoardHttpSized;
#endif
  public:
    inline Telemetry()
      : m_type(TYPE_NONE), m_key(NULL), m_value() { }

    // Constructs telemetry record from integer value.
    // EnableIf trick is required to overcome ambiguous float/integer conversion
    template <
      typename T,
      typename = ARDUINOJSON_NAMESPACE::enable_if<ARDUINOJSON_NAMESPACE::is_integral<T>::value>
      >
    inline Telemetry(const char *key, T val)
      : m_type(TYPE_INT), m_key(key), m_value()   {
      m_value.integer = val;
    }

    // Constructs telemetry record from boolean value.
    inline Telemetry(const char *key, bool val)
      : m_type(TYPE_BOOL), m_key(key), m_value()  {
      m_value.boolean = val;
    }

    // Constructs telemetry record from float value.
    inline Telemetry(const char *key, float val)
      : m_type(TYPE_REAL), m_key(key), m_value()  {
      m_value.real = val;
    }

    // Constructs telemetry record from string value.
    inline Telemetry(const char *key, const char *val)
      : m_type(TYPE_STR), m_key(key), m_value()   {
      m_value.str = val;
    }

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
    const bool serializeKeyval(JsonVariant &jsonObj) const;
};

// Convenient aliases

using Attribute = Telemetry;
using RPC_Response = Telemetry;
// JSON variant const (read only twice as small as JSON variant), is used to communicate RPC parameters to the client
using RPC_Data = const JsonVariantConst;
// JSON object const (read only twice as small as JSON object), is used to communicate Shared Attributes and Provision Data to the client
using Shared_Attribute_Data = const JsonObjectConst;
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
using Provision_Data = const JsonObjectConst;
#endif

// RPC callback wrapper
class RPC_Callback {
    template <size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
    friend class ThingsBoardSized;
  public:

    // RPC callback signature
    using processFn = std::function<RPC_Response(const RPC_Data &data)>;

    // Constructs empty callback
    inline RPC_Callback()
      : m_name(), m_cb(nullptr) {  }

    // Constructs callback that will be fired upon a RPC request arrival with
    // given method name
    inline RPC_Callback(const char *methodName, processFn cb)
      : m_name(methodName), m_cb(cb) {  }

  private:
    const char  *m_name;    // Method name
    processFn   m_cb;       // Callback to call
};

// Shared attributes callback wrapper
class Shared_Attribute_Callback {
    template <size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
    friend class ThingsBoardSized;
  public:

    // Shared attributes callback signature
    using processFn = std::function<void(const Shared_Attribute_Data &data)>;

    // Constructs empty callback
    inline Shared_Attribute_Callback()
      : m_att(), m_cb(nullptr) {  }

    // Constructs callback that will be fired upon a Shared attribute update arrival,
    // where one of the given keys was changed.
    inline Shared_Attribute_Callback(const std::vector<const char*>& att, processFn cb)
      : m_att(att), m_cb(cb) {  }

    // Constructs callback that will be fired upon a Shared attribute update arrival,
    // no matter which key was changed.
    inline Shared_Attribute_Callback(processFn cb)
      : m_att(), m_cb(cb) {  }

  private:
    std::vector<const char*> m_att;   // Attribute we want to request
    processFn                m_cb;    // Callback to call
};

// Shared attributes request callback wrapper
class Shared_Attribute_Request_Callback {
    template <size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
    friend class ThingsBoardSized;
  public:

    // Shared attributes callback signature
    using processFn = std::function<void(const Shared_Attribute_Data &data)>;

    // Constructs empty callback
    inline Shared_Attribute_Request_Callback()
      : m_request_id(0U), m_cb(nullptr) {  }

    // Constructs callback that will be fired upon a Shared attribute request arrival
    inline Shared_Attribute_Request_Callback(processFn cb)
      : m_request_id(0U), m_cb(cb) {  }

  private:
    uint32_t        m_request_id;   // Id the request was called with
    processFn       m_cb;           // Callback to call
};

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
// Provisioning callback wrapper
class Provision_Callback {
    template <size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
    friend class ThingsBoardSized;
  public:

    // Provisioning callback signature
    using processFn = std::function<void(const Provision_Data& data)>;

    // Constructs empty callback
    inline Provision_Callback()
      : m_cb(nullptr) {  }

    // Constructs callback that will be fired upon a Provision request arrival with
    // given attribute key
    inline Provision_Callback(processFn cb)
      : m_cb(cb) {  }

  private:
    processFn   m_cb;       // Callback to call
};
#endif

// ThingsBoardSized client class
template<size_t PayloadSize = Default_Payload,
         size_t MaxFieldsAmt = Default_Fields_Amt,
         typename Logger = ThingsBoardDefaultLogger>
class ThingsBoardSized
{
  public:
    // Initializes ThingsBoardSized class with network client.
    // Certain private members can not be set in the constructor initalizor list,
    // because using 2 instances of the ThingsBoard class (for example. provision and connected client)
    // will result in the variables being reset between method calls. Resulting in unexpected behaviour.
    inline ThingsBoardSized(Client& client)
      : m_client()
      , m_requestId(0)
    {
      reserve_callback_size();
      setClient(client);
    }

    // Initializes ThingsBoardSized class without network client. Ensure it is set via. setClient() before using the connect method.
    inline ThingsBoardSized()
      : m_client()
      , m_requestId(0)
    {
      reserve_callback_size();
    }

    // Destroys ThingsBoardSized class with network client.
    inline ~ThingsBoardSized() {
      // Nothing to do.
    }

    // Returns a reference to the PubSubClient.
    inline PubSubClient& getClient(void) {
      return m_client;
    }

    // Sets the underlying Client of the PubSubClient.
    inline void setClient(Client& client) {
      m_client.setClient(client);
      m_client.setBufferSize(PayloadSize);
      // Initalize callback.
      m_client.setCallback(std::bind(&ThingsBoardSized::on_message, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    // Connects to the specified ThingsBoard server and port.
    // Access token is used to authenticate a client.
    // Returns true on success, false otherwise.
    inline const bool connect(const char *host, const char *access_token = PROV_ACCESS_TOKEN, int port = 1883, const char *client_id = DEFAULT_CLIENT_ID, const char *password = NULL) {
      if (!host) {
        return false;
      }
      this->RPC_Unsubscribe(); // Cleanup all RPC subscriptions
      this->Shared_Attributes_Unsubscribe(); // Cleanup all shared attributes subscriptions
      this->Shared_Attributes_Request_Unsubscribe(); // Cleanup all shared attributes requests
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
      this->Provision_Unsubscribe();
#endif
#if defined(ESP8266) || defined(ESP32)
      this->Firmware_OTA_Unsubscribe();
#endif
      m_client.setServer(host, port);
      const bool connection_result = m_client.connect(client_id, access_token, password);
      if (!connection_result) {
        Logger::log(CONNECT_FAILED);
      }
      return connection_result;
    }

    // Disconnects from ThingsBoard.
    inline void disconnect() {
      m_client.disconnect();
    }

    // Returns true if connected, false otherwise.
    inline const bool connected() {
      return m_client.connected();
    }

    // Executes an event loop for PubSub client.
    inline void loop() {
      m_client.loop();
    }

    //----------------------------------------------------------------------------
    // Claiming API

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)

    inline const bool sendClaimingRequest(const char *secretKey, uint32_t durationMs) {
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> requestBuffer;
      JsonObject resp_obj = requestBuffer.to<JsonObject>();

      resp_obj[SECRET_KEY] = secretKey;
      resp_obj[DURATION_KEY] = durationMs;

      uint8_t objectSize = JSON_STRING_SIZE(measureJson(requestBuffer));
      char responsePayload[objectSize];
      serializeJson(resp_obj, responsePayload, objectSize);

      return m_client.publish(CLAIM_TOPIC, responsePayload);
    }

    // Provisioning API
    inline const bool sendProvisionRequest(const char* deviceName, const char* provisionDeviceKey, const char* provisionDeviceSecret) {
      // TODO Add ability to provide specific credentials from client side.
      StaticJsonDocument<JSON_OBJECT_SIZE(3)> requestBuffer;
      JsonObject requestObject = requestBuffer.to<JsonObject>();

      requestObject[DEVICE_NAME_KEY] = deviceName;
      requestObject[PROV_DEVICE_KEY] = provisionDeviceKey;
      requestObject[PROV_DEVICE_SECRET_KEY] = provisionDeviceSecret;

      uint8_t objectSize = JSON_STRING_SIZE(measureJson(requestBuffer));
      char requestPayload[objectSize];
      serializeJson(requestObject, requestPayload, objectSize);

      Logger::log(PROV_REQUEST);
      Logger::log(requestPayload);
      return m_client.publish(PROV_REQUEST_TOPIC, requestPayload);
    }

#endif
    //----------------------------------------------------------------------------
    // Telemetry API

    // Sends telemetry data to the ThingsBoard, returns true on success.
    template<class T>
    inline const bool sendTelemetryData(const char *key, T value)
    {
      return sendKeyval(key, value);
    }

    // Sends integer telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryInt(const char *key, int value) {
      return sendKeyval(key, value);
    }

    // Sends boolean telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryBool(const char *key, bool value) {
      return sendKeyval(key, value);
    }

    // Sends float telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryFloat(const char *key, float value) {
      return sendKeyval(key, value);
    }

    // Sends string telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryString(const char *key, const char *value) {
      return sendKeyval(key, value);
    }

    // Sends aggregated telemetry to the ThingsBoard.
    inline const bool sendTelemetry(const Telemetry *data, size_t data_count) {
      return sendDataArray(data, data_count);
    }

    // Sends custom JSON telemetry string to the ThingsBoard.
    inline const bool sendTelemetryJson(const char *json) {
      if (json == nullptr) {
        return false;
      }

      const uint32_t json_size = JSON_STRING_SIZE(strlen(json));
      if (json_size > PayloadSize) {
        char message[JSON_STRING_SIZE(strlen(INVALID_BUFFER_SIZE)) + detect_size(PayloadSize) + detect_size(json_size)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, PayloadSize, json_size);
        Logger::log(message);
        return false;
      }
      return m_client.publish(TELEMETRY_TOPIC, json);
    }

    // Sends custom JSON telemetry JsonObject to the ThingsBoard.
    inline const bool sendTelemetryJson(const JsonObject& jsonObject) {
      const uint32_t json_object_size = jsonObject.size();
      if (MaxFieldsAmt < json_object_size) {
        char message[JSON_STRING_SIZE(strlen(TOO_MANY_JSON_FIELDS)) + detect_size(json_object_size) + detect_size(MaxFieldsAmt)];
        snprintf_P(message, sizeof(message), TOO_MANY_JSON_FIELDS, json_object_size, MaxFieldsAmt);
        Logger::log(message);
        return false;
      }
      const uint32_t json_size = JSON_STRING_SIZE(measureJson(jsonObject));
      char json[json_size];
      serializeJson(jsonObject, json, json_size);
      return sendTelemetryJson(json);
    }

    //----------------------------------------------------------------------------
    // Attribute API

    // Sends an attribute with given name and value.
    template<class T>
    inline const bool sendAttributeData(const char *attrName, T value) {
      return sendKeyval(attrName, value, false);
    }

    // Sends integer attribute with given name and value.
    inline const bool sendAttributeInt(const char *attrName, int value) {
      return sendKeyval(attrName, value, false);
    }

    // Sends boolean attribute with given name and value.
    inline const bool sendAttributeBool(const char *attrName, bool value) {
      return sendKeyval(attrName, value, false);
    }

    // Sends float attribute with given name and value.
    inline const bool sendAttributeFloat(const char *attrName, float value) {
      return sendKeyval(attrName, value, false);
    }

    // Sends string attribute with given name and value.
    inline const bool sendAttributeString(const char *attrName, const char *value) {
      return sendKeyval(attrName, value, false);
    }

    // Sends aggregated attributes to the ThingsBoard.
    inline const bool sendAttributes(const Attribute *data, size_t data_count) {
      return sendDataArray(data, data_count, false);
    }

    // Sends custom JSON with attributes to the ThingsBoard.
    inline const bool sendAttributeJSON(const char *json) {
      if (json == nullptr) {
        return false;
      }

      const uint32_t json_size = JSON_STRING_SIZE(strlen(json));
      if (json_size > PayloadSize) {
        char message[JSON_STRING_SIZE(strlen(INVALID_BUFFER_SIZE)) + detect_size(PayloadSize) + detect_size(json_size)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, PayloadSize, json_size);
        Logger::log(message);
        return false;
      }
      return m_client.publish(ATTRIBUTE_TOPIC, json);
    }

    // Sends custom JsonObject with attributes to the ThingsBoard.
    inline const bool sendAttributeJSON(const JsonObject& jsonObject) {
      const uint32_t json_object_size = jsonObject.size();
      if (MaxFieldsAmt < json_object_size) {
        char message[JSON_STRING_SIZE(strlen(TOO_MANY_JSON_FIELDS)) + detect_size(json_object_size) + detect_size(MaxFieldsAmt)];
        snprintf_P(message, sizeof(message), TOO_MANY_JSON_FIELDS, json_object_size, MaxFieldsAmt);
        Logger::log(message);
        return false;
      }
      const uint32_t json_size = JSON_STRING_SIZE(measureJson(jsonObject));
      char json[json_size];
      serializeJson(jsonObject, json, json_size);
      return sendAttributeJSON(json);
    }

    //----------------------------------------------------------------------------
    // Server-side RPC API

    // Subscribes multiple RPC callbacks.
    inline const bool RPC_Subscribe(const std::vector<RPC_Callback>& callbacks) {
      if (m_rpcCallbacks.size() + callbacks.size() > m_rpcCallbacks.capacity()) {
        Logger::log(MAX_RPC_EXCEEDED);
        return false;
      }
      if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC)) {
        return false;
      }

      // Push back complete vector into our local m_rpcCallbacks vector.
      m_rpcCallbacks.insert(m_rpcCallbacks.end(), callbacks.begin(), callbacks.end());
      return true;
    }

    // Subscribe one RPC callback.
    inline const bool RPC_Subscribe(const RPC_Callback& callback) {
      if (m_rpcCallbacks.size() + 1 > m_rpcCallbacks.capacity()) {
        Logger::log(MAX_RPC_EXCEEDED);
        return false;
      }
      if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC)) {
        return false;
      }

      // Push back given callback into our local m_rpcCallbacks vector.
      m_rpcCallbacks.push_back(callback);
      return true;
    }

    inline const bool RPC_Unsubscribe() {
      // Empty all callbacks.
      m_rpcCallbacks.clear();
      return m_client.unsubscribe(RPC_SUBSCRIBE_TOPIC);
    }

    //----------------------------------------------------------------------------
    // Firmware OTA API

#if defined(ESP8266) || defined(ESP32)
    inline const bool Start_Firmware_Update(const char* currFwTitle, const char* currFwVersion, const std::function<void(const bool&)>& updatedCallback) {
      m_fwState = nullptr;
      m_fwChecksum = nullptr;

      // Send current firmware version
      if (currFwTitle == nullptr || currFwVersion == nullptr) {
        return false;
      }
      else if (!Firmware_Send_FW_Info(currFwTitle, currFwVersion)) {
        return false;
      }

      // Update state
      if (!Firmware_Send_State(FW_STATE_CHECKING)) {
        return false;
      }

      // Request the firmware informations
      const std::vector<const char*> fwSharedKeys {FW_CHKS_KEY, FW_CHKS_ALGO_KEY, FW_SIZE_KEY, FW_TITLE_KEY, FW_VER_KEY};
      if (!Shared_Attributes_Request(fwSharedKeys, Shared_Attribute_Request_Callback(std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1)))) {
        return false;
      }

      // Set private members needed for update.
      m_currFwTitle = currFwTitle;
      m_currFwVersion = currFwVersion;
      m_fwUpdatedCallback = updatedCallback;
      return true;
    }

    inline const bool Firmware_Send_FW_Info(const char* currFwTitle, const char* currFwVersion) {
      // Send our firmware title and version
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> currentFirmwareInfo;
      JsonObject currentFirmwareInfoObject = currentFirmwareInfo.to<JsonObject>();

      currentFirmwareInfoObject[CURR_FW_TITLE_KEY] = currFwTitle;
      currentFirmwareInfoObject[CURR_FW_VER_KEY] = currFwVersion;
      return sendTelemetryJson(currentFirmwareInfoObject);
    }

    inline const bool Firmware_Send_State(const char* currFwState) {
      // Send our firmware title and version
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> currentFirmwareState;
      JsonObject currentFirmwareStateObject = currentFirmwareState.to<JsonObject>();

      currentFirmwareStateObject[CURR_FW_STATE_KEY] = currFwState;
      return sendTelemetryJson(currentFirmwareStateObject);
    }

    inline const bool Firmware_OTA_Subscribe() {
      if (!m_client.subscribe(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC)) {
        return false;
      }

      return true;
    }

    inline const bool Firmware_OTA_Unsubscribe() {
      if (!m_client.unsubscribe(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC)) {
        return false;
      }

      return true;
    }

    inline void Firmware_Shared_Attribute_Received(const Shared_Attribute_Data& data) {
      // Check if firmware is available for our device
      if (!data.containsKey(FW_VER_KEY) || !data.containsKey(FW_TITLE_KEY)) {
        Logger::log(NO_FW);
        Firmware_Send_State(FW_STATE_NO_FW);
        return;
      }

      const char* fw_title = data[FW_TITLE_KEY].as<const char*>();
      const char* fw_version = data[FW_VER_KEY].as<const char*>();
      m_fwChecksum = data[FW_CHKS_KEY].as<const char*>();
      const char* fw_checksum_algorithm = data[FW_CHKS_ALGO_KEY].as<const char*>();
      m_fwSize = data[FW_SIZE_KEY].as<const uint16_t>();

      // If firmware is the same, we do not update it
      if (strncmp_P(m_currFwTitle, fw_title, strlen(m_currFwTitle)) == 0 && strncmp_P(m_currFwVersion, fw_version, strlen(m_currFwVersion) == 0)) {
        Logger::log(FW_UP_TO_DATE);
        Firmware_Send_State(FW_STATE_UP_TO_DATE);
        return;
      }

      // If firmware title is not the same, we quit now
      if (strncmp_P(m_currFwTitle, fw_title, strlen(m_currFwTitle)) != 0) {
        Logger::log(FW_NOT_FOR_US);
        Firmware_Send_State(FW_STATE_NO_FW);
        return;
      }

      if (strncmp_P(FW_CHECKSUM_VALUE, fw_checksum_algorithm, strlen(FW_CHECKSUM_VALUE)) != 0) {
        Logger::log(FW_CHKS_ALGO_NOT_SUPPORTED);
        Firmware_Send_State(FW_STATE_INVALID_CHKS);
        return;
      }

      // Subscribe to the needed OTA fw topics.
      Firmware_OTA_Subscribe();

      Logger::log(PAGE_BREAK);
      Logger::log(NEW_FW);
      char firmware[JSON_STRING_SIZE(strlen(FROM_TOO)) + JSON_STRING_SIZE(strlen(m_currFwVersion)) + JSON_STRING_SIZE(strlen(fw_version))];
      snprintf_P(firmware, sizeof(firmware), FROM_TOO, m_currFwVersion, fw_version);
      Logger::log(firmware);
      Logger::log(DOWNLOADING_FW);

      const uint16_t chunkSize = 4096U; // maybe less if we don't have enough RAM
      const uint16_t numberOfChunk = static_cast<uint16_t>(m_fwSize / chunkSize) + 1U;
      uint16_t currChunk = 0U;
      uint8_t nbRetry = 3U;

      // Get the previous buffer size and cache it so the previous settings can be restored.
      const uint16_t previousBufferSize = m_client.getBufferSize();
      const bool changeBufferSize = previousBufferSize < (chunkSize + 50U);

      // Increase size of receive buffer
      if (changeBufferSize && !m_client.setBufferSize(chunkSize + 50U)) {
        Logger::log(NOT_ENOUGH_RAM);
        return;
      }

      // Update state
      Firmware_Send_State(FW_STATE_DOWNLOADING);

      char size[JSON_STRING_SIZE(strlen(NUMBER_PRINTF)) + detect_size(chunkSize)];
      // Download the firmware
      do {
        char topic[JSON_STRING_SIZE(strlen(FIRMWARE_REQUEST_TOPIC)) + detect_size(currChunk)]; // Size adjuts dynamically to the current length of the currChunk number to ensure we don't cut it out of the topic string.
        snprintf_P(topic, sizeof(topic), FIRMWARE_REQUEST_TOPIC, currChunk);
        snprintf_P(size, sizeof(size), NUMBER_PRINTF, chunkSize);
        m_client.publish(topic, size);

        const uint64_t timeout = millis() + 3000U; // Amount of time we wait until we declare the download as failed in milliseconds.
        do {
          delay(5);
          loop();
        } while ((m_fwChunkReceive != currChunk) && (timeout >= millis()));

        if (m_fwChunkReceive == currChunk) {
          // Check if the current chunk is not the last one.
          if (numberOfChunk != (currChunk + 1)) {
            // Check if state is still DOWNLOADING and did not fail.
            if (strncmp_P(FW_STATE_DOWNLOADING, m_fwState, strlen(FW_STATE_DOWNLOADING) == 0)) {
              currChunk++;
            }
            else {
              nbRetry--;
              if (nbRetry == 0) {
                Logger::log(UNABLE_TO_WRITE);
                break;
              }
            }
          }
          // The last chunk
          else {
            currChunk++;
          }
        }
        // Timeout
        else {
          nbRetry--;
          if (nbRetry == 0) {
            Logger::log(UNABLE_TO_DOWNLOAD);
            break;
          }
        }
      } while (numberOfChunk != currChunk);

      // Buffer size has been set to another value by the method return to the previous value.
      if (changeBufferSize) {
        m_client.setBufferSize(previousBufferSize);
      }

      // Unsubscribe from now not needed topics anymore.
      Firmware_OTA_Unsubscribe();

      bool success = false;
      // Update current_fw_title and current_fw_version if updating was a success.
      if (strncmp_P(SUCCESS, m_fwState, strlen(SUCCESS)) == 0) {
        Firmware_Send_FW_Info(fw_title, fw_version);
        Firmware_Send_State(SUCCESS);
        success = true;
      }
      else {
        Firmware_Send_State(FW_STATE_FAILED);
      }

      if (m_fwUpdatedCallback != nullptr) {
        m_fwUpdatedCallback(success);
      }
    }
#endif

    //----------------------------------------------------------------------------
    // Shared attributes API

    inline const bool Shared_Attributes_Request(const std::vector<const char*>& attributes, Shared_Attribute_Request_Callback& callback) {
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> requestBuffer;
      JsonObject requestObject = requestBuffer.to<JsonObject>();

      std::string sharedKeys;
      for (const char* attribute : attributes) {
        // Check if the given attribute is null, if it is skip it.
        if (attribute == nullptr) {
          continue;
        }
        sharedKeys.append(attribute);
        sharedKeys.push_back(COMMA);
      }

      // Check if any sharedKeys were requested.
      if (sharedKeys.empty()) {
        Logger::log(NO_KEYS_TO_REQUEST);
        return false;
      }

      // Remove latest not needed ,
      sharedKeys.pop_back();

      requestObject[SHARED_KEYS] = sharedKeys.c_str();
      int objectSize = measureJson(requestBuffer) + 1;
      char buffer[objectSize];
      serializeJson(requestObject, buffer, objectSize);

      // Print requested keys.
      char message[JSON_STRING_SIZE(strlen(REQUEST_ATT)) + sharedKeys.length() + JSON_STRING_SIZE(strlen(buffer))];
      snprintf_P(message, sizeof(message), REQUEST_ATT, sharedKeys.c_str(), buffer);
      Logger::log(message);

      m_requestId++;
      callback.m_request_id = m_requestId;
      Shared_Attributes_Request_Subscribe(callback);

      char topic[JSON_STRING_SIZE(strlen(ATTRIBUTE_REQUEST_TOPIC)) + detect_size(m_requestId)];
      snprintf_P(topic, sizeof(topic), ATTRIBUTE_REQUEST_TOPIC, m_requestId);
      return m_client.publish(topic, buffer);
    }

    // Subscribes multiple Shared attributes callbacks.
    inline const bool Shared_Attributes_Subscribe(const std::vector<Shared_Attribute_Callback>& callbacks) {
      if (m_sharedAttributeUpdateCallbacks.size() + callbacks.size() > m_sharedAttributeUpdateCallbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
      if (!m_client.subscribe(ATTRIBUTE_TOPIC)) {
        return false;
      }

      // Push back complete vector into our local m_sharedAttributeUpdateCallbacks vector.
      m_sharedAttributeUpdateCallbacks.insert(m_sharedAttributeUpdateCallbacks.end(), callbacks.begin(), callbacks.end());
      return true;
    }

    // Subscribe one Shared attributes callback.
    bool Shared_Attributes_Subscribe(const Shared_Attribute_Callback& callback) {
      if (m_sharedAttributeUpdateCallbacks.size() + 1U > m_sharedAttributeUpdateCallbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
      if (!m_client.subscribe(ATTRIBUTE_TOPIC)) {
        return false;
      }

      // Push back given callback into our local m_sharedAttributeUpdateCallbacks vector.
      m_sharedAttributeUpdateCallbacks.push_back(callback);
      return true;
    }

    inline const bool Shared_Attributes_Unsubscribe() {
      // Empty all callbacks.
      m_sharedAttributeUpdateCallbacks.clear();
      if (!m_client.unsubscribe(ATTRIBUTE_TOPIC)) {
        return false;
      }
      return true;
    }

    // -------------------------------------------------------------------------------
    // Provisioning API

    // Subscribes to get provision response

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
    inline const bool Provision_Subscribe(const Provision_Callback callback) {
      if (!m_client.subscribe(PROV_RESPONSE_TOPIC)) {
        return false;
      }
      m_provisionCallback = callback;
      return true;
    }

    inline const bool Provision_Unsubscribe() {
      if (!m_client.unsubscribe(PROV_RESPONSE_TOPIC)) {
        return false;
      }
      return true;
    }
#endif

  private:

    // Returns the length in character places of a given signed number.
    inline const uint8_t detect_size(const uint32_t number) {
      return snprintf_P(nullptr, 0U, NUMBER_PRINTF, number);
    }

    // Reserves size of callback vectors to improve performance
    inline void reserve_callback_size(void) {
      // Reserve size of both m_sharedAttributeUpdateCallbacks, rpcCallbacks and m_sharedAttributeRequestCallbacks beforehand for performance reasons.
      m_rpcCallbacks.reserve(MaxFieldsAmt);
      m_sharedAttributeUpdateCallbacks.reserve(MaxFieldsAmt);
      m_sharedAttributeRequestCallbacks.reserve(MaxFieldsAmt);
    }

    // Subscribe one Shared attributes request callback.
    inline const bool Shared_Attributes_Request_Subscribe(const Shared_Attribute_Request_Callback& callback) {
      if (m_sharedAttributeRequestCallbacks.size() + 1 > m_sharedAttributeRequestCallbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_REQUEST_EXCEEDED);
        return false;
      }
      if (!m_client.subscribe(ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC)) {
        return false;
      }

      // Push back given callback into our local m_sharedAttributeRequestCallbacks vector.
      m_sharedAttributeRequestCallbacks.push_back(callback);
      return true;
    }

    inline const bool Shared_Attributes_Request_Unsubscribe() {
      // Empty all callbacks.
      m_sharedAttributeRequestCallbacks.clear();
      if (!m_client.unsubscribe(ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC)) {
        return false;
      }
      return true;
    }

    // Sends single key-value in a generic way.
    template<typename T>
    inline const bool sendKeyval(const char *key, T value, bool telemetry = true) {
      Telemetry t(key, value);
      StaticJsonDocument<JSON_OBJECT_SIZE(1)>jsonBuffer;
      JsonVariant object = jsonBuffer.template to<JsonVariant>();
      if (!t.serializeKeyval(object)) {
        Logger::log(UNABLE_TO_SERIALIZE);
        return false;
      }

      return telemetry ? sendTelemetryJson(object) : sendAttributeJSON(object);
    }

    // Processes RPC message
    inline void process_rpc_message(char* topic, uint8_t* payload, uint32_t length) {
      RPC_Response r;
      {
        StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
        DeserializationError error = deserializeJson(jsonBuffer, payload, length);
        if (error) {
          Logger::log(UNABLE_TO_DE_SERIALIZE_RPC);
          return;
        }
        const JsonObject &data = jsonBuffer.template as<JsonObject>();
        const char *methodName = data[RPC_METHOD_KEY].as<const char*>();
        const char *params = data[RPC_PARAMS_KEY].as<const char*>();

        if (methodName) {
          Logger::log(RECEIVED_RPC_LOG_MESSAGE);
          Logger::log(methodName);
        } else {
          Logger::log(RPC_METHOD_NULL);
          return;
        }

        for (const RPC_Callback& callback : m_rpcCallbacks) {
          if (callback.m_cb == nullptr || callback.m_name == nullptr) {
            Logger::log(RPC_CB_NULL);
            continue;
          }
          // Strncmp returns the ascis value difference of the ascii characters that are different,
          // meaning 0 is the same string and less and more than 0 is the difference in ascci values between the 2 chararacters.
          else if (strncmp(callback.m_name, methodName, strlen(callback.m_name)) != 0) {
            continue;
          }

          Logger::log(CALLING_RPC);
          Logger::log(methodName);
          // Do not inform client, if parameter field is missing for some reason
          if (!data.containsKey(RPC_PARAMS_KEY)) {
            Logger::log(NO_RPC_PARAMS_PASSED);
          }

          // try to de-serialize params
          StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> doc;
          DeserializationError err_param = deserializeJson(doc, params);
          //if failed to de-serialize params then send JsonObject instead
          Logger::log(RPC_PARAMS_KEY);
          if (err_param) {
            Logger::log(data[RPC_PARAMS_KEY].as<const char*>());
            r = callback.m_cb(data[RPC_PARAMS_KEY]);
          } else {
            Logger::log(params);
            const JsonObject &param = doc.template as<JsonObject>();
            // Getting non-existing field from JSON should automatically
            // set JSONVariant to null
            r = callback.m_cb(param);
          }
          break;
        }
      }
      // Fill in response
      char responsePayload[PayloadSize] = {0};
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> respBuffer;
      JsonVariant resp_obj = respBuffer.template to<JsonVariant>();

      if (r.serializeKeyval(resp_obj) == false) {
        Logger::log(UNABLE_TO_SERIALIZE);
        return;
      }

      const uint32_t json_size = JSON_STRING_SIZE(measureJson(respBuffer));
      if (json_size > PayloadSize) {
        char message[JSON_STRING_SIZE(strlen(INVALID_BUFFER_SIZE)) + detect_size(PayloadSize) + detect_size(json_size)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, PayloadSize, json_size);
        Logger::log(message);
        return;
      }
      serializeJson(resp_obj, responsePayload, sizeof(responsePayload));

      String responseTopic = topic;
      responseTopic.replace(RPC_REQUEST_KEY, RPC_RESPONSE_KEY);
      Logger::log(RPC_RESPONSE_KEY);
      Logger::log(responseTopic.c_str());
      Logger::log(responsePayload);
      m_client.publish(responseTopic.c_str(), responsePayload);
    }

#if defined(ESP8266) || defined(ESP32)

    // Processes firmware response
    inline void process_firmware_response(char* topic, uint8_t* payload, uint32_t length) {
      static uint32_t sizeReceive = 0;
      static MD5Builder md5;

      m_fwChunkReceive = atoi(strrchr(topic, SLASH) + 1U);

      char message[JSON_STRING_SIZE(strlen(FW_CHUNK)) + detect_size(m_fwChunkReceive) + detect_size(length)];
      snprintf_P(message, sizeof(message), FW_CHUNK, m_fwChunkReceive, length);
      Logger::log(message);

      m_fwState = FW_STATE_FAILED;

      if (m_fwChunkReceive == 0) {
        sizeReceive = 0;
        md5 = MD5Builder();
        md5.begin();

        // Initialize Flash
        if (!Update.begin(m_fwSize)) {
          Logger::log(ERROR_UPDATE_BEGIN);
          m_fwState = FW_STATE_UPDATE_ERROR;
          return;
        }
      }

      // Write data to Flash
      if (Update.write(payload, length) != length) {
        Logger::log(ERROR_UPDATE_BEGIN);
        m_fwState = FW_STATE_UPDATE_ERROR;
        return;
      }

      // Update value only if write flash success
      md5.add(payload, length);
      sizeReceive += length;

      // Receive Full Firmware
      if (m_fwSize == sizeReceive) {
        md5.calculate();
        String md5Str = md5.toString();
        char actual[JSON_STRING_SIZE(strlen(MD5_ACTUAL)) + md5Str.length()];
        snprintf_P(actual, sizeof(actual), MD5_ACTUAL, md5Str.c_str());
        Logger::log(actual);
        char expected[JSON_STRING_SIZE(strlen(MD5_EXPECTED)) + JSON_STRING_SIZE(strlen(m_fwChecksum))];
        snprintf_P(expected, sizeof(expected), MD5_EXPECTED, m_fwChecksum);
        Logger::log(expected);
        // Check MD5
        if (strncmp(md5Str.c_str(), m_fwChecksum, md5Str.length()) != 0) {
          Logger::log(CHKS_VER_FAILED);
#if defined(ESP32)
          Update.abort();
#endif
          m_fwState = FW_STATE_CHKS_ERROR;
        }
        else {
          Logger::log(CHKS_VER_SUCCESS);
          if (Update.end(true)) {
            Logger::log(FW_UPDATE_SUCCESS);
            m_fwState = SUCCESS;
          }
        }
      }
    }
#endif

    // Processes shared attribute update message
    inline void process_shared_attribute_update_message(char* topic, uint8_t* payload, uint32_t length) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        Logger::log(UNABLE_TO_DE_SERIALIZE_ATT_UPDATE);
        return;
      }
      JsonObject data = jsonBuffer.template as<JsonObject>();

      if (data && (data.size() >= 1)) {
        Logger::log(RECEIVED_ATT_UPDATE);
        if (data.containsKey(SHARED_KEY)) {
          data = data[SHARED_KEY];
        }
      }
      else {
        Logger::log(NOT_FOUND_ATT_UPDATE);
        return;
      }

      for (size_t i = 0; i < m_sharedAttributeUpdateCallbacks.size(); i++) {
        char id_message[JSON_STRING_SIZE(strlen(ATT_CB_ID)) + detect_size(i)];
        snprintf_P(id_message, sizeof(id_message), ATT_CB_ID, i);
        Logger::log(id_message);

        if (m_sharedAttributeUpdateCallbacks.at(i).m_cb == nullptr) {
          Logger::log(ATT_CB_IS_NULL);
          continue;
        }
        else if (m_sharedAttributeUpdateCallbacks.at(i).m_att.empty()) {
          Logger::log(ATT_CB_NO_KEYS);
          // No specifc keys were subscribed so we call the callback anyway.
          m_sharedAttributeUpdateCallbacks.at(i).m_cb(data);
          continue;
        }

        bool containsKey = false;
        const char* requested_att;
        for (const char* att : m_sharedAttributeUpdateCallbacks.at(i).m_att) {
          if (att == nullptr) {
            Logger::log(ATT_IS_NULL);
            continue;
          }
          // Check if the request contained any of our requested keys.
          containsKey = containsKey || data.containsKey(att);
          // Break early if the key was requested from this callback.
          if (containsKey) {
            char contained_message[JSON_STRING_SIZE(strlen(ATT_IN_ARRAY)) + JSON_STRING_SIZE(strlen(att))];
            snprintf_P(contained_message, sizeof(contained_message), ATT_IN_ARRAY, att);
            Logger::log(contained_message);
            requested_att = att;
            break;
          }
        }

        // This callback did not request any keys that were in this response,
        // therefore we continue with the next element in the loop.
        if (!containsKey || requested_att == nullptr) {
          Logger::log(ATT_NO_CHANGE);
          continue;
        }

        char calling_message[JSON_STRING_SIZE(strlen(CALLING_ATT_CB)) + JSON_STRING_SIZE(strlen(requested_att))];
        snprintf_P(calling_message, sizeof(calling_message), CALLING_ATT_CB, requested_att);
        Logger::log(calling_message);
        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        m_sharedAttributeUpdateCallbacks.at(i).m_cb(data);
      }
    }

    // Processes shared attribute request message
    inline void process_shared_attribute_request_message(char* topic, uint8_t* payload, uint32_t length) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        Logger::log(UNABLE_TO_DE_SERIALIZE_ATT_REQUEST);
        return;
      }
      JsonObject data = jsonBuffer.template as<JsonObject>();

      if (data && (data.size() >= 1)) {
        Logger::log(RECEIVED_ATT);
        if (data.containsKey(SHARED_KEY)) {
          data = data[SHARED_KEY];
        }
      } else {
        Logger::log(ATT_KEY_NOT_FOUND);
        return;
      }

      std::string response = topic;
      // Remove the not needed part of the topic.
      size_t index = strlen(ATTRIBUTE_RESPONSE_TOPIC) + 1U;
      response = response.substr(index, response.length() - index);
      // convert the remaining text to an integer
      uint32_t response_id = atoi(response.c_str());

      for (size_t i = 0; i < m_sharedAttributeRequestCallbacks.size(); i++) {
        if (m_sharedAttributeRequestCallbacks.at(i).m_cb == nullptr) {
          Logger::log(ATT_REQUEST_CB_IS_NULL);
          continue;
        }
        else if (m_sharedAttributeRequestCallbacks.at(i).m_request_id != response_id) {
          continue;
        }

        char message[JSON_STRING_SIZE(strlen(CALLING_REQUEST_ATT_CB)) + detect_size(response_id)];
        snprintf_P(message, sizeof(message), CALLING_REQUEST_ATT_CB, response_id);
        Logger::log(message);
        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        m_sharedAttributeRequestCallbacks.at(i).m_cb(data);
        // Delete callback because the changes have been requested and the callback is no longer needed.
        m_sharedAttributeRequestCallbacks.erase(std::next(m_sharedAttributeRequestCallbacks.begin(), i));
      }
    }

    // Processes provisioning response
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
    inline void process_provisioning_response(char* topic, uint8_t* payload, uint32_t length) {
      Logger::log(PROV_RESPONSE);

      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        Logger::log(UNABLE_TO_DE_SERIALIZE_PROV_RESPONSE);
        return;
      }

      const JsonObject &data = jsonBuffer.template as<JsonObject>();

      Logger::log(RECEIVED_PROV_RESPONSE);

      if (strncmp_P(SUCCESS, data[PROV_STATUS_KEY], strlen(SUCCESS)) == 0 && strncmp_P(PROV_CRED_TYPE_VALUE, data[PROV_CRED_TYPE_KEY], strlen(PROV_CRED_TYPE_VALUE)) == 0) {
        Logger::log(X509_NOT_SUPPORTED);
        return;
      }

      if (m_provisionCallback.m_cb) {
        m_provisionCallback.m_cb(data);
      }
    }
#endif

    // Sends array of attributes or telemetry to ThingsBoard
    inline const bool sendDataArray(const Telemetry *data, size_t data_count, bool telemetry = true) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      JsonVariant object = jsonBuffer.template to<JsonVariant>();

      for (size_t i = 0; i < data_count; ++i) {
        if (data[i].serializeKeyval(object) == false) {
          Logger::log(UNABLE_TO_SERIALIZE);
          return false;
        }
      }

      return telemetry ? sendTelemetryJson(object) : sendAttributeJSON(object);
    }

    PubSubClient m_client; // PubSub MQTT client instance.
    std::vector<RPC_Callback> m_rpcCallbacks; // RPC callbacks array
    std::vector<Shared_Attribute_Callback> m_sharedAttributeUpdateCallbacks; // Shared attribute update callbacks array
    std::vector<Shared_Attribute_Request_Callback> m_sharedAttributeRequestCallbacks; // Shared attribute request callbacks array

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
    Provision_Callback m_provisionCallback; // Provision response callback
#endif
    uint32_t m_requestId; // Allows nearly 4.3 million requests before wrapping back to 0.

#if defined(ESP8266) || defined(ESP32)
    // For Firmware Update
    const char* m_currFwTitle;
    const char* m_currFwVersion;
    const char* m_fwState;
    uint16_t m_fwSize;
    const char* m_fwChecksum;
    const std::function<void(const bool&)> m_fwUpdatedCallback;
    uint16_t m_fwChunkReceive;
#endif

    // The callback for when a PUBLISH message is received from the server.
    inline void on_message(char* topic, uint8_t* payload, uint32_t length) {
      char message[JSON_STRING_SIZE(strlen(CB_ON_MESSAGE)) + JSON_STRING_SIZE(strlen(topic))];
      snprintf_P(message, sizeof(message), CB_ON_MESSAGE, topic);
      Logger::log(message);

      if (strncmp_P(RPC_TOPIC, topic, strlen(RPC_TOPIC)) == 0) {
        process_rpc_message(topic, payload, length);
      } else if (strncmp_P(ATTRIBUTE_RESPONSE_TOPIC, topic, strlen(ATTRIBUTE_RESPONSE_TOPIC)) == 0) {
        process_shared_attribute_request_message(topic, payload, length);
      } else if (strncmp_P(ATTRIBUTE_TOPIC, topic, strlen(ATTRIBUTE_TOPIC)) == 0) {
        process_shared_attribute_update_message(topic, payload, length);
      } else {
        if (strncmp_P(PROV_RESPONSE_TOPIC, topic, strlen(PROV_RESPONSE_TOPIC)) == 0) {
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
          process_provisioning_response(topic, payload, length);
#endif
        } else if (strncmp_P(FIRMWARE_RESPONSE_TOPIC, topic, strlen(FIRMWARE_RESPONSE_TOPIC)) == 0) {
#if defined(ESP8266) || defined(ESP32)
          process_firmware_response(topic, payload, length);
#endif
        }
      }
    }
};

#if !defined(ESP8266) && !defined(ESP32)

// ThingsBoard HTTP client class
template<size_t PayloadSize = Default_Payload,
         size_t MaxFieldsAmt = Default_Fields_Amt,
         typename Logger = ThingsBoardDefaultLogger>
class ThingsBoardHttpSized
{
  public:
    // Initializes ThingsBoardHttpSized class with network client.
    inline ThingsBoardHttpSized(Client &client, const char *access_token,
                                const char *host, int port = 80)
      : m_client(client, host, port)
      , m_host(host)
      , m_token(access_token)
      , m_port(port)
    {
      // Nothing to do.
    }

    // Destroys ThingsBoardHttpSized class with network client.
    inline ~ThingsBoardHttpSized() {
      // Nothing to do.
    }

    //----------------------------------------------------------------------------
    // Telemetry API

    // Sends integer telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryInt(const char *key, int value) {
      return sendKeyval(key, value);
    }

    // Sends boolean telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryBool(const char *key, bool value) {
      return sendKeyval(key, value);
    }

    // Sends float telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryFloat(const char *key, float value) {
      return sendKeyval(key, value);
    }

    // Sends string telemetry data to the ThingsBoard, returns true on success.
    inline const bool sendTelemetryString(const char *key, const char *value) {
      return sendKeyval(key, value);
    }

    // Sends aggregated telemetry to the ThingsBoard.
    inline const bool sendTelemetry(const Telemetry *data, size_t data_count) {
      return sendDataArray(data, data_count);
    }

    // Sends custom JSON telemetry string to the ThingsBoard, using HTTP.
    inline const bool sendTelemetryJson(const char *json) {
      if (!json || !m_token) {
        return  false;
      }

      if (!m_client.connected()) {
        if (!m_client.connect(m_host, m_port)) {
          Logger::log(CONNECT_FAILED);
          return false;
        }
      }

      bool rc = true;

      char path[JSON_STRING_SIZE(strlen(HTTP_TELEMETRY_TOPIC)) + JSON_STRING_SIZE(strlen(m_token))];
      snprintf_P(path, sizeof(path), HTTP_TELEMETRY_TOPIC, m_token);
      if (!m_client.post(path, HTTP_POST_PATH, json) ||
          (m_client.responseStatusCode() != HTTP_SUCCESS)) {
        rc = false;
      }

      m_client.stop();
      return rc;
    }

    //----------------------------------------------------------------------------
    // Attribute API

    // Sends integer attribute with given name and value.
    inline const bool sendAttributeInt(const char *attrName, int value) {
      return sendKeyval(attrName, value, false);
    }

    // Sends boolean attribute with given name and value.
    inline const bool sendAttributeBool(const char *attrName, bool value) {
      return sendKeyval(attrName, value, false);
    }

    // Sends float attribute with given name and value.
    inline const bool sendAttributeFloat(const char *attrName, float value) {
      return sendKeyval(attrName, value, false);
    }

    // Sends string attribute with given name and value.
    inline const bool sendAttributeString(const char *attrName, const char *value) {
      return sendKeyval(attrName, value, false);
    }

    // Sends aggregated attributes to the ThingsBoard.
    inline const bool sendAttributes(const Attribute *data, size_t data_count) {
      return sendDataArray(data, data_count, false);
    }

    // Sends custom JSON with attributes to the ThingsBoard, using HTTP.
    inline const bool sendAttributeJSON(const char *json) {
      if (!json || !m_token) {
        return  false;
      }

      if (!m_client.connected()) {
        if (!m_client.connect(m_host, m_port)) {
          Logger::log(CONNECT_FAILED);
          return false;
        }
      }

      bool rc = true;

      char path[JSON_STRING_SIZE(strlen(HTTP_ATTRIBUTES_TOPIC)) + JSON_STRING_SIZE(strlen(m_token))];
      snprintf_P(path, sizeof(path), HTTP_ATTRIBUTES_TOPIC, m_token);
      if (!m_client.post(path, HTTP_POST_PATH, json)
          || (m_client.responseStatusCode() != HTTP_SUCCESS)) {
        rc = false;
      }

      m_client.stop();
      return rc;
    }

  private:
    // Sends array of attributes or telemetry to ThingsBoard
    inline const bool sendDataArray(const Telemetry *data, size_t data_count, bool telemetry = true) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      JsonVariant object = jsonBuffer.template to<JsonVariant>();

      for (size_t i = 0; i < data_count; ++i) {
        if (data[i].serializeKeyval(object) == false) {
          Logger::log(UNABLE_TO_SERIALIZE);
          return false;
        }
      }

      return telemetry ? sendTelemetryJson(object) : sendAttributeJSON(object);
    }

    // Sends single key-value in a generic way.
    template<typename T>
    inline const bool sendKeyval(const char *key, T value, bool telemetry = true) {
      Telemetry t(key, value);

      StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonBuffer;
      JsonVariant object = jsonBuffer.template to<JsonVariant>();
      if (t.serializeKeyval(object) == false) {
        Logger::log(UNABLE_TO_SERIALIZE);
        return false;
      }

      return telemetry ? sendTelemetryJson(object) : sendAttributeJSON(object);
    }

    HttpClient m_client;
    const char *m_host;
    int m_port;
    const char *m_token;
};

using ThingsBoardHttp = ThingsBoardHttpSized<>;

#endif

using ThingsBoard = ThingsBoardSized<>;

#endif // ThingsBoard_h
