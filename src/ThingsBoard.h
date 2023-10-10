#ifndef ThingsBoard_h
#define ThingsBoard_h

// Local includes.
#include "Constants.h"
#include "Vector.h"
#include "Helper.h"
#include "ThingsBoardDefaultLogger.h"
#include "Shared_Attribute_Callback.h"
#include "Attribute_Request_Callback.h"
#include "RPC_Callback.h"
#include "RPC_Request_Callback.h"
#include "Provision_Callback.h"
#include "OTA_Handler.h"
#include "IMQTT_Client.h"

// Library includes.
#if THINGSBOARD_ENABLE_STREAM_UTILS
#include <StreamUtils.h>
#endif // THINGSBOARD_ENABLE_STREAM_UTILS


/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
// Publish data topics.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char ATTRIBUTE_TOPIC[] PROGMEM = "v1/devices/me/attributes";
constexpr char TELEMETRY_TOPIC[] PROGMEM = "v1/devices/me/telemetry";
#else
constexpr char ATTRIBUTE_TOPIC[] = "v1/devices/me/attributes";
constexpr char TELEMETRY_TOPIC[] = "v1/devices/me/telemetry";
#endif // THINGSBOARD_ENABLE_PROGMEM

// RPC topics.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char RPC_SUBSCRIBE_TOPIC[] PROGMEM = "v1/devices/me/rpc/request/+";
constexpr char RPC_RESPONSE_SUBSCRIBE_TOPIC[] PROGMEM = "v1/devices/me/rpc/response/+";
constexpr char RPC_SEND_REQUEST_TOPIC[] PROGMEM = "v1/devices/me/rpc/request/%u";
constexpr char RPC_REQUEST_TOPIC[] PROGMEM = "v1/devices/me/rpc/request";
constexpr char RPC_RESPONSE_TOPIC[] PROGMEM = "v1/devices/me/rpc/response";
constexpr char RPC_SEND_RESPONSE_TOPIC[] = "v1/devices/me/rpc/response/%u";
#else
constexpr char RPC_SUBSCRIBE_TOPIC[] = "v1/devices/me/rpc/request/+";
constexpr char RPC_RESPONSE_SUBSCRIBE_TOPIC[] = "v1/devices/me/rpc/response/+";
constexpr char RPC_SEND_REQUEST_TOPIC[] = "v1/devices/me/rpc/request/%u";
constexpr char RPC_REQUEST_TOPIC[] = "v1/devices/me/rpc/request";
constexpr char RPC_RESPONSE_TOPIC[] = "v1/devices/me/rpc/response";
constexpr char RPC_SEND_RESPONSE_TOPIC[] = "v1/devices/me/rpc/response/%u";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Firmware topics.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char FIRMWARE_RESPONSE_TOPIC[] PROGMEM = "v2/fw/response/0/chunk";
#else
constexpr char FIRMWARE_RESPONSE_TOPIC[] = "v2/fw/response/0/chunk";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Shared attribute topics.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char ATTRIBUTE_REQUEST_TOPIC[] PROGMEM = "v1/devices/me/attributes/request/%u";
constexpr char ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC[] PROGMEM = "v1/devices/me/attributes/response/+";
constexpr char ATTRIBUTE_RESPONSE_TOPIC[] PROGMEM = "v1/devices/me/attributes/response";
#else
constexpr char ATTRIBUTE_REQUEST_TOPIC[] = "v1/devices/me/attributes/request/%u";
constexpr char ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC[] = "v1/devices/me/attributes/response/+";
constexpr char ATTRIBUTE_RESPONSE_TOPIC[] = "v1/devices/me/attributes/response";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Provision topics.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char PROV_RESPONSE_TOPIC[] PROGMEM = "/provision/response";
#else
constexpr char PROV_RESPONSE_TOPIC[] = "/provision/response";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Default login data.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char PROV_ACCESS_TOKEN[] PROGMEM = "provision";
constexpr char DEFAULT_CLIENT_ID[] PROGMEM = "TbDev";
#else
constexpr char PROV_ACCESS_TOKEN[] = "provision";
constexpr char DEFAULT_CLIENT_ID[] = "TbDev";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Shared attribute request keys.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char SHARED_REQUEST_KEY[] PROGMEM = "sharedKeys";
constexpr char SHARED_RESPONSE_KEY[] PROGMEM = "shared";
#else
constexpr char SHARED_REQUEST_KEY[] = "sharedKeys";
constexpr char SHARED_RESPONSE_KEY[] = "shared";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Client side attribute request keys.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char CLIENT_REQUEST_KEYS[] PROGMEM = "clientKeys";
constexpr char CLIENT_RESPONSE_KEY[] PROGMEM = "client";
#else
constexpr char CLIENT_REQUEST_KEYS[] = "clientKeys";
constexpr char CLIENT_RESPONSE_KEY[] = "client";
#endif // THINGSBOARD_ENABLE_PROGMEM

// RPC data keys.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char RPC_METHOD_KEY[] PROGMEM = "method";
constexpr char RPC_PARAMS_KEY[] PROGMEM = "params";
constexpr char RPC_EMPTY_PARAMS_VALUE[] PROGMEM = "{}";
#else
constexpr char RPC_METHOD_KEY[] = "method";
constexpr char RPC_PARAMS_KEY[] = "params";
constexpr char RPC_EMPTY_PARAMS_VALUE[] = "{}";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Log messages.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char UNABLE_TO_DE_SERIALIZE_JSON[] PROGMEM = "Unable to de-serialize received json data with error (DeserializationError::%s)";
constexpr char INVALID_BUFFER_SIZE[] PROGMEM = "Buffer size (%u) to small for the given payloads size (%u), increase with setBufferSize accordingly or set THINGSBOARD_ENABLE_STREAM_UTILS to 1 before including ThingsBoard";
#if THINGSBOARD_ENABLE_OTA
constexpr char NUMBER_PRINTF[] PROGMEM = "%u";
#endif // THINGSBOARD_ENABLE_OTA
#if !THINGSBOARD_ENABLE_DYNAMIC
constexpr char MAX_RPC_EXCEEDED[] PROGMEM = "Too many server-side RPC subscriptions, increase MaxFieldsAmt or unsubscribe";
constexpr char MAX_RPC_REQUEST_EXCEEDED[] PROGMEM = "Too many client-side RPC subscriptions, increase MaxFieldsAmt or unsubscribe";
constexpr char MAX_SHARED_ATT_UPDATE_EXCEEDED[] PROGMEM = "Too many shared attribute update callback subscriptions, increase MaxFieldsAmt or unsubscribe";
constexpr char MAX_SHARED_ATT_REQUEST_EXCEEDED[] PROGMEM = "Too many shared attribute request callback subscriptions, increase MaxFieldsAmt";
#else
constexpr char COLON PROGMEM = ':';
#endif // !THINGSBOARD_ENABLE_DYNAMIC
constexpr char COMMA PROGMEM = ',';
constexpr char NO_KEYS_TO_REQUEST[] PROGMEM = "No keys to request were given";
constexpr char RPC_METHOD_NULL[] PROGMEM = "RPC methodName is NULL";
constexpr char SUBSCRIBE_TOPIC_FAILED[] PROGMEM = "Subscribing the given topic failed";
#if THINGSBOARD_ENABLE_DEBUG
constexpr char NO_RPC_PARAMS_PASSED[] PROGMEM = "No parameters passed with RPC, passing null JSON";
constexpr char NOT_FOUND_ATT_UPDATE[] PROGMEM = "Shared attribute update key not found";
constexpr char ATT_KEY_NOT_FOUND[] PROGMEM = "Attribute key not found";
constexpr char ATT_CB_NO_KEYS[] PROGMEM = "No keys subscribed. Calling subscribed callback for any updated attributes, assumed to be subscribed to every possible key";
constexpr char ATT_IS_NULL[] PROGMEM = "Subscribed shared attribute update key is NULL";
constexpr char ATT_NO_CHANGE[] PROGMEM = "No keys that we subscribed too were changed, skipping callback";
constexpr char CALLING_RPC_CB[] PROGMEM = "Calling subscribed callback for rpc with methodname (%s)";
constexpr char CALLING_ATT_CB[] PROGMEM = "Calling subscribed callback for updated shared attribute (%s)";
constexpr char CALLING_REQUEST_CB[] PROGMEM = "Calling subscribed callback for request with response id (%u)";
constexpr char RECEIVE_MESSAGE[] PROGMEM = "Received data from server over topic (%s)";
constexpr char SEND_MESSAGE[] PROGMEM = "Sending data to server over topic (%s) with data (%s)";
constexpr char SEND_SERIALIZED[] PROGMEM = "Hidden, because json data is bigger than buffer, therefore showing in console is skipped";
#endif // THINGSBOARD_ENABLE_DEBUG
#else
constexpr char UNABLE_TO_DE_SERIALIZE_JSON[] = "Unable to de-serialize received json data with error (DeserializationError::%s)";
constexpr char INVALID_BUFFER_SIZE[] = "Buffer size (%u) to small for the given payloads size (%u), increase with setBufferSize accordingly or set THINGSBOARD_ENABLE_STREAM_UTILS to 1 before including ThingsBoard";
#if THINGSBOARD_ENABLE_OTA
constexpr char NUMBER_PRINTF[] = "%u";
#endif // THINGSBOARD_ENABLE_OTA
#if !THINGSBOARD_ENABLE_DYNAMIC
constexpr char MAX_RPC_EXCEEDED[] = "Too many server-side RPC subscriptions, increase MaxFieldsAmt or unsubscribe";
constexpr char MAX_RPC_REQUEST_EXCEEDED[] = "Too many client-side RPC subscriptions, increase MaxFieldsAmt or unsubscribe";
constexpr char MAX_SHARED_ATT_UPDATE_EXCEEDED[] = "Too many shared attribute update callback subscriptions, increase MaxFieldsAmt or unsubscribe";
constexpr char MAX_SHARED_ATT_REQUEST_EXCEEDED[] = "Too many shared attribute request callback subscriptions, increase MaxFieldsAmt";
#else
constexpr char COLON = ':';
#endif // !THINGSBOARD_ENABLE_DYNAMIC
constexpr char COMMA = ',';
constexpr char NO_KEYS_TO_REQUEST[] = "No keys to request were given";
constexpr char RPC_METHOD_NULL[] = "RPC methodName is NULL";
constexpr char SUBSCRIBE_TOPIC_FAILED[] = "Subscribing the given topic failed";
#if THINGSBOARD_ENABLE_DEBUG
constexpr char NO_RPC_PARAMS_PASSED[] = "No parameters passed with RPC, passing null JSON";
constexpr char NOT_FOUND_ATT_UPDATE[] = "Shared attribute update key not found";
constexpr char ATT_KEY_NOT_FOUND[] = "Attribute key not found";
constexpr char ATT_CB_NO_KEYS[] = "No keys subscribed. Calling subscribed callback for any updated attributes, assumed to be subscribed to every possible key";
constexpr char ATT_IS_NULL[] = "Subscribed shared attribute update key is NULL";
constexpr char ATT_NO_CHANGE[] = "No keys that we subscribed too were changed, skipping callback";
constexpr char CALLING_RPC_CB[] = "Calling subscribed callback for rpc with methodname (%s)";
constexpr char CALLING_ATT_CB[] = "Calling subscribed callback for updated shared attribute (%s)";
constexpr char CALLING_REQUEST_CB[] = "Calling subscribed callback for request with response id (%u)";
constexpr char RECEIVE_MESSAGE[] = "Received data from server over topic (%s)";
constexpr char SEND_MESSAGE[] = "Sending data to server over topic (%s) with data (%s)";
constexpr char SEND_SERIALIZED[] = "Hidden, because json data is bigger than buffer, therefore showing in console is skipped";
#endif // THINGSBOARD_ENABLE_DEBUG
#endif // THINGSBOARD_ENABLE_PROGMEM

// Claim topics.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char CLAIM_TOPIC[] PROGMEM = "v1/devices/me/claim";
#else
constexpr char CLAIM_TOPIC[] = "v1/devices/me/claim";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Provision topics.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char PROV_REQUEST_TOPIC[] PROGMEM = "/provision/request";
#else
constexpr char PROV_REQUEST_TOPIC[] = "/provision/request";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Claim data keys.
#if THINGSBOARD_ENABLE_PROGMEM
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
#else
constexpr char SECRET_KEY[] = "secretKey";
constexpr char DURATION_KEY[] = "durationMs";
constexpr char DEVICE_NAME_KEY[] = "deviceName";
constexpr char PROV_DEVICE_KEY[] = "provisionDeviceKey";
constexpr char PROV_DEVICE_SECRET_KEY[] = "provisionDeviceSecret";
constexpr char PROV_CRED_TYPE_KEY[] = "credentialsType";
constexpr char PROV_TOKEN[] = "token";
constexpr char PROV_CRED_USERNAME[] = "username";
constexpr char PROV_CRED_PASSWORD[] = "password";
constexpr char PROV_CRED_CLIENT_ID[] = "clientId";
constexpr char PROV_CRED_HASH[] = "hash";
#endif // THINGSBOARD_ENABLE_PROGMEM

#if THINGSBOARD_ENABLE_OTA

// Firmware topics.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC[] PROGMEM = "v2/fw/response/#";
constexpr char FIRMWARE_REQUEST_TOPIC[] PROGMEM = "v2/fw/request/0/chunk/%u";
#else
constexpr char FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC[] = "v2/fw/response/#";
constexpr char FIRMWARE_REQUEST_TOPIC[] = "v2/fw/request/0/chunk/%u";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Firmware data keys.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char CURR_FW_TITLE_KEY[] PROGMEM = "current_fw_title";
constexpr char CURR_FW_VER_KEY[] PROGMEM = "current_fw_version";
constexpr char FW_ERROR_KEY[] PROGMEM = "fw_error";
constexpr char FW_STATE_KEY[] PROGMEM = "fw_state";
constexpr char FW_VER_KEY[] PROGMEM = "fw_version";
constexpr char FW_TITLE_KEY[] PROGMEM = "fw_title";
constexpr char FW_CHKS_KEY[] PROGMEM = "fw_checksum";
constexpr char FW_CHKS_ALGO_KEY[] PROGMEM = "fw_checksum_algorithm";
constexpr char FW_SIZE_KEY[] PROGMEM = "fw_size";
constexpr char CHECKSUM_AGORITM_MD5[] PROGMEM = "MD5";
constexpr char CHECKSUM_AGORITM_SHA256[] PROGMEM = "SHA256";
constexpr char CHECKSUM_AGORITM_SHA384[] PROGMEM = "SHA384";
constexpr char CHECKSUM_AGORITM_SHA512[] PROGMEM = "SHA512";
#else
constexpr char CURR_FW_TITLE_KEY[] = "current_fw_title";
constexpr char CURR_FW_VER_KEY[] = "current_fw_version";
constexpr char FW_ERROR_KEY[] = "fw_error";
constexpr char FW_STATE_KEY[] = "fw_state";
constexpr char FW_VER_KEY[] = "fw_version";
constexpr char FW_TITLE_KEY[] = "fw_title";
constexpr char FW_CHKS_KEY[] = "fw_checksum";
constexpr char FW_CHKS_ALGO_KEY[] = "fw_checksum_algorithm";
constexpr char FW_SIZE_KEY[] = "fw_size";
constexpr char CHECKSUM_AGORITM_MD5[] = "MD5";
constexpr char CHECKSUM_AGORITM_SHA256[] = "SHA256";
constexpr char CHECKSUM_AGORITM_SHA384[] = "SHA384";
constexpr char CHECKSUM_AGORITM_SHA512[] = "SHA512";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Log messages.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char NO_FW[] PROGMEM = "No new firmware assigned on the given device";
constexpr char EMPTY_FW[] PROGMEM = "Given firmware was NULL";
constexpr char FW_UP_TO_DATE[] PROGMEM = "Firmware is already up to date";
constexpr char FW_NOT_FOR_US[] PROGMEM = "Firmware is not for us (title is different)";
constexpr char FW_CHKS_ALGO_NOT_SUPPORTED[] PROGMEM = "Checksum algorithm (%s) is not supported";
constexpr char NOT_ENOUGH_RAM[] PROGMEM = "Temporary allocating more internal client buffer failed, decrease OTA chunk size or decrease overall heap usage";
constexpr char RESETTING_FAILED[] PROGMEM = "Preparing for OTA firmware updates failed, attributes might be NULL";
#if THINGSBOARD_ENABLE_DEBUG
constexpr char PAGE_BREAK[] PROGMEM = "=================================";
constexpr char NEW_FW[] PROGMEM = "A new Firmware is available:";
constexpr char FROM_TOO[] PROGMEM = "(%s) => (%s)";
constexpr char DOWNLOADING_FW[] PROGMEM = "Attempting to download over MQTT...";
#endif // THINGSBOARD_ENABLE_DEBUG
#else
constexpr char NO_FW[] = "No new firmware assigned on the given device";
constexpr char EMPTY_FW[] = "Given firmware was NULL";
constexpr char FW_UP_TO_DATE[] = "Firmware is already up to date";
constexpr char FW_NOT_FOR_US[] = "Firmware is not for us (title is different)";
constexpr char FW_CHKS_ALGO_NOT_SUPPORTED[] = "Checksum algorithm (%s) is not supported";
constexpr char NOT_ENOUGH_RAM[] = "Temporary allocating more internal client buffer failed, decrease OTA chunk size or decrease overall heap usage";
constexpr char RESETTING_FAILED[] = "Preparing for OTA firmware updates failed, attributes might be NULL";
#if THINGSBOARD_ENABLE_DEBUG
constexpr char PAGE_BREAK[] = "=================================";
constexpr char NEW_FW[] = "A new Firmware is available:";
constexpr char FROM_TOO[] = "(%s) => (%s)";
constexpr char DOWNLOADING_FW[] = "Attempting to download over MQTT...";
#endif // THINGSBOARD_ENABLE_DEBUG
#endif // THINGSBOARD_ENABLE_PROGMEM

#endif // THINGSBOARD_ENABLE_OTA


#if THINGSBOARD_ENABLE_DYNAMIC
/// @brief Wrapper around any arbitrary MQTT Client implementing the IMQTT_Client interface, to allow connecting and sending / retrieving data from ThingsBoard over the MQTT or MQTT with TLS/SSL protocol.
/// BufferSize of the underlying data buffer can be changed during the runtime and the maximum amount of data points that can ever be sent or received are automatically deduced at runtime
/// If this feature of automatic deduction, is not needed, or not wanted because it allocates memory on the heap, then the values can be set once as template arguements.
/// Simply set THINGSBOARD_ENABLE_DYNAMIC to 0, before including ThingsBoard.h
/// @tparam Logger Logging class that should be used to print messages generated by internal processes, default = ThingsBoardDefaultLogger
template<typename Logger = ThingsBoardDefaultLogger>
#else
/// @brief Wrapper around any arbitrary MQTT Client implementing the IMQTT_Client interface, to allow connecting and sending / retrieving data from ThingsBoard over the MQTT or MQTT with TLS/SSL protocol.
/// BufferSize of the underlying data buffer can be changed during the runtime and the maximum amount of data points that can ever be can be set once as template arguements.
/// Changing is only possible if a new instance of this class is created. If these values should be automatically deduced at runtime instead then,
/// simply set THINGSBOARD_ENABLE_DYNAMIC to 1, before including ThingsBoard.h
/// @tparam MaxFieldsAmt Maximum amount of key value pair that we will be able to sent or received by ThingsBoard in one call, default = 8
/// @tparam Logger Logging class that should be used to print messages generated by internal processes, default = ThingsBoardDefaultLogger
template<size_t MaxFieldsAmt = Default_Fields_Amt,
         typename Logger = ThingsBoardDefaultLogger>
#endif // THINGSBOARD_ENABLE_DYNAMIC
class ThingsBoardSized {
  public:
    /// @brief Constructs a ThingsBoardSized instance with the given network client that should be used to establish the connection to ThingsBoard
    /// @param client MQTT Client implementation that should be used to establish the connection to ThingsBoard
    /// @param bufferSize Maximum amount of data that can be either received or sent to ThingsBoard at once, if bigger packets are received they are discarded
    /// and if we attempt to send data that is bigger, it will not be sent, the internal value can be changed later at any time with the setBufferSize() method
    /// alternatively setting THINGSBOARD_ENABLE_STREAM_UTILS to 1 allows to send arbitrary size payloads if that is done the internal buffer of the MQTT Client implementation
    /// can be theoretically set to only be as big as the biggest message we should every receive from ThingsBoard,
    /// this will mean though that all messages are sent over the StreamUtils library as long as they are bigger than the internal buffer,
    /// which needs more time than sending a message directly but has the advantage of requiring less memory.
    /// So if that is a problem on the board it might be useful to enable the THINGSBOARD_ENABLE_STREAM_UTILS option
    /// and decrease the internal buffer size of the mqtt client to what is needed to receive all MQTT messages,
    /// that size can vary but if all ThingsBoard features are used a buffer size of 256 bytes should suffice for receiving most responses.
    /// If the aforementioned feature is not enabled the buffer size might need to be much bigger though,
    /// but in that case if a message was too big to be sent the user will be informed with a message to the Logger.
    /// The aforementioned options can only be enabled if Arduino is used to build this library, because the StreamUtils library requires it, default = Default_Payload
    /// @param maxStackSize Maximum amount of bytes we want to allocate on the stack, default = Default_Max_Stack_Size
    /// @param bufferingSize Amount of bytes allocated to speed up serialization, default = Default_Buffering_Size
    inline ThingsBoardSized(IMQTT_Client& client, const uint16_t& bufferSize = Default_Payload, const size_t& maxStackSize = Default_Max_Stack_Size, const size_t& bufferingSize = Default_Buffering_Size)
      : m_client(client)
      , m_max_stack(maxStackSize)
      , m_buffering_size(bufferingSize)
      , m_rpc_callbacks()
      , m_rpc_request_callbacks()
      , m_shared_attribute_update_callbacks()
      , m_attribute_request_callbacks()
      , m_provision_callback()
      , m_request_id(0U)
#if THINGSBOARD_ENABLE_OTA
      , m_fw_callback(nullptr)
      , m_previous_buffer_size(0U)
      , m_change_buffer_size(false)
      , m_ota(std::bind(&ThingsBoardSized::Publish_Chunk_Request, this, std::placeholders::_1), std::bind(&ThingsBoardSized::Firmware_Send_State, this, std::placeholders::_1, std::placeholders::_2), std::bind(&ThingsBoardSized::Firmware_OTA_Unsubscribe, this))
#endif // THINGSBOARD_ENABLE_OTA
    {
      setBufferSize(bufferSize);

      // Initalize callback.
#if THINGSBOARD_ENABLE_STL
      m_client.set_callback(std::bind(&ThingsBoardSized::onMQTTMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
#else
      m_client.set_callback(ThingsBoardSized::onStaticMQTTMessage);
      m_subscribedInstance = this;
#endif // THINGSBOARD_ENABLE_STL

#if !THINGSBOARD_ENABLE_DYNAMIC
      reserve_callback_size(MaxFieldsAmt);
#endif // !THINGSBOARD_ENABLE_DYNAMIC
    }

    /// @brief Destructor
    inline ~ThingsBoardSized() {
      // Nothing to do
    }

    /// @brief Gets the currently connected MQTT Client implementation as a reference.
    /// Allows for calling method directly on the client itself, not advised in normal use cases,
    /// as it might cause problems if the library expects the client to be sending / receiving data
    /// but it can not do that anymore, because it has been disconnected or certain settings were changed
    /// @return Reference to the underlying MQTT Client implementation connected to ThingsBoard
    inline IMQTT_Client& getClient() {
      return m_client;
    }

    /// @brief Sets the maximum amount of bytes that we want to allocate on the stack, before the memory is allocated on the heap instead
    /// @param maxStackSize Maximum amount of bytes we want to allocate on the stack
    inline void setMaximumStackSize(const size_t& maxStackSize) {
      m_max_stack = maxStackSize;
    }

#if THINGSBOARD_ENABLE_STREAM_UTILS

    /// @brief Sets the amount of bytes that can be allocated to speed up fall back serialization with the StreamUtils class
    /// See https://github.com/bblanchon/ArduinoStreamUtils for more information on the underlying class used
    /// @param bufferingSize Amount of bytes allocated to speed up serialization
    inline void setBufferingSize(const size_t& bufferingSize) {
      m_buffering_size = bufferingSize;
    }

#endif // THINGSBOARD_ENABLE_STREAM_UTILS

    /// @brief Sets the size of the buffer for the underlying network client that will be used to establish the connection to ThingsBoard
    /// @param bufferSize Maximum amount of data that can be either received or sent to ThingsBoard at once, if bigger packets are received they are discarded
    /// and if we attempt to send data that is bigger, it will not be sent, the internal value can be changed later at any time with the setBufferSize() method
    /// alternatively setting THINGSBOARD_ENABLE_STREAM_UTILS to 1 allows to send arbitrary size payloads if that is done the internal buffer of the MQTT Client implementation
    /// can be theoretically set to only be as big as the biggest message we should every receive from ThingsBoard,
    /// this will mean though that all messages are sent over the StreamUtils library as long as they are bigger than the internal buffer,
    /// which needs more time than sending a message directly but has the advantage of requiring less memory.
    /// So if that is a problem on the board it might be useful to enable the THINGSBOARD_ENABLE_STREAM_UTILS option
    /// and decrease the internal buffer size of the mqtt client to what is needed to receive all MQTT messages,
    /// that size can vary but if all ThingsBoard features are used a buffer size of 256 bytes should suffice for receiving most responses.
    /// If the aforementioned feature is not enabled the buffer size might need to be much bigger though,
    /// but in that case if a message was too big to be sent the user will be informed with a message to the Logger.
    /// The aforementioned options can only be enabled if Arduino is used to build this library, because the StreamUtils library requires it
    /// @return Whether allocating the needed memory for the given bufferSize was successful or not
    inline bool setBufferSize(const uint16_t& bufferSize) {
      return m_client.set_buffer_size(bufferSize);
    }

    /// @brief Clears all currently subscribed callbacks and unsubscribed from all
    /// currently subscribed MQTT topics, any response that will stil be received is discarded
    /// and any ongoing firmware update is aborted and will not be finished.
    /// Was previously done automatically in the connect() method, but is not done anymore,
    /// because connect() method now reconencts to all previously subscribed MQTT topics instead,
    /// therefore there is no need anymore to discard all previously subscribed callbacks and letting the user resubscribe
    inline void Cleanup_Subscriptions() {
      // Cleanup all server-side RPC subscriptions
      this->RPC_Unsubscribe();
      // Cleanup all client-side RPC requests
      this->RPC_Request_Unsubscribe();
      // Cleanup all shared attributes subscriptions
      this->Shared_Attributes_Unsubscribe();
      // Cleanup all client-side or shared attributes requests
      this->Attributes_Request_Unsubscribe();
      // Cleanup all provision requests
      this->Provision_Unsubscribe();
      // Stop any ongoing Firmware update,
      // which will in turn cleanup the internal member variables of the OTAHandler class
      // as well as all firmware subscriptions
      // and inform the user of the failed firmware update
      this->Stop_Firmware_Update();
    }

    /// @brief Connects to the specified ThingsBoard server over the given port as the given device
    /// @param host ThingsBoard server instance we want to connect to
    /// @param access_token Access token that connects this device with a created device on the ThingsBoard server,
    /// can be "provision", if the device creates itself instead. See https://thingsboard.io/docs/user-guide/device-provisioning/?mqttprovisioning=without#provision-device-apis for more information
    /// @param port Port that will be used to establish a connection and send / receive data from ThingsBoard over
    /// @param client_id Client username that can be used to differentiate the user that is connecting the given device to ThingsBoard
    /// @param password Client password that can be used to authenticate the user that is connecting the given device to ThingsBoard
    /// @return Whether connecting to ThingsBoard was successful or not
    inline bool connect(const char *host, const char *access_token = PROV_ACCESS_TOKEN, const uint16_t& port = 1883, const char *client_id = DEFAULT_CLIENT_ID, const char *password = nullptr) {
      if (!host) {
        return false;
      }
      m_client.set_server(host, port);
      return connect_to_host(access_token, client_id, password);
    }

    /// @brief Disconnects any connection that has been established already
    inline void disconnect() {
      m_client.disconnect();
    }

    /// @brief Returns our current connection status to the cloud, true meaning we are connected,
    /// false meaning we have been disconnected or have not established a connection yet
    /// @return Whether the underlying MQTT Client is currently connected or not
    inline bool connected() {
      return m_client.connected();
    }

    /// @brief Receives / sends any outstanding messages from and to the MQTT broker
    /// @return Whether sending or receiving the oustanding the messages was successful or not
    inline bool loop() {
      return m_client.loop();
    }

    /// @brief Attempts to send key value pairs from custom source over the given topic to the server
    /// @tparam TSource Source class that should be used to serialize the json that is sent to the server
    /// @param topic Topic we want to send the data over
    /// @param source Data source containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the source
    /// @return Whether sending the data was successful or not
    template <typename TSource>
    inline bool Send_Json(const char* topic, const TSource& source, const size_t& jsonSize) {
      // Check if allocating needed memory failed when trying to create the JsonObject,
      // if it did the isNull() method will return true. See https://arduinojson.org/v6/api/jsonvariant/isnull/ for more information
      if (source.isNull()) {
        Logger::log(UNABLE_TO_ALLOCATE_MEMORY);
        return false;
      }
#if !THINGSBOARD_ENABLE_DYNAMIC
      const size_t amount = source.size();
      if (MaxFieldsAmt < amount) {
        char message[Helper::detectSize(TOO_MANY_JSON_FIELDS, amount, MaxFieldsAmt)];
        snprintf_P(message, sizeof(message), TOO_MANY_JSON_FIELDS, amount, MaxFieldsAmt);
        Logger::log(message);
        return false;
      }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
      bool result = false;

#if THINGSBOARD_ENABLE_STREAM_UTILS
      // Check if the size of the given message would be too big for the actual client,
      // if it is utilize the serialize json work around, so that the internal client buffer can be circumvented
      if (m_client.get_buffer_size() < jsonSize)  {
#if THINGSBOARD_ENABLE_DEBUG
        char message[JSON_STRING_SIZE(strlen(SEND_MESSAGE)) + JSON_STRING_SIZE(strlen(topic)) + JSON_STRING_SIZE(strlen(SEND_SERIALIZED))];
        snprintf_P(message, sizeof(message), SEND_MESSAGE, topic, SEND_SERIALIZED);
        Logger::log(message);
#endif // THINGSBOARD_ENABLE_DEBUG
        result = Serialize_Json(topic, source, jsonSize);
      }
      // Check if the remaining stack size of the current task would overflow the stack,
      // if it would allocate the memory on the heap instead to ensure no stack overflow occurs
      else
#endif // THINGSBOARD_ENABLE_STREAM_UTILS
      if (getMaximumStackSize() < jsonSize) {
        char* json = new char[jsonSize];
        if (serializeJson(source, json, jsonSize) < jsonSize - 1) {
          Logger::log(UNABLE_TO_SERIALIZE_JSON);
        }
        else {
          result = Send_Json_String(topic, json);
        }
        // Ensure to actually delete the memory placed onto the heap, to make sure we do not create a memory leak
        // and set the pointer to null so we do not have a dangling reference.
        delete[] json;
        json = nullptr;
      }
      else {
        char json[jsonSize];
        if (serializeJson(source, json, jsonSize) < jsonSize - 1) {
          Logger::log(UNABLE_TO_SERIALIZE_JSON);
          return result;
        }
        result = Send_Json_String(topic, json);
      }

      return result;
    }

    /// @brief Attempts to send custom json string over the given topic to the server
    /// @param topic Topic we want to send the data over
    /// @param json String containing our json key value pairs we want to attempt to send
    /// @return Whether sending the data was successful or not
    inline bool Send_Json_String(const char* topic, const char* json) {
      if (json == nullptr) {
        return false;
      }

      const uint16_t& currentBufferSize = m_client.get_buffer_size();
      const size_t jsonSize = strlen(json);

      if (currentBufferSize < jsonSize) {
        char message[Helper::detectSize(INVALID_BUFFER_SIZE, currentBufferSize, jsonSize)];
        snprintf_P(message, sizeof(message), INVALID_BUFFER_SIZE, currentBufferSize, jsonSize);
        Logger::log(message);
        return false;
      }

#if THINGSBOARD_ENABLE_DEBUG
      char message[JSON_STRING_SIZE(strlen(SEND_MESSAGE)) + JSON_STRING_SIZE(strlen(topic)) + jsonSize];
      snprintf_P(message, sizeof(message), SEND_MESSAGE, topic, json);
      Logger::log(message);
#endif // THINGSBOARD_ENABLE_DEBUG

      return m_client.publish(topic, reinterpret_cast<const uint8_t*>(json), jsonSize);
    }

    //----------------------------------------------------------------------------
    // Claiming API

    /// @brief Sends a claiming request for the given device, allowing any given user on the cloud to assign the device as their own (claim),
    /// as long as they enter the given device name and secret key in the given amount of time.
    /// Optionally a secret key can be passed or be left empty (cloud will allow any user to claim the device for the given amount of time).
    /// See https://thingsboard.io/docs/user-guide/claiming-devices/ for more information
    /// @param secretKey Password the user additionaly to the device name needs to enter to claim it as their own,
    /// pass nullptr or an empty string if the user should be able to claim the device without any password
    /// @param durationMs Total time in milliseconds the user has to claim their device as their own
    /// @return Whether sending the claiming request was successful or not
    inline bool Claim_Request(const char *secretKey, const size_t& durationMs) {
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> requestBuffer;
      const JsonObject respObj = requestBuffer.to<JsonObject>();

      // Make the secret key optional,
      // meaning if it is an empty string or null instead we don't send it at all.
      if (secretKey != nullptr && secretKey[0] != '\0') {
        respObj[SECRET_KEY] = secretKey;
      }
      respObj[DURATION_KEY] = durationMs;

      const size_t objectSize = Helper::Measure_Json(respObj);
      return Send_Json(CLAIM_TOPIC, respObj, objectSize);
    }

    //----------------------------------------------------------------------------
    // Provisioning API

    /// @brief Sends provisioning request for a new device, meaning we want to create a device that we can then connect over,
    /// where the given provision device key / secret decide which device profile is used to create the given device with.
    /// Optionally a device name can be passed or be left empty (cloud will use a random string as the name instead).
    /// The cloud then sends back json data containing our credentials, that will call the given callback, if creating the device was successful.
    /// The data contained in that callbackcan then be used to disconnect and reconnect to the ThingsBoard server as our newly created device.
    /// See https://thingsboard.io/docs/user-guide/device-provisioning/ for more information
    /// @param callback Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    /// @return Whether sending the provisioning request was successful or not
    inline bool Provision_Request(const Provision_Callback& callback) {
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
      // Meaning only the key-value pairs that are needed for the given provisioning method are set,
      // meaning the rest will not be sent and therefore the provisioning request has the correct formatting
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

      const size_t objectSize = Helper::Measure_Json(requestObject);
      return Send_Json(PROV_REQUEST_TOPIC, requestObject, objectSize);
    }

    //----------------------------------------------------------------------------
    // Telemetry API

    /// @brief Attempts to send telemetry data with the given key and value of the given type.
    /// See https://thingsboard.io/docs/user-guide/telemetry/ for more information
    /// @tparam T Type of the passed value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Whether sending the data was successful or not
    template<typename T>
    inline bool sendTelemetryData(const char *key, T value) {
      return sendKeyValue(key, value);
    }

    /// @brief Attempts to send aggregated telemetry data.
    /// See https://thingsboard.io/docs/user-guide/telemetry/ for more information
    /// @param data Array containing all the data we want to send
    /// @param data_count Amount of data entries in the array that we want to send
    /// @return Whether sending the data was successful or not
    inline bool sendTelemetry(const Telemetry *data, size_t data_count) {
      return sendDataArray(data, data_count);
    }

    /// @brief Attempts to send custom json telemetry string.
    /// See https://thingsboard.io/docs/user-guide/telemetry/ for more information
    /// @param json String containing our json key value pairs we want to attempt to send
    /// @return Whether sending the data was successful or not
    inline bool sendTelemetryJson(const char *json) {
      return Send_Json_String(TELEMETRY_TOPIC, json);
    }

    /// @brief Attempts to send telemetry key value pairs from custom source to the server.
    /// See https://thingsboard.io/docs/user-guide/telemetry/ for more information
    /// @tparam TSource Source class that should be used to serialize the json that is sent to the server
    /// @param source Data source containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the source
    /// @return Whether sending the data was successful or not
    template <typename TSource>
    inline bool sendTelemetryJson(const TSource& source, const size_t& jsonSize) {
      return Send_Json(TELEMETRY_TOPIC, source, jsonSize);
    }

    //----------------------------------------------------------------------------
    // Attribute API

    /// @brief Attempts to send attribute data with the given key and value of the given type.
    /// See https://thingsboard.io/docs/user-guide/attributes/ for more information
    /// @tparam T Type of the passed value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @return Whether sending the data was successful or not
    template<typename T>
    inline bool sendAttributeData(const char *key, T value) {
      return sendKeyValue(key, value, false);
    }

    /// @brief Attempts to send aggregated attribute data.
    /// See https://thingsboard.io/docs/user-guide/attributes/ for more information
    /// @param data Array containing all the data we want to send
    /// @param data_count Amount of data entries in the array that we want to send
    /// @return Whether sending the data was successful or not
    inline bool sendAttributes(const Attribute *data, size_t data_count) {
      return sendDataArray(data, data_count, false);
    }

    /// @brief Attempts to send custom json attribute string.
    /// See https://thingsboard.io/docs/user-guide/attributes/ for more information
    /// @param json String containing our json key value pairs we want to attempt to send
    /// @return Whether sending the data was successful or not
    inline bool sendAttributeJSON(const char *json) {
      return Send_Json_String(ATTRIBUTE_TOPIC, json);
    }

    /// @brief Attempts to send attribute key value pairs from custom source to the server.
    /// See https://thingsboard.io/docs/user-guide/attributes/ for more information
    /// @tparam TSource Source class that should be used to serialize the json that is sent to the server
    /// @param source Data source containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the source
    /// @return Whether sending the data was successful or not
    template <typename TSource>
    inline bool sendAttributeJSON(const TSource& source, const size_t& jsonSize) {
      return Send_Json(ATTRIBUTE_TOPIC, source, jsonSize);
    }

    /// @brief Requests one client-side attribute calllback,
    /// that will be called if the key-value pair from the server for the given client-side attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#request-attribute-values-from-the-server for more information
    /// @param callback Callback method that will be called
    /// @return Whether requesting the given callback was successful or not
    inline bool Client_Attributes_Request(const Attribute_Request_Callback& callback) {
      return Attributes_Request(callback, CLIENT_REQUEST_KEYS, CLIENT_RESPONSE_KEY);
    }

    //----------------------------------------------------------------------------
    // Server-side RPC API

#if THINGSBOARD_ENABLE_STL

    /// @brief Subscribes multiple server-side RPC callbacks,
    /// that will be called if a request from the server for the method with the given name is received.
    /// See https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc for more information
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array
    /// @param first_itr Iterator pointing to the first element in the data container
    /// @param last_itr Iterator pointing to the end of the data container (last element + 1)
    /// @return Whether subscribing the given callbacks was successful or not
    template<class InputIterator>
    inline bool RPC_Subscribe(const InputIterator& first_itr, const InputIterator& last_itr) {
#if !THINGSBOARD_ENABLE_DYNAMIC
      const size_t size = std::distance(first_itr, last_itr);
      if (m_rpc_callbacks.size() + size > m_rpc_callbacks.capacity()) {
        Logger::log(MAX_RPC_EXCEEDED);
        return false;
      }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
      if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back complete vector into our local m_rpc_callbacks vector.
      m_rpc_callbacks.insert(m_rpc_callbacks.end(), first_itr, last_itr);
      return true;
    }

#else

    /// @brief Subscribes multiple server-side RPC callbacks,
    /// that will be called if a request from the server for the method with the given name is received.
    /// See https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc for more information
    /// @param callbacks Pointer to the c-style array
    /// @param callbacksSize Amount of values that should be subscribed, ensure size matches the actual array,
    /// if not the system might crash unexpectedly at a later point
    /// @return Whether subscribing the given callbacks was successful or not
    inline bool RPC_Subscribe(const RPC_Callback *callbacks, const size_t& callbacksSize) {
#if !THINGSBOARD_ENABLE_DYNAMIC
      if (m_rpc_callbacks.size() + callbacksSize > m_rpc_callbacks.capacity()) {
        Logger::log(MAX_RPC_EXCEEDED);
        return false;
      }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
      if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      for (size_t i = 0; i < callbacksSize; i++) {
        m_rpc_callbacks.push_back(callbacks[i]);
      }
      return true;
    }

#endif // THINGSBOARD_ENABLE_STL

    /// @brief Subscribe one server-side RPC callback,
    /// that will be called if a request from the server for the method with the given name is received.
    /// See https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc for more information
    /// @param callback Callback method that will be called
    /// @return Whether subscribing the given callback was successful or not
    inline bool RPC_Subscribe(const RPC_Callback& callback) {
#if !THINGSBOARD_ENABLE_DYNAMIC
      if (m_rpc_callbacks.size() + 1 > m_rpc_callbacks.capacity()) {
        Logger::log(MAX_RPC_EXCEEDED);
        return false;
      }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
      if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local vector
      m_rpc_callbacks.push_back(callback);
      return true;
    }

    /// @brief Unsubcribes all server-side RPC callbacks.
    /// See https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc for more information
    /// @return Whether unsubcribing all the previously subscribed callbacks
    /// and from the rpc topic, was successful or not
    inline bool RPC_Unsubscribe() {
      // Empty all callbacks
      m_rpc_callbacks.clear();
      return m_client.unsubscribe(RPC_SUBSCRIBE_TOPIC);
    }

    //----------------------------------------------------------------------------
    // Client-side RPC API

    /// @brief Requests one client-side RPC callback,
    /// that will be called if a response from the server for the method with the given name is received.
    /// See https://thingsboard.io/docs/user-guide/rpc/#client-side-rpc for more information
    /// @param callback Callback method that will be called
    /// @return Whether requesting the given callback was successful or not
    inline bool RPC_Request(const RPC_Request_Callback& callback) {
      const char *methodName = callback.Get_Name();

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

      const JsonArray* parameters = callback.Get_Parameters();

#if THINGSBOARD_ENABLE_DYNAMIC
      // String are const char* and therefore stored as a pointer --> zero copy, meaning the size for the strings is 0 bytes,
      // Data structure size depends on the amount of key value pairs passed + the default methodName and params key needed for the request.
      // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
      const size_t dataStructureMemoryUsage = JSON_OBJECT_SIZE(parameters != nullptr ? parameters->size() + 2U : 2U);
      TBJsonDocument requestBuffer(dataStructureMemoryUsage);
#else
      // Ensure to have enough size for the infinite amount of possible parameters that could be sent to the cloud,
      // therefore we set the size to the MaxFieldsAmt instead of JSON_OBJECT_SIZE(1), which will result in a JsonDocument with a size of 16 bytes
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> requestBuffer;
#endif // !THINGSBOARD_ENABLE_DYNAMIC
      // The .template variant of createing the JsonVariant has to be used,
      // because we are passing a template to the StaticJsonDocument template list
      // and it will generate a compile time error if not used
      const JsonVariant requestVariant = requestBuffer.template as<JsonVariant>();

      requestVariant[RPC_METHOD_KEY] = methodName;

      // Make the parameters for the client side RPC optional,
      // meaning if it is an empty array or null instead we don't send it at all.
      if (parameters != nullptr && !parameters->isNull()) {
        requestVariant[RPC_PARAMS_KEY] = *parameters;
      }
      else {
        requestVariant[RPC_PARAMS_KEY] = RPC_EMPTY_PARAMS_VALUE;
      }

      m_request_id++;
      registeredCallback->Set_Request_ID(m_request_id);

      char topic[Helper::detectSize(RPC_SEND_REQUEST_TOPIC, m_request_id)];
      snprintf_P(topic, sizeof(topic), RPC_SEND_REQUEST_TOPIC, m_request_id);

      const size_t objectSize = Helper::Measure_Json(requestBuffer);
      return Send_Json(topic, requestBuffer, objectSize);
    }

    //----------------------------------------------------------------------------
    // Firmware OTA API

#if THINGSBOARD_ENABLE_OTA

    /// @brief Immediately starts a firmware update if firmware is assigned to the given device.
    /// See https://thingsboard.io/docs/user-guide/ota-updates/ for more information
    /// @param callback Callback method that will be called
    /// @return Whether subscribing the given callback was successful or not
    inline bool Start_Firmware_Update(const OTA_Update_Callback& callback) {
      if (!Prepare_Firmware_Settings(callback))  {
        Logger::log(RESETTING_FAILED);
        return false;
      }

      // Request the firmware information
      const std::vector<const char *> fw_shared_keys{FW_CHKS_KEY, FW_CHKS_ALGO_KEY, FW_SIZE_KEY, FW_TITLE_KEY, FW_VER_KEY};
      const Attribute_Request_Callback fw_request_callback(std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1), fw_shared_keys.cbegin(), fw_shared_keys.cend());
      return Shared_Attributes_Request(fw_request_callback);
    }

    /// @brief Stops the currently running firmware update, calls the finish callback with a failure if the update is running.
    /// See https://thingsboard.io/docs/user-guide/ota-updates/ for more information
    inline void Stop_Firmware_Update() {
      m_ota.Stop_Firmware_Update();
    }

    /// @brief Subscribes for any assignment of firmware to the given device device,
    /// which will then start a firmware update.
    /// See https://thingsboard.io/docs/user-guide/ota-updates/ for more information
    /// @param callback Callback method that will be called
    /// @return Whether subscribing the given callback was successful or not
    inline bool Subscribe_Firmware_Update(const OTA_Update_Callback& callback) {
      if (!Prepare_Firmware_Settings(callback))  {
        Logger::log(RESETTING_FAILED);
        return false;
      }

      // Subscribes to changes of the firmware information
      const std::vector<const char *> fw_shared_keys{FW_CHKS_KEY, FW_CHKS_ALGO_KEY, FW_SIZE_KEY, FW_TITLE_KEY, FW_VER_KEY};
      const Shared_Attribute_Callback fw_update_callback(std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1), fw_shared_keys.cbegin(), fw_shared_keys.cend());
      return Shared_Attributes_Subscribe(fw_update_callback);
    }

    /// @brief Sends the given firmware title and firmware version to the cloud.
    /// See https://thingsboard.io/docs/user-guide/ota-updates/ for more information
    /// @param currFwTitle Current device firmware title
    /// @param currFwVersion Current device firmware version
    /// @return Whether sending the current device firmware information was successful or not
    inline bool Firmware_Send_Info(const char *currFwTitle, const char *currFwVersion) {
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> currentFirmwareInfo;
      const JsonObject currentFirmwareInfoObject = currentFirmwareInfo.to<JsonObject>();

      currentFirmwareInfoObject[CURR_FW_TITLE_KEY] = currFwTitle;
      currentFirmwareInfoObject[CURR_FW_VER_KEY] = currFwVersion;
      return sendTelemetryJson(currentFirmwareInfoObject, Helper::Measure_Json(currentFirmwareInfoObject));
    }

    /// @brief Sends the given firmware state to the cloud.
    /// See https://thingsboard.io/docs/user-guide/ota-updates/ for more information
    /// @param currFwState Current firmware download state
    /// @param fwError Firmware error message that describes the current firmware state,
    /// pass nullptr or an empty string if the current state is not a failure state
    /// and therefore does not require any firmware error messsages
    /// @return Whether sending the current firmware download state was successful or not
    inline bool Firmware_Send_State(const char *currFwState, const char* fwError = nullptr) {
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> currentFirmwareState;
      const JsonObject currentFirmwareStateObject = currentFirmwareState.to<JsonObject>();

      // Make the fw error optional,
      // meaning if it is an empty string or null instead we don't send it at all.
      if (fwError != nullptr && fwError[0] != '\0') {
        currentFirmwareStateObject[FW_ERROR_KEY] = fwError;
      }
      currentFirmwareStateObject[FW_STATE_KEY] = currFwState;
      return sendTelemetryJson(currentFirmwareStateObject, Helper::Measure_Json(currentFirmwareStateObject));
    }

#endif // THINGSBOARD_ENABLE_OTA

    //----------------------------------------------------------------------------
    // Shared attributes API

    /// @brief Requests one shared attribute calllback,
    /// that will be called if the key-value pair from the server for the given shared attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#request-attribute-values-from-the-server for more information
    /// @param callback Callback method that will be called
    /// @return Whether requesting the given callback was successful or not
    inline bool Shared_Attributes_Request(const Attribute_Request_Callback& callback) {
      return Attributes_Request(callback, SHARED_REQUEST_KEY, SHARED_RESPONSE_KEY);
    }

#if THINGSBOARD_ENABLE_STL

    /// @brief Subscribes multiple shared attribute callbacks,
    /// that will be called if the key-value pair from the server for the given shared attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server for more information
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array
    /// @param first_itr Iterator pointing to the first element in the data container
    /// @param last_itr Iterator pointing to the end of the data container (last element + 1)
    /// @return Whether subscribing the given callbacks was successful or not
    template<class InputIterator>
    inline bool Shared_Attributes_Subscribe(const InputIterator& first_itr, const InputIterator& last_itr) {
#if !THINGSBOARD_ENABLE_DYNAMIC
      const size_t size = std::distance(first_itr, last_itr);
      if (m_shared_attribute_update_callbacks.size() + size > m_shared_attribute_update_callbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
      if (!m_client.subscribe(ATTRIBUTE_TOPIC)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back complete vector into our local m_shared_attribute_update_callbacks vector.
      m_shared_attribute_update_callbacks.insert(m_shared_attribute_update_callbacks.end(), first_itr, last_itr);
      return true;
    }

#else

    /// @brief  Subscribes multiple shared attribute callbacks,
    /// that will be called if the key-value pair from the server for the given shared attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server for more information
    /// @param callbacks Pointer to the c-style array
    /// @param callbacksSize Amount of values that should be subscribed, ensure size matches the actual array,
    /// if not the system might crash unexpectedly at a later point
    /// @return Whether subscribing the given callbacks was successful or not
    inline bool Shared_Attributes_Subscribe(const Shared_Attribute_Callback *callbacks, const size_t& callbacksSize) {
#if !THINGSBOARD_ENABLE_DYNAMIC
      if (m_shared_attribute_update_callbacks.size() + callbacksSize > m_shared_attribute_update_callbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
      if (!m_client.subscribe(ATTRIBUTE_TOPIC)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      for (size_t i = 0; i < callbacksSize; i++) {
        m_shared_attribute_update_callbacks.push_back(callbacks[i]);
      }
      return true;
    }

#endif // THINGSBOARD_ENABLE_STL

    /// @brief Subscribe one shared attribute callback,
    /// that will be called if the key-value pair from the server for the given shared attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server for more information
    /// @param callback Callback method that will be called
    /// @return Whether subscribing the given callback was successful or not
    inline bool Shared_Attributes_Subscribe(const Shared_Attribute_Callback& callback) {
#if !THINGSBOARD_ENABLE_DYNAMIC
      if (m_shared_attribute_update_callbacks.size() + 1U > m_shared_attribute_update_callbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_UPDATE_EXCEEDED);
        return false;
      }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
      if (!m_client.subscribe(ATTRIBUTE_TOPIC)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local vector
      m_shared_attribute_update_callbacks.push_back(callback);
      return true;
    }

    /// @brief Unsubcribes all shared attribute callbacks.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server for more information
    /// @return Whether unsubcribing all the previously subscribed callbacks
    /// and from the attribute topic, was successful or not
    inline bool Shared_Attributes_Unsubscribe() {
      // Empty all callbacks
      m_shared_attribute_update_callbacks.clear();
      return m_client.unsubscribe(ATTRIBUTE_TOPIC);
    }
  
  private:

#if THINGSBOARD_ENABLE_STREAM_UTILS

    /// @brief Serialize the custom attribute source into the underlying client.
    /// Sends the given bytes to the client without requiring any temporary buffer at the cost of hugely increased send times
    /// @tparam TSource Source class that should be used to serialize the json that is sent to the server
    /// @param topic Topic we want to send the data over
    /// @param source Data source containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the source
    /// @return Whether sending the data was successful or not
    template <typename TSource>
    inline bool Serialize_Json(const char* topic, const TSource& source, const size_t& jsonSize) {
      if (!m_client.begin_publish(topic, jsonSize)) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }
      BufferingPrint buffered_print(m_client, getBufferingSize());
      const size_t bytes_serialized = serializeJson(source, buffered_print);
      if (bytes_serialized < jsonSize) {
        Logger::log(UNABLE_TO_SERIALIZE_JSON);
        return false;
      }
      buffered_print.flush();
      return m_client.end_publish();
    }

#endif // THINGSBOARD_ENABLE_STREAM_UTILS

#if THINGSBOARD_ENABLE_OTA

    /// @brief Publishes a request via MQTT to request the given firmware chunk
    /// @param request_chunck Chunk index that should be requested from the server
    /// @return Whether publishing the message was successful or not
    inline bool Publish_Chunk_Request(const size_t& request_chunck) {
      // Calculate the number of chuncks we need to request,
      // in order to download the complete firmware binary
      const uint16_t& chunk_size = m_fw_callback->Get_Chunk_Size();

      // Convert the interger size into a readable string
      char size[Helper::detectSize(NUMBER_PRINTF, chunk_size)];
      snprintf_P(size, sizeof(size), NUMBER_PRINTF, chunk_size);
      const size_t jsonSize = strlen(size);

      // Size adjuts dynamically to the current length of the currChunk number to ensure we don't cut it out of the topic string.
      char topic[Helper::detectSize(FIRMWARE_REQUEST_TOPIC, request_chunck)];
      snprintf_P(topic, sizeof(topic), FIRMWARE_REQUEST_TOPIC, request_chunck);

      return m_client.publish(topic, reinterpret_cast<uint8_t*>(size), jsonSize);
    }

#endif // THINGSBOARD_ENABLE_OTA

    /// @brief Returns the maximum amount of bytes that we want to allocate on the stack, before the memory is allocated on the heap instead
    /// @return Maximum amount of bytes we want to allocate on the stack
    inline const size_t& getMaximumStackSize() const {
      return m_max_stack;
    }

#if THINGSBOARD_ENABLE_STREAM_UTILS

    /// @brief Returns the amount of bytes that can be allocated to speed up fall back serialization with the StreamUtils class
    /// See https://github.com/bblanchon/ArduinoStreamUtils for more information on the underlying class used
    /// @return Amount of bytes allocated to speed up serialization
    inline const size_t& getBufferingSize() const {
      return m_buffering_size;
    }

#endif // THINGSBOARD_ENABLE_STREAM_UTILS

    /// @brief Requests one client-side or shared attribute calllback,
    /// that will be called if the key-value pair from the server for the given client-side or shared attributes is received
    /// @param callback Callback method that will be called
    /// @param attributeRequestKey Key of the key-value pair that will contain the attributes we want to request
    /// @param attributeResponseKey Key of the key-value pair that will contain the attributes we got as a response
    /// @return Whether requesting the given callback was successful or not
    inline bool Attributes_Request(const Attribute_Request_Callback& callback, const char* attributeRequestKey, const char* attributeResponseKey) {
#if THINGSBOARD_ENABLE_STL
      const std::vector<const char *>& attributes = callback.Get_Attributes();

      // Check if any sharedKeys were requested
      if (attributes.empty()) {
        Logger::log(NO_KEYS_TO_REQUEST);
        return false;
      }
      else if (attributeRequestKey == nullptr || attributeResponseKey == nullptr) {
#if THINGSBOARD_ENABLE_DEBUG
        Logger::log(ATT_KEY_NOT_FOUND);
#endif // THINGSBOARD_ENABLE_DEBUG
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

      // String are const char* and therefore stored as a pointer --> zero copy, meaning the size for the strings is 0 bytes,
      // Data structure size depends on the amount of key value pairs passed + the default clientKeys or sharedKeys
      // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
      constexpr size_t dataStructureMemoryUsage = JSON_OBJECT_SIZE(1U);
      StaticJsonDocument<dataStructureMemoryUsage> requestBuffer;
      // The .template variant of createing the JsonVariant has to be used,
      // because we are passing a template to the StaticJsonDocument template list
      // and it will generate a compile time error if not used
      const JsonVariant requestVariant = requestBuffer.template as<JsonVariant>();

#if THINGSBOARD_ENABLE_STL
      std::string request;

      for (const char *att : attributes) {
        // Check if the given attribute is null, if it is skip it
        if (att == nullptr) {
#if THINGSBOARD_ENABLE_DEBUG
          Logger::log(ATT_IS_NULL);
#endif // THINGSBOARD_ENABLE_DEBUG
          continue;
        }

        request.append(att);
        request.push_back(COMMA);
      }

      // Remove latest not needed ,
      request.pop_back();

      requestVariant[attributeRequestKey] = request.c_str();
#else
      requestVariant[attributeRequestKey] = request;
#endif // THINGSBOARD_ENABLE_STL

      m_request_id++;
      registeredCallback->Set_Request_ID(m_request_id);
      registeredCallback->Set_Attribute_Key(attributeResponseKey);

      char topic[Helper::detectSize(ATTRIBUTE_REQUEST_TOPIC, m_request_id)];
      snprintf_P(topic, sizeof(topic), ATTRIBUTE_REQUEST_TOPIC, m_request_id);

      const size_t objectSize = Helper::Measure_Json(requestBuffer);
      return Send_Json(topic, requestBuffer, objectSize);
    }

    /// @brief Subscribes one provision callback,
    /// that will be called if a provision response from the server is received
    /// @param callback Callback method that will be called
    /// @return Whether requesting the given callback was successful or not
    inline bool Provision_Subscribe(const Provision_Callback& callback) {
      if (!m_client.subscribe(PROV_RESPONSE_TOPIC)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }
      m_provision_callback = callback;
      return true;
    }

    /// @brief Unsubcribes the provision callback
    /// @return Whether unsubcribing the previously subscribed callback
    /// and from the provision response topic, was successful or not
    inline bool Provision_Unsubscribe() {
      m_provision_callback = Provision_Callback();
      return m_client.unsubscribe(PROV_RESPONSE_TOPIC);
    }

#if THINGSBOARD_ENABLE_OTA

    /// @brief Checks the included information in the callback,
    /// and attempts to sends the current device firmware information to the cloud
    /// @param callback Callback method that will be called
    /// @return Whether checking and sending the current device firmware information was successful or not
    inline bool Prepare_Firmware_Settings(const OTA_Update_Callback& callback) {
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
      m_fw_callback = &callback;
      return true;
    }

    /// @brief Subscribes to the firmware response topic
    /// @return Whether subscribing to the firmware response topic was successful or not
    inline bool Firmware_OTA_Subscribe() {
      if (!m_client.subscribe(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        Firmware_Send_State(FW_STATE_FAILED, SUBSCRIBE_TOPIC_FAILED);
        return false;
      }
      return true;
    }

    /// @brief Unsubscribes from the firmware response topic and clears any memory associated with the firmware update,
    /// should not be called before actually fully completing the firmware update.
    /// @return Whether unsubscribing from the firmware response topic was successful or not
    inline bool Firmware_OTA_Unsubscribe() {
      // Buffer size has been set to another value before the update,
      // to allow to receive ota chunck packets that might be much bigger than the normal
      // buffer size would allow, therefore we return to the previous value to decrease overall memory usage
      if (m_change_buffer_size) {
          m_client.set_buffer_size(m_previous_buffer_size);
      }
      // Reset now not needed private member variables
      m_fw_callback = nullptr;
      // Unsubscribe from the topic
      return m_client.unsubscribe(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC);
    }

    /// @brief Callback that will be called upon firmware shared attribute arrival
    /// @param data Json data containing key-value pairs for the needed firmware information,
    /// to ensure we have a firmware assigned and can start the update over MQTT
    inline void Firmware_Shared_Attribute_Received(const Shared_Attribute_Data& data) {
      // Check if firmware is available for our device
      if (!data.containsKey(FW_VER_KEY) || !data.containsKey(FW_TITLE_KEY)) {
        Logger::log(NO_FW);
        Firmware_Send_State(FW_STATE_FAILED, NO_FW);
        return;
      }
      else if (m_fw_callback == nullptr) {
        Logger::log(OTA_CB_IS_NULL);
        Firmware_Send_State(FW_STATE_FAILED, OTA_CB_IS_NULL);
        return;
      }

      const char *fw_title = data[FW_TITLE_KEY].as<const char *>();
      const char *fw_version = data[FW_VER_KEY].as<const char *>();
      const std::string fw_checksum = data[FW_CHKS_KEY].as<std::string>();
      const std::string fw_algorithm = data[FW_CHKS_ALGO_KEY].as<std::string>();
      const size_t fw_size = data[FW_SIZE_KEY].as<const size_t>();

      const char *curr_fw_title = m_fw_callback->Get_Firmware_Title();
      const char *curr_fw_version = m_fw_callback->Get_Firmware_Version();

      if (fw_title == nullptr || fw_version == nullptr || curr_fw_title == nullptr || curr_fw_version == nullptr || fw_algorithm.empty() || fw_checksum.empty()) {
        Logger::log(EMPTY_FW);
        Firmware_Send_State(FW_STATE_FAILED, EMPTY_FW);
        return;
      }
      // If firmware version and title is the same, we do not initiate an update, because we expect the binary to be the same one we are currently using
      else if (strncmp_P(curr_fw_title, fw_title, JSON_STRING_SIZE(strlen(curr_fw_title))) == 0 && strncmp_P(curr_fw_version, fw_version, JSON_STRING_SIZE(strlen(curr_fw_version))) == 0) {
        Logger::log(FW_UP_TO_DATE);
        Firmware_Send_State(FW_STATE_FAILED, FW_UP_TO_DATE);
        return;
      }
      // If firmware title is not the same, we do not initiate an update, because we expect the binary to be for another device type 
      else if (strncmp_P(curr_fw_title, fw_title, JSON_STRING_SIZE(strlen(curr_fw_title))) != 0) {
        Logger::log(FW_NOT_FOR_US);
        Firmware_Send_State(FW_STATE_FAILED, FW_NOT_FOR_US);
        return;
      }

      mbedtls_md_type_t fw_checksum_algorithm = mbedtls_md_type_t();

      // Change the used firmware algorithm, depending on which type is set for the given firmware information
      if (fw_algorithm.compare(CHECKSUM_AGORITM_MD5) == 0) {
        fw_checksum_algorithm = mbedtls_md_type_t::MBEDTLS_MD_MD5;
      }
      else if (fw_algorithm.compare(CHECKSUM_AGORITM_SHA256) == 0) {
        fw_checksum_algorithm = mbedtls_md_type_t::MBEDTLS_MD_SHA256;
      }
      else if (fw_algorithm.compare(CHECKSUM_AGORITM_SHA384) == 0) {
        fw_checksum_algorithm = mbedtls_md_type_t::MBEDTLS_MD_SHA384;
      }
      else if (fw_algorithm.compare(CHECKSUM_AGORITM_SHA512) == 0) {
        fw_checksum_algorithm = mbedtls_md_type_t::MBEDTLS_MD_SHA512;
      }
      else {
        char message[JSON_STRING_SIZE(strlen(FW_CHKS_ALGO_NOT_SUPPORTED)) + JSON_STRING_SIZE(fw_algorithm.size())];
        snprintf_P(message, sizeof(message), FW_CHKS_ALGO_NOT_SUPPORTED, fw_algorithm.c_str());
        Logger::log(message);
        Firmware_Send_State(FW_STATE_FAILED, message);
        return;
      }

      if (!Firmware_OTA_Subscribe()) {
        return;
      }

#if THINGSBOARD_ENABLE_DEBUG
      Logger::log(PAGE_BREAK);
      Logger::log(NEW_FW);
      char firmware[JSON_STRING_SIZE(strlen(FROM_TOO)) + JSON_STRING_SIZE(strlen(curr_fw_version)) + JSON_STRING_SIZE(strlen(fw_version))];
      snprintf_P(firmware, sizeof(firmware), FROM_TOO, curr_fw_version, fw_version);
      Logger::log(firmware);
      Logger::log(DOWNLOADING_FW);
#endif // THINGSBOARD_ENABLE_DEBUG

      // Calculate the number of chuncks we need to request,
      // in order to download the complete firmware binary
      const uint16_t& chunk_size = m_fw_callback->Get_Chunk_Size();

      // Get the previous buffer size and cache it so the previous settings can be restored.
      m_previous_buffer_size = m_client.get_buffer_size();
      m_change_buffer_size = m_previous_buffer_size < (chunk_size + 50U);

      // Increase size of receive buffer
      if (m_change_buffer_size && !m_client.set_buffer_size(chunk_size + 50U)) {
        Logger::log(NOT_ENOUGH_RAM);
        Firmware_Send_State(FW_STATE_FAILED, NOT_ENOUGH_RAM);
        return;
      }

      m_ota.Start_Firmware_Update(m_fw_callback, fw_size, fw_algorithm, fw_checksum, fw_checksum_algorithm);
    }

#endif // THINGSBOARD_ENABLE_OTA

    /// @brief Connects to the previously set ThingsBoard server, as the given client with the given access token
    /// @param access_token Access token that connects this device with a created device on the ThingsBoard server,
    /// can be "provision", if the device creates itself instead
    /// @param client_id Client username that can be used to differentiate the user that is connecting the given device to ThingsBoard
    /// @param password Client password that can be used to authenticate the user that is connecting the given device to ThingsBoard
    /// @return Whether connecting to ThingsBoard was successful or not
    inline bool connect_to_host(const char *access_token, const char *client_id, const char *password) {
      const bool connection_result = m_client.connect(client_id, access_token, password);
      
      if (!connection_result) {
        Logger::log(CONNECT_FAILED);
        return connection_result;
      }

      // Only attempt to resubscribe if we connected successfully
      Resubscribe_Topics();
      return connection_result;
    }

    /// @brief Resubscribes to topics that establish a permanent connection with MQTT, meaning they may receive more than one event over their lifetime,
    /// whereas other events that are only ever called once and then deleted after they have been handled are not resubscribed.
    /// This is done, because the chance of disconnecting the moment when a request event (provisioning, attribute request, client-side rpc) was sent
    /// and then reconnecting and resubscribing to that topic fast enough to still receive the message is not feasible
    inline void Resubscribe_Topics() {
      if (!m_rpc_callbacks.empty()) {
        m_client.subscribe(RPC_SUBSCRIBE_TOPIC);
      }
      if (!m_shared_attribute_update_callbacks.empty()) {
        m_client.subscribe(ATTRIBUTE_TOPIC);
      }
    }

#if !THINGSBOARD_ENABLE_DYNAMIC
    /// @brief Reserves size for the given amount of items in our internal callback vectors beforehand for performance reasons,
    /// this ensures the internal memory blocks do not have to move if new data is inserted,
    /// because the current memory block is too small to hold the data for all the values
    /// @param reservedSize Size we want to reserve and should not use more if we do,
    /// the internal memory blocks might need to be moved to a new location
    inline void reserve_callback_size(const size_t& reservedSize) {
      m_rpc_callbacks.reserve(reservedSize);
      m_rpc_request_callbacks.reserve(reservedSize);
      m_shared_attribute_update_callbacks.reserve(reservedSize);
      m_attribute_request_callbacks.reserve(reservedSize);
    }
#endif // !THINGSBOARD_ENABLE_DYNAMIC

    /// @brief Subscribes to the client-side RPC response topic
    /// @param callback Callback method that will be called
    /// @param registeredCallback Editable pointer to a reference of the local version that was copied from the passed callback
    /// @return Whether requesting the given callback was successful or not
    inline bool RPC_Request_Subscribe(const RPC_Request_Callback& callback, RPC_Request_Callback*& registeredCallback = nullptr) {
#if !THINGSBOARD_ENABLE_DYNAMIC
      if (m_rpc_request_callbacks.size() + 1 > m_rpc_request_callbacks.capacity()) {
        Logger::log(MAX_RPC_REQUEST_EXCEEDED);
        return false;
      }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
      if (!m_client.subscribe(RPC_RESPONSE_SUBSCRIBE_TOPIC)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local vector
      m_rpc_request_callbacks.push_back(callback);
      registeredCallback = &m_rpc_request_callbacks.back();
      return true;
    }

    /// @brief Unsubscribes all client-side RPC request callbacks
    /// @return Whether unsubcribing the previously subscribed callbacks
    /// and from the client-side RPC response topic, was successful or not
    inline bool RPC_Request_Unsubscribe() {
      // Empty all callbacks
      m_rpc_request_callbacks.clear();
      return m_client.unsubscribe(RPC_RESPONSE_SUBSCRIBE_TOPIC);
    }

    /// @brief Subscribes to attribute response topic
    /// @param callback Callback method that will be called
    /// @param registeredCallback Editable pointer to a reference of the local version that was copied from the passed callback
    /// @return Whether requesting the given callback was successful or not
    inline bool Attributes_Request_Subscribe(const Attribute_Request_Callback& callback, Attribute_Request_Callback*& registeredCallback = nullptr) {
#if !THINGSBOARD_ENABLE_DYNAMIC
      if (m_attribute_request_callbacks.size() + 1 > m_attribute_request_callbacks.capacity()) {
        Logger::log(MAX_SHARED_ATT_REQUEST_EXCEEDED);
        return false;
      }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
      if (!m_client.subscribe(ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC)) {
        Logger::log(SUBSCRIBE_TOPIC_FAILED);
        return false;
      }

      // Push back given callback into our local vector
      m_attribute_request_callbacks.push_back(callback);
      registeredCallback = &m_attribute_request_callbacks.back();
      return true;
    }

    /// @brief Unsubscribes all client-side or shared attributes request callbacks
    /// @return Whether unsubcribing the previously subscribed callbacks
    /// and from the  attribute response topic, was successful or not
    inline bool Attributes_Request_Unsubscribe() {
      // Empty all callbacks
      m_attribute_request_callbacks.clear();
      return m_client.unsubscribe(ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC);
    }

    /// @brief Attempts to send a single key-value pair with the given key and value of the given type
    /// @tparam T Type of the passed value
    /// @param key Key of the key value pair we want to send
    /// @param value Value of the key value pair we want to send
    /// @param telemetry Whether the data we want to send should be sent as an attribute or telemetry data value
    /// @return Whether sending the data was successful or not
    template<typename T>
    inline bool sendKeyValue(const char *key, T value, bool telemetry = true) {
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

      return telemetry ? sendTelemetryJson(object, Helper::Measure_Json(object)) : sendAttributeJSON(object, Helper::Measure_Json(object));
    }

    /// @brief Process callback that will be called upon client-side RPC response arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    inline void process_rpc_request_message(char *topic, const JsonObjectConst& data) {
      // Remove the not needed part of the received topic string, which is everything before the response id,
      // therefore we remove the section before that which is the topic + an additional "/" character, that seperates the topic from the response id.
      // Meaning the index we want to get the substring from is the length of the topic + 1 for the additonal "/" character
      const size_t index = strlen(RPC_RESPONSE_TOPIC) + 1U;
#if THINGSBOARD_ENABLE_STL
      std::string response = topic;
      response = response.substr(index, response.length() - index);
#else
      String response = topic;
      response = response.substring(index);
#endif // THINGSBOARD_ENABLE_STL

      // Convert the remaining text after the topic to an integer, because it should now contain only the response id
      const size_t response_id = atoi(response.c_str());

      for (size_t i = 0; i < m_rpc_request_callbacks.size(); i++) {
        const RPC_Request_Callback& rpc_request = m_rpc_request_callbacks.at(i);

        if (rpc_request.Get_Request_ID() != response_id) {
          continue;
        }

#if THINGSBOARD_ENABLE_DEBUG
        char message[Helper::detectSize(CALLING_REQUEST_CB, response_id)];
        snprintf_P(message, sizeof(message), CALLING_REQUEST_CB, response_id);
        Logger::log(message);
#endif // THINGSBOARD_ENABLE_DEBUG

        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        rpc_request.Call_Callback<Logger>(data);

        // Delete callback because the changes have been requested and the callback is no longer needed
        Helper::remove(m_rpc_request_callbacks, i);
        break;
      }

      // Attempt to unsubscribe from the shared attribute request topic,
      // if we are not waiting for any further responses with shared attributes from the server.
      // Will be resubscribed if another request is sent anyway
      if (m_rpc_request_callbacks.empty()) {
        RPC_Request_Unsubscribe();
      }
    }

    /// @brief Process callback that will be called upon server-side RPC request arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    inline void process_rpc_message(char *topic, const JsonObjectConst& data) {
      const char *methodName = data[RPC_METHOD_KEY].as<const char *>();

      if (methodName == nullptr) {
        Logger::log(RPC_METHOD_NULL);
        return;
      }
 
      RPC_Response response;

      for (const RPC_Callback& rpc : m_rpc_callbacks) {
        const char *subscribedMethodName = rpc.Get_Name();
        if (subscribedMethodName == nullptr) {
          Logger::log(RPC_METHOD_NULL);
          continue;
        }
        // Strncmp returns the ascii value difference of the ascii characters that are different,
        // meaning 0 is the same string and less and more than 0 is the difference in ascci values between the 2 chararacters.
        else if (strncmp(subscribedMethodName, methodName, strlen(subscribedMethodName)) != 0) {
          continue;
        }

        // Do not inform client, if parameter field is missing for some reason
        if (!data.containsKey(RPC_PARAMS_KEY)) {
#if THINGSBOARD_ENABLE_DEBUG
          Logger::log(NO_RPC_PARAMS_PASSED);
#endif // THINGSBOARD_ENABLE_DEBUG
        }

#if THINGSBOARD_ENABLE_DEBUG
        char message[JSON_STRING_SIZE(strlen(CALLING_RPC_CB)) + JSON_STRING_SIZE(strlen(methodName))];
        snprintf_P(message, sizeof(message), CALLING_RPC_CB, methodName);
        Logger::log(message);
#endif // THINGSBOARD_ENABLE_DEBUG

        const JsonVariantConst param = data[RPC_PARAMS_KEY].as<JsonVariantConst>();
        response = rpc.Call_Callback<Logger>(param);
        break;
      }

      if (response.isNull()) {
        // Message is ignored and not sent at all.
        return;
      }

      // Remove the not needed part of the received topic string, which is everything before the request id,
      // therefore we remove the section before that which is the topic + an additional "/" character, that seperates the topic from the request id.
      // Meaning the index we want to get the substring from is the length of the topic + 1 for the additonal "/" character
      const size_t index = strlen(RPC_REQUEST_TOPIC) + 1U;
#if THINGSBOARD_ENABLE_STL
      std::string request = topic;
      request = request.substr(index, request.length() - index);
#else
      String request = topic;
      request = request.substring(index);
#endif // THINGSBOARD_ENABLE_STL

      // Convert the remaining text after the topic to an integer, because it should now contain only the request id
      const size_t request_id = atoi(request.c_str());

      char responseTopic[Helper::detectSize(RPC_SEND_RESPONSE_TOPIC, request_id)];
      snprintf_P(responseTopic, sizeof(responseTopic), RPC_SEND_RESPONSE_TOPIC, request_id);

      const size_t jsonSize = Helper::Measure_Json(response);
      Send_Json(responseTopic, response, jsonSize);
    }

#if THINGSBOARD_ENABLE_OTA

    /// @brief Process callback that will be called upon firmware response arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callback
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
    inline void process_firmware_response(char *topic, uint8_t *payload, const size_t& length) {
      // Remove the not needed part of the received topic string, which is everything before the request id,
      // therefore we remove the section before that which is the topic + an additional "/" character, that seperates the topic from the request id.
      // Meaning the index we want to get the substring from is the length of the topic + 1 for the additonal "/" character
      const size_t index = strlen(FIRMWARE_RESPONSE_TOPIC) + 1U;
#if THINGSBOARD_ENABLE_STL
      std::string request = topic;
      request = request.substr(index, request.length() - index);
#else
      String request = topic;
      request = request.substring(index);
#endif // THINGSBOARD_ENABLE_STL

      // Convert the remaining text after the topic to an integer, because it should now contain only the request id
      const size_t request_id = atoi(request.c_str());

      // Check if the remaining stack size of the current task would overflow the stack,
      // if it would allocate the memory on the heap instead to ensure no stack overflow occurs.
      if (getMaximumStackSize() < length) {
        uint8_t* binary = new uint8_t[length];
        memcpy(binary, payload, length);
        m_ota.Process_Firmware_Packet(request_id, binary, length);
        // Ensure to actually delete the memory placed onto the heap, to make sure we do not create a memory leak
        // and set the pointer to null so we do not have a dangling reference.
        delete[] binary;
        binary = nullptr;
      }
      else {
        uint8_t binary[length];
        memcpy(binary, payload, length);
        m_ota.Process_Firmware_Packet(request_id, binary, length);
      }
    }

#endif // THINGSBOARD_ENABLE_OTA

    /// @brief Process callback that will be called upon shared attribute update arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    inline void process_shared_attribute_update_message(char *topic, JsonObjectConst& data) {
      if (!data) {
#if THINGSBOARD_ENABLE_DEBUG
        Logger::log(NOT_FOUND_ATT_UPDATE);
#endif // THINGSBOARD_ENABLE_DEBUG
        return;
      }

      if (data.containsKey(SHARED_RESPONSE_KEY)) {
        data = data[SHARED_RESPONSE_KEY];
      }

      for (const Shared_Attribute_Callback& shared_attribute : m_shared_attribute_update_callbacks) {
#if THINGSBOARD_ENABLE_STL
        if (shared_attribute.Get_Attributes().empty()) {
#else
        if (shared_attribute.Get_Attributes() == nullptr) {
#endif // THINGSBOARD_ENABLE_STL
#if THINGSBOARD_ENABLE_DEBUG
          Logger::log(ATT_CB_NO_KEYS);
#endif // THINGSBOARD_ENABLE_DEBUG
          // No specifc keys were subscribed so we call the callback anyway
          shared_attribute.Call_Callback<Logger>(data);
          continue;
        }

        bool containsKey = false;
        const char *requested_att = nullptr;

#if THINGSBOARD_ENABLE_STL
        for (const char *att : shared_attribute.Get_Attributes()) {
#else
        String stringToSplit = shared_attribute.Get_Attributes();
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
#if THINGSBOARD_ENABLE_DEBUG
            Logger::log(ATT_IS_NULL);
#endif // THINGSBOARD_ENABLE_DEBUG
            continue;
          }
          // Check if the request contained any of our requested keys.
          containsKey = containsKey || data.containsKey(att);
          // Break early if the key was requested from this callback.
          if (containsKey) {
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
#if THINGSBOARD_ENABLE_DEBUG
          Logger::log(ATT_NO_CHANGE);
#endif // THINGSBOARD_ENABLE_DEBUG
          continue;
        }

#if THINGSBOARD_ENABLE_DEBUG
        char calling_message[JSON_STRING_SIZE(strlen(CALLING_ATT_CB)) + JSON_STRING_SIZE(strlen(requested_att))];
        snprintf_P(calling_message, sizeof(calling_message), CALLING_ATT_CB, requested_att);
        Logger::log(calling_message);
#endif // THINGSBOARD_ENABLE_DEBUG

        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        shared_attribute.Call_Callback<Logger>(data);
      }
    }

    /// @brief Process callback that will be called upon client-side or shared attribute request arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    inline void process_attribute_request_message(char *topic, JsonObjectConst& data) {
      // Remove the not needed part of the received topic string, which is everything before the response id,
      // therefore we remove the section before that which is the topic + an additional "/" character, that seperates the topic from the response id.
      // Meaning the index we want to get the substring from is the length of the topic + 1 for the additonal "/" character
      const size_t index = strlen(ATTRIBUTE_RESPONSE_TOPIC) + 1U;
#if THINGSBOARD_ENABLE_STL
      std::string response = topic;
      response = response.substr(index, response.length() - index);
#else
      String response = topic;
      response = response.substring(index);
#endif // THINGSBOARD_ENABLE_STL

      // Convert the remaining text after the topic to an integer, because it should now contain only the response id
      const size_t response_id = atoi(response.c_str());

#if THINGSBOARD_ENABLE_DEBUG
      char message[Helper::detectSize(CALLING_REQUEST_CB, response_id)];
#endif // THINGSBOARD_ENABLE_DEBUG
      for (size_t i = 0; i < m_attribute_request_callbacks.size(); i++) {
        const Attribute_Request_Callback& attribute_request = m_attribute_request_callbacks.at(i);

        if (attribute_request.Get_Request_ID() != response_id) {
          continue;
        }
        const char *attributeResponseKey = attribute_request.Get_Attribute_Key();
        if (attributeResponseKey == nullptr) {
#if THINGSBOARD_ENABLE_DEBUG
          Logger::log(ATT_KEY_NOT_FOUND);
#endif // THINGSBOARD_ENABLE_DEBUG
          goto delete_callback;
        }
        else if (!data) {
#if THINGSBOARD_ENABLE_DEBUG
          Logger::log(ATT_KEY_NOT_FOUND);
#endif // THINGSBOARD_ENABLE_DEBUG
          goto delete_callback;
        }

        if (data.containsKey(attributeResponseKey)) {
          data = data[attributeResponseKey];
        }

#if THINGSBOARD_ENABLE_DEBUG
        snprintf_P(message, sizeof(message), CALLING_REQUEST_CB, response_id);
        Logger::log(message);
#endif // THINGSBOARD_ENABLE_DEBUG

        // Getting non-existing field from JSON should automatically
        // set JSONVariant to null
        attribute_request.Call_Callback<Logger>(data);

        delete_callback:
        // Delete callback because the changes have been requested and the callback is no longer needed
        Helper::remove(m_attribute_request_callbacks, i);
        break;
      }

      // Unsubscribe from the shared attribute request topic,
      // if we are not waiting for any further responses with shared attributes from the server.
      // Will be resubscribed if another request is sent anyway
      if (m_attribute_request_callbacks.empty()) {
        Attributes_Request_Unsubscribe();
      }
    }

    /// @brief Process callback that will be called upon provision response arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callback
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    inline void process_provisioning_response(char *topic, const JsonObjectConst& data) {
      m_provision_callback.Call_Callback<Logger>(data);
      // Unsubscribe from the provision response topic,
      // Will be resubscribed if another request is sent anyway
      Provision_Unsubscribe();
    }

    /// @brief Attempts to send aggregated attribute or telemetry data
    /// @param data Array containing all the data we want to send
    /// @param data_count Amount of data entries in the array that we want to send
    /// @param telemetry Whether the data we want to send should be sent as an attribute or telemetry data value
    /// @return Whether sending the data was successful or not
    inline bool sendDataArray(const Telemetry *data, size_t data_count, bool telemetry = true) {
#if THINGSBOARD_ENABLE_DYNAMIC
      // String are const char* and therefore stored as a pointer --> zero copy, meaning the size for the strings is 0 bytes,
      // Data structure size depends on the amount of key value pairs passed.
      // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
      const size_t dataStructureMemoryUsage = JSON_OBJECT_SIZE(data_count);
      TBJsonDocument jsonBuffer(dataStructureMemoryUsage);
#else
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
#endif // !THINGSBOARD_ENABLE_DYNAMIC

      const JsonVariant object = jsonBuffer.template to<JsonVariant>();

      for (size_t i = 0; i < data_count; i++) {
        if (!data[i].SerializeKeyValue(object)) {
          Logger::log(UNABLE_TO_SERIALIZE);
          return false;
        }
      }

      return telemetry ? sendTelemetryJson(object, Helper::Measure_Json(object)) : sendAttributeJSON(object, Helper::Measure_Json(object));
    }

    /// @brief Vector signature
#if THINGSBOARD_ENABLE_STL
    template<typename T>
    using Vector = std::vector<T>;
#endif // THINGSBOARD_ENABLE_STL

    IMQTT_Client& m_client; // MQTT client instance.
    size_t m_max_stack; // Maximum stack size we allocate at once.
    size_t m_buffering_size; // Buffering size used to serialize directly into client.

    // Vectors hold copy of the actual passed data, this is to ensure they stay valid,
    // even if the user only temporarily created the object before the method was called.
    // This can be done because all Callback methods mostly consists of pointers to actual object so copying them
    // does not require a huge memory overhead and is acceptable especially in comparsion to possible problems that could
    // arise if references were used and the end user does not take care to ensure the Callbacks live on for the entirety
    // of its usage, which will lead to dangling references and undefined behaviour.
    // Therefore copy-by-value has been choosen as for this specific use case it is more advantageous,
    // especially because at most we copy a vector, that will only ever contain a few pointers
    Vector<RPC_Callback> m_rpc_callbacks; // Server side RPC callbacks vector, replacement for non C++ STL boards
    Vector<RPC_Request_Callback> m_rpc_request_callbacks; // Client side RPC callbacks vector, replacement for non C++ STL boards
    Vector<Shared_Attribute_Callback> m_shared_attribute_update_callbacks; // Shared attribute update callbacks vector, replacement for non C++ STL boards
    Vector<Attribute_Request_Callback> m_attribute_request_callbacks; // Client-side or shared attribute request callback vector, replacement for non C++ STL boards

    Provision_Callback m_provision_callback; // Provision response callback
    size_t m_request_id; // Allows nearly 4.3 million requests before wrapping back to 0

#if THINGSBOARD_ENABLE_OTA
    const OTA_Update_Callback *m_fw_callback; // Ota update response callback
    uint16_t m_previous_buffer_size; // Previous buffer size of the underlying client, used to revert to the previously configured buffer size if it was temporarily increased by the OTA update
    bool m_change_buffer_size; // Whether the buffer size had to be changed, because the previous internal buffer size was to small to hold the firmware chunks
    OTA_Handler<Logger> m_ota; // Class instance that handles the flashing and creating a hash from the given received binary firmware data
#endif // THINGSBOARD_ENABLE_OTA

    /// @brief MQTT callback that will be called if a publish message is received from the server
    /// @param topic Previously subscribed topic, we got the response over 
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
    inline void onMQTTMessage(char *topic, uint8_t *payload, unsigned int length) {
#if THINGSBOARD_ENABLE_DEBUG
      char message[JSON_STRING_SIZE(strlen(RECEIVE_MESSAGE)) + JSON_STRING_SIZE(strlen(topic))];
      snprintf_P(message, sizeof(message), RECEIVE_MESSAGE, topic);
      Logger::log(message);
#endif // THINGSBOARD_ENABLE_DEBUG

#if THINGSBOARD_ENABLE_OTA
      // When receiving the ota binary payload we do not want to deserialize it into json, because it only contains
      // firmware bytes that should be directly writtin into flash, therefore we can skip that step and directly process those bytes
      if (strncmp_P(FIRMWARE_RESPONSE_TOPIC, topic, strlen(FIRMWARE_RESPONSE_TOPIC)) == 0) {
        process_firmware_response(topic, payload, length);
        return;
      }
#endif // THINGSBOARD_ENABLE_OTA

#if THINGSBOARD_ENABLE_DYNAMIC
      // Buffer that we deserialize is writeable and not read only --> zero copy, meaning the size for the data is 0 bytes,
      // Data structure size depends on the amount of key value pairs received.
      // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
      const size_t dataStructureMemoryUsage = JSON_OBJECT_SIZE(Helper::getOccurences(reinterpret_cast<char*>(payload), COLON));
      TBJsonDocument jsonBuffer(dataStructureMemoryUsage);
#else
      StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmt)> jsonBuffer;
#endif // !THINGSBOARD_ENABLE_DYNAMIC

      // The deserializeJson method we use, can use the zero copy mode because a writeable input was passed,
      // if that were not the case the needed allocated memory would drastically increase, because the keys would need to be copied as well.
      // See https://arduinojson.org/v6/doc/deserialization/ for more info on ArduinoJson deserialization
      const DeserializationError error = deserializeJson(jsonBuffer, payload, length);
      if (error) {
        char message[Helper::detectSize(UNABLE_TO_DE_SERIALIZE_JSON, error.c_str())];
        snprintf_P(message, sizeof(message), UNABLE_TO_DE_SERIALIZE_JSON, error.c_str());
        Logger::log(message);
        return;
      }
      // .as() is used instead of .to(), because it is meant to cast the JsonDocument to the given type,
      // but it does not change the actual content of the JsonDocument, we don't want that because it already contents content
      // and would result in the data simply being "null", instead .as() allows accessing the data over a JsonObjectConst instead.
      JsonObjectConst data = jsonBuffer.template as<JsonObjectConst>();

      // Checks to ensure we forward the already json serialized data to the correct process function,
      // especially important is the order of ATTRIBUTE_RESPONSE_TOPIC and ATTRIBUTE_TOPIC,
      // that is the case because the more specific one that is longer but contains the same text has to be checked first,
      // because if we do not do that then even if we receive a message from the ATTRIBUTE_RESPONSE_TOPIC
      // we would call the process method for the ATTRIBUTE_TOPIC because we only compare until the end of the ATTRIBUTE_TOPIC string,
      // therefore the received topic is exactly the same. Therefore the ordering needs to stay the same for thoose two specific checks
      if (strncmp_P(RPC_RESPONSE_TOPIC, topic, strlen(RPC_RESPONSE_TOPIC)) == 0) {
        process_rpc_request_message(topic, data);
      }
      else if (strncmp_P(RPC_REQUEST_TOPIC, topic, strlen(RPC_REQUEST_TOPIC)) == 0) {
        process_rpc_message(topic, data);
      }
      else if (strncmp_P(ATTRIBUTE_RESPONSE_TOPIC, topic, strlen(ATTRIBUTE_RESPONSE_TOPIC)) == 0) {
        process_attribute_request_message(topic, data);
      }
      else if (strncmp_P(ATTRIBUTE_TOPIC, topic, strlen(ATTRIBUTE_TOPIC)) == 0) {
        process_shared_attribute_update_message(topic, data);
      }
      else if (strncmp_P(PROV_RESPONSE_TOPIC, topic, strlen(PROV_RESPONSE_TOPIC)) == 0) {
        process_provisioning_response(topic, data);
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

#if !THINGSBOARD_ENABLE_STL && !THINGSBOARD_ENABLE_DYNAMIC

template<size_t MaxFieldsAmt, typename Logger>
ThingsBoardSized<MaxFieldsAmt, Logger> *ThingsBoardSized<MaxFieldsAmt, Logger>::m_subscribedInstance = nullptr;

#elif !THINGSBOARD_ENABLE_STL && THINGSBOARD_ENABLE_DYNAMIC

template<typename Logger>
ThingsBoardSized<Logger> *ThingsBoardSized<Logger>::m_subscribedInstance = nullptr;

#endif

using ThingsBoard = ThingsBoardSized<>;

#endif // ThingsBoard_h
