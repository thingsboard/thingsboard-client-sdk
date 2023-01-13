/*
  ThingsBoard.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef ThingsBoard_h
#define ThingsBoard_h

// Library includes.
#include <PubSubClient.h>

#if defined ESP8266
#include <Updater.h>
#elif defined ESP32
#include <Update.h>
#endif

// Local includes.
#include "ThingsBoardDefaultLogger.h"
#include "HashGenerator.h"
#include "Telemetry.h"
#include "Constants.h"
#include "Shared_Attribute_Callback.h"
#include "Shared_Attribute_Request_Callback.h"
#include "RPC_Callback.h"
#include "RPC_Request_Callback.h"
#include "Provision_Callback.h"
#include "OTA_Update_Callback.h"

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
// Publish data topics.
constexpr char ATTRIBUTE_TOPIC[] PROGMEM = "v1/devices/me/attributes";
constexpr char TELEMETRY_TOPIC[] PROGMEM = "v1/devices/me/telemetry";

// RPC topics.
constexpr char RPC_SUBSCRIBE_TOPIC[] PROGMEM = "v1/devices/me/rpc/request/+";
constexpr char RPC_TOPIC[] PROGMEM = "v1/devices/me/rpc";
constexpr char RPC_RESPONSE_SUBSCRIBE_TOPIC[] PROGMEM = "v1/devices/me/rpc/response/+";
constexpr char RPC_REQUEST_TOPIC[] PROGMEM = "v1/devices/me/rpc/request/%u";
constexpr char RPC_RESPONSE_TOPIC[] PROGMEM = "v1/devices/me/rpc/response";

// Firmware topics.
constexpr char FIRMWARE_RESPONSE_TOPIC[] PROGMEM = "v2/fw/response";

// Shared attribute topics.
constexpr char ATTRIBUTE_REQUEST_TOPIC[] PROGMEM = "v1/devices/me/attributes/request/%u";
constexpr char ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC[] PROGMEM = "v1/devices/me/attributes/response/+";
constexpr char ATTRIBUTE_RESPONSE_TOPIC[] PROGMEM = "v1/devices/me/attributes/response";

// Provision topics.
constexpr char PROV_RESPONSE_TOPIC[] PROGMEM = "/provision/response";

// Default login data.
constexpr char PROV_ACCESS_TOKEN[] PROGMEM = "provision";
constexpr char DEFAULT_CLIENT_ID[] PROGMEM = "TbDev";

// Shared attribute request keys.
constexpr char SHARED_KEYS[] PROGMEM = "sharedKeys";
constexpr char SHARED_KEY[] PROGMEM = "shared";

// RPC data keys.
constexpr char RPC_REQUEST_KEY[] PROGMEM = "request";
constexpr char RPC_RESPONSE_KEY[] PROGMEM = "response";
constexpr char RPC_METHOD_KEY[] PROGMEM = "method";
constexpr char RPC_PARAMS_KEY[] PROGMEM = "params";
constexpr char RPC_EMPTY_PARAMS_VALUE[] PROGMEM = "{}";

// Log messages.
constexpr char MAX_RPC_EXCEEDED[] PROGMEM = "Too many server-side RPC subscriptions, increase MaxFieldsAmt or unsubscribe";
constexpr char MAX_RPC_REQUEST_EXCEEDED[] PROGMEM = "Too many client-side RPC subscriptions, increase MaxFieldsAmt or unsubscribe";
constexpr char MAX_SHARED_ATT_UPDATE_EXCEEDED[] PROGMEM = "Too many shared attribute update callback subscriptions, increase MaxFieldsAmt or unsubscribe";
constexpr char MAX_SHARED_ATT_REQUEST_EXCEEDED[] PROGMEM = "Too many shared attribute request callback subscriptions, increase MaxFieldsAmt";
constexpr char NUMBER_PRINTF[] PROGMEM = "%u";
constexpr char COMMA PROGMEM = ',';
constexpr char NO_KEYS_TO_REQUEST[] PROGMEM = "No keys to request were given";
constexpr char REQUEST_RPC[] PROGMEM = "Requesting client side RPC with the json (%s)";
constexpr char REQUEST_ATT[] PROGMEM = "Requesting shared attributes transformed from (%s) into json (%s)";
constexpr char RECEIVED_RPC_LOG_MESSAGE[] PROGMEM = "Received RPC:";
constexpr char RPC_METHOD_NULL[] PROGMEM = "RPC methodName is NULL";
constexpr char NO_RPC_PARAMS_PASSED[] PROGMEM = "No parameters passed with RPC, passing null JSON";
constexpr char CALLING_RPC[] PROGMEM = "Calling RPC:";
constexpr char RECEIVED_ATT_UPDATE[] PROGMEM = "Received shared attribute update";
constexpr char NOT_FOUND_ATT_UPDATE[] PROGMEM = "Shared attribute update key not found";
constexpr char ATT_CB_ID[] PROGMEM = "Shared attribute update callback id: (%u)";
constexpr char ATT_CB_NO_KEYS[] PROGMEM = "No keys subscribed. Calling subscribed callback for any updated attributes (assumed to be subscribed to every possible key)";
constexpr char ATT_IS_NULL[] PROGMEM = "Subscribed shared attribute update key is NULL";
constexpr char ATT_IN_ARRAY[] PROGMEM = "Shared attribute update key: (%s) is subscribed";
constexpr char ATT_NO_CHANGE[] PROGMEM = "No keys that we subscribed too were changed, skipping callback";
constexpr char CALLING_ATT_CB[] PROGMEM = "Calling subscribed callback for updated shared attribute (%s)";
constexpr char RECEIVED_ATT[] PROGMEM = "Received shared attribute request";
constexpr char ATT_KEY_NOT_FOUND[] PROGMEM = "Shared attribute key not found";
constexpr char CALLING_REQUEST_CB[] PROGMEM = "Calling subscribed callback for response id (%u)";
constexpr char CB_ON_MESSAGE[] PROGMEM = "Callback onMQTTMessage from topic: (%s)";
constexpr char SUBSCRIBE_TOPIC_FAILED[] PROGMEM = "Subscribing the given topic failed";
constexpr char UNABLE_TO_SERIALIZE_JSON[] PROGMEM = "Unable to serialize json data";

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
// Claim topics.
constexpr char CLAIM_TOPIC[] PROGMEM = "v1/devices/me/claim";

// Provision topics.
constexpr char PROV_REQUEST_TOPIC[] PROGMEM = "/provision/request";

// Claim data keys.
constexpr char SECRET_KEY[] PROGMEM = "secretKey";
constexpr char DURATION_KEY[] PROGMEM = "durationMs";
constexpr char DEVICE_NAME_KEY[] PROGMEM = "deviceName";
constexpr char PROV_DEVICE_KEY[] PROGMEM = "provisionDeviceKey";
constexpr char PROV_DEVICE_SECRET_KEY[] PROGMEM = "provisionDeviceSecret";

// Provision data keys.
constexpr char PROV_STATUS_KEY[] PROGMEM = "status";
constexpr char PROV_CRED_TYPE_KEY[] PROGMEM = "credentialsType";
constexpr char STATUS_SUCCESS[] PROGMEM = "SUCCESS";
constexpr char PROV_CRED_TYPE_VALUE[] PROGMEM = "X509_CERTIFICATE";

// Log messages.
constexpr char PROV_REQUEST[] PROGMEM = "Provision request:";
constexpr char UNABLE_TO_DE_SERIALIZE_PROV_RESPONSE[] PROGMEM = "Unable to de-serialize provision response";
constexpr char PROV_RESPONSE[] PROGMEM = "Process provisioning response";
constexpr char RECEIVED_PROV_RESPONSE[] PROGMEM = "Received provision response";
constexpr char X509_NOT_SUPPORTED[] PROGMEM = "Provision response contains X509_CERTIFICATE credentials, this is not supported yet";

#if !defined(ARDUINO_AVR_MEGA)
// Firmware topics.
constexpr char FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC[] PROGMEM = "v2/fw/response/#";
constexpr char FIRMWARE_REQUEST_TOPIC[] PROGMEM = "v2/fw/request/0/chunk/%u";

// Firmware data keys.
constexpr char CURR_FW_TITLE_KEY[] PROGMEM = "current_fw_title";
constexpr char CURR_FW_VER_KEY[] PROGMEM = "current_fw_version";
constexpr char CURR_FW_STATE_KEY[] PROGMEM = "current_fw_state";
constexpr char FW_VER_KEY[] PROGMEM = "fw_version";
constexpr char FW_TITLE_KEY[] PROGMEM = "fw_title";
constexpr char FW_CHKS_KEY[] PROGMEM = "fw_checksum";
constexpr char FW_CHKS_ALGO_KEY[] PROGMEM = "fw_checksum_algorithm";
constexpr char FW_SIZE_KEY[] PROGMEM = "fw_size";
constexpr char FW_STATE_CHECKING[] PROGMEM = "CHECKING FIRMWARE";
constexpr char FW_STATE_NO_FW[] PROGMEM = "NO FIRMWARE FOUND";
constexpr char FW_STATE_UP_TO_DATE[] PROGMEM = "UP TO DATE";
constexpr char FW_STATE_INVALID_CHKS[] PROGMEM = "CHECKSUM ALGORITHM INVALID";
constexpr char FW_STATE_DOWNLOADING[] PROGMEM = "DOWNLOADING";
constexpr char FW_STATE_FAILED[] PROGMEM = "FAILED";
constexpr char FW_STATE_UPDATE_ERROR[] PROGMEM = "UPDATE ERROR";
constexpr char FW_STATE_CHKS_ERROR[] PROGMEM = "CHECKSUM ERROR";
constexpr char CHECKSUM_AGORITM_MD5[] PROGMEM = "MD5";
constexpr char CHECKSUM_AGORITM_SHA256[] PROGMEM = "SHA256";
constexpr char CHECKSUM_AGORITM_SHA384[] PROGMEM = "SHA384";
constexpr char CHECKSUM_AGORITM_SHA512[] PROGMEM = "SHA512";

// Log messages.
constexpr char NO_FW[] PROGMEM = "No new firmware assigned on the given device";
constexpr char EMPTY_FW[] PROGMEM = "Given firmware was NULL";
constexpr char FW_UP_TO_DATE[] PROGMEM = "Firmware is already up to date";
constexpr char FW_NOT_FOR_US[] PROGMEM = "Firmware is not for us (title is different)";
constexpr char FW_CHKS_ALGO_NOT_SUPPORTED[] PROGMEM = "Checksum algorithm (%s) is not supported";
constexpr char PAGE_BREAK[] PROGMEM = "=================================";
constexpr char NEW_FW[] PROGMEM = "A new Firmware is available:";
constexpr char FROM_TOO[] PROGMEM = "(%s) => (%s)";
constexpr char DOWNLOADING_FW[] PROGMEM = "Attempting to download over MQTT...";
constexpr char NOT_ENOUGH_RAM[] PROGMEM = "Not enough RAM";
constexpr char SLASH PROGMEM = '/';
constexpr char UNABLE_TO_WRITE[] PROGMEM = "Unable to write firmware";
constexpr char UNABLE_TO_DOWNLOAD[] PROGMEM = "Unable to download firmware";
constexpr char FW_CHUNK[] PROGMEM = "Receive chunk (%i), with size (%u) bytes";
constexpr char ERROR_UPDATE_BEGIN[] PROGMEM = "Error during Update.begin";
constexpr char ERROR_UPDATE_WRITE[] PROGMEM = "Error during Update.write";
constexpr char ERROR_UPDATE_END[] PROGMEM = "Error during Update.end, not all bytes written";
constexpr char HASH_ACTUAL[] PROGMEM = "(%s) actual checksum: (%s)";
constexpr char HASH_EXPECTED[] PROGMEM = "(%s) expected checksum: (%s)";
constexpr char CHKS_VER_FAILED[] PROGMEM = "Checksum verification failed";
constexpr char CHKS_VER_SUCCESS[] PROGMEM = "Checksum is the same as expected";
constexpr char FW_UPDATE_SUCCESS[] PROGMEM = "Update success";
constexpr char RESETTING_FAILED[] PROGMEM = "Preparing for OTA firmware updates failed, attributes might be NULL";
#endif
#endif

// ThingsBoardSized client class
template<size_t PayloadSize = Default_Payload,
         size_t MaxFieldsAmt = Default_Fields_Amt,
         typename Logger = ThingsBoardDefaultLogger>
class ThingsBoardSized {
  public:
    // Initializes ThingsBoardSized class with network client.
    // Certain private members can not be set in the constructor initalizor list,
    // because using 2 instances of the ThingsBoard class (for example. provision and connected client)
    // will result in the variables being reset between method calls. Resulting in unexpected behaviour.
    inline ThingsBoardSized(Client& client, const bool& enableQoS = false)
      : m_client()
      , m_rpcCallbacks()
      , m_rpcRequestCallbacks()
      , m_sharedAttributeUpdateCallbacks()
      , m_sharedAttributeRequestCallbacks()
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
      , m_provisionCallback(nullptr)
#endif
      , m_requestId(0U)
      , m_qos(enableQoS)
#if defined(ESP8266) || defined(ESP32)
      , m_fwState(nullptr)
      , m_fwCallback(nullptr)
      , m_fwSize(0U)
      , m_fwChecksumAlgorithm()
      , m_fwAlgorithm()
      , m_fwChecksum()
      , m_fwSharedKeys{FW_CHKS_KEY, FW_CHKS_ALGO_KEY, FW_SIZE_KEY, FW_TITLE_KEY, FW_VER_KEY}
      , m_fwRequestCallback(m_fwSharedKeys.cbegin(), m_fwSharedKeys.cend(), std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1))
      , m_fwUpdateCallback(m_fwSharedKeys.cbegin(), m_fwSharedKeys.cend(), std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1))
      , m_fwChunkReceive(0U)
#endif
    {
      reserve_callback_size();
      setClient(client);
    }

    // Initializes ThingsBoardSized class without network client. Ensure it is set via. setClient() before using the connect method.
    inline ThingsBoardSized()
      : m_client()
      , m_rpcCallbacks()
      , m_rpcRequestCallbacks()
      , m_sharedAttributeUpdateCallbacks()
      , m_sharedAttributeRequestCallbacks()
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
      , m_provisionCallback(nullptr)
#endif
      , m_requestId(0U)
      , m_qos(false)
#if defined(ESP8266) || defined(ESP32)
      , m_fwState(nullptr)
      , m_fwCallback(nullptr)
      , m_fwSize(0U)
      , m_fwChecksumAlgorithm()
      , m_fwAlgorithm()
      , m_fwChecksum()
      , m_fwSharedKeys{FW_CHKS_KEY, FW_CHKS_ALGO_KEY, FW_SIZE_KEY, FW_TITLE_KEY, FW_VER_KEY}
      , m_fwRequestCallback(m_fwSharedKeys.cbegin(), m_fwSharedKeys.cend(), std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1))
      , m_fwUpdateCallback(m_fwSharedKeys.cbegin(), m_fwSharedKeys.cend(), std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1))
      , m_fwChunkReceive(0U)
#endif
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

    // Enables mqtt quality of service for the given underlying MQTT connection.
    // See https://assetwolf.com/learn/mqtt-qos-understanding-quality-of-service for more information, on quality of service.
    inline void enableMQTTQoS(const bool& enableQoS) {
      m_qos = enableQoS;
    }

    // Sets the underlying Client of the PubSubClient.
    inline void setClient(Client& client) {
      m_client.setClient(client);
      m_client.setBufferSize(PayloadSize);
      // Initalize callback.
      m_client.setCallback(std::bind(&ThingsBoardSized::onMQTTMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    // Connects to the specified ThingsBoard server and port.
    // Access token is used to authenticate a client.
    // Returns true on success, false otherwise.
    inline const bool connect(const char *host, const char *access_token = PROV_ACCESS_TOKEN, const uint16_t port = 1883, const char *client_id = DEFAULT_CLIENT_ID, const char *password = nullptr) {
      if (!host) {
        return false;
      }
      m_client.setServer(host, port);
      return connect_to_host(access_token, client_id, password);
    }

    inline const bool connect(const IPAddress& host, const char *access_token = PROV_ACCESS_TOKEN, const uint16_t port = 1883, const char *client_id = DEFAULT_CLIENT_ID, const char *password = nullptr) {
      m_client.setServer(host, port);
      return connect_to_host(access_token, client_id, password);
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

    inline const bool sendClaimingRequest(const char *secretKey, const uint32_t& durationMs) {
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> requestBuffer;
      const JsonObject respObj = requestBuffer.to<JsonObject>();

      // Make the secret key optional,
      // meaning if it is an empty string or null instead we don't send it at all.
      if (secretKey != nullptr && secretKey[0] != '\0') {
        respObj[SECRET_KEY] = secretKey;
      }
      respObj[DURATION_KEY] = durationMs;

      const size_t objectSize = JSON_STRING_SIZE(measureJson(respObj));
      char responsePayload[objectSize];
      // Serialize json does not include size of the string null terminator
      if (serializeJson(respObj, responsePayload, objectSize) < objectSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }

      return m_client.publish(CLAIM_TOPIC, responsePayload, m_qos);
    }

    // Provisioning API
    inline const bool sendProvisionRequest(const char *deviceName, const char *provisionDeviceKey, const char *provisionDeviceSecret) {
      // TODO Add ability to provide specific credentials from client side.
      StaticJsonDocument<JSON_OBJECT_SIZE(3)> requestBuffer;
      const JsonObject requestObject = requestBuffer.to<JsonObject>();

      // Make the device name optional,
      // meaning if it is an empty string or null instead we don't send it at all.
      if (deviceName != nullptr && deviceName[0] != '\0') {
        requestObject[DEVICE_NAME_KEY] = deviceName;
      }
      requestObject[PROV_DEVICE_KEY] = provisionDeviceKey;
      requestObject[PROV_DEVICE_SECRET_KEY] = provisionDeviceSecret;

      const size_t objectSize = JSON_STRING_SIZE(measureJson(requestObject));
      char requestPayload[objectSize];
      // Serialize json does not include size of the string null terminator
      if (serializeJson(requestObject, requestPayload, objectSize) < objectSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }

      Logger::log(PROV_REQUEST);
      Logger::log(requestPayload);
      return m_client.publish(PROV_REQUEST_TOPIC, requestPayload, m_qos);
    }

#endif
    //----------------------------------------------------------------------------
    // Telemetry API

    // Sends telemetry data to the ThingsBoard, returns true on success.
    template<class T>
    inline const bool sendTelemetryData(const char *key, T value)
    {
      return sendKeyValue(key, value);
    }

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

    // Sends custom JSON telemetry string to the ThingsBoard.
    inline const bool sendTelemetryJson(const char *json) {
      if (json == nullptr) {
        return false;
      }

      const uint32_t json_size = JSON_STRING_SIZE(strlen(json));
      if (PayloadSize < json_size) {
        char message[detectSize(INVALID_BUFFER_SIZE, PayloadSize, json_size)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, PayloadSize, json_size);
        Logger::log(message);
        return false;
      }
      return m_client.publish(TELEMETRY_TOPIC, json, m_qos);
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
      // Serialize json does not include size of the string null terminator
      if (serializeJson(jsonObject, json, jsonSize) < jsonSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }
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
      // Serialize json does not include size of the string null terminator
      if (serializeJson(jsonVariant, json, jsonSize) < jsonSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }
      return sendTelemetryJson(json);
    }

    //----------------------------------------------------------------------------
    // Attribute API

    // Sends an attribute with given name and value.
    template<class T>
    inline const bool sendAttributeData(const char *attrName, T value) {
      return sendKeyValue(attrName, value, false);
    }

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

    // Sends custom JSON with attributes to the ThingsBoard.
    inline const bool sendAttributeJSON(const char *json) {
      if (json == nullptr) {
        return false;
      }

      const uint32_t json_size = JSON_STRING_SIZE(strlen(json));
      if (PayloadSize < json_size) {
        char message[detectSize(INVALID_BUFFER_SIZE, PayloadSize, json_size)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, PayloadSize, json_size);
        Logger::log(message);
        return false;
      }
      return m_client.publish(ATTRIBUTE_TOPIC, json, m_qos);
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
      // Serialize json does not include size of the string null terminator
      if (serializeJson(jsonObject, json, jsonSize) < jsonSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }
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
      // Serialize json does not include size of the string null terminator
      if (serializeJson(jsonVariant, json, jsonSize) < jsonSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }
      return sendAttributeJSON(json);
    }

    //----------------------------------------------------------------------------
    // Server-side RPC API

    // Subscribes multiple server-side RPC callbacks.
    template<class InputIterator>
    inline const bool RPC_Subscribe(const InputIterator& first_itr, const InputIterator& last_itr) {
      const uint32_t size = std::distance(first_itr, last_itr);
      if (m_rpcCallbacks.size() + size > m_rpcCallbacks.capacity()) {
        Logger::log(MAX_RPC_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back complete vector into our local m_rpcCallbacks vector.
      m_rpcCallbacks.insert(m_rpcCallbacks.end(), first_itr, last_itr);
      return true;
    }

    // Subscribe one server-side RPC callback.
    inline const bool RPC_Subscribe(const RPC_Callback& callback) {
      if (m_rpcCallbacks.size() + 1 > m_rpcCallbacks.capacity()) {
        Logger::log(MAX_RPC_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local m_rpcCallbacks vector.
      m_rpcCallbacks.push_back(callback);
      return true;
    }

    // Unsubcrbes all server-side RPC callbacks.
    inline const bool RPC_Unsubscribe() {
      // Empty all callbacks.
      m_rpcCallbacks.clear();
      return m_client.unsubscribe(RPC_SUBSCRIBE_TOPIC);
    }

    //----------------------------------------------------------------------------
    // Client-side RPC API

    // Requests one client-side RPC callback.
    inline const bool RPC_Request(const RPC_Request_Callback& callback) {
      // Ensure to have enough size for the infinite amount of possible parameters that could be sent to the cloud,
      // therefore we set the size to the MaxFieldsAmt instead of JSON_OBJECT_SIZE(1), which will result in a JsonDocument with a size of 16 bytes
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> requestBuffer;
      // The .template variant of createing the JsonVariant has to be used,
      // because we are passing a template to the StaticJsonDocument template list
      // and it will generate a compile time error if not used
      const JsonVariant requestVariant = requestBuffer.template as<JsonVariant>();
      const char *methodName = callback.Get_Name();
      const JsonArray* parameters = callback.Get_Parameters();

      if (methodName == nullptr) {
        Logger::log(RPC_METHOD_NULL);
        return false;
      }
      RPC_Request_Callback* registeredCallback = nullptr;
      // Ensure the response topic has been subscribed
      if (!RPC_Request_Subscribe(callback, registeredCallback)) {
        return false;
      }
      else if (registeredCallback == nullptr) {
        return false;
      }

      requestVariant[RPC_METHOD_KEY] = methodName;

      // Make the parameters for the client side RPC optional,
      // meaning if it is an empty array or null instead we don't send it at all.
      if (parameters != nullptr && !parameters->isNull()) {
        requestVariant[RPC_PARAMS_KEY] = *parameters;
      }
      else {
        requestVariant[RPC_PARAMS_KEY] = RPC_EMPTY_PARAMS_VALUE;
      }

      const size_t objectSize = JSON_STRING_SIZE(measureJson(requestVariant));
      char buffer[objectSize];
      // Serialize json does not include size of the string null terminator
      if (serializeJson(requestVariant, buffer, objectSize) < objectSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }

      // Print requested client side RPC method
      char message[JSON_STRING_SIZE(strlen(REQUEST_RPC)) + JSON_STRING_SIZE(strlen(buffer))];
      snprintf_P(message, sizeof(message), REQUEST_RPC, buffer);
      Logger::log(message);

      m_requestId++;
      registeredCallback->Set_Request_ID(m_requestId);

      char topic[detectSize(RPC_REQUEST_TOPIC, m_requestId)];
      snprintf_P(topic, sizeof(topic), RPC_REQUEST_TOPIC, m_requestId);
      return m_client.publish(topic, buffer, m_qos);
    }

    //----------------------------------------------------------------------------
    // Firmware OTA API

#if defined(ESP8266) || defined(ESP32)
    inline const bool Start_Firmware_Update(const OTA_Update_Callback& callback) {
      if (!Prepare_Firmware_Settings(callback))  {
        Logger::log(RESETTING_FAILED);
        return false;
      }

      // Request the firmware informations
      return Shared_Attributes_Request(m_fwRequestCallback);
    }

    inline const bool Subscribe_Firmware_Update(const OTA_Update_Callback& callback) {
      if (!Prepare_Firmware_Settings(callback))  {
        Logger::log(RESETTING_FAILED);
        return false;
      }

      // Request the firmware informations
      return Shared_Attributes_Subscribe(m_fwUpdateCallback);
    }
#endif

    //----------------------------------------------------------------------------
    // Shared attributes API

    inline const bool Shared_Attributes_Request(const Shared_Attribute_Request_Callback& callback) {
      // Ensure to have enough size for the infinite amount of possible shared attributes that could be requested from the cloud,
      // therefore we set the size to the MaxFieldsAmt instead of JSON_OBJECT_SIZE(1), which will result in a JsonDocument with a size of 16 bytes
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> requestBuffer;
      // The .template variant of createing the JsonObject has to be used,
      // because we are passing a template to the StaticJsonDocument template list
      // and it will generate a compile time error if not used
      const JsonObject requestObject = requestBuffer.template as<JsonObject>();
      const std::vector<const char *>& sharedAttributes = callback.Get_Attributes();

      // Check if any sharedKeys were requested
      if (sharedAttributes.empty()) {
        Logger::log(NO_KEYS_TO_REQUEST);
        return false;
      }
      // Ensure the response topic has been subscribed
      Shared_Attribute_Request_Callback* registeredCallback = nullptr;
      if (!Shared_Attributes_Request_Subscribe(callback, registeredCallback)) {
        return false;
      }
      else if (registeredCallback == nullptr) {
        return false;
      }

      std::string request;
      for (const char *att : sharedAttributes) {
        // Check if the given attribute is null, if it is skip it
        if (att == nullptr) {
          Logger::log(ATT_IS_NULL);
          continue;
        }
        request.append(att);
        request.push_back(COMMA);
      }

      // Remove latest not needed ,
      request.pop_back();

      requestObject[SHARED_KEYS] = request.c_str();
      const size_t objectSize = JSON_STRING_SIZE(measureJson(requestObject));
      char buffer[objectSize];
      // Serialize json does not include size of the string null terminator
      if (serializeJson(requestObject, buffer, objectSize) < objectSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }

      // Print requested keys
      char message[JSON_STRING_SIZE(strlen(REQUEST_ATT)) + request.length() + JSON_STRING_SIZE(strlen(buffer))];
      snprintf_P(message, sizeof(message), REQUEST_ATT, request.c_str(), buffer);
      Logger::log(message);

      m_requestId++;
      registeredCallback->Set_Request_ID(m_requestId);

      char topic[detectSize(ATTRIBUTE_REQUEST_TOPIC, m_requestId)];
      snprintf_P(topic, sizeof(topic), ATTRIBUTE_REQUEST_TOPIC, m_requestId);
      return m_client.publish(topic, buffer, m_qos);
    }

    // Subscribes multiple Shared attributes callbacks.
    template<class InputIterator>
    inline const bool Shared_Attributes_Subscribe(const InputIterator& first_itr, const InputIterator& last_itr) {
      const uint32_t size = std::distance(first_itr, last_itr);
      if (m_sharedAttributeUpdateCallbacks.size() + size > m_sharedAttributeUpdateCallbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
      if (!m_client.subscribe(ATTRIBUTE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back complete vector into our local m_sharedAttributeUpdateCallbacks vector.
      m_sharedAttributeUpdateCallbacks.insert(m_sharedAttributeUpdateCallbacks.end(), first_itr, last_itr);
      return true;
    }

    // Subscribe one Shared attributes callback.
    inline const bool Shared_Attributes_Subscribe(const Shared_Attribute_Callback& callback) {
      if (m_sharedAttributeUpdateCallbacks.size() + 1U > m_sharedAttributeUpdateCallbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
      if (!m_client.subscribe(ATTRIBUTE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local m_sharedAttributeUpdateCallbacks vector.
      m_sharedAttributeUpdateCallbacks.push_back(callback);
      return true;
    }

    inline const bool Shared_Attributes_Unsubscribe() {
      // Empty all callbacks.
      m_sharedAttributeUpdateCallbacks.clear();
      return m_client.unsubscribe(ATTRIBUTE_TOPIC);
    }

    // -------------------------------------------------------------------------------
    // Provisioning API

    // Subscribes to get provision response
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
    inline const bool Provision_Subscribe(const Provision_Callback& callback) {
      if (!m_client.subscribe(PROV_RESPONSE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }
      m_provisionCallback = &callback;
      return true;
    }

    inline const bool Provision_Unsubscribe() {
      m_provisionCallback = nullptr;
      return m_client.unsubscribe(PROV_RESPONSE_TOPIC);
    }
#endif

  private:
#if defined(ESP8266) || defined(ESP32)
    inline const bool Prepare_Firmware_Settings(const OTA_Update_Callback& callback) {
      const char *currFwTitle = callback.Get_Firmware_Title();
      const char *currFwVersion = callback.Get_Firmware_Version();

      // Send current firmware version
      if (currFwTitle == nullptr || currFwVersion == nullptr) {
        return false;
      }
      else if (!Firmware_Send_FW_Info(currFwTitle, currFwVersion)) {
        return false;
      }

      // Set private members needed for update.
      m_fwCallback = &callback;
      return true;
    }

    inline const bool Firmware_Send_FW_Info(const char *currFwTitle, const char *currFwVersion) {
      // Send our firmware title and version
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> currentFirmwareInfo;
      const JsonObject currentFirmwareInfoObject = currentFirmwareInfo.to<JsonObject>();

      currentFirmwareInfoObject[CURR_FW_TITLE_KEY] = currFwTitle;
      currentFirmwareInfoObject[CURR_FW_VER_KEY] = currFwVersion;
      return sendTelemetryJson(currentFirmwareInfoObject, JSON_STRING_SIZE(measureJson(currentFirmwareInfoObject)));
    }

    inline const bool Firmware_Send_State(const char *currFwState) {
      // Send our firmware title and version
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> currentFirmwareState;
      const JsonObject currentFirmwareStateObject = currentFirmwareState.to<JsonObject>();

      currentFirmwareStateObject[CURR_FW_STATE_KEY] = currFwState;
      return sendTelemetryJson(currentFirmwareStateObject, JSON_STRING_SIZE(measureJson(currentFirmwareStateObject)));
    }

    inline const bool Firmware_OTA_Subscribe() {
      if (!m_client.subscribe(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }
      return true;
    }

    inline const bool Firmware_OTA_Unsubscribe() {
      // Reset now not needed private member variables
      m_fwState = nullptr;
      m_fwCallback = nullptr;
      m_fwSize = 0U;
      m_fwChecksumAlgorithm = mbedtls_md_type_t();
      m_fwChecksum.clear();
      m_fwAlgorithm.clear();
      m_fwChunkReceive = 0U;
      // Unsubscribe from the topic
      return m_client.unsubscribe(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC);
    }

    inline void Firmware_Shared_Attribute_Received(const Shared_Attribute_Data& data) {
      // Print out firmware shared attributes.
      const size_t jsonSize = JSON_STRING_SIZE(measureJson(data));
      char buffer[jsonSize];
      // Serialize json does not include size of the string null terminator
      if (serializeJson(data, buffer, jsonSize) < jsonSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return;
      }
      Logger::log(buffer);

      // Check if firmware is available for our device
      if (!data.containsKey(FW_VER_KEY) || !data.containsKey(FW_TITLE_KEY)) {
        Logger::log(NO_FW);
        Firmware_Send_State(FW_STATE_NO_FW);
        return;
      }
      else if (m_fwCallback == nullptr) {
        Logger::log(OTA_CB_IS_NULL);
        return;
      }

      const char *fw_title = data[FW_TITLE_KEY].as<const char *>();
      const char *fw_version = data[FW_VER_KEY].as<const char *>();
      m_fwChecksum = data[FW_CHKS_KEY].as<std::string>();
      m_fwAlgorithm = data[FW_CHKS_ALGO_KEY].as<std::string>();
      m_fwSize = data[FW_SIZE_KEY].as<const uint32_t>();

      const char *currFwTitle = m_fwCallback->Get_Firmware_Title();
      const char *currFwVersion = m_fwCallback->Get_Firmware_Version();

      if (fw_title == nullptr || fw_version == nullptr || currFwTitle == nullptr || currFwVersion == nullptr || m_fwAlgorithm.empty() || m_fwChecksum.empty()) {
        Logger::log(EMPTY_FW);
        Firmware_Send_State(FW_STATE_NO_FW);
        return;
      }
      // If firmware is the same, we do not update it
      else if (strncmp_P(currFwTitle, fw_title, JSON_STRING_SIZE(strlen(currFwTitle))) == 0 && strncmp_P(currFwVersion, fw_version, JSON_STRING_SIZE(strlen(currFwVersion))) == 0) {
        Logger::log(FW_UP_TO_DATE);
        Firmware_Send_State(FW_STATE_UP_TO_DATE);
        return;
      }
      // If firmware title is not the same, we quit now
      else if (strncmp_P(currFwTitle, fw_title, JSON_STRING_SIZE(strlen(currFwTitle))) != 0) {
        Logger::log(FW_NOT_FOR_US);
        Firmware_Send_State(FW_STATE_NO_FW);
        return;
      }

      // Set used firmware algorithm, depending on received message
      if (m_fwAlgorithm.compare(CHECKSUM_AGORITM_MD5) == 0) {
        m_fwChecksumAlgorithm = mbedtls_md_type_t::MBEDTLS_MD_MD5;
      }
      else if (m_fwAlgorithm.compare(CHECKSUM_AGORITM_SHA256) == 0) {
        m_fwChecksumAlgorithm = mbedtls_md_type_t::MBEDTLS_MD_SHA256;
      }
      else if (m_fwAlgorithm.compare(CHECKSUM_AGORITM_SHA384) == 0) {
        m_fwChecksumAlgorithm = mbedtls_md_type_t::MBEDTLS_MD_SHA384;
      }
      else if (m_fwAlgorithm.compare(CHECKSUM_AGORITM_SHA512) == 0) {
        m_fwChecksumAlgorithm = mbedtls_md_type_t::MBEDTLS_MD_SHA512;
      }
      else {
        char message[JSON_STRING_SIZE(strlen(FW_CHKS_ALGO_NOT_SUPPORTED)) + JSON_STRING_SIZE(m_fwAlgorithm.size())];
        snprintf_P(message, sizeof(message), FW_CHKS_ALGO_NOT_SUPPORTED, m_fwAlgorithm.c_str());
        Logger::log(message);
        Firmware_Send_State(FW_STATE_INVALID_CHKS);
        return;
      }

      // Subscribe to the needed OTA fw topics.
      Firmware_OTA_Subscribe();

      Logger::log(PAGE_BREAK);
      Logger::log(NEW_FW);
      char firmware[JSON_STRING_SIZE(strlen(FROM_TOO)) + JSON_STRING_SIZE(strlen(currFwVersion)) + JSON_STRING_SIZE(strlen(fw_version))];
      snprintf_P(firmware, sizeof(firmware), FROM_TOO, currFwVersion, fw_version);
      Logger::log(firmware);
      Logger::log(DOWNLOADING_FW);

      const uint16_t& chunckSize = m_fwCallback->Get_Chunk_Size();
      const uint32_t numberOfChunk = (m_fwSize / chunckSize) + 1U;
      uint32_t currChunk = 0U;
      uint8_t retries = m_fwCallback->Get_Chunk_Retries();

      // Get the previous buffer size and cache it so the previous settings can be restored.
      const uint16_t previousBufferSize = m_client.getBufferSize();
      const bool changeBufferSize = previousBufferSize < (chunckSize + 50U);

      // Increase size of receive buffer
      if (changeBufferSize && !m_client.setBufferSize(chunckSize + 50U)) {
        Logger::log(NOT_ENOUGH_RAM);
        return;
      }

      // Update state
      m_fwState = FW_STATE_DOWNLOADING;
      Firmware_Send_State(m_fwState);

      char size[detectSize(NUMBER_PRINTF, chunckSize)];
      // Download the firmware
      do {
        // Size adjuts dynamically to the current length of the currChunk number to ensure we don't cut it out of the topic string.
        char topic[detectSize(FIRMWARE_REQUEST_TOPIC, currChunk)];
        snprintf_P(topic, sizeof(topic), FIRMWARE_REQUEST_TOPIC, currChunk);
        snprintf_P(size, sizeof(size), NUMBER_PRINTF, chunckSize);
        m_client.publish(topic, size, m_qos);

        // Amount of time we wait until we declare the download as failed in milliseconds.
        const uint64_t timeout = millis() + 3000U;
        do {
          delay(5);
          loop();
        } while ((m_fwChunkReceive != currChunk) && (timeout >= millis()));

        if (m_fwChunkReceive == currChunk) {
          // Check if the current chunk is not the last one.
          if (numberOfChunk != (currChunk + 1)) {
            // Check if state is still DOWNLOADING and did not fail.
            if (strncmp_P(FW_STATE_DOWNLOADING, m_fwState, strlen(FW_STATE_DOWNLOADING)) == 0) {
              currChunk++;
              m_fwCallback->Call_Progress_Callback<Logger>(currChunk, numberOfChunk);
              // Current chunck was downloaded successfully, resetting retries accordingly.
              retries = m_fwCallback->Get_Chunk_Retries();
            }
            else {
              retries--;
              // Reset any possible errors that might have occured and retry them.
              // Until we run out of retries.
              m_fwState = FW_STATE_DOWNLOADING;
              if (retries == 0) {
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
          retries--;
          if (retries == 0) {
            Logger::log(UNABLE_TO_DOWNLOAD);
            break;
          }
        }
      } while (numberOfChunk != currChunk);

      // Buffer size has been set to another value by the method return to the previous value.
      if (changeBufferSize) {
        m_client.setBufferSize(previousBufferSize);
      }

      const bool success = strncmp_P(STATUS_SUCCESS, m_fwState, strlen(STATUS_SUCCESS)) == 0;
      // Update current_fw_title and current_fw_version if updating was a success.
      if (success) {
        Firmware_Send_FW_Info(fw_title, fw_version);
        Firmware_Send_State(STATUS_SUCCESS);
      }
      else {
        Firmware_Send_State(FW_STATE_FAILED);
      }

      m_fwCallback->Call_End_Callback<Logger>(success);

      // Unsubscribe from now not needed topics anymore,
      // call after everything has been handled, because this method
      // additionaly clears and resets all variables connected to the update itself
      Firmware_OTA_Unsubscribe();
    }
#endif

    // Connects to the client previously set with m_client.setServer().
    inline const bool connect_to_host(const char *access_token, const char *client_id, const char *password) {
      const bool connection_result = m_client.connect(client_id, access_token, password);
      if (connection_result) {
        this->RPC_Unsubscribe(); // Cleanup all server-side RPC subscriptions
        this->RPC_Request_Unsubscribe(); // Cleanup all client-side RPC requests
        this->Shared_Attributes_Unsubscribe(); // Cleanup all shared attributes subscriptions
        this->Shared_Attributes_Request_Unsubscribe(); // Cleanup all shared attributes requests
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
        this->Provision_Unsubscribe();
#endif
#if defined(ESP8266) || defined(ESP32)
        this->Firmware_OTA_Unsubscribe();
#endif
      }
      else {
        Logger::log(CONNECT_FAILED);
      }
      return connection_result;
    }

    // Returns the length in chars needed for a given value with the given argument string to be displayed completly.
    inline const uint8_t detectSize(const char *msg, ...) const {
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

    // Reserves size of callback vectors to improve performance
    inline void reserve_callback_size(void) {
      // Reserve size of both m_sharedAttributeUpdateCallbacks, rpcCallbacks and m_sharedAttributeRequestCallbacks beforehand for performance reasons.
      m_rpcCallbacks.reserve(MaxFieldsAmt);
      m_rpcRequestCallbacks.reserve(MaxFieldsAmt);
      m_sharedAttributeUpdateCallbacks.reserve(MaxFieldsAmt);
      m_sharedAttributeRequestCallbacks.reserve(MaxFieldsAmt);
    }

    // Requests one client-side RPC request callback and returns an editable pointer to a reference of the local version that was copied from the passed argument
    inline const bool RPC_Request_Subscribe(const RPC_Request_Callback& callback, RPC_Request_Callback*& registeredCallback = nullptr) {
      if (m_rpcRequestCallbacks.size() + 1 > m_rpcRequestCallbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_REQUEST_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(RPC_RESPONSE_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local m_sharedAttributeRequestCallbacks vector.
      m_rpcRequestCallbacks.push_back(callback);
      registeredCallback = &m_rpcRequestCallbacks.back();
      return true;
    }

    // Unsubscribes all client-side RPC request callbacks.
    inline const bool RPC_Request_Unsubscribe() {
      // Empty all callbacks.
      m_rpcRequestCallbacks.clear();
      return m_client.unsubscribe(RPC_RESPONSE_SUBSCRIBE_TOPIC);
    }

    // Subscribe one Shared attributes request callback and returns an editable pointer to a reference of the local version that was copied from the passed argument
    inline const bool Shared_Attributes_Request_Subscribe(const Shared_Attribute_Request_Callback& callback, Shared_Attribute_Callback*& registeredCallback = nullptr) {
      if (m_sharedAttributeRequestCallbacks.size() + 1 > m_sharedAttributeRequestCallbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_REQUEST_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local m_sharedAttributeRequestCallbacks vector.
      m_sharedAttributeRequestCallbacks.push_back(callback);
      registeredCallback = &m_sharedAttributeRequestCallbacks.back();
      return true;
    }

    // UNsubscribes all Shared attributes request callbacks.
    inline const bool Shared_Attributes_Request_Unsubscribe() {
      // Empty all callbacks.
      m_sharedAttributeRequestCallbacks.clear();
      return m_client.unsubscribe(ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC);
    }

    // Sends single key-value in a generic way.
    template<typename T>
    inline const bool sendKeyValue(const char *key, T value, bool telemetry = true) {
      const Telemetry t(key, value);
      if (t.IsEmpty()) {
        // Message is ignored and not sent at all.
        return false;
      }

      StaticJsonDocument<JSON_OBJECT_SIZE(1)>jsonBuffer;
      const JsonVariant object = jsonBuffer.to<JsonVariant>();
      if (!t.SerializeKeyValue(object)) {
        Logger::log(UNABLE_TO_SERIALIZE);
        return false;
      }

      return telemetry ? sendTelemetryJson(object, JSON_STRING_SIZE(measureJson(object))) : sendAttributeJSON(object, JSON_STRING_SIZE(measureJson(object)));
    }

    // Process client-side RPC message
    inline void process_rpc_request_message(char *topic, uint8_t *payload, const uint32_t length) {
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonBuffer;
      const DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
          char message[detectSize(UNABLE_TO_DE_SERIALIZE_JSON, error.c_str())];
          snprintf_P(message, sizeof(message), UNABLE_TO_DE_SERIALIZE_JSON, error.c_str());
          Logger::log(message);
          return;
      }
      // .as() is used instead of .to(), because it is meant to cast the JsonDocument to the given type,
      // but it does not change the actual content of the JsonDocument, we don't want that because it already contents content
      // and would result in the data simply being "null", instead .as() allows accessing the data over a JsonVariantConst instead.
      const JsonVariantConst data = jsonBuffer.as<JsonVariantConst>();

      std::string response = topic;
      // Remove the not needed part of the topic.
      const size_t index = strlen(RPC_RESPONSE_TOPIC) + 1U;
      response = response.substr(index, response.length() - index);
      // convert the remaining text to an integer
      const uint32_t responseId = atoi(response.c_str());

      for (size_t i = 0; i < m_rpcRequestCallbacks.size(); i++) {
        if (m_rpcRequestCallbacks.at(i).Get_Request_ID() != responseId) {
          continue;
        }

        char message[detectSize(CALLING_REQUEST_CB, responseId)];
        snprintf_P(message, sizeof(message), CALLING_REQUEST_CB, responseId);
        Logger::log(message);
        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        m_rpcRequestCallbacks.at(i).Call_Callback<Logger>(data);
        // Delete callback because the changes have been requested and the callback is no longer needed.
        m_rpcRequestCallbacks.erase(std::next(m_rpcRequestCallbacks.begin(), i));
      }

      // Attempt to unsubscribe from the shared attribute request topic,
      // if we are not waiting for any further responses with shared attributes from the server.
      // Will be resubscribed if another request is sent anyway
      if (m_rpcRequestCallbacks.empty()) {
        RPC_Request_Unsubscribe();
      }
    }

    // Processes server-side RPC message
    inline void process_rpc_message(char *topic, uint8_t *payload, const uint32_t length) {
      RPC_Response r; {
        StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
        const DeserializationError error = deserializeJson(jsonBuffer, payload, length);
        if (error) {
          char message[detectSize(UNABLE_TO_DE_SERIALIZE_JSON, error.c_str())];
          snprintf_P(message, sizeof(message), UNABLE_TO_DE_SERIALIZE_JSON, error.c_str());
          Logger::log(message);
          return;
        }
        const JsonObjectConst data = jsonBuffer.template as<JsonObjectConst>();
        const char *methodName = data[RPC_METHOD_KEY].as<const char *>();
        const char *params = data[RPC_PARAMS_KEY].as<const char *>();

        if (methodName != nullptr) {
          Logger::log(RECEIVED_RPC_LOG_MESSAGE);
          Logger::log(methodName);
        } else {
          Logger::log(RPC_METHOD_NULL);
          return;
        }

        for (const RPC_Callback& callback : m_rpcCallbacks) {
          const char *subscribedMethodName = callback.Get_Name();
          if (subscribedMethodName == nullptr) {
            Logger::log(RPC_METHOD_NULL);
            continue;
          }
          // Strncmp returns the ascis value difference of the ascii characters that are different,
          // meaning 0 is the same string and less and more than 0 is the difference in ascci values between the 2 chararacters.
          else if (strncmp(subscribedMethodName, methodName, strlen(subscribedMethodName)) != 0) {
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
          const DeserializationError err_param = deserializeJson(doc, params);
          //if failed to de-serialize params then send JsonObject instead
          Logger::log(RPC_PARAMS_KEY);
          if (err_param) {
            const JsonVariantConst param = data[RPC_PARAMS_KEY].as<JsonVariantConst>();
            const size_t jsonSize = JSON_STRING_SIZE(measureJson(param));
            char json[jsonSize];
            // Serialize json does not include size of the string null terminator
            if (serializeJson(param, json, jsonSize) < jsonSize - 1) {
              Logger::log(UNABLE_TO_SERIALIZE_JSON);
              return;
            }
          	Logger::log(json);
            r = callback.Call_Callback<Logger>(param);
          } else {
            Logger::log(params);
            const JsonObjectConst param = doc.template as<JsonObjectConst>();
            // Getting non-existing field from JSON should automatically
            // set JSONVariant to null
            r = callback.Call_Callback<Logger>(param);
          }
          break;
        }
      }
      // Fill in response
      char responsePayload[PayloadSize] = {0};
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> respBuffer;
      const JsonVariant respObject = respBuffer.to<JsonVariant>();

      if (r.IsEmpty()) {
        // Message is ignored and not sent at all.
        return;
      }
      else if (!r.SerializeKeyValue(respObject)) {
        Logger::log(UNABLE_TO_SERIALIZE);
        return;
      }

      const uint32_t jsonSize = JSON_STRING_SIZE(measureJson(respObject));
      if (PayloadSize < jsonSize) {
        char message[detectSize(INVALID_BUFFER_SIZE, PayloadSize, jsonSize)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, PayloadSize, jsonSize);
        Logger::log(message);
        return;
      }
      // Serialize json does not include size of the string null terminator
      else if (serializeJson(respObject, responsePayload, jsonSize) < jsonSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return;
      }

      std::string responseTopic = topic;
      responseTopic.replace(responseTopic.begin(), responseTopic.end(), RPC_REQUEST_KEY, RPC_RESPONSE_KEY);
      Logger::log(RPC_RESPONSE_KEY);
      Logger::log(responseTopic.c_str());
      Logger::log(responsePayload);
      m_client.publish(responseTopic.c_str(), responsePayload, m_qos);
    }

#if defined(ESP8266) || defined(ESP32)
    // Processes firmware response
    inline void process_firmware_response(char *topic, uint8_t *payload, const uint32_t length) {
      static uint32_t sizeReceive = 0;
      static HashGenerator hash(m_fwChecksumAlgorithm);

      m_fwChunkReceive = atoi(strrchr(topic, SLASH) + 1U);

      char message[detectSize(FW_CHUNK, m_fwChunkReceive, length)];
      snprintf_P(message, sizeof(message), FW_CHUNK, m_fwChunkReceive, length);
      Logger::log(message);

      if (m_fwChunkReceive == 0) {
        sizeReceive = 0;

        // Initialize Flash
        if (!Update.begin(m_fwSize)) {
          Logger::log(ERROR_UPDATE_BEGIN);
          m_fwState = FW_STATE_UPDATE_ERROR;
          Firmware_Send_State(m_fwState);
          // Ensure to call Update.abort after calling Update.begin even if it failed,
          // to make sure that any possibly started processes are stopped and freed.
#if defined(ESP32)
          Update.abort();
#endif
          return;
        }
      }

      // Write data to Flash
      if (Update.write(payload, length) != length) {
        Logger::log(ERROR_UPDATE_WRITE);
#if defined(ESP32)
          Update.abort();
#endif
        m_fwState = FW_STATE_UPDATE_ERROR;
        Firmware_Send_State(m_fwState);
        return;
      }

      // Update value only if writing to flash was a success
      hash.update(payload, length);
      sizeReceive += length;

      // Check if we received the full firmware already
      if (m_fwSize != sizeReceive) {
        return;
      }

      const std::string calculatedHash = hash.get_hash_string();
      char actual[JSON_STRING_SIZE(strlen(HASH_ACTUAL)) + JSON_STRING_SIZE(m_fwAlgorithm.size()) + calculatedHash.size()];
      snprintf_P(actual, sizeof(actual), HASH_ACTUAL, m_fwAlgorithm.c_str(), calculatedHash.c_str());
      Logger::log(actual);
      char expected[JSON_STRING_SIZE(strlen(HASH_EXPECTED)) + JSON_STRING_SIZE(m_fwAlgorithm.size()) + JSON_STRING_SIZE(m_fwChecksum.size())];
      snprintf_P(expected, sizeof(expected), HASH_EXPECTED, m_fwAlgorithm.c_str(), m_fwChecksum.c_str());
      Logger::log(expected);
      // Check MD5
      if (m_fwChecksum.compare(calculatedHash) != 0) {
        Logger::log(CHKS_VER_FAILED);
#if defined(ESP32)
        Update.abort();
#endif
        m_fwState = FW_STATE_CHKS_ERROR;
        Firmware_Send_State(m_fwState);
      }
      else {
        Logger::log(CHKS_VER_SUCCESS);
        if (!Update.end()) {
          Logger::log(ERROR_UPDATE_END);
          m_fwState = FW_STATE_UPDATE_ERROR;
          Firmware_Send_State(m_fwState);
          return;
        }
        Logger::log(FW_UPDATE_SUCCESS);
        m_fwState = STATUS_SUCCESS;
      }
    }
#endif

    // Processes shared attribute update message
    inline void process_shared_attribute_update_message(char *topic, uint8_t *payload, const uint32_t length) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      const DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        char message[detectSize(UNABLE_TO_DE_SERIALIZE_JSON, error.c_str())];
        snprintf_P(message, sizeof(message), UNABLE_TO_DE_SERIALIZE_JSON, error.c_str());
        Logger::log(message);
        return;
      }
      JsonObjectConst data = jsonBuffer.template as<JsonObjectConst>();

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
        char id_message[detectSize(ATT_CB_ID, i)];
        snprintf_P(id_message, sizeof(id_message), ATT_CB_ID, i);
        Logger::log(id_message);

        if (m_sharedAttributeUpdateCallbacks.at(i).Get_Attributes().empty()) {
          Logger::log(ATT_CB_NO_KEYS);
          // No specifc keys were subscribed so we call the callback anyway.
          m_sharedAttributeUpdateCallbacks.at(i).Call_Callback<Logger>(data);
          continue;
        }

        bool containsKey = false;
        const char *requested_att = nullptr;
        for (const char *att : m_sharedAttributeUpdateCallbacks.at(i).Get_Attributes()) {
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
        m_sharedAttributeUpdateCallbacks.at(i).Call_Callback<Logger>(data);
      }
    }

    // Processes shared attribute request message
    inline void process_shared_attribute_request_message(char *topic, uint8_t *payload, const uint32_t length) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      const DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        char message[detectSize(UNABLE_TO_DE_SERIALIZE_JSON, error.c_str())];
        snprintf_P(message, sizeof(message), UNABLE_TO_DE_SERIALIZE_JSON, error.c_str());
        Logger::log(message);
        return;
      }
      JsonObjectConst data = jsonBuffer.template as<JsonObjectConst>();

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
      const size_t index = strlen(ATTRIBUTE_RESPONSE_TOPIC) + 1U;
      response = response.substr(index, response.length() - index);
      // convert the remaining text to an integer
      const uint32_t response_id = atoi(response.c_str());

      for (size_t i = 0; i < m_sharedAttributeRequestCallbacks.size(); i++) {
        if (m_sharedAttributeRequestCallbacks.at(i).Get_Request_ID() != response_id) {
          continue;
        }

        char message[detectSize(CALLING_REQUEST_CB, response_id)];
        snprintf_P(message, sizeof(message), CALLING_REQUEST_CB, response_id);
        Logger::log(message);
        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        m_sharedAttributeRequestCallbacks.at(i).Call_Callback<Logger>(data);
        // Delete callback because the changes have been requested and the callback is no longer needed.
        m_sharedAttributeRequestCallbacks.erase(std::next(m_sharedAttributeRequestCallbacks.begin(), i));
      }

      // Unsubscribe from the shared attribute request topic,
      // if we are not waiting for any further responses with shared attributes from the server.
      // Will be resubscribed if another request is sent anyway
      if (m_sharedAttributeRequestCallbacks.empty()) {
        Shared_Attributes_Request_Unsubscribe();
      }
    }

    // Processes provisioning response
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
    inline void process_provisioning_response(char *topic, uint8_t *payload, const uint32_t length) {
      Logger::log(PROV_RESPONSE);

      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      const DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        char message[detectSize(UNABLE_TO_DE_SERIALIZE_JSON, error.c_str())];
        snprintf_P(message, sizeof(message), UNABLE_TO_DE_SERIALIZE_JSON, error.c_str());
        Logger::log(message);
        return;
      }
      const JsonObjectConst data = jsonBuffer.template as<JsonObjectConst>();

      Logger::log(RECEIVED_PROV_RESPONSE);

      const char *provision_status = data[PROV_STATUS_KEY].as<const char *>();
      const char *provision_credentials = data[PROV_CRED_TYPE_KEY].as<const char *>();

      if (strncmp_P(STATUS_SUCCESS, provision_status, strlen(STATUS_SUCCESS)) == 0 && strncmp_P(PROV_CRED_TYPE_VALUE, provision_credentials, strlen(PROV_CRED_TYPE_VALUE)) == 0) {
        Logger::log(X509_NOT_SUPPORTED);
        return;
      }

      if (m_provisionCallback != nullptr) {
        m_provisionCallback->Call_Callback<Logger>(data);
      }
    }
#endif

    // Sends array of attributes or telemetry to ThingsBoard
    inline const bool sendDataArray(const Telemetry *data, size_t data_count, bool telemetry = true) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      const JsonVariant object = jsonBuffer.template to<JsonVariant>();

      for (size_t i = 0; i < data_count; ++i) {
        if (!data[i].SerializeKeyValue(object)) {
          Logger::log(UNABLE_TO_SERIALIZE);
          return false;
        }
      }

      return telemetry ? sendTelemetryJson(object, JSON_STRING_SIZE(measureJson(object))) : sendAttributeJSON(object, JSON_STRING_SIZE(measureJson(object)));
    }

    PubSubClient m_client; // PubSub MQTT client instance.

    // Vectors hold copy of the actual passed data, this is to ensure they stay valid,
    // even if the user only temporarily created the object before the method was called.
    // This can be done because all Callback methods mostly consists of pointers to actual object so copying them
    // does not require a huge memory overhead and is acceptable especially in comparsion to possible problems that could
    // arise if references were used and the end user does not take care to ensure the Callbacks life on for the entirety
    // of its usage, which will lead to dangling references and undefined behaviour
    std::vector<RPC_Callback> m_rpcCallbacks; // Server side RPC callbacks vector
    std::vector<RPC_Request_Callback> m_rpcRequestCallbacks; // Client side RPC callbacks vector
    std::vector<Shared_Attribute_Callback> m_sharedAttributeUpdateCallbacks; // Shared attribute update callbacks vector
    std::vector<Shared_Attribute_Request_Callback> m_sharedAttributeRequestCallbacks; // Shared attribute request callbacks vector

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
    const Provision_Callback* m_provisionCallback; // Provision response callback
#endif
    uint32_t m_requestId; // Allows nearly 4.3 million requests before wrapping back to 0.
    bool m_qos; // Wheter QoS level 1 should be enabled or disabled (Resends the packet until the message was received and a PUBACK packet was returned).

#if defined(ESP8266) || defined(ESP32)
    const char *m_fwState;
    const OTA_Update_Callback* m_fwCallback;
    // Allows for a binary size of up to theoretically 4 GB.
    uint32_t m_fwSize;
    mbedtls_md_type_t m_fwChecksumAlgorithm;
    std::string m_fwAlgorithm;
    std::string m_fwChecksum;
    const std::vector<const char *> m_fwSharedKeys;
    const Shared_Attribute_Request_Callback m_fwRequestCallback;
    const Shared_Attribute_Callback m_fwUpdateCallback;
    uint16_t m_fwChunkReceive;
#endif

    // The callback for when a PUBLISH message is received from the server.
    inline void onMQTTMessage(char *topic, uint8_t *payload, uint32_t length) {
      char message[JSON_STRING_SIZE(strlen(CB_ON_MESSAGE)) + JSON_STRING_SIZE(strlen(topic))];
      snprintf_P(message, sizeof(message), CB_ON_MESSAGE, topic);
      Logger::log(message);

      if (strncmp_P(RPC_RESPONSE_TOPIC, topic, strlen(RPC_RESPONSE_TOPIC)) == 0) {
        process_rpc_request_message(topic, payload, length);
      } else if (strncmp_P(RPC_TOPIC, topic, strlen(RPC_TOPIC)) == 0) {
        process_rpc_message(topic, payload, length);
      } else if (strncmp_P(ATTRIBUTE_RESPONSE_TOPIC, topic, strlen(ATTRIBUTE_RESPONSE_TOPIC)) == 0) {
        process_shared_attribute_request_message(topic, payload, length);
      } else if (strncmp_P(ATTRIBUTE_TOPIC, topic, strlen(ATTRIBUTE_TOPIC)) == 0) {
        process_shared_attribute_update_message(topic, payload, length);
      } else if (strncmp_P(PROV_RESPONSE_TOPIC, topic, strlen(PROV_RESPONSE_TOPIC)) == 0) {
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
        process_provisioning_response(topic, payload, length);
#endif
      } else if (strncmp_P(FIRMWARE_RESPONSE_TOPIC, topic, strlen(FIRMWARE_RESPONSE_TOPIC)) == 0) {
#if defined(ESP8266) || defined(ESP32)
        process_firmware_response(topic, payload, length);
#endif
      }
    }
};

using ThingsBoard = ThingsBoardSized<>;

#endif // ThingsBoard_h
