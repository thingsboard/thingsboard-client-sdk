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
    bool serializeKeyval(JsonVariant &jsonObj) const;
};

// Convenient aliases

using Attribute = Telemetry;
using RPC_Response = Telemetry;
// JSON variant const (read only twice as small as JSON variant), is used to communicate RPC parameters to the client
using RPC_Data = JsonVariantConst;
// JSON object const (read only twice as small as JSON object), is used to communicate Shared Attributes and Provision Data to the client
using Shared_Attribute_Data = JsonObjectConst;
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
using Provision_Data = JsonObjectConst;
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
    inline ThingsBoardSized(Client &client)
      : m_client(client)
      , m_requestId(0)
    {
      m_client.setBufferSize(PayloadSize);
      // Reserve size of both m_sharedAttributeUpdateCallbacks, rpcCallbacks and m_sharedAttributeRequestCallbacks beforehand for performance reasons.
      m_rpcCallbacks.reserve(MaxFieldsAmt);
      m_sharedAttributeUpdateCallbacks.reserve(MaxFieldsAmt);
      m_sharedAttributeRequestCallbacks.reserve(MaxFieldsAmt);
    }

    // Destroys ThingsBoardSized class with network client.
    inline ~ThingsBoardSized() {
      // Nothing to do.
    }

    // Returns a reference to the PubSubClient.
    inline PubSubClient& getClient(void) {
      return m_client;
    }

    // Sets both subscribed instance and callback, this is out of the same reason as not initalizing certain,
    // private members in the constructor initalizor list and that would be because if multiple instances are used,
    // one will overwrite the other and cause server respones to not be registered and forwarded anymore.
    inline void setup_callback(void) {
      ThingsBoardSized::m_subscribedInstance = this;
      m_client.setCallback(ThingsBoardSized::on_message);
    }

    // Connects to the specified ThingsBoard server and port.
    // Access token is used to authenticate a client.
    // Returns true on success, false otherwise.
    bool connect(const char *host, const char *access_token = "provision", int port = 1883, const char *client_id = "TbDev", const char *password = NULL) {
      if (!host) {
        return false;
      }
      this->RPC_Unsubscribe(); // Cleanup all RPC subscriptions
      this->Shared_Attributes_Unsubscribe(); // Cleanup all shared attributes subscriptions
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
      this->Provision_Unsubscribe();
#endif
      m_client.setServer(host, port);
      bool connection_result = m_client.connect(client_id, access_token, password);
      return connection_result;
    }

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

    //----------------------------------------------------------------------------
    // Claiming API

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)

    bool sendClaimingRequest(const char *secretKey, unsigned int durationMs) {
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> requestBuffer;
      JsonObject resp_obj = requestBuffer.to<JsonObject>();

      resp_obj["secretKey"] = secretKey;
      resp_obj["durationMs"] = durationMs;

      uint8_t objectSize = JSON_STRING_SIZE(measureJson(requestBuffer));
      char responsePayload[objectSize];
      serializeJson(resp_obj, responsePayload, objectSize);

      return m_client.publish("v1/devices/me/claim", responsePayload);
    }

    // Provisioning API
    bool sendProvisionRequest(const char* deviceName, const char* provisionDeviceKey, const char* provisionDeviceSecret) {
      // TODO Add ability to provide specific credentials from client side.
      StaticJsonDocument<JSON_OBJECT_SIZE(3)> requestBuffer;
      JsonObject requestObject = requestBuffer.to<JsonObject>();

      requestObject["deviceName"] = deviceName;
      requestObject["provisionDeviceKey"] = provisionDeviceKey;
      requestObject["provisionDeviceSecret"] = provisionDeviceSecret;

      uint8_t objectSize = JSON_STRING_SIZE(measureJson(requestBuffer));
      char requestPayload[objectSize];
      serializeJson(requestObject, requestPayload, objectSize);

      Logger::log("Provision request:");
      Logger::log(requestPayload);
      return m_client.publish("/provision/request", requestPayload);
    }

#endif
    //----------------------------------------------------------------------------
    // Telemetry API

    // Sends telemetry data to the ThingsBoard, returns true on success.
    template<class T>
    inline bool sendTelemetryData(const char *key, T value)
    {
      return sendKeyval(key, value);
    }

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

    // Sends custom JSON telemetry JsonObject to the ThingsBoard.
    inline bool sendTelemetryJson(const JsonObject& jsonObject) {
      uint32_t json_size = JSON_STRING_SIZE(measureJson(jsonObject));
      char json[json_size];
      serializeJson(jsonObject, json, json_size);
      return m_client.publish("v1/devices/me/telemetry", json);
    }

    //----------------------------------------------------------------------------
    // Attribute API

    // Sends an attribute with given name and value.
    template<class T>
    inline bool sendAttributeData(const char *attrName, T value) {
      return sendKeyval(attrName, value, false);
    }

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

    // Sends custom JsonObject with attributes to the ThingsBoard.
    inline bool sendAttributeJSON(const JsonObject& jsonObject) {
      uint32_t json_size = JSON_STRING_SIZE(measureJson(jsonObject));
      char json[json_size];
      serializeJson(jsonObject, json, json_size);
      return m_client.publish("v1/devices/me/attributes", json);
    }

    //----------------------------------------------------------------------------
    // Server-side RPC API

    // Subscribes multiple RPC callbacks.
    bool RPC_Subscribe(const std::vector<RPC_Callback>& callbacks) {
      if (m_rpcCallbacks.size() + callbacks.size() > m_rpcCallbacks.capacity()) {
        Logger::log("Too many rpc subscriptions, increase MaxFieldsAmt or unsubscribe.");
        return false;
      }
      if (!m_client.subscribe("v1/devices/me/rpc/request/+")) {
        return false;
      }

      // Push back complete vector into our local m_rpcCallbacks vector.
      m_rpcCallbacks.insert(m_rpcCallbacks.end(), callbacks.begin(), callbacks.end());
      return true;
    }

    // Subscribe one RPC callback.
    bool RPC_Subscribe(const RPC_Callback& callback) {
      if (m_rpcCallbacks.size() + 1 > m_rpcCallbacks.capacity()) {
        Logger::log("Too many rpc subscriptions, increase MaxFieldsAmt or unsubscribe.");
        return false;
      }
      if (!m_client.subscribe("v1/devices/me/rpc/request/+")) {
        return false;
      }

      // Push back given callback into our local m_rpcCallbacks vector.
      m_rpcCallbacks.push_back(callback);
      return true;
    }

    inline bool RPC_Unsubscribe() {
      // Empty all callbacks.
      m_rpcCallbacks.clear();
      return m_client.unsubscribe("v1/devices/me/rpc/request/+");
    }

    //----------------------------------------------------------------------------
    // Firmware OTA API

#if defined(ESP8266) || defined(ESP32)
    bool Firmware_Update(const char* currFwTitle, const char* currFwVersion) {
      m_fwState.clear();
      m_fwTitle.clear();
      m_fwVersion.clear();

      // Send current firmware version
      if (!Firmware_Send_FW_Info(currFwTitle, currFwVersion)) {
        return false;
      }

      // Update state
      Firmware_Send_State("CHECKING FIRMWARE");

      // Request the firmware informations
      const std::vector<const char*> sharedKeys {"fw_checksum", "fw_checksum_algorithm", "fw_size", "fw_title", "fw_version"};
      if (!Shared_Attributes_Request(sharedKeys, Shared_Attribute_Callback())) {
        return false;
      }

      // Wait receive m_fwVersion and m_fwTitle
      unsigned long timeout = millis() + 3000;
      do {
        delay(5);
        loop();
      } while (m_fwVersion.isEmpty() && m_fwTitle.isEmpty() && (timeout >= millis()));

      // Check if firmware is available for our device
      if (m_fwVersion.isEmpty() || m_fwTitle.isEmpty()) {
        Logger::log("No firmware found !");
        Firmware_Send_State("NO FIRMWARE FOUND");
        return false;
      }

      // If firmware is the same, we do not update it
      if ((String(currFwTitle) == m_fwTitle) and (String(currFwVersion) == m_fwVersion)) {
        Logger::log("Firmware is already up to date !");
        Firmware_Send_State("UP TO DATE");
        return false;
      }

      // If firmware title is not the same, we quit now
      if (String(currFwTitle) != m_fwTitle) {
        Logger::log("Firmware is not for us (title is different) !");
        Firmware_Send_State("NO FIRMWARE FOUND");
        return false;
      }

      if (m_fwChecksumAlgorithm != "MD5") {
        Logger::log("Checksum algorithm is not supported, please use MD5 only");
        Firmware_Send_State("CHKS IS NOT MD5");
        return false;
      }

      Logger::log("=================================");
      Logger::log("A new Firmware is available :");
      Logger::log(String(String(currFwVersion) + " => " + m_fwVersion).c_str());
      Logger::log("Try to download it...");

      int chunkSize = 4096; // maybe less if we don't have enough RAM
      int numberOfChunk = static_cast<int>(m_fwSize/chunkSize) + 1;
      int currChunk = 0;
      int nbRetry = 3;

      // Increase size of receive buffer
      if (!m_client.setBufferSize(chunkSize + 50)) {
        Logger::log("Not enough RAM");
        return false;
      }

      // Update state
      Firmware_Send_State("DOWNLOADING");

      // Download the firmware
      do {
        m_client.publish(String("v2/fw/request/0/chunk/" + String(currChunk)).c_str(), String(chunkSize).c_str());

        timeout = millis() + 3000; // Amount of time we wait until we declare the download as failed in milliseconds.
        do {
          delay(5);
          loop();
        } while ((m_fwChunkReceive != currChunk) && (timeout >= millis()));

        if (m_fwChunkReceive == currChunk) {
          // Check if chunk is not the last
          if (numberOfChunk != (currChunk + 1))
          {
            // Check if state is OK
            if ((m_fwState == "DOWNLOADING")) {
              currChunk++;
            }
            else {
              nbRetry--;
              if (nbRetry == 0) {
                Logger::log("Unable to write firmware");
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
            Logger::log("Unable to download firmware");
            break;
          }
        }

      } while (numberOfChunk != currChunk);

      // Update current_fw_title and current_fw_version if updating was a success.
      if (m_fwState == "SUCCESS") {
        Firmware_Send_FW_Info(m_fwTitle.c_str(), m_fwVersion.c_str());
        Firmware_Send_State(m_fwState.c_str());
        return true;
      }
      else {
        Firmware_Send_State("FAILED");
      }
      return false;
    }

    bool Firmware_Send_FW_Info(const char* currFwTitle, const char* currFwVersion) {
      // Send our firmware title and version
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> currentFirmwareInfo;
      JsonObject currentFirmwareInfoObject = currentFirmwareInfo.to<JsonObject>();

      currentFirmwareInfoObject["current_fw_title"] = currFwTitle;
      currentFirmwareInfoObject["current_fw_version"] = currFwVersion;
      return sendTelemetryJson(currentFirmwareInfoObject);
    }

    bool Firmware_Send_State(const char* currFwState) {
      // Send our firmware title and version
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> currentFirmwareState;
      JsonObject currentFirmwareStateObject = currentFirmwareState.to<JsonObject>();

      currentFirmwareStateObject["current_fw_state"] = currFwState;
      return sendTelemetryJson(currentFirmwareStateObject);
    }

    bool Firmware_OTA_Subscribe() {
      if (!m_client.subscribe("v2/fw/response/#")) {
        return false;
      }

      return true;
    }

    bool Firmware_OTA_Unsubscribe() {
      if (!m_client.unsubscribe("v2/fw/response/#")) {
        return false;
      }

      return true;
    }
#endif

    //----------------------------------------------------------------------------
    // Shared attributes API

    bool Shared_Attributes_Request(const std::vector<const char*>& att, Shared_Attribute_Request_Callback& callback) {
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> requestBuffer;
      JsonObject requestObject = requestBuffer.to<JsonObject>();

      std::string sharedKeys = "";
      for (const char* attribute : att) {
        // Check if the given attribute is null, if it is skip it.
        if (attribute == nullptr) {
          continue;
        }
        sharedKeys.append(attribute);
        sharedKeys.append(",");
      }

      // Check if any sharedKeys were requested.
      if (sharedKeys.empty()) {
        Logger::log("No keys to request were given.");
        return false;
      }

      // Remove latest not needed ,
      sharedKeys.pop_back();

      requestObject["sharedKeys"] = sharedKeys.c_str();
      int objectSize = measureJson(requestBuffer) + 1;
      char buffer[objectSize];
      serializeJson(requestObject, buffer, objectSize);

      m_requestId++;
      callback.m_request_id = m_requestId;
      Shared_Attributes_Request_Subscribe(callback);
      return m_client.publish(String("v1/devices/me/attributes/request/" + String(m_requestId)).c_str(), buffer);
    }

    // Subscribes multiple Shared attributes callbacks.
    bool Shared_Attributes_Subscribe(const std::vector<Shared_Attribute_Callback>& callbacks) {
      if (m_sharedAttributeUpdateCallbacks.size() + callbacks.size() > m_sharedAttributeUpdateCallbacks.capacity()) {
        Logger::log("Too many shared attribute update callback subscriptions, increase MaxFieldsAmt or unsubscribe.");
        return false;
      }
      if (!m_client.subscribe("v1/devices/me/attributes")) {
        return false;
      }

      // Push back complete vector into our local m_sharedAttributeUpdateCallbacks vector.
      m_sharedAttributeUpdateCallbacks.insert(m_sharedAttributeUpdateCallbacks.end(), callbacks.begin(), callbacks.end());
      return true;
    }

    // Subscribe one Shared attributes callback.
    bool Shared_Attributes_Subscribe(const Shared_Attribute_Callback& callback) {
      if (m_sharedAttributeUpdateCallbacks.size() + 1 > m_sharedAttributeUpdateCallbacks.capacity()) {
        Logger::log("Too many shared attribute update callback subscriptions, increase MaxFieldsAmt or unsubscribe.");
        return false;
      }
      if (!m_client.subscribe("v1/devices/me/attributes")) {
        return false;
      }

      // Push back given callback into our local m_sharedAttributeUpdateCallbacks vector.
      m_sharedAttributeUpdateCallbacks.push_back(callback);
      return true;
    }

    inline bool Shared_Attributes_Unsubscribe() {
      // Empty all callbacks.
      m_sharedAttributeUpdateCallbacks.clear();
      if (!m_client.unsubscribe("v1/devices/me/attributes")) {
        return false;
      }
      return true;
    }

    // -------------------------------------------------------------------------------
    // Provisioning API

    // Subscribes to get provision response

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
    bool Provision_Subscribe(const Provision_Callback callback) {
      if (!m_client.subscribe("/provision/response")) {
        return false;
      }
      m_provisionCallback = callback;
      return true;
    }

    bool Provision_Unsubscribe() {
      if (!m_client.unsubscribe("/provision/response")) {
        return false;
      }
      return true;
    }
#endif

  private:

    // Subscribe one Shared attributes request callback.
    bool Shared_Attributes_Request_Subscribe(const Shared_Attribute_Request_Callback& callback) {
      if (m_sharedAttributeRequestCallbacks.size() + 1 > m_sharedAttributeRequestCallbacks.capacity()) {
        Logger::log("Too many shared attribute request callback subscriptions, increase MaxFieldsAmt.");
        return false;
      }
      if (!m_client.subscribe("v1/devices/me/attributes/response/+")) {
        return false;
      }

      // Push back given callback into our local m_sharedAttributeRequestCallbacks vector.
      m_sharedAttributeRequestCallbacks.push_back(callback);
      return true;
    }

    // Sends single key-value in a generic way.
    template<typename T>
    bool sendKeyval(const char *key, T value, bool telemetry = true) {
      Telemetry t(key, value);

      char payload[PayloadSize];
      {
        Telemetry t(key, value);
        StaticJsonDocument<JSON_OBJECT_SIZE(1)>jsonBuffer;
        JsonVariant object = jsonBuffer.template to<JsonVariant>();
        if (t.serializeKeyval(object) == false) {
          Logger::log("unable to serialize data");
          return false;
        }

        if (JSON_STRING_SIZE(measureJson(jsonBuffer)) > PayloadSize) {
          Logger::log("too small buffer for JSON data");
          return false;
        }
        serializeJson(object, payload, sizeof(payload));
      }
      return telemetry ? sendTelemetryJson(payload) : sendAttributeJSON(payload);
    }

    // Processes RPC message
    void process_rpc_message(char* topic, uint8_t* payload, unsigned int length) {
      RPC_Response r;
      {
        StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
        DeserializationError error = deserializeJson(jsonBuffer, payload, length);
        if (error) {
          Logger::log("unable to de-serialize RPC");
          return;
        }
        const JsonObject &data = jsonBuffer.template as<JsonObject>();
        const char *methodName = data["method"];
        const char *params = data["params"];

        if (methodName) {
          Logger::log("received RPC:");
          Logger::log(methodName);
        } else {
          Logger::log("RPC method is NULL");
          return;
        }

        for (const RPC_Callback& callback : m_rpcCallbacks) {
          // Strcmp returns the ascis value difference of the ascii characters that are different,
          // meaning 0 is the same string and less and more than 0 is the difference in ascci values between the 2 chararacters.
          if (callback.m_cb == nullptr || strcmp(callback.m_name, methodName) != 0) {
            continue;
          }

          Logger::log("calling RPC:");
          Logger::log(methodName);
          // Do not inform client, if parameter field is missing for some reason
          if (!data.containsKey("params")) {
            Logger::log("no parameters passed with RPC, passing null JSON");
          }

          // try to de-serialize params
          StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> doc;
          DeserializationError err_param = deserializeJson(doc, params);
          //if failed to de-serialize params then send JsonObject instead
          if (err_param) {
            Logger::log("params:");
            Logger::log(data["params"].as<String>().c_str());
            r = callback.m_cb(data["params"]);
          } else {
            Logger::log("params:");
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
        Logger::log("unable to serialize data");
        return;
      }

      if (JSON_STRING_SIZE(measureJson(respBuffer)) > PayloadSize) {
        Logger::log("too small buffer for JSON data");
        return;
      }
      serializeJson(resp_obj, responsePayload, sizeof(responsePayload));

      String responseTopic = String(topic);
      responseTopic.replace("request", "response");
      Logger::log("response:");
      Logger::log(responsePayload);
      m_client.publish(responseTopic.c_str(), responsePayload);
    }

#if defined(ESP8266) || defined(ESP32)

    // Processes firmware response
    void process_firmware_response(char* topic, uint8_t* payload, unsigned int length) {
      static unsigned int sizeReceive = 0;
      static MD5Builder md5;

      m_fwChunkReceive = atoi(strrchr(topic, '/') + 1);
      Logger::log(String("Receive chunk " + String(m_fwChunkReceive) + ", size " + String(length) + " bytes").c_str());

      m_fwState = "FAILED";

      if (m_fwChunkReceive == 0) {
        sizeReceive = 0;
        md5 = MD5Builder();
        md5.begin();

        // Initialize Flash
        if (!Update.begin(m_fwSize)) {
          Logger::log("Error during Update.begin");
          m_fwState = "UPDATE ERROR";
          return;
        }
      }

      // Write data to Flash
      if (Update.write(payload, length) != length) {
        Logger::log("Error during Update.write");
        m_fwState = "UPDATE ERROR";
        return;
      }

      // Update value only if write flash success
      md5.add(payload, length);
      sizeReceive += length;

      // Receive Full Firmware
      if (m_fwSize == sizeReceive) {
        md5.calculate();
        String md5Str = md5.toString();
        Logger::log(String("md5 compute:  " + md5Str).c_str());
        Logger::log(String("md5 firmware: " + m_fwChecksum).c_str());
        // Check MD5
        if (md5Str != m_fwChecksum) {
          Logger::log("Checksum verification failed !");
#if defined(ESP32)
          Update.abort();
#endif
          m_fwState = "CHECKSUM ERROR";
        }
        else {
          Logger::log("Checksum is OK !");
          if (Update.end(true)) {
            Logger::log("Update Success !");
            m_fwState = "SUCCESS";
          }
        }
      }
    }
#endif

    // Processes shared attribute update message
    void process_shared_attribute_update_message(char* topic, uint8_t* payload, unsigned int length) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        Logger::log("Unable to de-serialize Shared attribute update request");
        return;
      }
      JsonObject data = jsonBuffer.template as<JsonObject>();

      if (data && (data.size() >= 1)) {
        Logger::log("Received shared attribute update request");
        if (data["shared"]) {
          data = data["shared"];
        }
      } else {
        Logger::log("Shared attribute update key not found.");
        return;
      }

      for (size_t i = 0; i < m_sharedAttributeUpdateCallbacks.size(); i++) {
        if (m_sharedAttributeUpdateCallbacks.at(i).m_cb == nullptr) {
          continue;
        }
        else if (m_sharedAttributeUpdateCallbacks.at(i).m_att.empty()) {
          // No specifc keys were subscribed so we call the callback anyway.
          m_sharedAttributeUpdateCallbacks.at(i).m_cb(data);
          continue;
        }

        bool containsKey = false;
        String requested_att;
        for (const char* att : m_sharedAttributeUpdateCallbacks.at(i).m_att) {
          if (att == nullptr) {
            continue;
          }
          // Check if the request contained any of our requested keys.
          containsKey = containsKey || data.containsKey(att);
          // Break early if the key was requested from this callback.
          if (containsKey) {
            requested_att = att;
            break;
          }
        }

        // This callback did not request any keys that were in this response,
        // therefore we continue with the next element in the loop.
        if (!containsKey) {
          continue;
        }

        Logger::log(String("Calling callback for updated attribute " + requested_att).c_str());
        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        m_sharedAttributeUpdateCallbacks.at(i).m_cb(data);
      }
    }

    // Processes shared attribute request message
    void process_shared_attribute_request_message(char* topic, uint8_t* payload, unsigned int length) {
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        Logger::log("Unable to de-serialize Shared attribute request");
        return;
      }
      JsonObject data = jsonBuffer.template as<JsonObject>();

      if (data && (data.size() >= 1)) {
        Logger::log("Received shared attribute request");
        if (data["shared"]) {
          data = data["shared"];
        }
      } else {
        Logger::log("Shared attribute key not found.");
        return;
      }

      // Save data for firmware update
#if defined(ESP8266) || defined(ESP32)
      if (data["fw_title"]) {
        m_fwTitle = data["fw_title"].as<String>();
      }
      if (data["fw_version"]) {
        m_fwVersion = data["fw_version"].as<String>();
      }
      if (data["fw_checksum"]) {
        m_fwChecksum = data["fw_checksum"].as<String>();
      }
      if (data["fw_checksum_algorithm"]) {
        m_fwChecksumAlgorithm = data["fw_checksum_algorithm"].as<String>();
      }
      if (data["fw_size"]) {
        m_fwSize = data["fw_size"].as<int>();
      }
#endif

      std::string response = topic;
      // Remove the not needed part of the topic.
      size_t index = strlen("v1/devices/me/attributes/response/");
      response = response.substr(index, response.length() - index);
      // convert the remaining text to an integer
      uint32_t response_id = atoi(response.c_str());

      for (size_t i = 0; i < m_sharedAttributeRequestCallbacks.size(); i++) {
        if (m_sharedAttributeRequestCallbacks.at(i).m_cb == nullptr) {
          continue;
        }
        else if (m_sharedAttributeRequestCallbacks.at(i).m_request_id != response_id) {
          continue;
        }

        Logger::log((String("Calling callback for reponse id ") + String(response_id)).c_str());
        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        m_sharedAttributeRequestCallbacks.at(i).m_cb(data);
        // Delete callback because the changes have been requested and the callback is no longer needed.
        m_sharedAttributeRequestCallbacks.erase(std::next(m_sharedAttributeRequestCallbacks.begin(), i));
      }
    }

    // Processes provisioning response
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
    void process_provisioning_response(char* topic, uint8_t* payload, unsigned int length) {
      Logger::log("Process provisioning response");

      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
      DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        Logger::log("Unable to de-serialize provision response");
        return;
      }

      const JsonObject &data = jsonBuffer.template as<JsonObject>();

      Logger::log("Received provision response");

      if (data["status"] == "SUCCESS" && data["credentialsType"] == "X509_CERTIFICATE") {
        Logger::log("Provision response contains X509_CERTIFICATE credentials, it is not supported yet.");
        return;
      }

      if (m_provisionCallback.m_cb) {
        m_provisionCallback.m_cb(data);
      }
    }
#endif

    // Sends array of attributes or telemetry to ThingsBoard
    bool sendDataArray(const Telemetry *data, size_t data_count, bool telemetry = true) {
      if (MaxFieldsAmt < data_count) {
        Logger::log("too much JSON fields passed");
        return false;
      }
      char payload[PayloadSize];
      {
        StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
        JsonVariant object = jsonBuffer.template to<JsonVariant>();

        for (size_t i = 0; i < data_count; ++i) {
          if (data[i].serializeKeyval(object) == false) {
            Logger::log("unable to serialize data");
            return false;
          }
        }
        if (JSON_STRING_SIZE(measureJson(jsonBuffer)) > PayloadSize) {
          Logger::log("too small buffer for JSON data");
          return false;
        }
        serializeJson(object, payload, sizeof(payload));
      }

      return telemetry ? sendTelemetryJson(payload) : sendAttributeJSON(payload);
    }

    PubSubClient m_client; // PubSub MQTT client instance.
    std::vector<RPC_Callback> m_rpcCallbacks; // RPC callbacks array
    std::vector<Shared_Attribute_Callback> m_sharedAttributeUpdateCallbacks; // Shared attribute update callbacks array
    std::vector<Shared_Attribute_Request_Callback> m_sharedAttributeRequestCallbacks; // Shared attribute request callbacks array

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
    Provision_Callback m_provisionCallback; // Provision response callback
#endif
    unsigned int m_requestId;

#if defined(ESP8266) || defined(ESP32)
    // For Firmware Update
    String m_fwVersion;
    String m_fwState;
    String m_fwTitle;
    String m_fwChecksum;
    String m_fwChecksumAlgorithm;
    unsigned int m_fwSize;
    int m_fwChunkReceive;
#endif

    // PubSub client cannot call a method when message arrives on subscribed topic.
    // Only free-standing function is allowed.
    // To be able to forward event to an instance, rather than to a function, this pointer exists.
    static ThingsBoardSized* m_subscribedInstance;

    // The callback for when a PUBLISH message is received from the server.
    static void on_message(char* topic, uint8_t* payload, unsigned int length) {
      Logger::log(String("Callback on_message from topic: " + String(topic)).c_str());
      if (!ThingsBoardSized::m_subscribedInstance) {
        return;
      }
      if (strncmp("v1/devices/me/rpc", topic, strlen("v1/devices/me/rpc")) == 0) {
        ThingsBoardSized::m_subscribedInstance->process_rpc_message(topic, payload, length);
      } else if (strncmp("v1/devices/me/attributes/response", topic, strlen("v1/devices/me/attributes/response")) == 0) {
        ThingsBoardSized::m_subscribedInstance->process_shared_attribute_request_message(topic, payload, length);
      } else if (strncmp("v1/devices/me/attributes", topic, strlen("v1/devices/me/attributes")) == 0) {
        ThingsBoardSized::m_subscribedInstance->process_shared_attribute_update_message(topic, payload, length);
      } else {
        if (strncmp("/provision/response", topic, strlen("/provision/response")) == 0) {
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
          ThingsBoardSized::m_subscribedInstance->process_provisioning_response(topic, payload, length);
#endif
        } else if (strncmp("v2/fw/response", topic, strlen("v2/fw/response")) == 0) {
#if defined(ESP8266) || defined(ESP32)
          ThingsBoardSized::m_subscribedInstance->process_firmware_response(topic, payload, length);
#endif
        }
      }
    }
};

template<size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
ThingsBoardSized<PayloadSize, MaxFieldsAmt, Logger>* ThingsBoardSized<PayloadSize, MaxFieldsAmt, Logger>::m_subscribedInstance;

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

    // Sends custom JSON telemetry string to the ThingsBoard, using HTTP.
    inline bool sendTelemetryJson(const char *json) {
      if (!json || !m_token) {
        return  false;
      }

      if (!m_client.connected()) {
        if (!m_client.connect(m_host, m_port)) {
          Logger::log("connect to server failed");
          return false;
        }
      }

      bool rc = true;

      const String path = String("/api/v1/") + m_token + "/telemetry";
      if (!m_client.post(path, "application/json", json) ||
          (m_client.responseStatusCode() != HTTP_SUCCESS)) {
        rc = false;
      }

      m_client.stop();
      return rc;
    }

    //----------------------------------------------------------------------------
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

    // Sends custom JSON with attributes to the ThingsBoard, using HTTP.
    inline bool sendAttributeJSON(const char *json) {
      if (!json || !m_token) {
        return  false;
      }

      if (!m_client.connected()) {
        if (!m_client.connect(m_host, m_port)) {
          Logger::log("connect to server failed");
          return false;
        }
      }

      bool rc = true;

      const String path = String("/api/v1/") + m_token + "/attributes";
      if (!m_client.post(path, "application/json", json)
          || (m_client.responseStatusCode() != HTTP_SUCCESS)) {
        rc = false;
      }

      m_client.stop();
      return rc;
    }

  private:
    // Sends array of attributes or telemetry to ThingsBoard
    bool sendDataArray(const Telemetry *data, size_t data_count, bool telemetry = true) {
      if (MaxFieldsAmt < data_count) {
        Logger::log("too much JSON fields passed");
        return false;
      }
      char payload[PayloadSize];
      {
        StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
        JsonVariant object = jsonBuffer.template to<JsonVariant>();

        for (size_t i = 0; i < data_count; ++i) {
          if (data[i].serializeKeyval(object) == false) {
            Logger::log("unable to serialize data");
            return false;
          }
        }

        if (JSON_STRING_SIZE(measureJson(jsonBuffer)) > PayloadSize) {
          Logger::log("too small buffer for JSON data");
          return false;
        }
        serializeJson(object, payload, sizeof(payload));
      }

      return telemetry ? sendTelemetryJson(payload) : sendAttributeJSON(payload);
    }

    // Sends single key-value in a generic way.
    template<typename T>
    bool sendKeyval(const char *key, T value, bool telemetry = true) {
      Telemetry t(key, value);

      char payload[PayloadSize];
      {
        Telemetry t(key, value);
        StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonBuffer;
        JsonVariant object = jsonBuffer.template to<JsonVariant>();
        if (t.serializeKeyval(object) == false) {
          Logger::log("unable to serialize data");
          return false;
        }

        if (JSON_STRING_SIZE(measureJson(jsonBuffer)) > PayloadSize) {
          Logger::log("too small buffer for JSON data");
          return false;
        }
        serializeJson(object, payload, sizeof(payload));
      }
      return telemetry ? sendTelemetryJson(payload) : sendAttributeJSON(payload);
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
