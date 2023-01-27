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

#if defined(ESP8266)
#include <Updater.h>
#elif defined(ESP32)
#include <Update.h>
#endif

// Local includes.
#include "Configuration.h"
#include "ThingsBoardDefaultLogger.h"
#include "HashGenerator.h"
#include "Telemetry.h"
#include "Constants.h"
#include "Shared_Attribute_Callback.h"
#include "Attribute_Request_Callback.h"
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
constexpr char SHARED_REQUEST_KEY[] PROGMEM = "sharedKeys";
constexpr char SHARED_RESPONSE_KEY[] PROGMEM = "shared";

// Client side attribute request keys.
constexpr char CLIENT_REQUEST_KEYS[] PROGMEM = "clientKeys";
constexpr char CLIENT_RESPONSE_KEY[] PROGMEM = "client";

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
constexpr char ATT_CB_NO_KEYS[] PROGMEM = "No keys subscribed. Calling subscribed callback for any updated attributes (assumed to be subscribed to every possible key)";
constexpr char ATT_IS_NULL[] PROGMEM = "Subscribed shared attribute update key is NULL";
constexpr char ATT_IN_ARRAY[] PROGMEM = "Shared attribute update key: (%s) is subscribed";
constexpr char ATT_NO_CHANGE[] PROGMEM = "No keys that we subscribed too were changed, skipping callback";
constexpr char CALLING_ATT_CB[] PROGMEM = "Calling subscribed callback for updated shared attribute (%s)";
constexpr char RECEIVED_ATT[] PROGMEM = "Received shared attribute request";
constexpr char ATT_KEY_NOT_FOUND[] PROGMEM = "Attribute key not found";
constexpr char CALLING_REQUEST_CB[] PROGMEM = "Calling subscribed callback for response id (%u)";
constexpr char CB_ON_MESSAGE[] PROGMEM = "Callback onMQTTMessage from topic: (%s)";
constexpr char SUBSCRIBE_TOPIC_FAILED[] PROGMEM = "Subscribing the given topic failed";

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
constexpr char PROV_CRED_TYPE_KEY[] PROGMEM = "credentialsType";
constexpr char PROV_TOKEN[] PROGMEM = "token";
constexpr char PROV_CRED_USERNAME[] PROGMEM = "username";
constexpr char PROV_CRED_PASSWORD[] PROGMEM = "password";
constexpr char PROV_CRED_CLIENT_ID[] PROGMEM = "clientId";
constexpr char PROV_CRED_HASH[] PROGMEM = "hash";

// Log messages.
constexpr char PROV_REQUEST[] PROGMEM = "Provision request:";
constexpr char UNABLE_TO_DE_SERIALIZE_PROV_RESPONSE[] PROGMEM = "Unable to de-serialize provision response";
constexpr char PROV_RESPONSE[] PROGMEM = "Process provisioning response";
constexpr char RECEIVED_PROV_RESPONSE[] PROGMEM = "Received provision response";

#if defined(ESP8266) || defined(ESP32)

// Firmware topics.
constexpr char FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC[] PROGMEM = "v2/fw/response/#";
constexpr char FIRMWARE_REQUEST_TOPIC[] PROGMEM = "v2/fw/request/0/chunk/%u";

// Firmware data keys.
constexpr char CURR_FW_TITLE_KEY[] PROGMEM = "current_fw_title";
constexpr char CURR_FW_VER_KEY[] PROGMEM = "current_fw_version";
constexpr char FW_ERROR_KEY[] PROGMEM = "fw_error";
constexpr char FW_STATE_KEY[] PROGMEM = "fw_state";
constexpr char FW_VER_KEY[] PROGMEM = "fw_version";
constexpr char FW_TITLE_KEY[] PROGMEM = "fw_title";
constexpr char FW_CHKS_KEY[] PROGMEM = "fw_checksum";
constexpr char FW_CHKS_ALGO_KEY[] PROGMEM = "fw_checksum_algorithm";
constexpr char FW_SIZE_KEY[] PROGMEM = "fw_size";
constexpr char FW_STATE_DOWNLOADING[] PROGMEM = "DOWNLOADING";
constexpr char FW_STATE_DOWNLOADED[] PROGMEM = "DOWNLOADED";
constexpr char FW_STATE_VERIFIED[] PROGMEM = "VERIFIED";
constexpr char FW_STATE_UPDATING[] PROGMEM = "UPDATING";
constexpr char FW_STATE_FAILED[] PROGMEM = "FAILED";
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

#endif // defined(ESP8266) || defined(ESP32)

/// @brief Wrapper around the PubSubClient to allow connecting and sending / retrieving data from ThingsBoard over the MQTT or MQTT with TLS/SSL protocol
/// @tparam Logger Logging class that should be used to print messages generated by ThingsBoard, default = ThingsBoardDefaultLogger
/// @tparam PayloadSize Maximum amount of data that can be either received or sent to ThingsBoard at once, if bigger packets are received they are discarded
/// and if we attempt to send data that is bigger, it will not be sent, default = 64
/// @tparam MaxFieldsAmt Maximum amount of key value pair that we will be able to sent to ThingsBoard in one call, default = 8
template<size_t PayloadSize = Default_Payload,
         size_t MaxFieldsAmt = Default_Fields_Amt,
         typename Logger = ThingsBoardDefaultLogger>
class ThingsBoardSized {
  public:
    /// @brief Constructs a ThingsBoardSized instance with the given network client
    /// @param client Network client that should be used to establish the connection to ThingsBoard
    /// @param enableQoS Wheter the PubSubClient should use Quality of Service Level 1 or not,
    /// false means Level 0 is used, which means we simply send the data and hope it arrives if it is lost in transit it isn't resent.
    /// Level 2 however stores the given message we wanted to send until a response is received from the server,
    /// meaning that the message has been sent at least once, but it could have been sent multiple times as well,
    /// will not be a problem if the optional timestamp is added to the sent data,
    /// making duplicate sent data irrelevant because it simply overrides the data with the same timestamp,
    /// see https://www.hivemq.com/blog/mqtt-essentials-part-6-mqtt-quality-of-service-levels/ for more information
    inline ThingsBoardSized(Client& client, const bool& enableQoS = false)
      : ThingsBoardSized(enableQoS)
    {
      setClient(client);
    }

    /// @brief Constructs a ThingsBoardSized instance without a network client, meaning it has to be added later with the setClient() method
    /// @param enableQoS Wheter the PubSubClient should use Quality of Service Level 1 or not,
    /// false means Level 0 is used, which means we simply send the data and hope it arrives if it is lost in transit it isn't resent.
    /// Level 2 however stores the given message we wanted to send until a response is received from the server,
    /// meaning that the message has been sent at least once, but it could have been sent multiple times as well,
    /// will not be a problem if the optional timestamp is added to the sent data,
    /// making duplicate sent data irrelevant because it simply overrides the data with the same timestamp,
    /// see https://www.hivemq.com/blog/mqtt-essentials-part-6-mqtt-quality-of-service-levels/ for more information
    inline ThingsBoardSized(const bool& enableQoS = false)
      : m_client()
#if !THINGSBOARD_ENABLE_STL
      , m_rpcIndex(0U)
#endif // !THINGSBOARD_ENABLE_STL
      , m_rpcCallbacks()
#if !THINGSBOARD_ENABLE_STL
      , m_rpcRequestIndex(0U)
#endif // !THINGSBOARD_ENABLE_STL
      , m_rpcRequestCallbacks()
#if !THINGSBOARD_ENABLE_STL
      , m_sharedAttributeIndex(0U)
#endif // !THINGSBOARD_ENABLE_STL
      , m_sharedAttributeUpdateCallbacks()
#if !THINGSBOARD_ENABLE_STL
      , m_attributeRequestIndex(0U)
#endif // !THINGSBOARD_ENABLE_STL
      , m_attributeRequestCallbacks()
      , m_provisionCallback()
      , m_requestId(0U)
      , m_qos(false)
#if defined(ESP8266) || defined(ESP32)
      , m_fwState(false)
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
#if !THINGSBOARD_ENABLE_STL
      m_subscribedInstance = this;
#endif // !THINGSBOARD_ENABLE_STL
      reserve_callback_size(MaxFieldsAmt);
    }

    /// @brief Destructor
    inline ~ThingsBoardSized() {
      // Nothing to do.
    }

    /// @brief Gets the currently connected PubSubClient as a reference,
    /// allows for calling method directly on the client itself, not advised in normal use cases,
    /// as it might cause problems if the library expects the client to be sending / receiving data
    /// but can't anymore because it has been disconnected or certain settings were changed
    /// @return Reference to the underlying PubSubClient connected to ThingsBoard
    inline PubSubClient& getClient() {
      return m_client;
    }

    /// @brief Sets the Quality of Service Level used by the MQTT client either true for Level 1 or false for Level 0
    /// @param enableQoS  Wheter the PubSubClient should use Quality of Service Level 1 or not,
    /// false means Level 0 is used, which means we simply send the data and hope it arrives if it is lost in transit it isn't resent.
    /// Level 2 however stores the given message we wanted to send until a response is received from the server,
    /// meaning that the message has been sent at least once, but it could have been sent multiple times as well,
    /// will not be a problem if the optional timestamp is added to the sent data,
    /// making duplicate sent data irrelevant because it simply overrides the data with the same timestamp,
    /// see https://www.hivemq.com/blog/mqtt-essentials-part-6-mqtt-quality-of-service-levels/ for more information
    inline void enableMQTTQoS(const bool& enableQoS) {
      m_qos = enableQoS;
    }
    
    /// @brief Sets the underlying network client that should be used to establish the connection to ThingsBoard,
    /// ensure to disconnect and connect again after changing the underlying client
    /// @param client Network client that should be used to establish the connection to ThingsBoard
    inline void setClient(Client& client) {
      m_client.setClient(client);
      m_client.setBufferSize(PayloadSize);
      // Initalize callback.
#if THINGSBOARD_ENABLE_STL
      m_client.setCallback(std::bind(&ThingsBoardSized::onMQTTMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
#else
      m_client.setCallback(ThingsBoardSized::onStaticMQTTMessage);
#endif // THINGSBOARD_ENABLE_STL
    }

    /// @brief Connects to the specified ThingsBoard server over the given port as the given device
    /// @param host ThingsBoard server instance we want to connect to
    /// @param access_token Access token that connects this device with a created device on the ThingsBoard server,
    /// can be "provision", if the device creates itself instead
    /// @param port Port that will be used to establish a connection and send / receive data from ThingsBoard over
    /// @param client_id Client username that can be used to differentiate the user that is connecting the given device to ThingsBoard
    /// @param password Client password that can be used to authenticate the user that is connecting the given device to ThingsBoard
    /// @return Wheter connecting to ThingsBoard was successful or not
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

    /// @brief Disconnects any connection that has been established already
    inline void disconnect() {
      m_client.disconnect();
    }

    /// @brief Returns our current connection status to the cloud, true meaning we are connected,
    /// false meaning we have been disconnected or have not established a connection yet
    /// @return Wheter the underlying PubSubClient is currently connected or not
    inline const bool connected() {
      return m_client.connected();
    }

    /// @brief Receives / sends any outstanding messages from and to the MQTT broker
    inline void loop() {
      m_client.loop();
    }

    //----------------------------------------------------------------------------
    // Claiming API

    /// @brief Sends a claiming request for the given device, allowing any given user on the cloud to assign the device as their own (claim),
    /// as long as they enter the given device name and secret key in the given amount of time.
    /// Optionally a secret key can be passed or be left empty (cloud will allow any user to claim the device for the given amount of time)
    /// @param secretKey Password the user additionaly to the device name needs to enter to claim it as their own,
    /// pass nullptr or an empty string if the user should be able to claim the device without any password
    /// @param durationMs Total time in milliseconds the user has to claim their device as their own
    /// @return Wheter sending the claiming request was successful or not
    inline const bool Claim_Request(const char *secretKey, const uint32_t& durationMs) {
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
      if (PayloadSize < objectSize) {
        char message[detectSize(INVALID_BUFFER_SIZE, PayloadSize, objectSize)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, PayloadSize, objectSize);
        Logger::log(message);
        return false;
      }
      // Serialize json does not include size of the string null terminator
      else if (serializeJson(respObj, responsePayload, objectSize) < objectSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }

      return m_client.publish(CLAIM_TOPIC, responsePayload, m_qos ? 1 : 0);
    }

    //----------------------------------------------------------------------------
    // Provisioning API

    /// @brief Sends provisioning request for a new device, meaning we want to create a device that we can then connect over,
    /// where the given provision device key / secret decide which device profile is used to create the given device with.
    /// Optionally a device name can be passed or be left empty (cloud will use the unqiue access token as a name instead).
    /// The cloud then sends back json data containing our credentials, that the given callback, if creating the device was successful.
    /// The data contained in that callbackcan then be used to disconnect and reconnect to the ThingsBoard server as our newly created device
    /// @param callback Callback method that will be called
    /// @return Wheter sending the provisioning request was successful or not
    inline const bool Provision_Request(const Provision_Callback& callback) {
      StaticJsonDocument<JSON_OBJECT_SIZE(9)> requestBuffer;
      const JsonObject requestObject = requestBuffer.to<JsonObject>();

      const char *provisionDeviceKey = callback.Get_Device_Key();
      const char *provisionDeviceSecret = callback.Get_Device_Secret();

      if (provisionDeviceKey == nullptr || provisionDeviceSecret == nullptr) {
        return false;
      }
      // Ensure the response topic has been subscribed
      else if (!Provision_Subscribe(callback)) {
        return false;
      }

      const char *deviceName = callback.Get_Device_Name();
      const char *accessToken = callback.Get_Device_Access_Token();
      const char *credUsername = callback.Get_Credentials_Username();
      const char *credPassword = callback.Get_Credentials_Password();
      const char *credClientID = callback.Get_Credentials_Client_ID();
      const char *hash = callback.Get_Certificate_Hash();
      const char *credentialsType = callback.Get_Credentials_Type();

      // Make the key-value pairs we don't want to send optional,
      // meaning if it is an empty string or null instead we don't send it at all.
      // Deciding which underlying provisioning method is restricted, by the Provision_Callback class.
      // Meaning only the key-value pairs that are needed for the given provisionign method are set,
      // meaning the rest will not be sent and therefore the provisoning request has the correct formatting
      if (deviceName != nullptr && deviceName[0] != '\0') {
        requestObject[DEVICE_NAME_KEY] = deviceName;
      }
      if (accessToken != nullptr && accessToken[0] != '\0') {
        requestObject[PROV_TOKEN] = accessToken;
      }
      if (credUsername != nullptr && credUsername[0] != '\0') {
        requestObject[PROV_CRED_USERNAME] = credUsername;
      }
      if (credPassword != nullptr && credPassword[0] != '\0') {
        requestObject[PROV_CRED_PASSWORD] = credPassword;
      }
      if (credClientID != nullptr && credClientID[0] != '\0') {
        requestObject[PROV_CRED_CLIENT_ID] = credClientID;
      }
      if (hash != nullptr && hash[0] != '\0') {
        requestObject[PROV_CRED_HASH] = hash;
      }
      if (credentialsType != nullptr && credentialsType[0] != '\0') {
        requestObject[PROV_CRED_TYPE_KEY] = credentialsType;
      }
      requestObject[PROV_DEVICE_KEY] = provisionDeviceKey;
      requestObject[PROV_DEVICE_SECRET_KEY] = provisionDeviceSecret;

      const size_t objectSize = JSON_STRING_SIZE(measureJson(requestObject));
      char requestPayload[objectSize];
      if (PayloadSize < objectSize) {
        char message[detectSize(INVALID_BUFFER_SIZE, PayloadSize, objectSize)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, PayloadSize, objectSize);
        Logger::log(message);
        return false;
      }
      // Serialize json does not include size of the string null terminator
      else if (serializeJson(requestObject, requestPayload, objectSize) < objectSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }

      Logger::log(PROV_REQUEST);
      Logger::log(requestPayload);
      return m_client.publish(PROV_REQUEST_TOPIC, requestPayload, m_qos ? 1 : 0);
    }

    //----------------------------------------------------------------------------
    // Telemetry API

    /// @brief Attempts to send telemetry data with the given key and value of the given type
    /// @tparam T Type of the data value we want to send
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Wheter sending the data was successful or not
    template<class T>
    inline const bool sendTelemetryData(const char *key, T value) {
      return sendKeyValue(key, value);
    }

    /// @brief Attempts to send integer telemetry data with the given key and value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Wheter sending the data was successful or not
    inline const bool sendTelemetryInt(const char *key, int value) {
      return sendKeyValue(key, value);
    }

    /// @brief Attempts to send boolean telemetry data with the given key and value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Wheter sending the data was successful or not
    inline const bool sendTelemetryBool(const char *key, bool value) {
      return sendKeyValue(key, value);
    }

    /// @brief Attempts to send float telemetry data with the given key and value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Wheter sending the data was successful or not
    inline const bool sendTelemetryFloat(const char *key, float value) {
      return sendKeyValue(key, value);
    }

    /// @brief Attempts to send string telemetry data with the given key and value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Wheter sending the data was successful or not
    inline const bool sendTelemetryString(const char *key, const char *value) {
      return sendKeyValue(key, value);
    }

    /// @brief Attempts to send aggregated telemetry data
    /// @param data Array containing all the data we want to send
    /// @param data_count Amount of data entries in the array that we want to send
    /// @return Wheter sending the data was successful or not
    inline const bool sendTelemetry(const Telemetry *data, size_t data_count) {
      return sendDataArray(data, data_count);
    }

    /// @brief Attempts to send custom json telemetry string
    /// @param json String containing our json key value pairs we want to attempt to send
    /// @return Wheter sending the data was successful or not
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
      return m_client.publish(TELEMETRY_TOPIC, json, m_qos ? 1 : 0);
    }

    /// @brief Attempts to send custom telemetry JsonObject
    /// @param jsonObject JsonObject containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the JsonObject
    /// @return Wheter sending the data was successful or not
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

    /// @brief Attempts to send custom telemetry JsonVariant
    /// @param jsonVariant JsonVariant containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the JsonVariant
    /// @return Wheter sending the data was successful or not
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

    /// @brief Attempts to send attribute data with the given key and value of the given type
    /// @tparam T Type of the data value we want to send
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Wheter sending the data was successful or not
    template<class T>
    inline const bool sendAttributeData(const char *attrName, T value) {
      return sendKeyValue(attrName, value, false);
    }

    /// @brief Attempts to send integer attribute data with the given key and value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Wheter sending the data was successful or not
    inline const bool sendAttributeInt(const char *attrName, int value) {
      return sendKeyValue(attrName, value, false);
    }

    /// @brief Attempts to send boolean attribute data with the given key and value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Wheter sending the data was successful or not
    inline const bool sendAttributeBool(const char *attrName, bool value) {
      return sendKeyValue(attrName, value, false);
    }

    /// @brief Attempts to send float attribute data with the given key and value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Wheter sending the data was successful or not
    inline const bool sendAttributeFloat(const char *attrName, float value) {
      return sendKeyValue(attrName, value, false);
    }

    /// @brief Attempts to send string attribute data with the given key and value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Wheter sending the data was successful or not
    inline const bool sendAttributeString(const char *attrName, const char *value) {
      return sendKeyValue(attrName, value, false);
    }

    /// @brief Attempts to send aggregated attribute data
    /// @param data Array containing all the data we want to send
    /// @param data_count Amount of data entries in the array that we want to send
    /// @return Wheter sending the data was successful or not
    inline const bool sendAttributes(const Attribute *data, size_t data_count) {
      return sendDataArray(data, data_count, false);
    }

    /// @brief Attempts to send custom json attribute string
    /// @param json String containing our json key value pairs we want to attempt to send
    /// @return Wheter sending the data was successful or not
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
      return m_client.publish(ATTRIBUTE_TOPIC, json, m_qos ? 1 : 0);
    }

    /// @brief Attempts to send custom attribute JsonObject
    /// @param jsonObject JsonObject containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the JsonObject
    /// @return Wheter sending the data was successful or not
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

    /// @brief Attempts to send custom attribute JsonVariant
    /// @param jsonVariant JsonVariant containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the JsonVariant
    /// @return Wheter sending the data was successful or not
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

    /// @brief Requests one client-side attribute calllback,
    /// that will be called if the key-value pair from the server for the given client-side attributes is received
    /// @param callback Callback method that will be called
    /// @return Wheter requesting the given callback was successful or not
    inline const bool Client_Attributes_Request(const Attribute_Request_Callback& callback) {
      return Attributes_Request(callback, CLIENT_REQUEST_KEYS, CLIENT_RESPONSE_KEY);
    }

    //----------------------------------------------------------------------------
    // Server-side RPC API


#if THINGSBOARD_ENABLE_STL

    /// @brief Subscribes multiple server-side RPC callbacks,
    /// that will be called if a request from the server for the method with the given name is received
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array
    /// @param first_itr Iterator pointing to the first element in the data container
    /// @param last_itr Iterator pointing to the end of the data container (last element + 1)
    /// @return Wheter subscribing the given callbacks was successful or not
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

#else

    /// @brief Subscribes multiple server-side RPC callbacks,
    /// that will be called if a request from the server for the method with the given name is received
    /// @param callbacks Pointer to the c-style array
    /// @param callbacksSize Amount of values that should be subscribed, ensure size matches the actual array,
    /// if not the system might crash unexpectedly at a later point
    /// @return Wheter subscribing the given callbacks was successful or not
    inline const bool RPC_Subscribe(const RPC_Callback *callbacks, const size_t& callbacksSize) {
      if (callbacksSize + m_rpcIndex > sizeof(m_rpcCallbacks) / sizeof(*m_rpcCallbacks)) {
        Logger::log(MAX_RPC_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      for (size_t i = 0; i < callbacksSize; ++i) {
        m_rpcCallbacks[m_rpcIndex] = callbacks[i];
        m_rpcIndex++;
      }

      return true;
    }

#endif // THINGSBOARD_ENABLE_STL

    /// @brief Subscribe one server-side RPC callback,
    /// that will be called if a request from the server for the method with the given name is received
    /// @param callback Callback method that will be called
    /// @return Wheter subscribing the given callback was successful or not
    inline const bool RPC_Subscribe(const RPC_Callback& callback) {
#if THINGSBOARD_ENABLE_STL
      if (m_rpcCallbacks.size() + 1 > m_rpcCallbacks.capacity()) {
        Logger::log(MAX_RPC_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local vector
      m_rpcCallbacks.push_back(callback);
#else
      if (1 + m_rpcIndex > sizeof(m_rpcCallbacks) / sizeof(*m_rpcCallbacks)) {
        Logger::log(MAX_RPC_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      m_rpcCallbacks[m_rpcIndex] = callback;
      m_rpcIndex++;
#endif // THINGSBOARD_ENABLE_STL
      return true;
    }

    /// @brief Unsubcribes all server-side RPC callbacks
    /// @return Wheter unsubcribing all the previously subscribed callbacks
    /// and from the rpc topic, was successful or not
    inline const bool RPC_Unsubscribe() {
      // Empty all callbacks
#if THINGSBOARD_ENABLE_STL
      m_rpcCallbacks.clear();
#else
      m_rpcIndex = 0U;
#endif // THINGSBOARD_ENABLE_STL
      return m_client.unsubscribe(RPC_SUBSCRIBE_TOPIC);
    }

    //----------------------------------------------------------------------------
    // Client-side RPC API

    /// @brief Requests one client-side RPC callback,
    /// that will be called if a response from the server for the method with the given name is received
    /// @param callback Callback method that will be called
    /// @return Wheter requesting the given callback was successful or not
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
      if (PayloadSize < objectSize) {
        char message[detectSize(INVALID_BUFFER_SIZE, PayloadSize, objectSize)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, PayloadSize, objectSize);
        Logger::log(message);
        return false;
      }
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
      return m_client.publish(topic, buffer, m_qos ? 1 : 0);
    }

    //----------------------------------------------------------------------------
    // Firmware OTA API

#if defined(ESP8266) || defined(ESP32)

    /// @brief Immediately starts a firmware update if firmware is assigned to the given device
    /// @param callback Callback method that will be called
    /// @return Wheter subscribing the given callback was successful or not
    inline const bool Start_Firmware_Update(const OTA_Update_Callback& callback) {
      if (!Prepare_Firmware_Settings(callback))  {
        Logger::log(RESETTING_FAILED);
        return false;
      }

      // Request the firmware information
      return Shared_Attributes_Request(m_fwRequestCallback);
    }

    /// @brief Subscribes for any assignment of firmware to the given device device,
    /// which will then start a firmware update
    /// @param callback Callback method that will be called
    /// @return Wheter subscribing the given callback was successful or not
    inline const bool Subscribe_Firmware_Update(const OTA_Update_Callback& callback) {
      if (!Prepare_Firmware_Settings(callback))  {
        Logger::log(RESETTING_FAILED);
        return false;
      }

      // Subscribes to changes of the firmware information
      return Shared_Attributes_Subscribe(m_fwUpdateCallback);
    }

    /// @brief Sends the given firmware title and firmware version to the cloud
    /// @param currFwTitle Current device firmware title
    /// @param currFwVersion Current device firmware version
    /// @return Wheter sending the current device firmware information was successful or not
    inline const bool Firmware_Send_Info(const char *currFwTitle, const char *currFwVersion) {
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> currentFirmwareInfo;
      const JsonObject currentFirmwareInfoObject = currentFirmwareInfo.to<JsonObject>();

      currentFirmwareInfoObject[CURR_FW_TITLE_KEY] = currFwTitle;
      currentFirmwareInfoObject[CURR_FW_VER_KEY] = currFwVersion;
      return sendTelemetryJson(currentFirmwareInfoObject, JSON_STRING_SIZE(measureJson(currentFirmwareInfoObject)));
    }

    /// @brief Sends the given firmware state to the cloud
    /// @param currFwState Current firmware download state
    /// @param fwError Firmware error message that describes the current firmware state,
    /// pass nullptr or an empty string if the current state is not a failure state
    /// and therefore does not require any firmware error messsages
    /// @return Wheter sending the current firmware download state was successful or not
    inline const bool Firmware_Send_State(const char *currFwState, const char* fwError = nullptr) {
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> currentFirmwareState;
      const JsonObject currentFirmwareStateObject = currentFirmwareState.to<JsonObject>();

      // Make the fw error optional,
      // meaning if it is an empty string or null instead we don't send it at all.
      if (fwError != nullptr && fwError[0] != '\0') {
        currentFirmwareStateObject[FW_ERROR_KEY] = fwError;
      }
      currentFirmwareStateObject[FW_STATE_KEY] = currFwState;
      return sendTelemetryJson(currentFirmwareStateObject, JSON_STRING_SIZE(measureJson(currentFirmwareStateObject)));
    }

#endif

    //----------------------------------------------------------------------------
    // Shared attributes API

    /// @brief Requests one shared attribute calllback,
    /// that will be called if the key-value pair from the server for the given shared attributes is received
    /// @param callback Callback method that will be called
    /// @return Wheter requesting the given callback was successful or not
    inline const bool Shared_Attributes_Request(const Attribute_Request_Callback& callback) {
      return Attributes_Request(callback, SHARED_REQUEST_KEY, SHARED_RESPONSE_KEY);
    }

#if THINGSBOARD_ENABLE_STL

    /// @brief Subscribes multiple shared attribute callbacks,
    /// that will be called if the key-value pair from the server for the given shared attributes is received
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array
    /// @param first_itr Iterator pointing to the first element in the data container
    /// @param last_itr Iterator pointing to the end of the data container (last element + 1)
    /// @return Wheter subscribing the given callbacks was successful or not
    template<class InputIterator>
    inline const bool Shared_Attributes_Subscribe(const InputIterator& first_itr, const InputIterator& last_itr) {
      const uint32_t size = std::distance(first_itr, last_itr);
      if (m_sharedAttributeUpdateCallbacks.size() + size > m_sharedAttributeUpdateCallbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(ATTRIBUTE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back complete vector into our local m_sharedAttributeUpdateCallbacks vector.
      m_sharedAttributeUpdateCallbacks.insert(m_sharedAttributeUpdateCallbacks.end(), first_itr, last_itr);
      return true;
    }

#else

    /// @brief  Subscribes multiple shared attribute callbacks,
    /// that will be called if the key-value pair from the server for the given shared attributes is received
    /// @param callbacks Pointer to the c-style array
    /// @param callbacksSize Amount of values that should be subscribed, ensure size matches the actual array,
    /// if not the system might crash unexpectedly at a later point
    /// @return Wheter subscribing the given callbacks was successful or not
    inline const bool Shared_Attributes_Subscribe(const Shared_Attribute_Callback *callbacks, const size_t& callbacksSize) {
      if (callbacksSize + m_sharedAttributeIndex + 1U  > sizeof(m_sharedAttributeUpdateCallbacks) / sizeof(*m_sharedAttributeUpdateCallbacks)) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(ATTRIBUTE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      for (size_t i = 0; i < callbacksSize; ++i) {
        m_sharedAttributeUpdateCallbacks[m_sharedAttributeIndex] = callbacks[i];
        m_sharedAttributeIndex++;
      }

      return true;
    }

#endif // THINGSBOARD_ENABLE_STL

    /// @brief Subscribe one shared attribute callback,
    /// that will be called if the key-value pair from the server for the given shared attributes is received
    /// @param callback Callback method that will be called
    /// @return Wheter subscribing the given callback was successful or not
    inline const bool Shared_Attributes_Subscribe(const Shared_Attribute_Callback& callback) {
#if THINGSBOARD_ENABLE_STL
      if (m_sharedAttributeUpdateCallbacks.size() + 1U > m_sharedAttributeUpdateCallbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(ATTRIBUTE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local vector
      m_sharedAttributeUpdateCallbacks.push_back(callback);
#else
      if (1 + m_sharedAttributeIndex > sizeof(m_sharedAttributeUpdateCallbacks) / sizeof(*m_sharedAttributeUpdateCallbacks)) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(ATTRIBUTE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      m_sharedAttributeUpdateCallbacks[m_sharedAttributeIndex] = callback;
      m_sharedAttributeIndex++;
#endif // THINGSBOARD_ENABLE_STL
      return true;
    }

    /// @brief Unsubcribes all shared attribute callbacks
    /// @return Wheter unsubcribing all the previously subscribed callbacks
    /// and from the attribute topic, was successful or not
    inline const bool Shared_Attributes_Unsubscribe() {
      // Empty all callbacks
#if THINGSBOARD_ENABLE_STL
      m_sharedAttributeUpdateCallbacks.clear();
#else
      m_sharedAttributeIndex = 0U;
#endif // THINGSBOARD_ENABLE_STL
      return m_client.unsubscribe(ATTRIBUTE_TOPIC);
    }

  private:

    /// @brief Requests one client-side or shared attribute calllback,
    /// that will be called if the key-value pair from the server for the given client-side or shared attributes is received
    /// @param callback Callback method that will be called
    /// @param attributeRequestKey Key of the key-value pair that will contain the attributes we want to request
    /// @param attributeResponseKey Key of the key-value pair that will contain the attributes we got as a response
    /// @return Wheter requesting the given callback was successful or not
    inline const bool Attributes_Request(const Attribute_Request_Callback& callback, const char* attributeRequestKey, const char* attributeResponseKey) {
      // Ensure to have enough size for the infinite amount of possible shared attributes that could be requested from the cloud,
      // therefore we set the size to the MaxFieldsAmt instead of JSON_OBJECT_SIZE(1), which will result in a JsonDocument with a size of 16 bytes
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> requestBuffer;
      // The .template variant of createing the JsonVariant has to be used,
      // because we are passing a template to the StaticJsonDocument template list
      // and it will generate a compile time error if not used
      const JsonVariant requestObject = requestBuffer.template as<JsonVariant>();
#if THINGSBOARD_ENABLE_STL
      const std::vector<const char *>& attributes = callback.Get_Attributes();

      // Check if any sharedKeys were requested
      if (attributes.empty()) {
        Logger::log(NO_KEYS_TO_REQUEST);
        return false;
      }
      else if (attributeRequestKey == nullptr || attributeResponseKey == nullptr) {
        Logger::log(ATT_KEY_NOT_FOUND);
        return false;
      }
#else
      const char* request = callback.Get_Attributes();

      if (request == nullptr) {
        Logger::log(NO_KEYS_TO_REQUEST);
        return false;
      }
#endif // THINGSBOARD_ENABLE_STL

      // Ensure the response topic has been subscribed
      Attribute_Request_Callback* registeredCallback = nullptr;
      if (!Attributes_Request_Subscribe(callback, registeredCallback)) {
        return false;
      }
      else if (registeredCallback == nullptr) {
        return false;
      }

#if THINGSBOARD_ENABLE_STL
      std::string request;

      for (const char *att : attributes) {
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

      requestObject[attributeRequestKey] = request.c_str();
#else
      requestObject[attributeRequestKey] = request;
#endif // THINGSBOARD_ENABLE_STL

      const size_t objectSize = JSON_STRING_SIZE(measureJson(requestObject));
      char buffer[objectSize];
      if (PayloadSize < objectSize) {
        char message[detectSize(INVALID_BUFFER_SIZE, PayloadSize, objectSize)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, PayloadSize, objectSize);
        Logger::log(message);
        return false;
      }
      // Serialize json does not include size of the string null terminator
      else if (serializeJson(requestObject, buffer, objectSize) < objectSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }

      // Print requested keys
#if THINGSBOARD_ENABLE_STL
      char message[JSON_STRING_SIZE(strlen(REQUEST_ATT)) + request.length() + JSON_STRING_SIZE(strlen(buffer))];
      snprintf_P(message, sizeof(message), REQUEST_ATT, request.c_str(), buffer);
#else
      char message[JSON_STRING_SIZE(strlen(REQUEST_ATT)) + JSON_STRING_SIZE(strlen(request)) + JSON_STRING_SIZE(strlen(buffer))];
      snprintf_P(message, sizeof(message), REQUEST_ATT, request, buffer);
#endif // THINGSBOARD_ENABLE_STL
      Logger::log(message);

      m_requestId++;
      registeredCallback->Set_Request_ID(m_requestId);
      registeredCallback->Set_Attribute_Key(attributeResponseKey);

      char topic[detectSize(ATTRIBUTE_REQUEST_TOPIC, m_requestId)];
      snprintf_P(topic, sizeof(topic), ATTRIBUTE_REQUEST_TOPIC, m_requestId);
      return m_client.publish(topic, buffer, m_qos ? 1 : 0);
    }

    /// @brief Subscribes one provision callback,
    /// that will be called if a provision response from the server is received
    /// @param callback Callback method that will be called
    /// @return Wheter requesting the given callback was successful or not
    inline const bool Provision_Subscribe(const Provision_Callback& callback) {
      if (!m_client.subscribe(PROV_RESPONSE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }
      m_provisionCallback = callback;
      return true;
    }

    /// @brief Unsubcribes the provision callback
    /// @return Wheter unsubcribing the previously subscribed callback
    /// and from the provision response topic, was successful or not
    inline const bool Provision_Unsubscribe() {
      m_provisionCallback = Provision_Callback();
      return m_client.unsubscribe(PROV_RESPONSE_TOPIC);
    }

#if defined(ESP8266) || defined(ESP32)

    /// @brief Checks the included information in the callback,
    /// and attempts to sends the current device firmware information to the cloud
    /// @param callback Callback method that will be called
    /// @return Wheter checking and sending the current device firmware information was successful or not
    inline const bool Prepare_Firmware_Settings(const OTA_Update_Callback& callback) {
      const char *currFwTitle = callback.Get_Firmware_Title();
      const char *currFwVersion = callback.Get_Firmware_Version();

      // Send current firmware version
      if (currFwTitle == nullptr || currFwVersion == nullptr) {
        return false;
      }
      else if (!Firmware_Send_Info(currFwTitle, currFwVersion)) {
        return false;
      }

      // Set private members needed for update
      m_fwCallback = &callback;
      return true;
    }

    /// @brief Subscribes to the firmware response topic
    /// @return Wheter subscribing to the firmware response topic was successful or not
    inline const bool Firmware_OTA_Subscribe() {
      if (!m_client.subscribe(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }
      return true;
    }

    /// @brief Unsubscribes from the firmware response topic and clears any memory associated with the firmware update,
    /// should not be called before actually fully completing the firmware update.
    /// @return Wheter unsubscribing from the firmware response topic was successful or not
    inline const bool Firmware_OTA_Unsubscribe() {
      // Reset now not needed private member variables
      m_fwState = false;
      m_fwCallback = nullptr;
      m_fwSize = 0U;
      m_fwChecksumAlgorithm = mbedtls_md_type_t();
      m_fwChecksum.clear();
      m_fwAlgorithm.clear();
      m_fwChunkReceive = 0U;
      // Unsubscribe from the topic
      return m_client.unsubscribe(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC);
    }

    /// @brief Callback that will be called upon firmware shared attribute arrival
    /// @param data Json data containing key-value pairs for the needed firmware information,
    /// to ensure we have a firmware assigned and can start the update over MQTT
    inline void Firmware_Shared_Attribute_Received(const Shared_Attribute_Data& data) {
      const size_t jsonSize = JSON_STRING_SIZE(measureJson(data));
      char buffer[jsonSize];
      // Serialize json does not include size of the string null terminator
      if (serializeJson(data, buffer, jsonSize) < jsonSize - 1) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return;
      }
      // Print out received firmware shared attributes
      Logger::log(buffer);

      // Check if firmware is available for our device
      if (!data.containsKey(FW_VER_KEY) || !data.containsKey(FW_TITLE_KEY)) {
        Logger::log(NO_FW);
        Firmware_Send_State(FW_STATE_FAILED, NO_FW);
        return;
      }
      else if (m_fwCallback == nullptr) {
        Logger::log(OTA_CB_IS_NULL);
        Firmware_Send_State(FW_STATE_FAILED, OTA_CB_IS_NULL);
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
        Firmware_Send_State(FW_STATE_FAILED, EMPTY_FW);
        return;
      }
      // If firmware version and title is the same, we do not initiate an update, because we expect the binary to be the same one we are currently using
      else if (strncmp_P(currFwTitle, fw_title, JSON_STRING_SIZE(strlen(currFwTitle))) == 0 && strncmp_P(currFwVersion, fw_version, JSON_STRING_SIZE(strlen(currFwVersion))) == 0) {
        Logger::log(FW_UP_TO_DATE);
        Firmware_Send_State(FW_STATE_FAILED, FW_UP_TO_DATE);
        return;
      }
      // If firmware title is not the same, we do not initiate an update, because we expect the binary to be for another device type 
      else if (strncmp_P(currFwTitle, fw_title, JSON_STRING_SIZE(strlen(currFwTitle))) != 0) {
        Logger::log(FW_NOT_FOR_US);
        Firmware_Send_State(FW_STATE_FAILED, FW_NOT_FOR_US);
        return;
      }

      // Change the used firmware algorithm, depending on which type is set for the given firmware information
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
        Firmware_Send_State(FW_STATE_FAILED, message);
        return;
      }

      if (!Firmware_OTA_Subscribe()) {
        return;
      }

      Logger::log(PAGE_BREAK);
      Logger::log(NEW_FW);
      char firmware[JSON_STRING_SIZE(strlen(FROM_TOO)) + JSON_STRING_SIZE(strlen(currFwVersion)) + JSON_STRING_SIZE(strlen(fw_version))];
      snprintf_P(firmware, sizeof(firmware), FROM_TOO, currFwVersion, fw_version);
      Logger::log(firmware);
      Logger::log(DOWNLOADING_FW);

      // Calculate the number of chuncks we need to request,
      // in order to download the complete firmware binary
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
      m_fwState = true;
      Firmware_Send_State(FW_STATE_DOWNLOADING);

      // Convert the interger size into a readable string
      char size[detectSize(NUMBER_PRINTF, chunckSize)];
      snprintf_P(size, sizeof(size), NUMBER_PRINTF, chunckSize);

      // Start the firmware download
      do {
        // Size adjuts dynamically to the current length of the currChunk number to ensure we don't cut it out of the topic string.
        char topic[detectSize(FIRMWARE_REQUEST_TOPIC, currChunk)];
        snprintf_P(topic, sizeof(topic), FIRMWARE_REQUEST_TOPIC, currChunk);
        m_client.publish(topic, size, m_qos ? 1 : 0);

        // Amount of time we wait until we declare the download as failed in milliseconds
        const uint32_t startTime = millis();
        const uint16_t& timeout = m_fwCallback->Get_Timeout();
        do {
          delay(5);
          loop();
        } while ((m_fwChunkReceive != currChunk) && (millis() - startTime < timeout));

        // Check if downloaded the currentChunk timed out, before the packet was received
        if (m_fwChunkReceive != currChunk) {
          retries--;
          if (retries == 0) {
            Logger::log(UNABLE_TO_DOWNLOAD);
            Firmware_Send_State(FW_STATE_FAILED, UNABLE_TO_DOWNLOAD);
            break;
          }
          continue;
        }

        // Check if the current chunk is the last one
        if (numberOfChunk == (currChunk + 1)) {
          currChunk++;
          break;
        }

        // Check if downloading the last chunk failed somehow
        if (!m_fwState) {
          retries--;
          if (retries == 0) {
            Logger::log(UNABLE_TO_WRITE);
            Firmware_Send_State(FW_STATE_FAILED, UNABLE_TO_WRITE);
            break;
          }
          // Reset any possible errors that might have occured and retry them.
          // Until we run out of retries.
          m_fwState = true;
          Firmware_Send_State(FW_STATE_DOWNLOADING);
          continue;
        }

        currChunk++;
        m_fwCallback->Call_Progress_Callback<Logger>(currChunk, numberOfChunk);
        // Current chunck was downloaded successfully, resetting retries accordingly.
        retries = m_fwCallback->Get_Chunk_Retries();
      } while (numberOfChunk != currChunk);

      // Buffer size has been set to another value before the update,
      // to allow to receive ota chunck packets that might be much bigger than the normal
      // buffer size would allow, therefore we return to the previous value to decrease overall memory usage
      if (changeBufferSize) {
        m_client.setBufferSize(previousBufferSize);
      }

      // Send the current firmware information, because the update was succesful
      if (m_fwState) {
        Firmware_Send_State(FW_STATE_UPDATING);
      }
      else {
        Firmware_Send_State(FW_STATE_FAILED);
      }

      m_fwCallback->Call_End_Callback<Logger>(m_fwState);

      // Unsubscribe from now not needed topics anymore,
      // call after everything has been handled, because this method
      // additionaly clears and resets all variables connected to the update itself
      Firmware_OTA_Unsubscribe();
    }

#endif

    /// @brief Connects to the previously set ThingsBoard server, as the given client with the given access token
    /// @param access_token Access token that connects this device with a created device on the ThingsBoard server,
    /// can be "provision", if the device creates itself instead
    /// @param client_id Client username that can be used to differentiate the user that is connecting the given device to ThingsBoard
    /// @param password Client password that can be used to authenticate the user that is connecting the given device to ThingsBoard
    /// @return Wheter connecting to ThingsBoard was successful or not
    inline const bool connect_to_host(const char *access_token, const char *client_id, const char *password) {
      const bool connection_result = m_client.connect(client_id, access_token, password);
      if (!connection_result) {
        Logger::log(CONNECT_FAILED);
        return connection_result;
      }

      this->RPC_Unsubscribe(); // Cleanup all server-side RPC subscriptions
      this->RPC_Request_Unsubscribe(); // Cleanup all client-side RPC requests
      this->Shared_Attributes_Unsubscribe(); // Cleanup all shared attributes subscriptions
      this->Attributes_Request_Unsubscribe(); // Cleanup all client-side or shared attributes requests
      this->Provision_Unsubscribe(); // Cleanup all provision subscriptions
#if defined(ESP8266) || defined(ESP32)
      this->Firmware_OTA_Unsubscribe(); // Cleanup all firmware subscriptions
#endif
      return connection_result;
    }

    /// @brief Returns the length in characters needed for a given value with the given argument string to be displayed completly
    /// @param msg Formating message that the given argument will be inserted into
    /// @param ... Additional arguments that should be inserted into the message at the given points,
    /// see https://cplusplus.com/reference/cstdio/printf/ for more information on the possible arguments
    /// @return Length in characters, needed for the given message with the given values inserted to be displayed completly
    inline const uint8_t detectSize(const char *msg, ...) const {
      va_list args;
      va_start(args, msg);
      // Result is what would have been written if the passed buffer would have been large enough not counting null character,
      // or if an error occured while creating the string a negative number is returned instead. TO ensure this will not crash the system
      // when creating an array with negative size we assert beforehand with a clear error message.
      const int32_t result = JSON_STRING_SIZE(vsnprintf_P(nullptr, 0U, msg, args));
#if THINGSBOARD_ENABLE_STL
      assert(result >= 0);
#else
      if (result < 0) {
        abort();
      }
#endif // THINGSBOARD_ENABLE_STL
      va_end(args);
      return result;
    }

    /// @brief Reserves size for the given amount of items in our internal callback vectors beforehand for performance reasons,
    /// this ensures the internal memory blocks do not have to move if new data is inserted,
    /// because the current memory block is too small to hold the data for all the values
    /// @param reservedSize Size we want to reserve and should not use more if we do,
    /// the internal memory blocks might need to be moved to a new location
    inline void reserve_callback_size(const size_t& reservedSize) {
#if THINGSBOARD_ENABLE_STL
      m_rpcCallbacks.reserve(reservedSize);
      m_rpcRequestCallbacks.reserve(reservedSize);
      m_sharedAttributeUpdateCallbacks.reserve(reservedSize);
      m_attributeRequestCallbacks.reserve(reservedSize);
#endif // THINGSBOARD_ENABLE_STL
    }

    /// @brief Subscribes to the client-side RPC response topic
    /// @param callback Callback method that will be called
    /// @param registeredCallback Editable pointer to a reference of the local version that was copied from the passed callback
    /// @return Wheter requesting the given callback was successful or not
    inline const bool RPC_Request_Subscribe(const RPC_Request_Callback& callback, RPC_Request_Callback*& registeredCallback = nullptr) {
#if THINGSBOARD_ENABLE_STL
      if (m_rpcRequestCallbacks.size() + 1 > m_rpcRequestCallbacks.capacity()) {
        Logger::log(MAX_RPC_REQUEST_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(RPC_RESPONSE_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local vector
      m_rpcRequestCallbacks.push_back(callback);
      registeredCallback = &m_rpcRequestCallbacks.back();
#else
      if (1 + m_rpcIndex > sizeof(m_rpcRequestCallbacks) / sizeof(*m_rpcRequestCallbacks)) {
        Logger::log(MAX_RPC_REQUEST_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(RPC_RESPONSE_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      m_rpcRequestCallbacks[m_rpcIndex] = callback;
      m_rpcIndex++;
      registeredCallback = &m_rpcRequestCallbacks[m_rpcIndex];
#endif // THINGSBOARD_ENABLE_STL
      return true;
    }

    /// @brief Unsubscribes all client-side RPC request callbacks
    /// @return Wheter unsubcribing the previously subscribed callbacks
    /// and from the client-side RPC response topic, was successful or not
    inline const bool RPC_Request_Unsubscribe() {
      // Empty all callbacks
#if THINGSBOARD_ENABLE_STL
      m_rpcRequestCallbacks.clear();
#else
      m_rpcRequestIndex = 0U;
#endif // THINGSBOARD_ENABLE_STL
      return m_client.unsubscribe(RPC_RESPONSE_SUBSCRIBE_TOPIC);
    }

    /// @brief Subscribes to attribute response topic
    /// @param callback Callback method that will be called
    /// @param registeredCallback Editable pointer to a reference of the local version that was copied from the passed callback
    /// @return Wheter requesting the given callback was successful or not
    inline const bool Attributes_Request_Subscribe(const Attribute_Request_Callback& callback, Attribute_Request_Callback*& registeredCallback = nullptr) {
#if THINGSBOARD_ENABLE_STL
      if (m_attributeRequestCallbacks.size() + 1 > m_attributeRequestCallbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_REQUEST_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local vector
      m_attributeRequestCallbacks.push_back(callback);
      registeredCallback = &m_attributeRequestCallbacks.back();
#else
      if (1 + m_attributeRequestIndex > sizeof(m_attributeRequestCallbacks) / sizeof(*m_attributeRequestCallbacks)) {
        Logger::log(MAX_SHARED_ATT_REQUEST_EXCEEDED);
        return false;
      }
      else if (!m_client.subscribe(ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC, m_qos ? 1 : 0)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      m_attributeRequestCallbacks[m_attributeRequestIndex] = callback;
      m_attributeRequestIndex++;
      registeredCallback = &m_attributeRequestCallbacks[m_attributeRequestIndex];
#endif // THINGSBOARD_ENABLE_STL
      return true;
    }

    /// @brief Unsubscribes all client-side or shared attributes request callbacks
    /// @return Wheter unsubcribing the previously subscribed callbacks
    /// and from the  attribute response topic, was successful or not
    inline const bool Attributes_Request_Unsubscribe() {
      // Empty all callbacks
#if THINGSBOARD_ENABLE_STL
      m_attributeRequestCallbacks.clear();
#else
      m_attributeRequestIndex = 0U;
#endif // THINGSBOARD_ENABLE_STL
      return m_client.unsubscribe(ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC);
    }

    /// @brief Attempts to send a single key-value pair with the given key and value of the given type
    /// @tparam T Type of the data value we want to send
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @param telemetry Wheter the data we want to send should be sent as an attribute or telemetry data value
    /// @return Wheter sending the data was successful or not
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

    /// @brief Process callback that will be called upon client-side RPC response arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
    inline void process_rpc_request_message(char *topic, uint8_t *payload, const uint32_t length) {
      // Ensure to have enough size for the infinite amount of possible response values that could be sent by the cloud,
      // therefore we set the size to the MaxFieldsAmt instead of JSON_OBJECT_SIZE(1), which will result in a JsonDocument with a size of 16 bytes
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
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
      const JsonVariantConst data = jsonBuffer.template as<JsonVariantConst>();

      // Remove the not needed part of the topic
      const size_t index = strlen(RPC_RESPONSE_TOPIC) + 1U;
#if THINGSBOARD_ENABLE_STL
      std::string response = topic;
      response = response.substr(index, response.length() - index);
#else
      String response = topic;
      response = response.substring(index);
#endif // THINGSBOARD_ENABLE_STL

      // Convert the remaining text to an integer
      const uint32_t responseId = atoi(response.c_str());

#if THINGSBOARD_ENABLE_STL
      for (size_t i = 0; i < m_rpcRequestCallbacks.size(); i++) {
        const RPC_Request_Callback& callback = m_rpcRequestCallbacks.at(i);
#else
      for (size_t i = 0; i < m_rpcRequestIndex; i++) {
        const RPC_Request_Callback& callback = m_rpcRequestCallbacks[i];
#endif // THINGSBOARD_ENABLE_STL
        if (callback.Get_Request_ID() != responseId) {
          continue;
        }

        char message[detectSize(CALLING_REQUEST_CB, responseId)];
        snprintf_P(message, sizeof(message), CALLING_REQUEST_CB, responseId);
        Logger::log(message);

        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        callback.Call_Callback<Logger>(data);
#if THINGSBOARD_ENABLE_STL
        // Delete callback because the changes have been requested and the callback is no longer needed
        m_rpcRequestCallbacks.erase(std::next(m_rpcRequestCallbacks.begin(), i));
#else
        // Delete callback because the changes have been requested and the callback is no longer needed
        m_rpcRequestCallbacks[i] = RPC_Request_Callback();
#endif // THINGSBOARD_ENABLE_STL
        break;
      }

#if THINGSBOARD_ENABLE_STL
      // Attempt to unsubscribe from the shared attribute request topic,
      // if we are not waiting for any further responses with shared attributes from the server.
      // Will be resubscribed if another request is sent anyway
      if (m_rpcRequestCallbacks.empty()) {
        RPC_Request_Unsubscribe();
      }
#endif // THINGSBOARD_ENABLE_STL
    }

    /// @brief Process callback that will be called upon server-side RPC request arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
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

#if THINGSBOARD_ENABLE_STL
      std::string responseTopic = topic;
      responseTopic.replace(responseTopic.begin(), responseTopic.end(), RPC_REQUEST_KEY, RPC_RESPONSE_KEY);
#else
      String responseTopic = topic;
      responseTopic.replace(RPC_REQUEST_KEY, RPC_RESPONSE_KEY);
#endif // THINGSBOARD_ENABLE_STL

      Logger::log(RPC_RESPONSE_KEY);
      Logger::log(responseTopic.c_str());
      Logger::log(responsePayload);
      m_client.publish(responseTopic.c_str(), responsePayload, m_qos ? 1 : 0);
    }

#if defined(ESP8266) || defined(ESP32)

    /// @brief Process callback that will be called upon firmware response arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callback
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
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
          m_fwState = false;
          Firmware_Send_State(FW_STATE_FAILED, ERROR_UPDATE_BEGIN);
          // Ensure to call Update.abort after calling Update.begin even if it failed,
          // to make sure that any possibly started processes are stopped and freed.
#if defined(ESP32)
          Update.abort();
#endif
          return;
        }
      }

      // Write received binary data to flash partition
      if (Update.write(payload, length) != length) {
        Logger::log(ERROR_UPDATE_WRITE);
#if defined(ESP32)
          Update.abort();
#endif
        m_fwState = false;
        Firmware_Send_State(FW_STATE_FAILED, ERROR_UPDATE_WRITE);
        return;
      }

      // Update value only if writing to flash was a success
      hash.update(payload, length);
      sizeReceive += length;

      // Check if we received the full firmware already
      if (m_fwSize != sizeReceive) {
        return;
      }

      Firmware_Send_State(FW_STATE_DOWNLOADED);

      const std::string calculatedHash = hash.get_hash_string();
      char actual[JSON_STRING_SIZE(strlen(HASH_ACTUAL)) + JSON_STRING_SIZE(m_fwAlgorithm.size()) + JSON_STRING_SIZE(calculatedHash.size())];
      snprintf_P(actual, sizeof(actual), HASH_ACTUAL, m_fwAlgorithm.c_str(), calculatedHash.c_str());
      Logger::log(actual);

      char expected[JSON_STRING_SIZE(strlen(HASH_EXPECTED)) + JSON_STRING_SIZE(m_fwAlgorithm.size()) + JSON_STRING_SIZE(m_fwChecksum.size())];
      snprintf_P(expected, sizeof(expected), HASH_EXPECTED, m_fwAlgorithm.c_str(), m_fwChecksum.c_str());
      Logger::log(expected);

      // Check if the initally received checksum is the same as the one we calculated from the received binary data,
      // if not we assume the binary data has been changed or not completly downloaded --> Firmware update failed
      if (m_fwChecksum.compare(calculatedHash) != 0) {
        Logger::log(CHKS_VER_FAILED);
#if defined(ESP32)
        Update.abort();
#endif
        m_fwState = false;
        Firmware_Send_State(FW_STATE_FAILED, CHKS_VER_FAILED);
        return;
      }

      Logger::log(CHKS_VER_SUCCESS);
      if (!Update.end()) {
        Logger::log(ERROR_UPDATE_END);
        m_fwState = false;
        Firmware_Send_State(FW_STATE_FAILED, ERROR_UPDATE_END);
        return;
      }
      Logger::log(FW_UPDATE_SUCCESS);
      m_fwState = true;
      Firmware_Send_State(FW_STATE_VERIFIED);
    }

#endif

    /// @brief Process callback that will be called upon shared attribute update arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
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

      if (data && (data.size() < 1)) {
        Logger::log(NOT_FOUND_ATT_UPDATE);
        return;
      }

      Logger::log(RECEIVED_ATT_UPDATE);
      if (data.containsKey(SHARED_RESPONSE_KEY)) {
        data = data[SHARED_RESPONSE_KEY];
      }

      for (const Shared_Attribute_Callback& callback : m_sharedAttributeUpdateCallbacks) {
#if THINGSBOARD_ENABLE_STL
        if (callback.Get_Attributes().empty()) {
#else
        if (callback.Get_Attributes() == nullptr) {
#endif // THINGSBOARD_ENABLE_STL
          Logger::log(ATT_CB_NO_KEYS);
          // No specifc keys were subscribed so we call the callback anyway.
          callback.Call_Callback<Logger>(data);
          continue;
        }

        bool containsKey = false;
        const char *requested_att = nullptr;

#if THINGSBOARD_ENABLE_STL
        for (const char *att : callback.Get_Attributes()) {
#else
        String stringToSplit = callback.Get_Attributes();
        int32_t previousIndex = 0;
        int32_t currentIndex = 0;

        do {
          // Find the index of the next comma in the string
          currentIndex = stringToSplit.indexOf(COMMA, previousIndex);
        
          // If no more commas are found, set the currentIndex to the end of the string
          if (currentIndex == -1) {
            currentIndex = stringToSplit.length();
          }

          // Extract the attribute from the original string
          const char *att = stringToSplit.substring(previousIndex, currentIndex).c_str();
#endif // THINGSBOARD_ENABLE_STL
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
#if THINGSBOARD_ENABLE_STL
        }
#else
          previousIndex = currentIndex + 1;
        } while (currentIndex != stringToSplit.length());
#endif // THINGSBOARD_ENABLE_STL

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
        callback.Call_Callback<Logger>(data);
      }
    }

    /// @brief Process callback that will be called upon client-side or shared attribute request arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
    inline void process_attribute_request_message(char *topic, uint8_t *payload, const uint32_t length) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      const DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        char message[detectSize(UNABLE_TO_DE_SERIALIZE_JSON, error.c_str())];
        snprintf_P(message, sizeof(message), UNABLE_TO_DE_SERIALIZE_JSON, error.c_str());
        Logger::log(message);
        return;
      }
      JsonObjectConst data = jsonBuffer.template as<JsonObjectConst>();

      // Remove the not needed part of the topic
      const size_t index = strlen(ATTRIBUTE_RESPONSE_TOPIC) + 1U;
#if THINGSBOARD_ENABLE_STL
      std::string response = topic;
      response = response.substr(index, response.length() - index);
#else
      String response = topic;
      response = response.substring(index);
#endif // THINGSBOARD_ENABLE_STL

      // Convert the remaining text to an integer
      const uint32_t response_id = atoi(response.c_str());

      char message[detectSize(CALLING_REQUEST_CB, response_id)];
#if THINGSBOARD_ENABLE_STL
      for (size_t i = 0; i < m_attributeRequestCallbacks.size(); i++) {
        const Attribute_Request_Callback& callback = m_attributeRequestCallbacks.at(i);
#else
      for (size_t i = 0; i < m_attributeRequestIndex; i++) {
        const Attribute_Request_Callback& callback = m_attributeRequestCallbacks[i];
#endif // THINGSBOARD_ENABLE_STL
        if (callback.Get_Request_ID() != response_id) {
          continue;
        }
        const char *attributeResponseKey = callback.Get_Attribute_Key();
        if (attributeResponseKey == nullptr) {
          Logger::log(ATT_KEY_NOT_FOUND);
          goto delete_callback;
        }
        else if (!data || (data.size() < 0)) {
          Logger::log(ATT_KEY_NOT_FOUND);
          goto delete_callback;
        }

        Logger::log(RECEIVED_ATT);
        if (data.containsKey(attributeResponseKey)) {
          data = data[attributeResponseKey];
        }

        snprintf_P(message, sizeof(message), CALLING_REQUEST_CB, response_id);
        Logger::log(message);
        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        callback.Call_Callback<Logger>(data);

        delete_callback:
#if THINGSBOARD_ENABLE_STL
        // Delete callback because the changes have been requested and the callback is no longer needed
        m_attributeRequestCallbacks.erase(std::next(m_attributeRequestCallbacks.begin(), i));
#else
        // Delete callback because the changes have been requested and the callback is no longer needed
        m_attributeRequestCallbacks[i] = Attribute_Request_Callback();
#endif // THINGSBOARD_ENABLE_STL
        break;
      }

#if THINGSBOARD_ENABLE_STL
      // Unsubscribe from the shared attribute request topic,
      // if we are not waiting for any further responses with shared attributes from the server.
      // Will be resubscribed if another request is sent anyway
      if (m_attributeRequestCallbacks.empty()) {
        Attributes_Request_Unsubscribe();
      }
#endif // THINGSBOARD_ENABLE_STL
    }

    /// @brief Process callback that will be called upon provision response arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callback
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
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
      m_provisionCallback.Call_Callback<Logger>(data);

      // Unsubscribe from the provision response topic,
      // Will be resubscribed if another request is sent anyway
      Provision_Unsubscribe();
    }

    /// @brief Attempts to send aggregated attribute or telemetry data
    /// @param data Array containing all the data we want to send
    /// @param data_count Amount of data entries in the array that we want to send
    /// @param telemetry Wheter the data we want to send should be sent as an attribute or telemetry data value
    /// @return Wheter sending the data was successful or not
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

#if THINGSBOARD_ENABLE_STL
    // Vectors hold copy of the actual passed data, this is to ensure they stay valid,
    // even if the user only temporarily created the object before the method was called.
    // This can be done because all Callback methods mostly consists of pointers to actual object so copying them
    // does not require a huge memory overhead and is acceptable especially in comparsion to possible problems that could
    // arise if references were used and the end user does not take care to ensure the Callbacks life on for the entirety
    // of its usage, which will lead to dangling references and undefined behaviour
    std::vector<RPC_Callback> m_rpcCallbacks; // Server side RPC callbacks vector
    std::vector<RPC_Request_Callback> m_rpcRequestCallbacks; // Client side RPC callbacks vector
    std::vector<Shared_Attribute_Callback> m_sharedAttributeUpdateCallbacks; // Shared attribute update callbacks vector
    std::vector<Attribute_Request_Callback> m_attributeRequestCallbacks; // Client-side or shared attribute request callbacks vector
#else
    size_t m_rpcIndex;
    RPC_Callback m_rpcCallbacks[MaxFieldsAmt]; // Server side RPC callbacks c-style array
    size_t m_rpcRequestIndex;
    RPC_Request_Callback m_rpcRequestCallbacks[MaxFieldsAmt]; // Client side RPC callbacks c-style array
    size_t m_sharedAttributeIndex;
    Shared_Attribute_Callback m_sharedAttributeUpdateCallbacks[MaxFieldsAmt]; // Shared attribute update callbacks c-style array
    size_t m_attributeRequestIndex;
    Attribute_Request_Callback m_attributeRequestCallbacks[MaxFieldsAmt]; // Client-side or shared attribute request callbacks c-style array
#endif

    Provision_Callback m_provisionCallback; // Provision response callback
    uint32_t m_requestId; // Allows nearly 4.3 million requests before wrapping back to 0.
    bool m_qos; // Wheter QoS level 1 should be enabled or disabled (Resends the packet until the message was received and a PUBACK packet was returned).

#if defined(ESP8266) || defined(ESP32)

    bool m_fwState;
    const OTA_Update_Callback* m_fwCallback;
    // Allows for a binary size of up to theoretically 4 GB.
    uint32_t m_fwSize;
    mbedtls_md_type_t m_fwChecksumAlgorithm;
    std::string m_fwAlgorithm;
    std::string m_fwChecksum;
    const std::vector<const char *> m_fwSharedKeys;
    const Attribute_Request_Callback m_fwRequestCallback;
    const Shared_Attribute_Callback m_fwUpdateCallback;
    uint16_t m_fwChunkReceive;

#endif

    /// @brief MQTT callback that will be called if a publish message is received from the server
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
    inline void onMQTTMessage(char *topic, uint8_t *payload, uint32_t length) {
      char message[JSON_STRING_SIZE(strlen(CB_ON_MESSAGE)) + JSON_STRING_SIZE(strlen(topic))];
      snprintf_P(message, sizeof(message), CB_ON_MESSAGE, topic);
      Logger::log(message);

      if (strncmp_P(RPC_RESPONSE_TOPIC, topic, strlen(RPC_RESPONSE_TOPIC)) == 0) {
        process_rpc_request_message(topic, payload, length);
      } else if (strncmp_P(RPC_TOPIC, topic, strlen(RPC_TOPIC)) == 0) {
        process_rpc_message(topic, payload, length);
      } else if (strncmp_P(ATTRIBUTE_RESPONSE_TOPIC, topic, strlen(ATTRIBUTE_RESPONSE_TOPIC)) == 0) {
        process_attribute_request_message(topic, payload, length);
      } else if (strncmp_P(ATTRIBUTE_TOPIC, topic, strlen(ATTRIBUTE_TOPIC)) == 0) {
        process_shared_attribute_update_message(topic, payload, length);
      } else if (strncmp_P(PROV_RESPONSE_TOPIC, topic, strlen(PROV_RESPONSE_TOPIC)) == 0) {
        process_provisioning_response(topic, payload, length);
      } else if (strncmp_P(FIRMWARE_RESPONSE_TOPIC, topic, strlen(FIRMWARE_RESPONSE_TOPIC)) == 0) {
#if defined(ESP8266) || defined(ESP32)
        process_firmware_response(topic, payload, length);
#endif
      }
    }

#if !THINGSBOARD_ENABLE_STL

    // PubSub client cannot call a method when message arrives on subscribed topic.
    // Only free-standing function is allowed.
    // To be able to forward event to an instance, rather than to a function, this pointer exists.
    static ThingsBoardSized *m_subscribedInstance;

    static void onStaticMQTTMessage(char *topic, uint8_t *payload, unsigned int length) {
      if (m_subscribedInstance == nullptr) {
        return;
      }
      m_subscribedInstance->onMQTTMessage(topic, payload, length);
    }

#endif // !THINGSBOARD_ENABLE_STL

};

#if !THINGSBOARD_ENABLE_STL

template<size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
ThingsBoardSized<PayloadSize, MaxFieldsAmt, Logger> *ThingsBoardSized<PayloadSize, MaxFieldsAmt, Logger>::m_subscribedInstance = nullptr;

#endif // !THINGSBOARD_ENABLE_STL

using ThingsBoard = ThingsBoardSized<>;

#endif // ThingsBoard_h
