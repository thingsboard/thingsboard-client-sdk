/*
  Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Callback_h
#define Callback_h

// Local includes.
#include "Configuration.h"

// Library includes.
#if THINGSBOARD_ENABLE_STL
#include <functional>
#endif // THINGSBOARD_ENABLE_STL

/// @brief General purpose callback wrapper
/// @tparam returnType Type the given callback method should return
/// @tparam argumentTypes Types the given callback method should receive
template<typename returnType, typename... argumentTypes>
class Callback {
  public:
    /// @brief Callback signature
#if THINGSBOARD_ENABLE_STL
    using function = std::function<returnType(argumentTypes... arguments)>;
#else
    using function = returnType (*)(argumentTypes... arguments);
#endif // THINGSBOARD_ENABLE_STL

    /// @brief Constructs base callback, will be called upon specific arrival of json message
    /// where the requested data was sent by the cloud and received by the client
    /// @param cb Callback method that will be called
    /// @param message Message that is logged if the callback given initally is a nullptr and can therefore not be called
    inline Callback(function cb, const char *message)
      : m_cb(cb)
      , m_message(message)
    {
        // Nothing to do
    }

    /// @brief Calls the callback that was subscribed, when this class instance was initally created
    /// @tparam Logger Logging class that should be used to print messages
    /// @param arguments Received client-side or shared attribute request data that include
    /// the client-side or shared attributes that were requested and their current values
    template<typename Logger>
    inline returnType Call_Callback(argumentTypes... arguments) const {
        // Check if the callback is a nullptr,
        // meaning it has not been assigned any valid callback method
        if (!m_cb) {
          Logger::log(m_message);
          return returnType();
        }
        return m_cb(arguments...);
    }

    /// @brief Sets the callback method that will be called
    /// @param cb Callback method that will be called
    inline void Set_Callback(function cb) {
        m_cb = cb;
    }

  private:
    function   m_cb;       // Callback to call
    const char *m_message; // Message to print if callback is a nullptr
};

#endif // Callback_h
