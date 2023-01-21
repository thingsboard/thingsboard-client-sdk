/*
  Attribute_Request_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Attribute_Request_Callback_h
#define Attribute_Request_Callback_h

// Library includes.
#include <ArduinoJson.h>
#include "Configuration.h"
#if THINGSBOARD_ENABLE_STL
#include <functional>
#include <vector>
#endif // THINGSBOARD_ENABLE_STL

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
constexpr char ATT_REQUEST_CB_IS_NULL[] PROGMEM = "Client-side or shared attribute request callback is NULL";

// Convenient aliases
// JSON object const (read only twice as small as JSON object), is used to communicate Shared Attributes and Provision Data to the client
using Attribute_Data = const JsonObjectConst;

/// @brief Client-side or shared attributes request callback wrapper
class Attribute_Request_Callback {
  public:
    /// @brief Client-side or shared attributes callback signature
    using returnType = void;
    using argumentType = const Attribute_Data&;
#if THINGSBOARD_ENABLE_STL
    using processFn = std::function<returnType(argumentType data)>;
#else
    using processFn = returnType (*)(argumentType data);
#endif // THINGSBOARD_ENABLE_STL

    /// @brief Constructs empty callback, will result in never being called
    inline Attribute_Request_Callback()
      : m_attributes(), m_request_id(0U), m_attribute_key(nullptr), m_cb(nullptr) {  }

#if THINGSBOARD_ENABLE_STL

    /// @brief Constructs callback, will be called upon client-side or shared attribute request arrival
    /// where the given multiple requested client-side or shared attributes were sent by the cloud and received by the client
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array
    /// @param first_itr Iterator pointing to the first element in the data container
    /// @param last_itr Iterator pointing to the end of the data container (last element + 1)
    /// @param cb Callback method that will be called
    template<class InputIterator>
    inline Attribute_Request_Callback(const InputIterator& first_itr, const InputIterator& last_itr, processFn cb)
      : m_attributes(first_itr, last_itr), m_request_id(0U), m_attribute_key(nullptr), m_cb(cb) {  }

#else

    /// @brief Constructs callback, will be called upon client-side or shared attribute request arrival
    /// where the given multiple requested client-side or shared attributes were sent by the cloud and received by the client
    /// @param attributes Comma seperated string containing all attributes we want to request (test1, test2, ...)
    /// @param cb Callback method that will be called
    inline Attribute_Request_Callback(const char* attributes, processFn cb)
      : m_attributes(attributes), m_request_id(0U), m_attribute_key(nullptr), m_cb(cb) {  }

#endif // THINGSBOARD_ENABLE_STL

    /// @brief Calls the callback that was subscribed, when this class instance was initally created
    /// @tparam Logger Logging class that should be used to print messages
    /// @param data Received client-side or shared attribute request data that include
    /// the client-side or shared attributes that were requested and their current values
    template<typename Logger>
    inline returnType Call_Callback(argumentType data) const {
      // Check if the callback is a nullptr,
      // meaning it has not been assigned any valid callback method.
      if (!m_cb) {
        Logger::log(ATT_REQUEST_CB_IS_NULL);
        return returnType();
      }
      return m_cb(data);
    }

    /// @brief Gets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which Attribute_Request_Callback
    /// is connected to which received client-side or shared attributes
    /// @return Unique identifier connected to the requested client-side or shared attributes
    inline const uint32_t& Get_Request_ID() const {
      return m_request_id;
    }

    /// @brief Sets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which Attribute_Request_Callback
    /// is connected to which received client-side or shared attributes
    /// @param request_id Unqiue identifier of the request for client-side or shared attributes
    inline void Set_Request_ID(const uint32_t& request_id) {
      m_request_id = request_id;
    }

    /// @brief Sets the response key of the key-value pair,
    /// that we expect the client-side or shared attribute payload json data to be contained in
    /// @param attribute_key Key that the data is saved into,
    /// client for client-side attributes and shared for shared attributes
    inline void Set_Attribute_Key(const char* attribute_key) {
      m_attribute_key = attribute_key;
    }

    /// @brief Gets the response key of the key-value pair,
    /// that we expect the client-side or shared attribute payload json data to be contained in
    /// @return Key that the data is saved into,
    /// client for client-side attributes and shared for shared attributes
    inline const char* Get_Attribute_Key() const {
      return m_attribute_key;
    }

#if THINGSBOARD_ENABLE_STL

    /// @brief Gets all the requested client-side or shared attributes that will result,
    /// in the subscribed method being called when the response with their current value
    // is sent from the cloud and received by the client,
    /// passed when this class instance was initally created
    /// @return Requested client-side or shared attributes
    inline const std::vector<const char *>& Get_Attributes() const {
      return m_attributes;
    }

#else

    /// @brief Gets the string containing all the requested client-side or shared attributes that will result,
    /// in the subscribed method being called when the response with their current value
    // is sent from the cloud and received by the client,
    /// passed when this class instance was initally created
    /// @return Requested client-side or shared attributes
    inline const char* Get_Attributes() const {
      return m_attributes;
    }

#endif // THINGSBOARD_ENABLE_STL

  private:
#if THINGSBOARD_ENABLE_STL
    std::vector<const char *>      m_attributes;      // Attribute we want to request
#else
    const char                     *m_attributes;     // Attribute we want to request
#endif // THINGSBOARD_ENABLE_STL
    uint32_t                       m_request_id;      // Id the request was called with
    const char                     *m_attribute_key;  // Attribute key that we wil receive the response on ("client" or "shared")
    processFn                      m_cb;              // Callback to call
};

#endif // Attribute_Request_Callback_h
