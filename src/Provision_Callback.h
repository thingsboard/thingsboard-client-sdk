/*
  Provision_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Provision_Callback_h
#define Provision_Callback_h

#if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)

// Library includes.
#include <ArduinoJson.h>
#include <functional>

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
constexpr char PROVISION_CB_IS_NULL[] PROGMEM = "Provisioning callback is NULL";

// Convenient aliases
using Provision_Data = const JsonObjectConst;

/// @brief Provisioning callback wrapper
class Provision_Callback {
  public:
    /// @brief Provisioning callback signature
    using returnType = void;
    using argumentType = const Provision_Data&;
    using processFn = std::function<returnType(argumentType data)>;

    /// @brief Constructs empty callback, will result in never being called
    inline Provision_Callback()
      : Provision_Callback(nullptr) {  }

    /// @brief Constructs callback that will be fired upon a provision request arrival
    /// where the requested credentials were sent by the cloud and received by the client
    /// @param cb Callback method that will be called
    inline Provision_Callback(processFn cb)
      : m_cb(cb) {  }

    /// @brief Calls the callback that was subscribed, when this class instance was initally created
    /// @tparam Logger Logging class that should be used to print messages
    /// @param data Received shared attribute request data that include
    /// the credentials that was requested
    template<typename Logger>
    inline returnType Call_Callback(argumentType data) const {
      // Check if the callback is a nullptr,
      // meaning it has not been assigned any valid callback method.
      if (!m_cb) {
        Logger::log(PROVISION_CB_IS_NULL);
        return returnType();
      }
      return m_cb(data);
    }

  private:
    processFn   m_cb;       // Callback to call
};
#endif // defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA)

#endif // Provision_Callback_h
