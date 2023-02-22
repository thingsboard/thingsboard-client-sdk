/*
  RPC_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef RPC_Callback_h
#define RPC_Callback_h

// Local includes.
#include "Configuration.h"

// Library includes.
#include <Telemetry.h>
#if THINGSBOARD_ENABLE_STL
#include <functional>
#endif // THINGSBOARD_ENABLE_STL

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char RPC_CB_NULL[] PROGMEM = "Server-side RPC callback is NULL";
#else
constexpr char RPC_CB_NULL[] = "Server-side RPC callback is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Convenient aliases
using RPC_Response = Telemetry;
// JSON variant const (read only twice as small as JSON variant), is used to communicate RPC parameters to the client
using RPC_Data = const JsonVariantConst;

/// @brief RPC callback wrapper
class RPC_Callback {
  public:
    /// @brief RPC callback signatures
    using returnType = const RPC_Response;
    using argumentType = const RPC_Data&;
#if THINGSBOARD_ENABLE_STL
    using processFn = std::function<returnType(argumentType data)>;
#else
    using processFn = returnType (*)(argumentType data);
#endif // THINGSBOARD_ENABLE_STL

    /// @brief Constructs empty callback, will result in never being called
    inline RPC_Callback()
      : RPC_Callback(nullptr, nullptr) {  }

    /// @brief Constructs callback, will be called upon RPC request arrival with the given methodName
    /// @param methodName Name we expect to be sent via. RPC so this callback will be called
    /// @param cb Callback method that will be called and should return a response if excpected
    inline RPC_Callback(const char *methodName, processFn cb)
      : m_methodName(methodName), m_cb(cb) {  }

    /// @brief Calls the callback that was subscribed
    /// @tparam Logger Logging class that should be used to print messages
    /// @param data Received RPC Data that include optional parameters for the method sent by the cloud
    /// @return Optional RPC Response that include key-value pair,
    /// that the sending widget can use to display the gotten information (temperature etc.)
    template<typename Logger>
    inline returnType Call_Callback(argumentType data) const {
      // Check if the callback is a nullptr,
      // meaning it has not been assigned any valid callback method.
      if (!m_cb) {
        Logger::log(RPC_CB_NULL);
        return returnType();
      }
      return m_cb(data);
    }

    /// @brief Sets the callback method that will be called
    /// @param cb Callback method that will be called
    inline void Set_Callback(processFn cb) {
      m_cb = cb;
    }

    /// @brief Gets the poiner to the underlying name
    /// @return Pointer to the passed methodName
    inline const char* Get_Name() const {
      return m_methodName;
    }

    /// @brief Sets the poiner to the underlying name
    /// @param methodName Pointer to the passed methodName
    inline void Set_Name(const char *methodName) {
      m_methodName = methodName;
    }

  private:
    const char  *m_methodName;  // Method name
    processFn   m_cb;           // Callback to call
};

#endif // RPC_Callback_h
