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
#include "ArduinoJson/Polyfills/type_traits.hpp"

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
// JSON object is used to communicate RPC parameters to the client
using RPC_Data = JsonVariant;
using Shared_Attribute_Data = JsonObject;
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
using Provision_Data = JsonObject;
#endif

// RPC callback wrapper
class RPC_Callback {
    template <size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
    friend class ThingsBoardSized;
  public:

    // RPC callback signature
    using processFn = RPC_Response (*)(const RPC_Data &data);

    // Constructs empty callback
    inline RPC_Callback()
      : m_name(), m_cb(NULL)                {  }

    // Constructs callback that will be fired upon a RPC request arrival with
    // given method name
    inline RPC_Callback(const char *methodName, processFn cb)
      : m_name(methodName), m_cb(cb)        {  }

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
    using processFn = void (*)(const Shared_Attribute_Data &data);

    // Constructs empty callback
    inline Shared_Attribute_Callback()
      : m_cb(NULL)                {  }

    // Constructs callback that will be fired upon a Shared attribute request arrival with
    // given attribute key
    inline Shared_Attribute_Callback(processFn cb)
      : m_cb(cb)        {  }

  private:
    processFn   m_cb;       // Callback to call
};

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
// Provisioning callback wrapper
class Provision_Callback {
    template <size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
    friend class ThingsBoardSized;
  public:

    // Provisioning callback signature
    using processFn = void (*)(const Provision_Data &data);

    // Constructs empty callback
    inline Provision_Callback()
      : m_cb(NULL)                {  }

    // Constructs callback that will be fired upon a Provision request arrival with
    // given attribute key
    inline Provision_Callback(processFn cb)
      : m_cb(cb)        {  }

  private:
    processFn   m_cb;       // Callback to call
};
#endif

class ThingsBoardDefaultLogger
{
  public:
    static void log(const char *msg);
};

// ThingsBoardSized client class
template<size_t PayloadSize = Default_Payload,
         size_t MaxFieldsAmt = Default_Fields_Amt,
         typename Logger = ThingsBoardDefaultLogger>
class ThingsBoardSized
{

    bool provision_mode = false;

  public:
    // Initializes ThingsBoardSized class with network client.
#if defined(ESP8266) || defined(ESP32)
    inline ThingsBoardSized(Client &client)
      : m_client(client)
      , m_requestId(0)
      , m_fwVersion("")
      , m_fwTitle("")
      , m_fwChecksum("")
      , m_fwChecksumAlgorithm("")
      , m_fwState("")
      , m_fwSize(0)
      , m_fwChunkReceive(-1)
    { }
#else
    inline ThingsBoardSized(Client &client)
      : m_client(client)
      , m_requestId(0)
    { }
#endif

    // Destroys ThingsBoardSized class with network client.
    inline ~ThingsBoardSized() { }

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
      if (!strcmp(access_token, "provision")) {
        provision_mode = true;
      }
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

      uint8_t objectSize = measureJson(requestBuffer) + 1;
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

      uint8_t objectSize = measureJson(requestBuffer) + 1;
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

    //----------------------------------------------------------------------------
    // Server-side RPC API

    // Subscribes multiple RPC callbacks with given size
    bool RPC_Subscribe(const RPC_Callback *callbacks, size_t callbacks_size) {
      if (callbacks_size > sizeof(m_rpcCallbacks) / sizeof(*m_rpcCallbacks)) {
        return false;
      }
      if (ThingsBoardSized::m_subscribedInstance) {
        return false;
      }

      if (!m_client.subscribe("v1/devices/me/rpc/request/+")) {
        return false;
      }

      ThingsBoardSized::m_subscribedInstance = this;
      for (size_t i = 0; i < callbacks_size; ++i) {
        m_rpcCallbacks[i] = callbacks[i];
      }

      m_client.setCallback(ThingsBoardSized::on_message);

      return true;
    }

    inline bool RPC_Unsubscribe() {
      ThingsBoardSized::m_subscribedInstance = NULL;
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
      if (!Shared_Attributes_Request("fw_checksum,fw_checksum_algorithm,fw_size,fw_title,fw_version")) {
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

      int chunkSize = 4096;   // maybe less if we don't have enough RAM
      int numberOfChunk = int(m_fwSize / chunkSize) + 1;
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

        timeout = millis() + 3000;
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
                return false;
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
            return false;
          }
        }

      } while (numberOfChunk != currChunk);

      // Update state
      Firmware_Send_State(m_fwState.c_str());

      return m_fwState == "SUCCESS" ? true : false;
    }

    bool Firmware_Send_FW_Info(const char* currFwTitle, const char* currFwVersion) {
      // Send our firmware title and version
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> currentFirmwareInfo;
      JsonObject currentFirmwareInfoObject = currentFirmwareInfo.to<JsonObject>();

      currentFirmwareInfoObject["current_fw_title"] = currFwTitle;
      currentFirmwareInfoObject["current_fw_version"] = currFwVersion;

      int objectSize = measureJson(currentFirmwareInfo) + 1;
      char buffer[objectSize];
      serializeJson(currentFirmwareInfoObject, buffer, objectSize);

      return sendTelemetryJson(buffer);
    }

    bool Firmware_Send_State(const char* currFwState) {
      // Send our firmware title and version
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> currentFirmwareState;
      JsonObject currentFirmwareStateObject = currentFirmwareState.to<JsonObject>();

      currentFirmwareStateObject["current_fw_state"] = currFwState;

      int objectSize = measureJson(currentFirmwareState) + 1;
      char buffer[objectSize];
      serializeJson(currentFirmwareStateObject, buffer, objectSize);

      return sendTelemetryJson(buffer);
    }

    bool Firmware_OTA_Subscribe() {
      if (ThingsBoardSized::m_subscribedInstance) {
        return false;
      }

      // Subscribe at 3 topics
      if (!m_client.subscribe("v1/devices/me/attributes/response/+")) {
        return false;
      }
      if (!m_client.subscribe("v1/devices/me/attributes")) {
        return false;
      }
      if (!m_client.subscribe("v2/fw/response/#")) {
        return false;
      }

      ThingsBoardSized::m_subscribedInstance = this;

      // Set callback for receive message
      m_client.setCallback(ThingsBoardSized::on_message);

      return true;
    }

    bool Firmware_OTA_Unsubscribe() {
      ThingsBoardSized::m_subscribedInstance = NULL;

      // Unsubscribe at 3 topics
      if (!m_client.unsubscribe("v1/devices/me/attributes/response/+")) {
        return false;
      }
      if (!m_client.unsubscribe("v1/devices/me/attributes")) {
        return false;
      }
      if (!m_client.unsubscribe("v2/fw/response/#")) {
        return false;
      }

      return true;
    }
#endif

    //----------------------------------------------------------------------------
    // Shared attributes API

    bool Shared_Attributes_Request(const char* attributes) {
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> requestBuffer;
      JsonObject requestObject = requestBuffer.to<JsonObject>();

      requestObject["sharedKeys"] = attributes;

      int objectSize = measureJson(requestBuffer) + 1;
      char buffer[objectSize];
      serializeJson(requestObject, buffer, objectSize);

      m_requestId++;

      return m_client.publish(String("v1/devices/me/attributes/request/" + String(m_requestId)).c_str(), buffer);
    }

    // Subscribes multiple Shared attributes callbacks with given size
    bool Shared_Attributes_Subscribe(const Shared_Attribute_Callback *callbacks, size_t callbacks_size) {
      if (callbacks_size > sizeof(m_sharedAttributeUpdateCallbacks) / sizeof(*m_sharedAttributeUpdateCallbacks)) {
        return false;
      }
      if (ThingsBoardSized::m_subscribedInstance) {
        return false;
      }

      if (!m_client.subscribe("v1/devices/me/attributes/response/+")) {
        return false;
      }
      if (!m_client.subscribe("v1/devices/me/attributes")) {
        return false;
      }

      ThingsBoardSized::m_subscribedInstance = this;
      for (size_t i = 0; i < callbacks_size; ++i) {
        m_sharedAttributeUpdateCallbacks[i] = callbacks[i];
      }

      m_client.setCallback(ThingsBoardSized::on_message);

      return true;
    }

    inline bool Shared_Attributes_Unsubscribe() {
      ThingsBoardSized::m_subscribedInstance = NULL;
      if (!m_client.unsubscribe("v1/devices/me/attributes/response/+")) {
        return false;
      }
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

      if (ThingsBoardSized::m_subscribedInstance) {
        return false;
      }

      if (!m_client.subscribe("/provision/response")) {
        return false;
      }

      ThingsBoardSized::m_subscribedInstance = this;
      m_provisionCallback = callback;
      m_client.setCallback(ThingsBoardSized::on_message);

      return true;
    }

    bool Provision_Unsubscribe() {
      ThingsBoardSized::m_subscribedInstance = NULL;
      if (!m_client.unsubscribe("/provision/response")) {
        return false;
      }
      return true;
    }
#endif

  private:
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

        if (measureJson(jsonBuffer) > PayloadSize - 1) {
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

        for (size_t i = 0; i < sizeof(m_rpcCallbacks) / sizeof(*m_rpcCallbacks); ++i) {
          if (m_rpcCallbacks[i].m_cb && !strcmp(m_rpcCallbacks[i].m_name, methodName)) {

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
              r = m_rpcCallbacks[i].m_cb(data["params"]);
            } else {
              Logger::log("params:");
              Logger::log(params);
              const JsonObject &param = doc.template as<JsonObject>();
              // Getting non-existing field from JSON should automatically
              // set JSONVariant to null
              r = m_rpcCallbacks[i].m_cb(param);
            }
            break;
          }
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

      if (measureJson(respBuffer) > PayloadSize - 1) {
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

      m_fwState = "DOWNLOADING";

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
          else {
            Logger::log("Update Fail !");
            m_fwState = "FAILED";
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

      // Save data for firmware update
#if defined(ESP8266) || defined(ESP32)
      if (data["fw_title"])
        m_fwTitle = data["fw_title"].as<String>();

      if (data["fw_version"])
        m_fwVersion = data["fw_version"].as<String>();

      if (data["fw_checksum"])
        m_fwChecksum = data["fw_checksum"].as<String>();

      if (data["fw_checksum_algorithm"])
        m_fwChecksumAlgorithm = data["fw_checksum_algorithm"].as<String>();

      if (data["fw_size"])
        m_fwSize = data["fw_size"].as<int>();
#endif

      for (size_t i = 0; i < sizeof(m_sharedAttributeUpdateCallbacks) / sizeof(*m_sharedAttributeUpdateCallbacks); ++i) {
        if (m_sharedAttributeUpdateCallbacks[i].m_cb) {

          Logger::log("Calling callbacks for updated attribute");

          // Getting non-existing field from JSON should automatically
          // set JSONVariant to null
          m_sharedAttributeUpdateCallbacks[i].m_cb(data);
        }
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
        if (measureJson(jsonBuffer) > PayloadSize - 1) {
          Logger::log("too small buffer for JSON data");
          return false;
        }
        serializeJson(object, payload, sizeof(payload));
      }

      return telemetry ? sendTelemetryJson(payload) : sendAttributeJSON(payload);
    }

    PubSubClient m_client;              // PubSub MQTT client instance.
    RPC_Callback m_rpcCallbacks[8];     // RPC callbacks array
    Shared_Attribute_Callback m_sharedAttributeUpdateCallbacks[8];     // Shared attribute update callbacks array

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
    Provision_Callback m_provisionCallback; // Provision response callback
#endif
    unsigned int m_requestId;

#if defined(ESP8266) || defined(ESP32)
    // For Firmware Update
    String m_fwVersion, m_fwTitle, m_fwChecksum, m_fwChecksumAlgorithm, m_fwState;
    unsigned int m_fwSize;
    int m_fwChunkReceive;
#endif

    // PubSub client cannot call a method when message arrives on subscribed topic.
    // Only free-standing function is allowed.
    // To be able to forward event to an instance, rather than to a function, this pointer exists.
    static ThingsBoardSized *m_subscribedInstance;

    // The callback for when a PUBLISH message is received from the server.
    static void on_message(char* topic, uint8_t* payload, unsigned int length) {
      Logger::log(String("Callback on_message from topic: " + String(topic)).c_str());
      if (!ThingsBoardSized::m_subscribedInstance) {
        return;
      }
      if (strncmp("v1/devices/me/rpc", topic, strlen("v1/devices/me/rpc")) == 0) {
        ThingsBoardSized::m_subscribedInstance->process_rpc_message(topic, payload, length);
      } else if (strncmp("v1/devices/me/attributes", topic, strlen("v1/devices/me/attributes")) == 0) {
        ThingsBoardSized::m_subscribedInstance->process_shared_attribute_update_message(topic, payload, length);
      } else {
        if (strncmp("/provision/response", topic, strlen("/provision/response")) == 0) {
#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)
          ThingsBoardSized::m_subscribedInstance->process_provisioning_response(topic, payload, length);
#endif
        } else if (strncmp("v2/fw/response/", topic, strlen("v2/fw/response/")) == 0) {
#if defined(ESP8266) || defined(ESP32)
          ThingsBoardSized::m_subscribedInstance->process_firmware_response(topic, payload, length);
#endif
    }
    }
    }

};

template<size_t PayloadSize, size_t MaxFieldsAmt, typename Logger>
ThingsBoardSized<PayloadSize, MaxFieldsAmt, Logger> *ThingsBoardSized<PayloadSize, MaxFieldsAmt, Logger>::m_subscribedInstance;

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
    { }

    // Destroys ThingsBoardHttpSized class with network client.
    inline ~ThingsBoardHttpSized() { }

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

      String path = String("/api/v1/") + m_token + "/telemetry";
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

      String path = String("/api/v1/") + m_token + "/attributes";
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

        if (measureJson(jsonBuffer) > PayloadSize - 1) {
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

        if (measureJson(jsonBuffer) > PayloadSize - 1) {
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
