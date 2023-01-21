/*
  Shared_Attribute_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Shared_Attribute_Callback_h
#define Shared_Attribute_Callback_h

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
constexpr char ATT_CB_IS_NULL[] PROGMEM = "Shared attribute update callback is NULL";

// Convenient aliases
// JSON object const (read only twice as small as JSON object), is used to communicate Shared Attributes and Provision Data to the client
using Shared_Attribute_Data = const JsonObjectConst;

/// @brief Shared attributes callback wrapper
class Shared_Attribute_Callback {
  public:
    /// @brief Shared attributes callback signature
    using returnType = void;
    using argumentType = const Shared_Attribute_Data&;
#if THINGSBOARD_ENABLE_STL
    using processFn = std::function<returnType(argumentType data)>;
#else
    using processFn = returnType (*)(argumentType data);
#endif // THINGSBOARD_ENABLE_STL

    /// @brief Constructs empty callback, will result in never being called
    inline Shared_Attribute_Callback()
      : Shared_Attribute_Callback(nullptr) {  }

    /// @brief Constructs callback, will be called upon shared attribute update arrival,
    /// of any existing or new shared attribute on the given device
    /// @param cb Callback method that will be called
    inline Shared_Attribute_Callback(processFn cb)
      : m_attributes(), m_cb(cb) {  }

#if THINGSBOARD_ENABLE_STL

    /// @brief Constructs callback, will be called upon shared attribute update arrival,
    /// where atleast one of the given multiple shared attributes passed was updated by the cloud.
    /// If the update does not include any of the given shared attributes the callback is not called
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array
    /// @param first_itr Iterator pointing to the first element in the data container
    /// @param last_itr Iterator pointing to the end of the data container (last element + 1)
    /// @param cb Callback method that will be called
    template<class InputIterator>
    inline Shared_Attribute_Callback(const InputIterator& first_itr, const InputIterator& last_itr, processFn cb)
      : m_attributes(first_itr, last_itr), m_cb(cb) {  }

#else

    /// @brief Constructs callback, will be called upon upon shared attribute update arrival,
    /// where atleast one of the given multiple shared attributes passed was updated by the cloud.
    /// If the update does not include any of the given shared attributes the callback is not called
    /// @param attributes Comma seperated string containing all attributes we want to subscribe (test1, test2, ...)
    /// @param cb Callback method that will be called
    inline Shared_Attribute_Callback(const char* attributes, processFn cb)
      : m_attributes(attributes), m_cb(cb) {  }

#endif // THINGSBOARD_ENABLE_STL

    /// @brief Calls the callback that was subscribed, when this class instance was initally created
    /// @tparam Logger Logging class that should be used to print messages
    /// @param data Received shared attribute update data that include
    /// the shared attributes that were updated and their new values
    template<typename Logger>
    inline returnType Call_Callback(argumentType data) const {
      // Check if the callback is a nullptr,
      // meaning it has not been assigned any valid callback method.
      if (!m_cb) {
        Logger::log(ATT_CB_IS_NULL);
        return returnType();
      }
      return m_cb(data);
    }
#if THINGSBOARD_ENABLE_STL

    /// @brief Gets all the subscribed shared attributes that will result,
    /// in the subscribed method being called if changed by the cloud
    /// passed when this class instance was initally created
    /// @return Subscribed shared attributes
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
    const std::vector<const char *>      m_attributes;      // Attribute we want to request
#else
    const char                           *m_attributes;     // Attribute we want to request
#endif // THINGSBOARD_ENABLE_STL
    processFn                            m_cb;              // Callback to call
};

#endif // Shared_Attribute_Callback
