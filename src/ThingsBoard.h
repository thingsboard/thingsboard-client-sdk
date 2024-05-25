#ifndef ThingsBoard_h
#define ThingsBoard_h

// Local includes.
#include "Shared_Attribute_Callback.h"
#include "Attribute_Request_Callback.h"
#include "RPC_Callback.h"
#include "RPC_Request_Callback.h"
#include "Provision_Callback.h"
#include "OTA_Handler.h"
#include "IMQTT_Client.h"
#include "DefaultLogger.h"
#include "Telemetry.h"

// Library includes.
#if THINGSBOARD_ENABLE_STREAM_UTILS
#include <StreamUtils.h>
#endif // THINGSBOARD_ENABLE_STREAM_UTILS
#if THINGSBOARD_ENABLE_OTA
#include <array>
#endif // THINGSBOARD_ENABLE_OTA


// Publish data topics.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr ATTRIBUTE_TOPIC[] PROGMEM = "v1/devices/me/attributes";
char constexpr TELEMETRY_TOPIC[] PROGMEM = "v1/devices/me/telemetry";
#else
char constexpr ATTRIBUTE_TOPIC[] = "v1/devices/me/attributes";
char constexpr TELEMETRY_TOPIC[] = "v1/devices/me/telemetry";
#endif // THINGSBOARD_ENABLE_PROGMEM

// RPC topics.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr RPC_SUBSCRIBE_TOPIC[] PROGMEM = "v1/devices/me/rpc/request/+";
char constexpr RPC_RESPONSE_SUBSCRIBE_TOPIC[] PROGMEM = "v1/devices/me/rpc/response/+";
char constexpr RPC_SEND_REQUEST_TOPIC[] PROGMEM = "v1/devices/me/rpc/request/%u";
char constexpr RPC_REQUEST_TOPIC[] PROGMEM = "v1/devices/me/rpc/request";
char constexpr RPC_RESPONSE_TOPIC[] PROGMEM = "v1/devices/me/rpc/response";
char constexpr RPC_SEND_RESPONSE_TOPIC[] = "v1/devices/me/rpc/response/%u";
#else
char constexpr RPC_SUBSCRIBE_TOPIC[] = "v1/devices/me/rpc/request/+";
char constexpr RPC_RESPONSE_SUBSCRIBE_TOPIC[] = "v1/devices/me/rpc/response/+";
char constexpr RPC_SEND_REQUEST_TOPIC[] = "v1/devices/me/rpc/request/%u";
char constexpr RPC_REQUEST_TOPIC[] = "v1/devices/me/rpc/request";
char constexpr RPC_RESPONSE_TOPIC[] = "v1/devices/me/rpc/response";
char constexpr RPC_SEND_RESPONSE_TOPIC[] = "v1/devices/me/rpc/response/%u";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Firmware topics.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr FIRMWARE_RESPONSE_TOPIC[] PROGMEM = "v2/fw/response/0/chunk";
#else
char constexpr FIRMWARE_RESPONSE_TOPIC[] = "v2/fw/response/0/chunk";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Shared attribute topics.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr ATTRIBUTE_REQUEST_TOPIC[] PROGMEM = "v1/devices/me/attributes/request/%u";
char constexpr ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC[] PROGMEM = "v1/devices/me/attributes/response/+";
char constexpr ATTRIBUTE_RESPONSE_TOPIC[] PROGMEM = "v1/devices/me/attributes/response";
#else
char constexpr ATTRIBUTE_REQUEST_TOPIC[] = "v1/devices/me/attributes/request/%u";
char constexpr ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC[] = "v1/devices/me/attributes/response/+";
char constexpr ATTRIBUTE_RESPONSE_TOPIC[] = "v1/devices/me/attributes/response";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Provision topics.
#if THINGSBOARD_ENABLE_PROGMEM
uint16_t constexpr PROGMEM DEFAULT_MQTT_PORT = 1883U;
char constexpr PROV_ACCESS_TOKEN[] PROGMEM = "provision";
char constexpr PROV_RESPONSE_TOPIC[] PROGMEM = "/provision/response";
#else
uint16_t constexpr DEFAULT_MQTT_PORT = 1883U;
char constexpr PROV_ACCESS_TOKEN[] = "provision";
char constexpr PROV_RESPONSE_TOPIC[] = "/provision/response";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Shared attribute request keys.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr SHARED_REQUEST_KEY[] PROGMEM = "sharedKeys";
char constexpr SHARED_RESPONSE_KEY[] PROGMEM = "shared";
#else
char constexpr SHARED_REQUEST_KEY[] = "sharedKeys";
char constexpr SHARED_RESPONSE_KEY[] = "shared";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Client side attribute request keys.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr CLIENT_REQUEST_KEYS[] PROGMEM = "clientKeys";
char constexpr CLIENT_RESPONSE_KEY[] PROGMEM = "client";
#else
char constexpr CLIENT_REQUEST_KEYS[] = "clientKeys";
char constexpr CLIENT_RESPONSE_KEY[] = "client";
#endif // THINGSBOARD_ENABLE_PROGMEM

// RPC data keys.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr RPC_METHOD_KEY[] PROGMEM = "method";
char constexpr RPC_PARAMS_KEY[] PROGMEM = "params";
char constexpr RPC_EMPTY_PARAMS_VALUE[] PROGMEM = "{}";
#else
char constexpr RPC_METHOD_KEY[] = "method";
char constexpr RPC_PARAMS_KEY[] = "params";
char constexpr RPC_EMPTY_PARAMS_VALUE[] = "{}";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Log messages.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr UNABLE_TO_DE_SERIALIZE_JSON[] PROGMEM = "Unable to de-serialize received json data with error (DeserializationError::%s)";
char constexpr INVALID_BUFFER_SIZE[] PROGMEM = "Buffer size (%u) to small for the given payloads size (%u), increase with setBufferSize accordingly or set THINGSBOARD_ENABLE_STREAM_UTILS to 1 before including ThingsBoard";
char constexpr UNABLE_TO_ALLOCATE_BUFFER[] PROGMEM = "Allocating memory for the internal MQTT buffer failed";
#if THINGSBOARD_ENABLE_OTA
char constexpr NUMBER_PRINTF[] PROGMEM = "%u";
#endif // THINGSBOARD_ENABLE_OTA
#if !THINGSBOARD_ENABLE_DYNAMIC
char constexpr RPC_RESPONSE_OVERFLOWED[] PROGMEM = "Server-side RPC response overflowed, increase MaxRPC (%u)";
char constexpr SERVER_SIDE_RPC_SUBSCRIPTIONS[] PROGMEM = "server-side RPC";
char constexpr CLIENT_SIDE_RPC_SUBSCRIPTIONS[] PROGMEM = "client-side RPC";
char constexpr SHARED_ATTRIBUTE_UPDATE_SUBSCRIPTIONS[] PROGMEM = "shared attribute update";
char constexpr CLIENT_SHARED_ATTRIBUTE_SUBSCRIPTIONS[] PROGMEM = "client or shared attribute request";
char constexpr MAX_SUBSCRIPTIONS_EXCEEDED[] PROGMEM = "Too many (%s) subscriptions, increase MaxSubscribtions or unsubscribe";
#else
char constexpr RPC_RESPONSE_OVERFLOWED[] PROGMEM = "Server-side RPC response overflowed, increase responseSize (%u)";
char constexpr COLON PROGMEM = ':';
#endif // !THINGSBOARD_ENABLE_DYNAMIC
char constexpr COMMA[] PROGMEM = ",";
char constexpr NO_KEYS_TO_REQUEST[] PROGMEM = "No keys to request were given";
char constexpr RPC_METHOD_NULL[] PROGMEM = "RPC methodName is NULL";
char constexpr SUBSCRIBE_TOPIC_FAILED[] PROGMEM = "Subscribing the given topic (%s) failed";
#if THINGSBOARD_ENABLE_DEBUG
char constexpr NO_RPC_PARAMS_PASSED[] PROGMEM = "No parameters passed with RPC, passing null JSON";
char constexpr NOT_FOUND_ATT_UPDATE[] PROGMEM = "Shared attribute update key not found";
char constexpr ATT_KEY_NOT_FOUND[] PROGMEM = "Attribute key not found";
char constexpr ATT_CB_NO_KEYS[] PROGMEM = "No keys subscribed. Calling subscribed callback for any updated attributes, assumed to be subscribed to every possible key";
char constexpr ATT_IS_NULL[] PROGMEM = "Subscribed shared attribute update key is NULL";
char constexpr ATT_NO_CHANGE[] PROGMEM = "No keys that we subscribed too were changed, skipping callback";
char constexpr CALLING_RPC_CB[] PROGMEM = "Calling subscribed callback for rpc with methodname (%s)";
char constexpr CALLING_ATT_CB[] PROGMEM = "Calling subscribed callback for updated shared attribute (%s)";
char constexpr CALLING_REQUEST_CB[] PROGMEM = "Calling subscribed callback for request with response id (%u)";
char constexpr RECEIVE_MESSAGE[] PROGMEM = "Received data from server over topic (%s)";
char constexpr SEND_MESSAGE[] PROGMEM = "Sending data to server over topic (%s) with data (%s)";
char constexpr SEND_SERIALIZED[] PROGMEM = "Hidden, because json data is bigger than buffer, therefore showing in console is skipped";
#endif // THINGSBOARD_ENABLE_DEBUG
#else
char constexpr UNABLE_TO_DE_SERIALIZE_JSON[] = "Unable to de-serialize received json data with error (DeserializationError::%s)";
char constexpr INVALID_BUFFER_SIZE[] = "Buffer size (%u) to small for the given payloads size (%u), increase with setBufferSize accordingly or set THINGSBOARD_ENABLE_STREAM_UTILS to 1 before including ThingsBoard";
char constexpr UNABLE_TO_ALLOCATE_BUFFER[] = "Allocating memory for the internal MQTT buffer failed";
#if THINGSBOARD_ENABLE_OTA
char constexpr NUMBER_PRINTF[] = "%u";
#endif // THINGSBOARD_ENABLE_OTA
#if !THINGSBOARD_ENABLE_DYNAMIC
char constexpr RPC_RESPONSE_OVERFLOWED[] = "Server-side RPC response overflowed, increase MaxRPC (%u)";
char constexpr SERVER_SIDE_RPC_SUBSCRIPTIONS[] = "server-side RPC";
char constexpr CLIENT_SIDE_RPC_SUBSCRIPTIONS[] = "client-side RPC";
char constexpr SHARED_ATTRIBUTE_UPDATE_SUBSCRIPTIONS[] = "shared attribute update";
char constexpr CLIENT_SHARED_ATTRIBUTE_SUBSCRIPTIONS[] = "client or shared attribute request";
char constexpr MAX_SUBSCRIPTIONS_EXCEEDED[] = "Too many (%s) subscriptions, increase MaxSubscribtions or unsubscribe";
#else
char constexpr RPC_RESPONSE_OVERFLOWED[] = "Server-side RPC response overflowed, increase responseSize (%u)";
char constexpr COLON = ':';
#endif // !THINGSBOARD_ENABLE_DYNAMIC
char constexpr COMMA[] = ",";
char constexpr NO_KEYS_TO_REQUEST[] = "No keys to request were given";
char constexpr RPC_METHOD_NULL[] = "RPC methodName is NULL";
char constexpr SUBSCRIBE_TOPIC_FAILED[] = "Subscribing the given topic (%s) failed";
#if THINGSBOARD_ENABLE_DEBUG
char constexpr NO_RPC_PARAMS_PASSED[] = "No parameters passed with RPC, passing null JSON";
char constexpr NOT_FOUND_ATT_UPDATE[] = "Shared attribute update key not found";
char constexpr ATT_KEY_NOT_FOUND[] = "Attribute key not found";
char constexpr ATT_CB_NO_KEYS[] = "No keys subscribed. Calling subscribed callback for any updated attributes, assumed to be subscribed to every possible key";
char constexpr ATT_IS_NULL[] = "Subscribed shared attribute update key is NULL";
char constexpr ATT_NO_CHANGE[] = "No keys that we subscribed too were changed, skipping callback";
char constexpr CALLING_RPC_CB[] = "Calling subscribed callback for rpc with methodname (%s)";
char constexpr CALLING_ATT_CB[] = "Calling subscribed callback for updated shared attribute (%s)";
char constexpr CALLING_REQUEST_CB[] = "Calling subscribed callback for request with response id (%u)";
char constexpr RECEIVE_MESSAGE[] = "Received data from server over topic (%s)";
char constexpr SEND_MESSAGE[] = "Sending data to server over topic (%s) with data (%s)";
char constexpr SEND_SERIALIZED[] = "Hidden, because json data is bigger than buffer, therefore showing in console is skipped";
#endif // THINGSBOARD_ENABLE_DEBUG
#endif // THINGSBOARD_ENABLE_PROGMEM

// Claim topics.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr CLAIM_TOPIC[] PROGMEM = "v1/devices/me/claim";
#else
char constexpr CLAIM_TOPIC[] = "v1/devices/me/claim";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Provision topics.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr PROV_REQUEST_TOPIC[] PROGMEM = "/provision/request";
#else
char constexpr PROV_REQUEST_TOPIC[] = "/provision/request";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Claim data keys.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr SECRET_KEY[] PROGMEM = "secretKey";
char constexpr DURATION_KEY[] PROGMEM = "durationMs";
char constexpr DEVICE_NAME_KEY[] PROGMEM = "deviceName";
char constexpr PROV_DEVICE_KEY[] PROGMEM = "provisionDeviceKey";
char constexpr PROV_DEVICE_SECRET_KEY[] PROGMEM = "provisionDeviceSecret";
char constexpr PROV_CRED_TYPE_KEY[] PROGMEM = "credentialsType";
char constexpr PROV_TOKEN[] PROGMEM = "token";
char constexpr PROV_CRED_USERNAME[] PROGMEM = "username";
char constexpr PROV_CRED_PASSWORD[] PROGMEM = "password";
char constexpr PROV_CRED_CLIENT_ID[] PROGMEM = "clientId";
char constexpr PROV_CRED_HASH[] PROGMEM = "hash";
#else
char constexpr SECRET_KEY[] = "secretKey";
char constexpr DURATION_KEY[] = "durationMs";
char constexpr DEVICE_NAME_KEY[] = "deviceName";
char constexpr PROV_DEVICE_KEY[] = "provisionDeviceKey";
char constexpr PROV_DEVICE_SECRET_KEY[] = "provisionDeviceSecret";
char constexpr PROV_CRED_TYPE_KEY[] = "credentialsType";
char constexpr PROV_TOKEN[] = "token";
char constexpr PROV_CRED_USERNAME[] = "username";
char constexpr PROV_CRED_PASSWORD[] = "password";
char constexpr PROV_CRED_CLIENT_ID[] = "clientId";
char constexpr PROV_CRED_HASH[] = "hash";
#endif // THINGSBOARD_ENABLE_PROGMEM

#if THINGSBOARD_ENABLE_OTA
// Firmware topics.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC[] PROGMEM = "v2/fw/response/#";
char constexpr FIRMWARE_REQUEST_TOPIC[] PROGMEM = "v2/fw/request/0/chunk/%u";
#else
char constexpr FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC[] = "v2/fw/response/#";
char constexpr FIRMWARE_REQUEST_TOPIC[] = "v2/fw/request/0/chunk/%u";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Firmware data keys.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr CURR_FW_TITLE_KEY[] PROGMEM = "current_fw_title";
char constexpr CURR_FW_VER_KEY[] PROGMEM = "current_fw_version";
char constexpr FW_ERROR_KEY[] PROGMEM = "fw_error";
char constexpr FW_STATE_KEY[] PROGMEM = "fw_state";
char constexpr FW_VER_KEY[] PROGMEM = "fw_version";
char constexpr FW_TITLE_KEY[] PROGMEM = "fw_title";
char constexpr FW_CHKS_KEY[] PROGMEM = "fw_checksum";
char constexpr FW_CHKS_ALGO_KEY[] PROGMEM = "fw_checksum_algorithm";
char constexpr FW_SIZE_KEY[] PROGMEM = "fw_size";
char constexpr CHECKSUM_AGORITM_MD5[] PROGMEM = "MD5";
char constexpr CHECKSUM_AGORITM_SHA256[] PROGMEM = "SHA256";
char constexpr CHECKSUM_AGORITM_SHA384[] PROGMEM = "SHA384";
char constexpr CHECKSUM_AGORITM_SHA512[] PROGMEM = "SHA512";
#else
char constexpr CURR_FW_TITLE_KEY[] = "current_fw_title";
char constexpr CURR_FW_VER_KEY[] = "current_fw_version";
char constexpr FW_ERROR_KEY[] = "fw_error";
char constexpr FW_STATE_KEY[] = "fw_state";
char constexpr FW_VER_KEY[] = "fw_version";
char constexpr FW_TITLE_KEY[] = "fw_title";
char constexpr FW_CHKS_KEY[] = "fw_checksum";
char constexpr FW_CHKS_ALGO_KEY[] = "fw_checksum_algorithm";
char constexpr FW_SIZE_KEY[] = "fw_size";
char constexpr CHECKSUM_AGORITM_MD5[] = "MD5";
char constexpr CHECKSUM_AGORITM_SHA256[] = "SHA256";
char constexpr CHECKSUM_AGORITM_SHA384[] = "SHA384";
char constexpr CHECKSUM_AGORITM_SHA512[] = "SHA512";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Log messages.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr NO_FW[] PROGMEM = "No new firmware assigned on the given device";
char constexpr EMPTY_FW[] PROGMEM = "Given firmware was NULL";
char constexpr FW_UP_TO_DATE[] PROGMEM = "Firmware version (%s) already up to date";
char constexpr FW_NOT_FOR_US[] PROGMEM = "Firmware title (%s) not same as received title (%s)";
char constexpr FW_CHKS_ALGO_NOT_SUPPORTED[] PROGMEM = "Checksum algorithm (%s) is not supported";
char constexpr NOT_ENOUGH_RAM[] PROGMEM = "Temporary allocating more internal client buffer failed, decrease OTA chunk size or decrease overall heap usage";
char constexpr RESETTING_FAILED[] PROGMEM = "Preparing for OTA firmware updates failed, attributes might be NULL";
#if THINGSBOARD_ENABLE_DEBUG
char constexpr PAGE_BREAK[] PROGMEM = "=================================";
char constexpr NEW_FW[] PROGMEM = "A new Firmware is available:";
char constexpr FROM_TOO[] PROGMEM = "(%s) => (%s)";
char constexpr DOWNLOADING_FW[] PROGMEM = "Attempting to download over MQTT...";
#endif // THINGSBOARD_ENABLE_DEBUG
#else
char constexpr NO_FW[] = "No new firmware assigned on the given device";
char constexpr EMPTY_FW[] = "Given firmware was NULL";
char constexpr FW_UP_TO_DATE[] = "Firmware version (%s) already up to date";
char constexpr FW_NOT_FOR_US[] = "Firmware title (%s) not same as received title (%s)";
char constexpr FW_CHKS_ALGO_NOT_SUPPORTED[] = "Checksum algorithm (%s) is not supported";
char constexpr NOT_ENOUGH_RAM[] = "Temporary allocating more internal client buffer failed, decrease OTA chunk size or decrease overall heap usage";
char constexpr RESETTING_FAILED[] = "Preparing for OTA firmware updates failed, attributes might be NULL";
#if THINGSBOARD_ENABLE_DEBUG
char constexpr PAGE_BREAK[] = "=================================";
char constexpr NEW_FW[] = "A new Firmware is available:";
char constexpr FROM_TOO[] = "(%s) => (%s)";
char constexpr DOWNLOADING_FW[] = "Attempting to download over MQTT...";
#endif // THINGSBOARD_ENABLE_DEBUG
#endif // THINGSBOARD_ENABLE_PROGMEM
#endif // THINGSBOARD_ENABLE_OTA


#if THINGSBOARD_ENABLE_DYNAMIC
/// @brief Wrapper around any arbitrary MQTT Client implementing the IMQTT_Client interface, to allow connecting and sending / retrieving data from ThingsBoard over the MQTT or MQTT with TLS/SSL protocol.
/// BufferSize of the underlying data buffer can be changed during the runtime and the maximum amount of data points that can ever be sent or received are automatically deduced at runtime.
/// Additionally, there are internal vectors that hold all subscriptions and requests and dynamically allocate memory on the heap, depending on how much space we currently require.
/// Furthermore, there are internal vectors in the Shared_Attribute_Callback and the Attribute_Request_Callback, which hold the amount of keys we want to request or subscribe to updates too.
/// Dynamically increasing the internal size, allows to adjust how much space we require depending on the amount of subscribed or requested keys.
/// If this feature of automatic deduction, is not needed, or not wanted because it allocates memory on the heap, then the values can be set once as template arguements.
/// Simply set THINGSBOARD_ENABLE_DYNAMIC to 0, before including ThingsBoard.h
/// @tparam Logger Implementation that should be used to print error messages generated by internal processes and additional debugging messages if THINGSBOARD_ENABLE_DEBUG is set, default = DefaultLogger
template <typename Logger = DefaultLogger>
#else
/// @brief Wrapper around any arbitrary MQTT Client implementing the IMQTT_Client interface, to allow connecting and sending / retrieving data from ThingsBoard over the MQTT or MQTT with TLS/SSL protocol.
/// BufferSize of the underlying data buffer can be changed during the runtime and the maximum amount of data points that can ever be can be set once as template argument.
/// Additionally, there are internal arrays that hold all subscriptions and requests and statically allocate memory on the stack, which can also be set once as a template argument.
/// Furthermore, there are the maximum amount of values for the internal arrays of the Shared_Attribute_Callback and the Attribute_Request_Callback, which hold the amount of keys we want to request or subscribe to updates too
/// Setting a fixed size, allows to allocate the variables in the container on the stack, which can also be set once as a template argument.
/// Changing is only possible if a new instance of this class is created. If these values should be automatically deduced at runtime instead then, and then dynamically allocated on the heap,
/// simply set THINGSBOARD_ENABLE_DYNAMIC to 1, before including ThingsBoard.h
/// @tparam MaxFieldsAmount Maximum amount of key value pair that we will be able to sent or received by ThingsBoard in one call, default = Default_Fields_Amount (8)
/// @tparam MaxSubscribtions Maximum amount of simultaneous shared attribute or server side rpc subscriptions and shared or client scope attribute or client side rpc requests.
/// Each aforementioned type has its own maximum and does not count to the max allowed simultaneous subscriptions or requests of any other.
/// Once the maximum amount has been reached it is not possible to increase the size, this is done because it allows to allcoate the memory on the stack instead of the heap, default = Default_Subscriptions_Amount (2)
/// @tparam MaxAttributes Maximum amount of attributes that will ever be requested with the Attribute_Request_Callback or the Shared_Attribute_Callback, allows to use an array on the stack in the background.
/// Be aware though the size set in this template and the size passed to the ThingsBoard MaxAttributes template need to be the same or the value in this class lower, if not some of the requested keys may be lost.
/// Furthermore, if OTA Updates are utilized the size should never be decreased to less than 5, because that amount of attributes needs to be requested or updated to start the OTA update.
/// Meaning if the number is decreased the OTA update will not work correctly anymore and will not be able to be started anymore, default = Default_Attributes_Amount (5)
/// @tparam MaxRPC Maximum amount of key-value pairs that will ever be sent in the subscribed callback method of an RPC_Callback, allows to use a StaticJsonDocument on the stack in the background.
/// If we simply use .to<JsonVariant>(); on the received document and use .set() to change the internal value then the size requirements are 0.
/// However if we attempt to send multiple key-value pairs, we have to adjust the size accordingly. See https://arduinojson.org/v6/assistant/ for more information on how to estimate the required size and divide the reulst by 16 to receive the required MaxRPC value, default = Default_RPC_Amount (0)
/// @tparam Logger Implementation that should be used to print error messages generated by internal processes and additional debugging messages if THINGSBOARD_ENABLE_DEBUG is set, default = DefaultLogger
template<size_t MaxFieldsAmount = Default_Fields_Amount, size_t MaxSubscribtions = Default_Subscriptions_Amount, size_t MaxAttributes = Default_Attributes_Amount, size_t MaxRPC = Default_RPC_Amount, typename Logger = DefaultLogger>
#endif // THINGSBOARD_ENABLE_DYNAMIC
class ThingsBoardSized {
  public:
    /// @brief Constructs a ThingsBoardSized instance with the given network client that should be used to establish the connection to ThingsBoard
    /// @param client MQTT Client implementation that should be used to establish the connection to ThingsBoard
    /// @param logger Logger implementation that should be used to print messages generated by internal processes
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
    /// The aforementioned options can only be enabled if Arduino is used to build this library, because the StreamUtils library requires it, default = Default_Payload (64)
    /// @param maxStackSize Maximum amount of bytes we want to allocate on the stack, default = Default_Max_Stack_Size (1024)
#if !THINGSBOARD_ENABLE_STREAM_UTILS
    ThingsBoardSized(IMQTT_Client& client, uint16_t const & bufferSize = Default_Payload, size_t const & maxStackSize = Default_Max_Stack_Size)
#else
    /// @param bufferingSize Amount of bytes allocated to speed up serialization, default = Default_Buffering_Size (64)
    ThingsBoardSized(IMQTT_Client& client, uint16_t const & bufferSize = Default_Payload, size_t const & maxStackSize = Default_Max_Stack_Size, size_t const & bufferingSize = Default_Buffering_Size)
#endif // THINGSBOARD_ENABLE_STREAM_UTILS
      : m_client(client)
      , m_max_stack(maxStackSize)
#if THINGSBOARD_ENABLE_STREAM_UTILS
      , m_buffering_size(bufferingSize)
#endif // THINGSBOARD_ENABLE_STREAM_UTILS
      , m_rpc_callbacks()
      , m_rpc_request_callbacks()
      , m_shared_attribute_update_callbacks()
      , m_attribute_request_callbacks()
      , m_provision_callback()
      , m_request_id(0U)
#if THINGSBOARD_ENABLE_OTA
      , m_fw_callback()
      , m_previous_buffer_size(0U)
      , m_change_buffer_size(false)
      , m_ota(std::bind(&ThingsBoardSized::Publish_Chunk_Request, this, std::placeholders::_1), std::bind(&ThingsBoardSized::Firmware_Send_State, this, std::placeholders::_1, std::placeholders::_2), std::bind(&ThingsBoardSized::Firmware_OTA_Unsubscribe, this))
#endif // THINGSBOARD_ENABLE_OTA
    {
        (void)setBufferSize(bufferSize);
        // Initalize callback.
#if THINGSBOARD_ENABLE_STL
        m_client.set_data_callback(std::bind(&ThingsBoardSized::onMQTTMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_client.set_connect_callback(std::bind(&ThingsBoardSized::Resubscribe_Topics, this));
#else
        m_client.set_data_callback(ThingsBoardSized::onStaticMQTTMessage);
        m_client.set_connect_callback(ThingsBoardSized::onStaticMQTTConnect);
        m_subscribedInstance = this;
#endif // THINGSBOARD_ENABLE_STL
    }

    /// @brief Gets the currently connected MQTT Client implementation as a reference.
    /// Allows for calling method directly on the client itself, not advised in normal use cases,
    /// as it might cause problems if the library expects the client to be sending / receiving data
    /// but it can not do that anymore, because it has been disconnected or certain settings were changed
    /// @return Reference to the underlying MQTT Client implementation connected to ThingsBoard
    IMQTT_Client & getClient() {
        return m_client;
    }

    /// @brief Sets the maximum amount of bytes that we want to allocate on the stack, before the memory is allocated on the heap instead
    /// @param maxStackSize Maximum amount of bytes we want to allocate on the stack
    void setMaximumStackSize(size_t const & maxStackSize) {
        m_max_stack = maxStackSize;
    }

#if THINGSBOARD_ENABLE_STREAM_UTILS
    /// @brief Sets the amount of bytes that can be allocated to speed up fall back serialization with the StreamUtils class
    /// See https://github.com/bblanchon/ArduinoStreamUtils for more information on the underlying class used
    /// @param bufferingSize Amount of bytes allocated to speed up serialization
    void setBufferingSize(size_t const & bufferingSize) {
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
    /// but in that case if a message was too big to be sent the user will be informed with a message to the logger implementation.
    /// The aforementioned options can only be enabled if Arduino is used to build this library, because the StreamUtils library requires it
    /// @return Whether allocating the needed memory for the given bufferSize was successful or not
    bool setBufferSize(uint16_t const & bufferSize) {
        bool const result = m_client.set_buffer_size(bufferSize);
        if (!result) {
            Logger::println(UNABLE_TO_ALLOCATE_BUFFER);
        }
        return result;
    }

    /// @brief Clears all currently subscribed callbacks and unsubscribed from all
    /// currently subscribed MQTT topics, any response that will stil be received is discarded
    /// and any ongoing firmware update is aborted and will not be finished.
    /// Was previously done automatically in the connect() method, but is not done anymore,
    /// because connect() method now reconencts to all previously subscribed MQTT topics instead,
    /// therefore there is no need anymore to discard all previously subscribed callbacks and letting the user resubscribe
    void Cleanup_Subscriptions() {
        // Results are ignored, because the important part of clearing internal data structures always succeeds.
        // Cleanup all server-side RPC subscriptions
        (void)RPC_Unsubscribe();
        // Cleanup all client-side RPC requests
        (void)RPC_Request_Unsubscribe();
        // Cleanup all shared attributes subscriptions
        (void)Shared_Attributes_Unsubscribe();
        // Cleanup all client-side or shared attributes requests
        (void)Attributes_Request_Unsubscribe();
        // Cleanup all provision requests
        (void)Provision_Unsubscribe();
#if THINGSBOARD_ENABLE_OTA
        // Stop any ongoing Firmware update,
        // which will in turn cleanup the internal member variables of the OTAHandler class
        // as well as all firmware subscriptions
        // and inform the user of the failed firmware update
        Stop_Firmware_Update();
#endif // THINGSBOARD_ENABLE_OTA
    }

    /// @brief Connects to the specified ThingsBoard server over the given port as the given device.
    /// If there are still active server-side RPC or Shared Attribute subscriptions, the aforementioned topics will be resubscribed automatically.
    /// Additionally internal vectors are kept the same so any permanent subscriptions, does not need to be resubscribed by calling the appropriate subscribe methods again.
    /// @param host ThingsBoard server instance we want to connect to
    /// @param access_token Access token that connects this device with a created device on the ThingsBoard server,
    /// can be "provision", if the device creates itself instead. See https://thingsboard.io/docs/user-guide/device-provisioning/?mqttprovisioning=without#provision-device-apis for more information, default = PROV_ACCESS_TOKEN ("provision")
    /// @param port Port that will be used to establish a connection and send / receive data from ThingsBoard over, default = DEFAULT_MQTT_PORT (1883)
    /// @param client_id Client username that can be used to differentiate the user that is connecting the given device to ThingsBoard, recommended to be a unique identifier
    /// so it possible to discern which device is communicating, default = Value of passed access token
    /// @param password Client password that can be used to authenticate the user that is connecting the given device to ThingsBoard, default = nullptr
    /// @return Whether connecting to ThingsBoard was successful or not
    bool connect(char const * const host, char const * const access_token = PROV_ACCESS_TOKEN, uint16_t const & port = DEFAULT_MQTT_PORT, char const * const client_id = nullptr, char const * const password = nullptr) {
        if (host == nullptr) {
            return false;
        }
        m_client.set_server(host, port);
        return connect_to_host(access_token, (client_id == nullptr) ? access_token : client_id, password);
    }

    /// @brief Disconnects any connection that has been established already
    void disconnect() {
        m_client.disconnect();
    }

    /// @brief Returns our current connection status to the cloud, true meaning we are connected,
    /// false meaning we have been disconnected or have not established a connection yet
    /// @return Whether the underlying MQTT Client is currently connected or not
    bool connected() {
        return m_client.connected();
    }

    /// @brief Receives / sends any outstanding messages from and to the MQTT broker
    /// @return Whether sending or receiving the oustanding the messages was successful or not
    bool loop() {
        return m_client.loop();
    }

    /// @brief Attempts to send key value pairs from custom source over the given topic to the server
    /// @tparam TSource Source class that should be used to serialize the json that is sent to the server
    /// @param topic Topic we want to send the data over
    /// @param source Data source containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the source
    /// @return Whether sending the data was successful or not
    template <typename TSource>
    bool Send_Json(char const * const topic, TSource const & source, size_t const & jsonSize) {
        // Check if allocating needed memory failed when trying to create the JsonObject,
        // if it did the isNull() method will return true. See https://arduinojson.org/v6/api/jsonvariant/isnull/ for more information
        if (source.isNull()) {
            Logger::println(UNABLE_TO_ALLOCATE_JSON);
            return false;
        }
#if !THINGSBOARD_ENABLE_DYNAMIC
        size_t const amount = source.size();
        if (MaxFieldsAmount < amount) {
            Logger::printfln(TOO_MANY_JSON_FIELDS, amount, MaxFieldsAmount);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        bool result = false;

#if THINGSBOARD_ENABLE_STREAM_UTILS
        // Check if the size of the given message would be too big for the actual client,
        // if it is utilize the serialize json work around, so that the internal client buffer can be circumvented
        if (m_client.get_buffer_size() < jsonSize)  {
#if THINGSBOARD_ENABLE_DEBUG
            Logger::printfln(SEND_MESSAGE, topic, SEND_SERIALIZED);
#endif // THINGSBOARD_ENABLE_DEBUG
            result = Serialize_Json(topic, source, jsonSize - 1);
        }
        // Check if the remaining stack size of the current task would overflow the stack,
        // if it would allocate the memory on the heap instead to ensure no stack overflow occurs
        else
#endif // THINGSBOARD_ENABLE_STREAM_UTILS
        if (getMaximumStackSize() < jsonSize) {
            char* json = new char[jsonSize]();
            if (serializeJson(source, json, jsonSize) < jsonSize - 1) {
                Logger::println(UNABLE_TO_SERIALIZE_JSON);
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
            char json[jsonSize] = {};
            if (serializeJson(source, json, jsonSize) < jsonSize - 1) {
                Logger::println(UNABLE_TO_SERIALIZE_JSON);
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
    bool Send_Json_String(char const * const topic, char const * const json) {
        if (json == nullptr) {
            return false;
        }

        uint16_t const & currentBufferSize = m_client.get_buffer_size();
        size_t const jsonSize = strlen(json);

        if (currentBufferSize < jsonSize) {
            Logger::printfln(INVALID_BUFFER_SIZE, currentBufferSize, jsonSize);
            return false;
        }

#if THINGSBOARD_ENABLE_DEBUG
        Logger::printfln(SEND_MESSAGE, topic, json);
#endif // THINGSBOARD_ENABLE_DEBUG
        return m_client.publish(topic, reinterpret_cast<uint8_t const *>(json), jsonSize);
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
    bool Claim_Request(char const * const secretKey, size_t const & durationMs) {
        StaticJsonDocument<JSON_OBJECT_SIZE(2)> requestBuffer;

        if (!Helper::stringIsNullorEmpty(secretKey)) {
            requestBuffer[SECRET_KEY] = secretKey;
        }
        requestBuffer[DURATION_KEY] = durationMs;

        size_t const objectSize = Helper::Measure_Json(requestBuffer);
        return Send_Json(CLAIM_TOPIC, requestBuffer, objectSize);
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
    bool Provision_Request(Provision_Callback const & callback) {
        char const * provisionDeviceKey = callback.Get_Device_Key();
        char const * provisionDeviceSecret = callback.Get_Device_Secret();

        if (Helper::stringIsNullorEmpty(provisionDeviceKey) || Helper::stringIsNullorEmpty(provisionDeviceSecret)) {
            return false;
        }
        else if (!Provision_Subscribe(callback)) {
            return false;
        }

        StaticJsonDocument<JSON_OBJECT_SIZE(9)> requestBuffer;
        char const * deviceName = callback.Get_Device_Name();
        char const * accessToken = callback.Get_Device_Access_Token();
        char const * credUsername = callback.Get_Credentials_Username();
        char const * credPassword = callback.Get_Credentials_Password();
        char const * credClientID = callback.Get_Credentials_Client_ID();
        char const * hash = callback.Get_Certificate_Hash();
        char const * credentialsType = callback.Get_Credentials_Type();

        // Deciding which underlying provisioning method is restricted, by the Provision_Callback class.
        // Meaning only the key-value pairs that are needed for the given provisioning method are set,
        // resulting in the rest not being sent and therefore the provisioning request having the correct formatting
        if (!Helper::stringIsNullorEmpty(deviceName)) {
            requestBuffer[DEVICE_NAME_KEY] = deviceName;
        }
        if (!Helper::stringIsNullorEmpty(accessToken)) {
            requestBuffer[PROV_TOKEN] = accessToken;
        }
        if (!Helper::stringIsNullorEmpty(credUsername)) {
            requestBuffer[PROV_CRED_USERNAME] = credUsername;
        }
        if (!Helper::stringIsNullorEmpty(credPassword)) {
            requestBuffer[PROV_CRED_PASSWORD] = credPassword;
        }
        if (!Helper::stringIsNullorEmpty(credClientID)) {
            requestBuffer[PROV_CRED_CLIENT_ID] = credClientID;
        }
        if (!Helper::stringIsNullorEmpty(hash)) {
            requestBuffer[PROV_CRED_HASH] = hash;
        }
        if (!Helper::stringIsNullorEmpty(credentialsType)) {
            requestBuffer[PROV_CRED_TYPE_KEY] = credentialsType;
        }
        requestBuffer[PROV_DEVICE_KEY] = provisionDeviceKey;
        requestBuffer[PROV_DEVICE_SECRET_KEY] = provisionDeviceSecret;

        size_t const objectSize = Helper::Measure_Json(requestBuffer);
        return Send_Json(PROV_REQUEST_TOPIC, requestBuffer, objectSize);
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
    bool sendTelemetryData(char const * const key, T const & value) {
        return sendKeyValue(key, value);
    }

    /// @brief Attempts to send aggregated telemetry data, expects iterators to a container containing Telemetry class instances.
    /// See https://thingsboard.io/docs/user-guide/telemetry/ for more information
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param first Iterator pointing to the first element in the data container
    /// @param last Iterator pointing to the end of the data container (last element + 1)
    /// @return Whether sending the aggregated telemetry data was successful or not
    template<typename InputIterator>
    bool sendTelemetry(InputIterator const & first, InputIterator const & last) {
        return sendDataArray(first, last, true);
    }

    /// @brief Attempts to send custom json telemetry string.
    /// See https://thingsboard.io/docs/user-guide/telemetry/ for more information
    /// @param json String containing our json key value pairs we want to attempt to send
    /// @return Whether sending the data was successful or not
    bool sendTelemetryJson(char const * const json) {
        return Send_Json_String(TELEMETRY_TOPIC, json);
    }

    /// @brief Attempts to send telemetry key value pairs from custom source to the server.
    /// See https://thingsboard.io/docs/user-guide/telemetry/ for more information
    /// @tparam TSource Source class that should be used to serialize the json that is sent to the server
    /// @param source Data source containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the source
    /// @return Whether sending the data was successful or not
    template <typename TSource>
    bool sendTelemetryJson(TSource const & source, size_t const & jsonSize) {
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
    bool sendAttributeData(char const * const key, T const & value) {
        return sendKeyValue(key, value, false);
    }

    /// @brief Attempts to send aggregated attribute data, expects iterators to a container containing Attribute class instances.
    /// See https://thingsboard.io/docs/user-guide/attributes/ for more information
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param first Iterator pointing to the first element in the data container
    /// @param last Iterator pointing to the end of the data container (last element + 1)
    /// @return Whether sending the aggregated attribute data was successful or not
    template<typename InputIterator>
    bool sendAttributes(InputIterator const & first, InputIterator const & last) {
        return sendDataArray(first, last, false);
    }

    /// @brief Attempts to send custom json attribute string.
    /// See https://thingsboard.io/docs/user-guide/attributes/ for more information
    /// @param json String containing our json key value pairs we want to attempt to send
    /// @return Whether sending the data was successful or not
    bool sendAttributeJson(char const * const json) {
        return Send_Json_String(ATTRIBUTE_TOPIC, json);
    }

    /// @brief Attempts to send attribute key value pairs from custom source to the server.
    /// See https://thingsboard.io/docs/user-guide/attributes/ for more information
    /// @tparam TSource Source class that should be used to serialize the json that is sent to the server
    /// @param source Data source containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the source
    /// @return Whether sending the data was successful or not
    template <typename TSource>
    bool sendAttributeJson(TSource const & source, size_t const & jsonSize) {
        return Send_Json(ATTRIBUTE_TOPIC, source, jsonSize);
    }

    /// @brief Requests one client-side attribute calllback,
    /// that will be called if the key-value pair from the server for the given client-side attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#request-attribute-values-from-the-server for more information
    /// @param callback Callback method that will be called
    /// @return Whether requesting the given callback was successful or not
#if THINGSBOARD_ENABLE_DYNAMIC
    bool Client_Attributes_Request(Attribute_Request_Callback const & callback) {
#else
    bool Client_Attributes_Request(Attribute_Request_Callback<MaxAttributes> const & callback) {
#endif // THINGSBOARD_ENABLE_DYNAMIC
        return Attributes_Request(callback, CLIENT_REQUEST_KEYS, CLIENT_RESPONSE_KEY);
    }

    //----------------------------------------------------------------------------
    // Server-side RPC API

    /// @brief Subscribes multiple server-side RPC callbacks,
    /// that will be called if a request from the server for the method with the given name is received.
    /// See https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc for more information
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param first Iterator pointing to the first element in the data container
    /// @param last Iterator pointing to the end of the data container (last element + 1)
    /// @return Whether subscribing the given callbacks was successful or not
    template<typename InputIterator>
    bool RPC_Subscribe(InputIterator const & first, InputIterator const & last) {
#if !THINGSBOARD_ENABLE_DYNAMIC
        size_t const size = Helper::distance(first, last);
        if (m_rpc_callbacks.size() + size > m_rpc_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, SERVER_SIDE_RPC_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, RPC_SUBSCRIBE_TOPIC);
            return false;
        }

        // Push back complete vector into our local m_rpc_callbacks vector.
        m_rpc_callbacks.insert(m_rpc_callbacks.end(), first, last);
        return true;
    }

    /// @brief Subscribe one server-side RPC callback,
    /// that will be called if a request from the server for the method with the given name is received.
    /// See https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc for more information
    /// @param callback Callback method that will be called
    /// @return Whether subscribing the given callback was successful or not
    bool RPC_Subscribe(RPC_Callback const & callback) {
#if !THINGSBOARD_ENABLE_DYNAMIC
        if (m_rpc_callbacks.size() + 1 > m_rpc_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, SERVER_SIDE_RPC_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_client.subscribe(RPC_SUBSCRIBE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, RPC_SUBSCRIBE_TOPIC);
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
    bool RPC_Unsubscribe() {
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
    bool RPC_Request(RPC_Request_Callback const & callback) {
        char const * methodName = callback.Get_Name();

        if (Helper::stringIsNullorEmpty(methodName)) {
            Logger::println(RPC_METHOD_NULL);
            return false;
        }
        RPC_Request_Callback * registeredCallback = nullptr;
        if (!RPC_Request_Subscribe(callback, registeredCallback)) {
            return false;
        }
        else if (registeredCallback == nullptr) {
            return false;
        }

        JsonArray const * const parameters = callback.Get_Parameters();

#if THINGSBOARD_ENABLE_DYNAMIC
        // String are const char* and therefore stored as a pointer --> zero copy, meaning the size for the strings is 0 bytes,
        // Data structure size depends on the amount of key value pairs passed + the default methodName and params key needed for the request.
        // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
        TBJsonDocument requestBuffer(JSON_OBJECT_SIZE(parameters != nullptr ? parameters->size() + 2U : 2U));
#else
        // Ensure to have enough size for the infinite amount of possible parameters that could be sent to the cloud,
        // therefore we set the size to the MaxFieldsAmount instead of JSON_OBJECT_SIZE(1), which will result in a JsonDocument with a size of 16 bytes
        StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmount)> requestBuffer;
#endif // THINGSBOARD_ENABLE_DYNAMIC

        requestBuffer[RPC_METHOD_KEY] = methodName;

        if (parameters != nullptr && !parameters->isNull()) {
            requestBuffer[RPC_PARAMS_KEY] = *parameters;
        }
        else {
            requestBuffer[RPC_PARAMS_KEY] = RPC_EMPTY_PARAMS_VALUE;
        }

        m_request_id++;
        registeredCallback->Set_Request_ID(m_request_id);

        char topic[Helper::detectSize(RPC_SEND_REQUEST_TOPIC, m_request_id)] = {};
        (void)snprintf(topic, sizeof(topic), RPC_SEND_REQUEST_TOPIC, m_request_id);

        size_t const objectSize = Helper::Measure_Json(requestBuffer);
        return Send_Json(topic, requestBuffer, objectSize);
    }

    //----------------------------------------------------------------------------
    // Firmware OTA API

#if THINGSBOARD_ENABLE_OTA

    /// @brief Checks if firmware settings are assigned to the connected device and if they are attempts to use those settings to start a firmware update.
    /// Will only be checked once and if there is no firmware assigned or if the assigned firmware is already installed this method will not update.
    /// This firmware status is only checked once, meaning to recheck the status either call this method again or use the Subscribe_Firmware_Update method.
    /// to be automatically informed and start the update if firmware has been assigned and it is not already installed.
    /// See https://thingsboard.io/docs/user-guide/ota-updates/ for more information
    /// @param callback Callback method that will be called
    /// @return Whether subscribing the given callback was successful or not
    bool Start_Firmware_Update(OTA_Update_Callback const & callback) {
        if (!Prepare_Firmware_Settings(callback))  {
            Logger::println(RESETTING_FAILED);
            return false;
        }

        // Request the firmware information
#if THINGSBOARD_ENABLE_DYNAMIC
        constexpr std::array<char const *, 5U> fw_shared_keys{FW_CHKS_KEY, FW_CHKS_ALGO_KEY, FW_SIZE_KEY, FW_TITLE_KEY, FW_VER_KEY};
        const Attribute_Request_Callback fw_request_callback(std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1), fw_shared_keys.cbegin(), fw_shared_keys.cend());
#else
        constexpr std::array<char const *, MaxAttributes> fw_shared_keys{FW_CHKS_KEY, FW_CHKS_ALGO_KEY, FW_SIZE_KEY, FW_TITLE_KEY, FW_VER_KEY};
        const Attribute_Request_Callback<MaxAttributes> fw_request_callback(std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1), fw_shared_keys.cbegin(), fw_shared_keys.cend());
#endif //THINGSBOARD_ENABLE_DYNAMIC
        return Shared_Attributes_Request(fw_request_callback);
    }

    /// @brief Stops the currently ongoing firmware update, calls the subscribed user finish callback with a failure if any update was stopped.
    /// See https://thingsboard.io/docs/user-guide/ota-updates/ for more information
    void Stop_Firmware_Update() {
        m_ota.Stop_Firmware_Update();
    }

    /// @brief Subscribes to any changes of the assigned firmware information on the connected device,
    /// meaning once we subscribed if we register any changes we will start the update if the given firmware is not already installed.
    /// Unlike Start_Firmware_Update this method only registers changes to the firmware information,
    /// meaning if the change occured while this device was asleep or turned off we will not update,
    /// to achieve that, it is instead recommended to call the Start_Firmware_Update method when the device has started once to check for that edge case.
    /// See https://thingsboard.io/docs/user-guide/ota-updates/ for more information
    /// @param callback Callback method that will be called
    /// @return Whether subscribing the given callback was successful or not
    bool Subscribe_Firmware_Update(OTA_Update_Callback const & callback) {
        if (!Prepare_Firmware_Settings(callback))  {
            Logger::println(RESETTING_FAILED);
            return false;
        }

        // Subscribes to changes of the firmware information
#if THINGSBOARD_ENABLE_DYNAMIC
        constexpr std::array<char const *, 5U> fw_shared_keys{FW_CHKS_KEY, FW_CHKS_ALGO_KEY, FW_SIZE_KEY, FW_TITLE_KEY, FW_VER_KEY};
        const Shared_Attribute_Callback fw_update_callback(std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1), fw_shared_keys.cbegin(), fw_shared_keys.cend());
#else
        constexpr std::array<char const *, MaxAttributes> fw_shared_keys{FW_CHKS_KEY, FW_CHKS_ALGO_KEY, FW_SIZE_KEY, FW_TITLE_KEY, FW_VER_KEY};
        const Shared_Attribute_Callback<MaxAttributes> fw_update_callback(std::bind(&ThingsBoardSized::Firmware_Shared_Attribute_Received, this, std::placeholders::_1), fw_shared_keys.cbegin(), fw_shared_keys.cend());
#endif //THINGSBOARD_ENABLE_DYNAMIC
        return Shared_Attributes_Subscribe(fw_update_callback);
    }

    /// @brief Sends the given firmware title and firmware version to the cloud.
    /// See https://thingsboard.io/docs/user-guide/ota-updates/ for more information
    /// @param currFwTitle Current device firmware title
    /// @param currFwVersion Current device firmware version
    /// @return Whether sending the current device firmware information was successful or not
    bool Firmware_Send_Info(char const * const currFwTitle, char const * const currFwVersion) {
        StaticJsonDocument<JSON_OBJECT_SIZE(2)> currentFirmwareInfo;
        currentFirmwareInfo[CURR_FW_TITLE_KEY] = currFwTitle;
        currentFirmwareInfo[CURR_FW_VER_KEY] = currFwVersion;
        return sendTelemetryJson(currentFirmwareInfo, Helper::Measure_Json(currentFirmwareInfo));
    }

    /// @brief Sends the given firmware state to the cloud.
    /// See https://thingsboard.io/docs/user-guide/ota-updates/ for more information
    /// @param currFwState Current firmware download state
    /// @param fwError Firmware error message that describes the current firmware state,
    /// pass nullptr or an empty string if the current state is not a failure state
    /// and therefore does not require any firmware error messsages, default = nullptr
    /// @return Whether sending the current firmware download state was successful or not
    bool Firmware_Send_State(char const * const currFwState, char const * const fwError = nullptr) {
        StaticJsonDocument<JSON_OBJECT_SIZE(2)> currentFirmwareState;
        if (!Helper::stringIsNullorEmpty(fwError)) {
            currentFirmwareState[FW_ERROR_KEY] = fwError;
        }
        currentFirmwareState[FW_STATE_KEY] = currFwState;
        return sendTelemetryJson(currentFirmwareState, Helper::Measure_Json(currentFirmwareState));
    }

#endif // THINGSBOARD_ENABLE_OTA

    //----------------------------------------------------------------------------
    // Shared attributes API

    /// @brief Requests one shared attribute calllback,
    /// that will be called if the key-value pair from the server for the given shared attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#request-attribute-values-from-the-server for more information
    /// @param callback Callback method that will be called
    /// @return Whether requesting the given callback was successful or not
#if THINGSBOARD_ENABLE_DYNAMIC
    bool Shared_Attributes_Request(Attribute_Request_Callback const & callback) {
#else
    bool Shared_Attributes_Request(Attribute_Request_Callback<MaxAttributes> const & callback) {
#endif // THINGSBOARD_ENABLE_DYNAMIC
        return Attributes_Request(callback, SHARED_REQUEST_KEY, SHARED_RESPONSE_KEY);
    }

    /// @brief Subscribes multiple shared attribute callbacks,
    /// that will be called if the key-value pair from the server for the given shared attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server for more information
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param first Iterator pointing to the first element in the data container
    /// @param last Iterator pointing to the end of the data container (last element + 1)
    /// @return Whether subscribing the given callbacks was successful or not
    template<typename InputIterator>
    bool Shared_Attributes_Subscribe(InputIterator const & first, InputIterator const & last) {
#if !THINGSBOARD_ENABLE_DYNAMIC
        size_t const size = Helper::distance(first, last);
        if (m_shared_attribute_update_callbacks.size() + size > m_shared_attribute_update_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, SHARED_ATTRIBUTE_UPDATE_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_client.subscribe(ATTRIBUTE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, ATTRIBUTE_TOPIC);
            return false;
        }

        // Push back complete vector into our local m_shared_attribute_update_callbacks vector.
        m_shared_attribute_update_callbacks.insert(m_shared_attribute_update_callbacks.end(), first, last);
        return true;
    }

    /// @brief Subscribe one shared attribute callback,
    /// that will be called if the key-value pair from the server for the given shared attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server for more information
    /// @param callback Callback method that will be called
    /// @return Whether subscribing the given callback was successful or not
#if THINGSBOARD_ENABLE_DYNAMIC
    bool Shared_Attributes_Subscribe(Shared_Attribute_Callback const & callback) {
#else
    bool Shared_Attributes_Subscribe(Shared_Attribute_Callback<MaxAttributes> const & callback) {
#endif // THINGSBOARD_ENABLE_DYNAMIC
#if !THINGSBOARD_ENABLE_DYNAMIC
        if (m_shared_attribute_update_callbacks.size() + 1U > m_shared_attribute_update_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, SHARED_ATTRIBUTE_UPDATE_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_client.subscribe(ATTRIBUTE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, ATTRIBUTE_TOPIC);
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
    bool Shared_Attributes_Unsubscribe() {
        m_shared_attribute_update_callbacks.clear();
        return m_client.unsubscribe(ATTRIBUTE_TOPIC);
    }
  
  private:
    IMQTT_Client&                                                     m_client;                             // MQTT client instance.
    size_t                                                            m_max_stack;                          // Maximum stack size we allocate at once.
#if THINGSBOARD_ENABLE_STREAM_UTILS
    size_t                                                            m_buffering_size;                     // Buffering size used to serialize directly into client.
#endif // THINGSBOARD_ENABLE_STREAM_UTILS

    // Vectors or array (depends on wheter if THINGSBOARD_ENABLE_DYNAMIC is set to 1 or 0), hold copy of the actual passed data, this is to ensure they stay valid,
    // even if the user only temporarily created the object before the method was called.
    // This can be done because all Callback methods mostly consists of pointers to actual object so copying them
    // does not require a huge memory overhead and is acceptable especially in comparsion to possible problems that could
    // arise if references were used and the end user does not take care to ensure the Callbacks live on for the entirety
    // of its usage, which will lead to dangling references and undefined behaviour.
    // Therefore copy-by-value has been choosen as for this specific use case it is more advantageous,
    // especially because at most we copy internal vectors or array, that will only ever contain a few pointers
#if THINGSBOARD_ENABLE_DYNAMIC
    Vector<RPC_Callback>                                               m_rpc_callbacks;                      // Server side RPC callbacks vector
    Vector<RPC_Request_Callback>                                       m_rpc_request_callbacks;              // Client side RPC callbacks vector
    Vector<Shared_Attribute_Callback>                                  m_shared_attribute_update_callbacks;  // Shared attribute update callbacks vector
    Vector<Attribute_Request_Callback>                                 m_attribute_request_callbacks;        // Client-side or shared attribute request callback vector
#else
    Array<RPC_Callback, MaxSubscribtions>                              m_rpc_callbacks;                     // Server side RPC callbacks array
    Array<RPC_Request_Callback, MaxSubscribtions>                      m_rpc_request_callbacks;             // Client side RPC callbacks array
    Array<Shared_Attribute_Callback<MaxAttributes>, MaxSubscribtions>  m_shared_attribute_update_callbacks; // Shared attribute update callbacks array
    Array<Attribute_Request_Callback<MaxAttributes>, MaxSubscribtions> m_attribute_request_callbacks;       // Client-side or shared attribute request callback array
#endif // THINGSBOARD_ENABLE_DYNAMIC

    Provision_Callback                                                 m_provision_callback;                // Provision response callback
    size_t                                                             m_request_id;                        // Allows nearly 4.3 million requests before wrapping back to 0

#if THINGSBOARD_ENABLE_OTA
    OTA_Update_Callback                                                m_fw_callback;                       // Ota update response callback
    uint16_t                                                           m_previous_buffer_size;              // Previous buffer size of the underlying client, used to revert to the previously configured buffer size if it was temporarily increased by the OTA update
    bool                                                               m_change_buffer_size;                // Whether the buffer size had to be changed, because the previous internal buffer size was to small to hold the firmware chunks
    OTA_Handler<Logger>                                                m_ota;                               // Class instance that handles the flashing and creating a hash from the given received binary firmware data
#endif // THINGSBOARD_ENABLE_OTA

#if THINGSBOARD_ENABLE_STREAM_UTILS
    /// @brief Serialize the custom attribute source into the underlying client.
    /// Sends the given bytes to the client without requiring any temporary buffer at the cost of hugely increased send times
    /// @tparam TSource Source class that should be used to serialize the json that is sent to the server
    /// @param topic Topic we want to send the data over
    /// @param source Data source containing our json key value pairs we want to send
    /// @param jsonSize Size of the data inside the source
    /// @return Whether sending the data was successful or not
    template <typename TSource>
    bool Serialize_Json(char const * const topic, TSource const & source, size_t const & jsonSize) {
        if (!m_client.begin_publish(topic, jsonSize)) {
            Logger::println(UNABLE_TO_SERIALIZE_JSON);
            return false;
        }
        BufferingPrint buffered_print(m_client, getBufferingSize());
        size_t const bytes_serialized = serializeJson(source, buffered_print);
        if (bytes_serialized < jsonSize) {
            Logger::println(UNABLE_TO_SERIALIZE_JSON);
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
    bool Publish_Chunk_Request(size_t const & request_chunck) {
        // Calculate the number of chuncks we need to request,
        // in order to download the complete firmware binary
        uint16_t const & chunk_size = m_fw_callback.Get_Chunk_Size();

        // Convert the interger size into a readable string
        char size[Helper::detectSize(NUMBER_PRINTF, chunk_size)] = {};
        (void)snprintf(size, sizeof(size), NUMBER_PRINTF, chunk_size);
        size_t const jsonSize = strlen(size);

        // Size adjuts dynamically to the current length of the currChunk number to ensure we don't cut it out of the topic string.
        char topic[Helper::detectSize(FIRMWARE_REQUEST_TOPIC, request_chunck)] = {};
        (void)snprintf(topic, sizeof(topic), FIRMWARE_REQUEST_TOPIC, request_chunck);

        return m_client.publish(topic, reinterpret_cast<uint8_t *>(size), jsonSize);
    }
#endif // THINGSBOARD_ENABLE_OTA

    /// @brief Returns the maximum amount of bytes that we want to allocate on the stack, before the memory is allocated on the heap instead
    /// @return Maximum amount of bytes we want to allocate on the stack
    size_t const & getMaximumStackSize() const {
        return m_max_stack;
    }

#if THINGSBOARD_ENABLE_STREAM_UTILS
    /// @brief Returns the amount of bytes that can be allocated to speed up fall back serialization with the StreamUtils class
    /// See https://github.com/bblanchon/ArduinoStreamUtils for more information on the underlying class used
    /// @return Amount of bytes allocated to speed up serialization
    size_t const & getBufferingSize() const {
      return m_buffering_size;
    }
#endif // THINGSBOARD_ENABLE_STREAM_UTILS

    /// @brief Requests one client-side or shared attribute calllback,
    /// that will be called if the key-value pair from the server for the given client-side or shared attributes is received
    /// @param callback Callback method that will be called
    /// @param attributeRequestKey Key of the key-value pair that will contain the attributes we want to request
    /// @param attributeResponseKey Key of the key-value pair that will contain the attributes we got as a response
    /// @return Whether requesting the given callback was successful or not
#if THINGSBOARD_ENABLE_DYNAMIC
    bool Attributes_Request(Attribute_Request_Callback const & callback, char const * const attributeRequestKey, char const * const attributeResponseKey) {
#else
    bool Attributes_Request(Attribute_Request_Callback<MaxAttributes> const & callback, char const * const attributeRequestKey, char const * const attributeResponseKey) {
#endif // THINGSBOARD_ENABLE_DYNAMIC
        auto const & attributes = callback.Get_Attributes();

        // Check if any sharedKeys were requested
        if (attributes.empty()) {
            Logger::println(NO_KEYS_TO_REQUEST);
            return false;
        }
        else if (attributeRequestKey == nullptr || attributeResponseKey == nullptr) {
#if THINGSBOARD_ENABLE_DEBUG
            Logger::println(ATT_KEY_NOT_FOUND);
#endif // THINGSBOARD_ENABLE_DEBUG
            return false;
        }

#if THINGSBOARD_ENABLE_DYNAMIC
        Attribute_Request_Callback * registeredCallback = nullptr;
#else
        Attribute_Request_Callback<MaxAttributes> * registeredCallback = nullptr;
#endif // THINGSBOARD_ENABLE_DYNAMIC
        if (!Attributes_Request_Subscribe(callback, registeredCallback)) {
            return false;
        }
        else if (registeredCallback == nullptr) {
            return false;
        }

        // String are const char* and therefore stored as a pointer --> zero copy, meaning the size for the strings is 0 bytes,
        // Data structure size depends on the amount of key value pairs passed + the default clientKeys or sharedKeys
        // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
        StaticJsonDocument<JSON_OBJECT_SIZE(1)> requestBuffer;

        // Calculate the size required for the char buffer containing all the attributes seperated by a comma,
        // before initalizing it so it is possible to allocate it on the stack
        size_t size = 0U;
        for (const auto & att : attributes) {
            if (Helper::stringIsNullorEmpty(att)) {
                continue;
            }

            size += strlen(att);
            size += strlen(COMMA);
        }

        // Initalizes complete array to 0, required because strncat needs both destination and source to contain proper null terminated strings
        char request[size] = {};
        for (const auto & att : attributes) {
            if (Helper::stringIsNullorEmpty(att)) {
#if THINGSBOARD_ENABLE_DEBUG
                Logger::println(ATT_IS_NULL);
#endif // THINGSBOARD_ENABLE_DEBUG
                continue;
            }

            strncat(request, att, size);
            size -= strlen(att);
            strncat(request, COMMA, size);
            size -= strlen(COMMA);
        }

        // Ensure to cast to const, this is done so that ArduinoJson does not copy the value but instead simply store the pointer, which does not require any more memory,
        // besides the base size needed to allocate one key-value pair. Because if we don't the char array would be copied
        // and because there is not enough space the value would simply be "undefined" instead. Which would cause the request to not be sent correctly
        requestBuffer[attributeRequestKey] = static_cast<const char*>(request);

        m_request_id++;
        registeredCallback->Set_Request_ID(m_request_id);
        registeredCallback->Set_Attribute_Key(attributeResponseKey);

        char topic[Helper::detectSize(ATTRIBUTE_REQUEST_TOPIC, m_request_id)] = {};
        (void)snprintf(topic, sizeof(topic), ATTRIBUTE_REQUEST_TOPIC, m_request_id);

        size_t const objectSize = Helper::Measure_Json(requestBuffer);
        return Send_Json(topic, requestBuffer, objectSize);
    }

    /// @brief Subscribes one provision callback,
    /// that will be called if a provision response from the server is received
    /// @param callback Callback method that will be called
    /// @return Whether requesting the given callback was successful or not
    bool Provision_Subscribe(Provision_Callback const & callback) {
        if (!m_client.subscribe(PROV_RESPONSE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, PROV_RESPONSE_TOPIC);
            return false;
        }
        m_provision_callback = callback;
        return true;
    }

    /// @brief Unsubcribes the provision callback
    /// @return Whether unsubcribing the previously subscribed callback
    /// and from the provision response topic, was successful or not
    bool Provision_Unsubscribe() {
        m_provision_callback = Provision_Callback();
        return m_client.unsubscribe(PROV_RESPONSE_TOPIC);
    }

#if THINGSBOARD_ENABLE_OTA
    /// @brief Checks the included information in the callback,
    /// and attempts to sends the current device firmware information to the cloud
    /// @param callback Callback method that will be called
    /// @return Whether checking and sending the current device firmware information was successful or not
    bool Prepare_Firmware_Settings(OTA_Update_Callback const & callback) {
        char const * const currFwTitle = callback.Get_Firmware_Title();
        char const * const currFwVersion = callback.Get_Firmware_Version();

        if (Helper::stringIsNullorEmpty(currFwTitle) || Helper::stringIsNullorEmpty(currFwVersion)) {
            return false;
        }
        else if (!Firmware_Send_Info(currFwTitle, currFwVersion)) {
            return false;
        }

        m_fw_callback = callback;
        return true;
    }

    /// @brief Subscribes to the firmware response topic
    /// @return Whether subscribing to the firmware response topic was successful or not
    bool Firmware_OTA_Subscribe() {
        if (!m_client.subscribe(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC)) {
            char message[JSON_STRING_SIZE(strlen(SUBSCRIBE_TOPIC_FAILED)) + JSON_STRING_SIZE(strlen(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC))] = {};
            (void)snprintf(message, sizeof(message), SUBSCRIBE_TOPIC_FAILED, FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC);
            Logger::println(message);
            Firmware_Send_State(FW_STATE_FAILED, message);
            return false;
        }
        return true;
    }

    /// @brief Unsubscribes from the firmware response topic and clears any memory associated with the firmware update,
    /// should not be called before actually fully completing the firmware update.
    /// @return Whether unsubscribing from the firmware response topic was successful or not
    bool Firmware_OTA_Unsubscribe() {
        // Buffer size has been set to another value before the update,
        // to allow to receive ota chunck packets that might be much bigger than the normal
        // buffer size would allow, therefore we return to the previous value to decrease overall memory usage
        if (m_change_buffer_size) {
            (void)setBufferSize(m_previous_buffer_size);
        }
        // Reset now not needed private member variables
        m_fw_callback = OTA_Update_Callback();
        // Unsubscribe from the topic
        return m_client.unsubscribe(FIRMWARE_RESPONSE_SUBSCRIBE_TOPIC);
    }

    /// @brief Callback that will be called upon firmware shared attribute arrival
    /// @param data Json data containing key-value pairs for the needed firmware information,
    /// to ensure we have a firmware assigned and can start the update over MQTT
    void Firmware_Shared_Attribute_Received(JsonObjectConst const & data) {
        // Check if firmware is available for our device
        if (!data.containsKey(FW_VER_KEY) || !data.containsKey(FW_TITLE_KEY) || !data.containsKey(FW_CHKS_KEY) || !data.containsKey(FW_CHKS_ALGO_KEY) || !data.containsKey(FW_SIZE_KEY)) {
            Logger::println(NO_FW);
            Firmware_Send_State(FW_STATE_FAILED, NO_FW);
            return;
        }

        char const * const fw_title = data[FW_TITLE_KEY];
        char const * const fw_version = data[FW_VER_KEY];
        char const * const fw_checksum = data[FW_CHKS_KEY];
        char const * const fw_algorithm = data[FW_CHKS_ALGO_KEY];
        size_t const fw_size = data[FW_SIZE_KEY];

        char const * const curr_fw_title = m_fw_callback.Get_Firmware_Title();
        char const * const curr_fw_version = m_fw_callback.Get_Firmware_Version();

        if (fw_title == nullptr || fw_version == nullptr || curr_fw_title == nullptr || curr_fw_version == nullptr || fw_algorithm == nullptr || fw_checksum == nullptr) {
            Logger::println(EMPTY_FW);
            Firmware_Send_State(FW_STATE_FAILED, EMPTY_FW);
            return;
        }
        // If firmware version and title is the same, we do not initiate an update, because we expect the type of binary to be the same one we are currently using and therefore updating would be useless
        else if (strncmp(curr_fw_title, fw_title, strlen(curr_fw_title)) == 0 && strncmp(curr_fw_version, fw_version, strlen(curr_fw_version)) == 0) {
            char message[JSON_STRING_SIZE(strlen(FW_UP_TO_DATE)) + JSON_STRING_SIZE(strlen(curr_fw_version))] = {};
            (void)snprintf(message, sizeof(message), FW_UP_TO_DATE, curr_fw_version);
            Logger::println(message);
            Firmware_Send_State(FW_STATE_FAILED, message);
            return;
        }
        // If firmware title is not the same, we do not initiate an update, because we expect the binary to be for another type of device and downloading it on this device could possibly cause hardware issues
        else if (strncmp(curr_fw_title, fw_title, strlen(curr_fw_title)) != 0) {
            char message[JSON_STRING_SIZE(strlen(FW_NOT_FOR_US)) + JSON_STRING_SIZE(strlen(curr_fw_title)) + JSON_STRING_SIZE(strlen(fw_title))] = {};
            (void)snprintf(message, sizeof(message), FW_NOT_FOR_US, curr_fw_title, fw_title);
            Logger::println(message);
            Firmware_Send_State(FW_STATE_FAILED, message);
            return;
        }

        mbedtls_md_type_t fw_checksum_algorithm = mbedtls_md_type_t{};

        if (strncmp(CHECKSUM_AGORITM_MD5, fw_algorithm, strlen(CHECKSUM_AGORITM_MD5)) == 0) {
            fw_checksum_algorithm = mbedtls_md_type_t::MBEDTLS_MD_MD5;
        }
        else if (strncmp(CHECKSUM_AGORITM_SHA256, fw_algorithm, strlen(CHECKSUM_AGORITM_SHA256)) == 0) {
            fw_checksum_algorithm = mbedtls_md_type_t::MBEDTLS_MD_SHA256;
        }
        else if (strncmp(CHECKSUM_AGORITM_SHA384, fw_algorithm, strlen(CHECKSUM_AGORITM_SHA384)) == 0) {
            fw_checksum_algorithm = mbedtls_md_type_t::MBEDTLS_MD_SHA384;
        }
        else if (strncmp(CHECKSUM_AGORITM_SHA512, fw_algorithm, strlen(CHECKSUM_AGORITM_SHA512)) == 0) {
            fw_checksum_algorithm = mbedtls_md_type_t::MBEDTLS_MD_SHA512;
        }
        else {
            char message[JSON_STRING_SIZE(strlen(FW_CHKS_ALGO_NOT_SUPPORTED)) + JSON_STRING_SIZE(strlen(fw_algorithm))] = {};
            (void)snprintf(message, sizeof(message), FW_CHKS_ALGO_NOT_SUPPORTED, fw_algorithm);
            Logger::println(message);
            Firmware_Send_State(FW_STATE_FAILED, message);
            return;
        }

        if (!Firmware_OTA_Subscribe()) {
            return;
        }

#if THINGSBOARD_ENABLE_DEBUG
        Logger::println(PAGE_BREAK);
        Logger::println(NEW_FW);
        char firmware[JSON_STRING_SIZE(strlen(FROM_TOO)) + JSON_STRING_SIZE(strlen(curr_fw_version)) + JSON_STRING_SIZE(strlen(fw_version))] = {};
        (void)snprintf(firmware, sizeof(firmware), FROM_TOO, curr_fw_version, fw_version);
        Logger::println(firmware);
        Logger::println(DOWNLOADING_FW);
#endif // THINGSBOARD_ENABLE_DEBUG

        // Calculate the number of chuncks we need to request,
        // in order to download the complete firmware binary
        const uint16_t& chunk_size = m_fw_callback.Get_Chunk_Size();

        // Get the previous buffer size and cache it so the previous settings can be restored.
        m_previous_buffer_size = m_client.get_buffer_size();
        m_change_buffer_size = m_previous_buffer_size < (chunk_size + 50U);

        // Increase size of receive buffer
        if (m_change_buffer_size && !setBufferSize(chunk_size + 50U)) {
            Logger::println(NOT_ENOUGH_RAM);
            Firmware_Send_State(FW_STATE_FAILED, NOT_ENOUGH_RAM);
            return;
        }

        m_ota.Start_Firmware_Update(m_fw_callback, fw_size, fw_checksum, fw_checksum_algorithm);
    }
#endif // THINGSBOARD_ENABLE_OTA

    /// @brief Connects to the previously set ThingsBoard server, as the given client with the given access token
    /// @param access_token Access token that connects this device with a created device on the ThingsBoard server,
    /// can be "provision", if the device creates itself instead
    /// @param client_id Client username that can be used to differentiate the user that is connecting the given device to ThingsBoard
    /// @param password Client password that can be used to authenticate the user that is connecting the given device to ThingsBoard
    /// @return Whether connecting to ThingsBoard was successful or not
    bool connect_to_host(char const * const access_token, char const * const client_id, char const * const password) {
        bool const connection_result = m_client.connect(client_id, access_token, password);
        if (!connection_result) {
            Logger::println(CONNECT_FAILED);
        }
        return connection_result;
    }

    /// @brief Resubscribes to topics that establish a permanent connection with MQTT, meaning they may receive more than one event over their lifetime,
    /// whereas other events that are only ever called once and then deleted after they have been handled are not resubscribed.
    /// Only the topics that establish a permanent connection are resubscribed, because all not yet received data is discard on the MQTT broker,
    // once we establish a connection again. This is the case because we connect with the cleanSession attribute set to true.
    // Therefore we can also clear the buffer of all non-permanent topics.
    void Resubscribe_Topics() {
        if (!m_rpc_callbacks.empty() && !m_client.subscribe(RPC_SUBSCRIBE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, RPC_SUBSCRIBE_TOPIC);
        }
        if (!m_shared_attribute_update_callbacks.empty() && !m_client.subscribe(ATTRIBUTE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, ATTRIBUTE_TOPIC);
        }
        // Clean up any not yet answered single event subscriptions
        (void)RPC_Request_Unsubscribe();
        (void)Attributes_Request_Unsubscribe();
        (void)Provision_Unsubscribe();
    }

    /// @brief Subscribes to the client-side RPC response topic
    /// @param callback Callback method that will be called
    /// @param registeredCallback Editable pointer to a reference of the local version that was copied from the passed callback
    /// @return Whether requesting the given callback was successful or not
    bool RPC_Request_Subscribe(RPC_Request_Callback const & callback, RPC_Request_Callback * & registeredCallback) {
#if !THINGSBOARD_ENABLE_DYNAMIC
        if (m_rpc_request_callbacks.size() + 1 > m_rpc_request_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, CLIENT_SIDE_RPC_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_client.subscribe(RPC_RESPONSE_SUBSCRIBE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, RPC_RESPONSE_SUBSCRIBE_TOPIC);
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
    bool RPC_Request_Unsubscribe() {
        m_rpc_request_callbacks.clear();
        return m_client.unsubscribe(RPC_RESPONSE_SUBSCRIBE_TOPIC);
    }

    /// @brief Subscribes to attribute response topic
    /// @param callback Callback method that will be called
    /// @param registeredCallback Editable pointer to a reference of the local version that was copied from the passed callback
    /// @return Whether requesting the given callback was successful or not
#if THINGSBOARD_ENABLE_DYNAMIC
    bool Attributes_Request_Subscribe(Attribute_Request_Callback const & callback, Attribute_Request_Callback * & registeredCallback) {
#else
    bool Attributes_Request_Subscribe(Attribute_Request_Callback<MaxAttributes> const & callback, Attribute_Request_Callback<MaxAttributes> * & registeredCallback) {
#endif // THINGSBOARD_ENABLE_DYNAMIC
#if !THINGSBOARD_ENABLE_DYNAMIC
        if (m_attribute_request_callbacks.size() + 1 > m_attribute_request_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, CLIENT_SHARED_ATTRIBUTE_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_client.subscribe(ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, ATTRIBUTE_RESPONSE_SUBSCRIBE_TOPIC);
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
    bool Attributes_Request_Unsubscribe() {
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
    bool sendKeyValue(char const * const key, T const & value, bool telemetry = true) {
        const Telemetry t(key, value);
        if (t.IsEmpty()) {
            return false;
        }

        StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonBuffer;
        if (!t.SerializeKeyValue(jsonBuffer)) {
            Logger::println(UNABLE_TO_SERIALIZE);
            return false;
        }

        return telemetry ? sendTelemetryJson(jsonBuffer, Helper::Measure_Json(jsonBuffer)) : sendAttributeJson(jsonBuffer, Helper::Measure_Json(jsonBuffer));
    }

    /// @brief Process callback that will be called upon client-side RPC response arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    void process_rpc_request_message(char * const topic, JsonObjectConst const & data) {
        size_t const request_id = Helper::parseRequestId(RPC_RESPONSE_TOPIC, topic);

        for (auto it = m_rpc_request_callbacks.cbegin(); it != m_rpc_request_callbacks.cend(); ++it) {
            auto const & rpc_request = *it;

            if (rpc_request.Get_Request_ID() != request_id) {
                continue;
            }

#if THINGSBOARD_ENABLE_DEBUG
            Logger::printfln(CALLING_REQUEST_CB, request_id);
#endif // THINGSBOARD_ENABLE_DEBUG

            rpc_request.template Call_Callback<Logger>(data);

            // Delete callback because the changes have been requested and the callback is no longer needed
            Helper::remove(m_rpc_request_callbacks, it);
            break;
        }

        // Attempt to unsubscribe from the shared attribute request topic,
        // if we are not waiting for any further responses with shared attributes from the server.
        // Will be resubscribed if another request is sent anyway
        if (m_rpc_request_callbacks.empty()) {
            (void)RPC_Request_Unsubscribe();
        }
    }

    /// @brief Process callback that will be called upon server-side RPC request arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    void process_rpc_message(char * const topic, JsonObjectConst const & data) {
        char const * const methodName = data[RPC_METHOD_KEY];

        if (methodName == nullptr) {
            Logger::println(RPC_METHOD_NULL);
            return;
        }

        for (auto const & rpc : m_rpc_callbacks) {
            char const * const subscribedMethodName = rpc.Get_Name();
            if (Helper::stringIsNullorEmpty(subscribedMethodName)) {
              Logger::println(RPC_METHOD_NULL);
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
                Logger::println(NO_RPC_PARAMS_PASSED);
#endif // THINGSBOARD_ENABLE_DEBUG
            }

#if THINGSBOARD_ENABLE_DEBUG
            Logger::printfln(CALLING_RPC_CB, methodName);
#endif // THINGSBOARD_ENABLE_DEBUG

            JsonVariantConst const param = data[RPC_PARAMS_KEY];
#if THINGSBOARD_ENABLE_DYNAMIC
            size_t const & rpc_response_size = rpc.Get_Response_Size();
            // String are char const * and therefore stored as a pointer --> zero copy, meaning the size for the strings is 0 bytes,
            // Data structure size depends on the amount of key value pairs passed.
            // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
            TBJsonDocument jsonBuffer(rpc_response_size);
#else
            size_t constexpr rpc_response_size = MaxRPC;
            StaticJsonDocument<JSON_OBJECT_SIZE(MaxRPC)> jsonBuffer;
#endif // THINGSBOARD_ENABLE_DYNAMIC
            rpc.template Call_Callback<Logger>(param, jsonBuffer);

            if (jsonBuffer.isNull()) {
                // Message is ignored and not sent at all.
                break;
            }
            else if (jsonBuffer.overflowed()) {
                Logger::printfln(RPC_RESPONSE_OVERFLOWED, rpc_response_size);
                break;
            }

            size_t const request_id = Helper::parseRequestId(RPC_REQUEST_TOPIC, topic);
            char responseTopic[Helper::detectSize(RPC_SEND_RESPONSE_TOPIC, request_id)] = {};
            (void)snprintf(responseTopic, sizeof(responseTopic), RPC_SEND_RESPONSE_TOPIC, request_id);

            size_t const jsonSize = Helper::Measure_Json(jsonBuffer);
            Send_Json(responseTopic, jsonBuffer, jsonSize);
            break;
        }
    }

#if THINGSBOARD_ENABLE_OTA

    /// @brief Process callback that will be called upon firmware response arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callback
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
    void process_firmware_response(char * const topic, uint8_t * const payload, size_t const & length) {
        size_t const request_id = Helper::parseRequestId(FIRMWARE_RESPONSE_TOPIC, topic);

        // Check if the remaining stack size of the current task would overflow the stack,
        // if it would allocate the memory on the heap instead to ensure no stack overflow occurs.
        if (getMaximumStackSize() < length) {
            uint8_t* binary = new uint8_t[length]();
            (void)memcpy(binary, payload, length);
            m_ota.Process_Firmware_Packet(request_id, binary, length);
            // Ensure to actually delete the memory placed onto the heap, to make sure we do not create a memory leak
            // and set the pointer to null so we do not have a dangling reference.
            delete[] binary;
            binary = nullptr;
        }
        else {
            uint8_t binary[length] = {};
            (void)memcpy(binary, payload, length);
            m_ota.Process_Firmware_Packet(request_id, binary, length);
        }
    }

#endif // THINGSBOARD_ENABLE_OTA

    /// @brief Process callback that will be called upon shared attribute update arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    void process_shared_attribute_update_message(char * const topic, JsonObjectConst & data) {
        if (!data) {
#if THINGSBOARD_ENABLE_DEBUG
            Logger::println(NOT_FOUND_ATT_UPDATE);
#endif // THINGSBOARD_ENABLE_DEBUG
            return;
        }

        if (data.containsKey(SHARED_RESPONSE_KEY)) {
            data = data[SHARED_RESPONSE_KEY];
        }

        for (auto const & shared_attribute : m_shared_attribute_update_callbacks) {
            if (shared_attribute.Get_Attributes().empty()) {
#if THINGSBOARD_ENABLE_DEBUG
                Logger::println(ATT_CB_NO_KEYS);
#endif // THINGSBOARD_ENABLE_DEBUG
                // No specifc keys were subscribed so we call the callback anyway, assumed to be subscribed to any update
                shared_attribute.template Call_Callback<Logger>(data);
                continue;
            }

            char const * requested_att = nullptr;

            for (auto const & att : shared_attribute.Get_Attributes()) {
                if (Helper::stringIsNullorEmpty(att)) {
#if THINGSBOARD_ENABLE_DEBUG
                    Logger::println(ATT_IS_NULL);
#endif // THINGSBOARD_ENABLE_DEBUG
                    continue;
                }
                // Check if the request contained any of our requested keys and
                // break early if the key was requested from this callback.
                if (data.containsKey(att)) {
                    requested_att = att;
                    break;
                }
            }

            // Check if this callback did not request any keys that were in this response,
            // if there were not we simply continue with the next subscribed callback.
            if (requested_att == nullptr) {
#if THINGSBOARD_ENABLE_DEBUG
                Logger::println(ATT_NO_CHANGE);
#endif // THINGSBOARD_ENABLE_DEBUG
                continue;
            }

#if THINGSBOARD_ENABLE_DEBUG
            Logger::printfln(CALLING_ATT_CB, requested_att);
#endif // THINGSBOARD_ENABLE_DEBUG

            shared_attribute.template Call_Callback<Logger>(data);
        }
    }

    /// @brief Process callback that will be called upon client-side or shared attribute request arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    void process_attribute_request_message(char * const topic, JsonObjectConst & data) {
        size_t const request_id = Helper::parseRequestId(ATTRIBUTE_RESPONSE_TOPIC, topic);

        for (auto it = m_attribute_request_callbacks.cbegin(); it != m_attribute_request_callbacks.cend(); ++it) {
            auto const & attribute_request = *it;

            if (attribute_request.Get_Request_ID() != request_id) {
                continue;
            }
            char const * const attributeResponseKey = attribute_request.Get_Attribute_Key();
            if (attributeResponseKey == nullptr) {
#if THINGSBOARD_ENABLE_DEBUG
                Logger::println(ATT_KEY_NOT_FOUND);
#endif // THINGSBOARD_ENABLE_DEBUG
                goto delete_callback;
            }
            else if (!data) {
#if THINGSBOARD_ENABLE_DEBUG
                Logger::println(ATT_KEY_NOT_FOUND);
#endif // THINGSBOARD_ENABLE_DEBUG
                goto delete_callback;
            }

            if (data.containsKey(attributeResponseKey)) {
                data = data[attributeResponseKey];
            }

#if THINGSBOARD_ENABLE_DEBUG
            Logger::printfln(CALLING_REQUEST_CB, request_id);
#endif // THINGSBOARD_ENABLE_DEBUG

            attribute_request.template Call_Callback<Logger>(data);

            delete_callback:
            // Delete callback because the changes have been requested and the callback is no longer needed
            Helper::remove(m_attribute_request_callbacks, it);
            break;
        }

        // Unsubscribe from the shared attribute request topic,
        // if we are not waiting for any further responses with shared attributes from the server.
        // Will be resubscribed if another request is sent anyway
        if (m_attribute_request_callbacks.empty()) {
            (void)Attributes_Request_Unsubscribe();
        }
    }

    /// @brief Process callback that will be called upon provision response arrival
    /// and is responsible for handling the payload and calling the appropriate previously subscribed callback
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    void process_provisioning_response(char * const topic, JsonObjectConst const & data) {
        m_provision_callback.template Call_Callback<Logger>(data);
        // Unsubscribe from the provision response topic,
        // Will be resubscribed if another request is sent anyway
        (void)Provision_Unsubscribe();
    }

    /// @brief Attempts to send aggregated attribute or telemetry data
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param first Iterator pointing to the first element in the data container
    /// @param last Iterator pointing to the end of the data container (last element + 1)
    /// @param telemetry Whether the data we want to send should be sent over the attribute or telemtry topic
    /// @return Whether sending the aggregated data was successful or not
    template<typename InputIterator>
    bool sendDataArray(InputIterator const & first, InputIterator const & last, bool telemetry) {
#if THINGSBOARD_ENABLE_DYNAMIC
        // String are char const * and therefore stored as a pointer --> zero copy, meaning the size for the strings is 0 bytes,
        // Data structure size depends on the amount of key value pairs passed.
        // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
        size_t const size = Helper::distance(first, last);
        TBJsonDocument jsonBuffer(JSON_OBJECT_SIZE(size));
#else
        StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmount)> jsonBuffer;
#endif // THINGSBOARD_ENABLE_DYNAMIC

        for (auto it = first; it != last; ++it) {
            auto const & data = *it;
            if (!data.SerializeKeyValue(jsonBuffer)) {
                Logger::println(UNABLE_TO_SERIALIZE);
                return false;
            }
        }

        return telemetry ? sendTelemetryJson(jsonBuffer, Helper::Measure_Json(jsonBuffer)) : sendAttributeJson(jsonBuffer, Helper::Measure_Json(jsonBuffer));
    }

    /// @brief MQTT callback that will be called if a publish message is received from the server
    /// @param topic Previously subscribed topic, we got the response over 
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
    void onMQTTMessage(char * const topic, uint8_t * const payload, unsigned int length) {
#if THINGSBOARD_ENABLE_DEBUG
        Logger::printfln(RECEIVE_MESSAGE, topic);
#endif // THINGSBOARD_ENABLE_DEBUG

#if THINGSBOARD_ENABLE_OTA
        // When receiving the ota binary payload we do not want to deserialize it into json, because it only contains
        // firmware bytes that should be directly writtin into flash, therefore we can skip that step and directly process those bytes
        if (strncmp(FIRMWARE_RESPONSE_TOPIC, topic, strlen(FIRMWARE_RESPONSE_TOPIC)) == 0) {
            process_firmware_response(topic, payload, length);
            return;
        }
#endif // THINGSBOARD_ENABLE_OTA

#if THINGSBOARD_ENABLE_DYNAMIC
        // Buffer that we deserialize is writeable and not read only --> zero copy, meaning the size for the data is 0 bytes,
        // Data structure size depends on the amount of key value pairs received.
        // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
        TBJsonDocument jsonBuffer(JSON_OBJECT_SIZE(Helper::getOccurences(reinterpret_cast<char *>(payload), COLON)));
#else
        StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmount)> jsonBuffer;
#endif // THINGSBOARD_ENABLE_DYNAMIC

        // The deserializeJson method we use, can use the zero copy mode because a writeable input was passed,
        // if that were not the case the needed allocated memory would drastically increase, because the keys would need to be copied as well.
        // See https://arduinojson.org/v6/doc/deserialization/ for more info on ArduinoJson deserialization
        DeserializationError const error = deserializeJson(jsonBuffer, payload, length);
        if (error) {
            Logger::printfln(UNABLE_TO_DE_SERIALIZE_JSON, error.c_str());
            return;
        }
        // .as() is used instead of .to(), because it is meant to cast the JsonDocument to the given type,
        // but it does not change the actual content of the JsonDocument, we don't want that because it contains content
        // and .to() would result in the data being cleared ()"null"), instead .as() which allows accessing the data over a JsonObjectConst instead
        JsonObjectConst data = jsonBuffer.template as<JsonObjectConst>();

        // Checks to ensure we forward the already json serialized data to the correct process function,
        // especially important is the order of ATTRIBUTE_RESPONSE_TOPIC and ATTRIBUTE_TOPIC,
        // that is the case because the more specific one that is longer but contains the same text has to be checked first,
        // because if we do not do that then even if we receive a message from the ATTRIBUTE_RESPONSE_TOPIC
        // we would call the process method for the ATTRIBUTE_TOPIC because we only compare until the end of the ATTRIBUTE_TOPIC string,
        // therefore the received topic is exactly the same. Therefore the ordering needs to stay the same for thoose two specific checks
        if (strncmp(RPC_RESPONSE_TOPIC, topic, strlen(RPC_RESPONSE_TOPIC)) == 0) {
            process_rpc_request_message(topic, data);
        }
        else if (strncmp(RPC_REQUEST_TOPIC, topic, strlen(RPC_REQUEST_TOPIC)) == 0) {
            process_rpc_message(topic, data);
        }
        else if (strncmp(ATTRIBUTE_RESPONSE_TOPIC, topic, strlen(ATTRIBUTE_RESPONSE_TOPIC)) == 0) {
            process_attribute_request_message(topic, data);
        }
        else if (strncmp(ATTRIBUTE_TOPIC, topic, strlen(ATTRIBUTE_TOPIC)) == 0) {
            process_shared_attribute_update_message(topic, data);
        }
        else if (strncmp(PROV_RESPONSE_TOPIC, topic, strlen(PROV_RESPONSE_TOPIC)) == 0) {
            process_provisioning_response(topic, data);
        }
    }

#if !THINGSBOARD_ENABLE_STL
    // PubSub client cannot call a method when message arrives on subscribed topic.
    // Only free-standing function is allowed.
    // To be able to forward event to an instance, rather than to a function, this pointer exists.
    static ThingsBoardSized *m_subscribedInstance;

    static void onStaticMQTTMessage(char * const topic, uint8_t * const payload, unsigned int length) {
        if (m_subscribedInstance == nullptr) {
            return;
        }
        m_subscribedInstance->onMQTTMessage(topic, payload, length);
    }

    static void onStaticMQTTConnect() {
        if (m_subscribedInstance == nullptr) {
            return;
        }
        m_subscribedInstance->Resubscribe_Topics();
    }
#endif // !THINGSBOARD_ENABLE_STL
};

#if !THINGSBOARD_ENABLE_STL && !THINGSBOARD_ENABLE_DYNAMIC
template<size_t MaxFieldsAmount, size_t MaxSubscribtions, size_t MaxAttributes, size_t MaxRPC, typename Logger>
ThingsBoardSized<MaxFieldsAmount, MaxSubscribtions, MaxAttributes, MaxRPC, Logger> *ThingsBoardSized<MaxFieldsAmount, MaxSubscribtions, MaxAttributes, MaxRPC, Logger>::m_subscribedInstance = nullptr;
#elif !THINGSBOARD_ENABLE_STL && THINGSBOARD_ENABLE_DYNAMIC
template<typename Logger>
ThingsBoardSized<Logger> *ThingsBoardSized<Logger>::m_subscribedInstance = nullptr;
#endif

using ThingsBoard = ThingsBoardSized<>;

#endif // ThingsBoard_h
