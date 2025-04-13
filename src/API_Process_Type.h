#ifndef API_Process_Type_h
#define API_Process_Type_h

// Library include.
#include <stdint.h>


/// @brief Possible processing types an API Implementation uses to handle responses from the server.
/// @note Only ever calls either the Process_Response (RAW) or Process_Json_Response (JSON) method, this is the case because data that can not be serialized into JSON, should also never be serialized into JSON,
/// This is the case because it would cause deserialization errors that will be printed in the console, even tough the response was already handled in the RAW format.
///
/// If the received data is unserialized binary data, which we should not serialize into JSON because the received data is not JSON in the first place Process_Response method will be called (OTA Firmware Update).
/// Alterantively, if the received data needs to be serialized into JSON data the Process_JSON_Response method will be called with the data already serialized into the JSON format instead (everything else).
///
/// Additionally the actual data is never copied in both cases to safe space. Instead the underlying MQTT buffer is expected to keep the buffer unchanged and alive and we simply use a non-owning pointer for our operations.
/// This can cause the data to become garbage if the user code resizes the underlying MQTT buffer size and therefore moves the actual data in the MQTT buffer, before the data is handled by the code itself.
/// Before version v0.15.0, the TBPubSubClient used by the Arduino_MQTT_Client could also cause this issue, if data is sent before received data was handled. This was the case because the input and output Heap buffer were not seperate,
/// but instead it was handled by the same underlying Heap buffer, meaning if we sent data in the callback handling received data that data was partially overwritten and became magnled
enum class API_Process_Type : uint8_t {
    RAW, ///< Calls Process_Response with the received unserialized binary data
    JSON ///< Calls Process_Json_Response with the received unserialized binary data serialized into JSON
};

#endif // API_Process_Type_h
