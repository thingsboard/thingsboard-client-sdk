/*
  RPC_Request_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef RPC_Request_Callback_h
#define RPC_Request_Callback_h

// Library includes.
#include <Telemetry.h>
#include "Configuration.h"
#if THINGSBOARD_ENABLE_STL
#include <functional>
#endif // THINGSBOARD_ENABLE_STL

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
constexpr char RPC_REQUEST_CB_NULL[] PROGMEM = "Client-side RPC request callback is NULL";

/// @brief RPC request callback wrapper
class RPC_Request_Callback {
  public:
    /// @brief RPC request callback signatures
    using returnType = void;
    using argumentType = const JsonVariantConst&;
#if THINGSBOARD_ENABLE_STL
    using processFn = std::function<returnType(argumentType data)>;
#else
    using processFn = returnType (*)(argumentType data);
#endif // THINGSBOARD_ENABLE_STL

    /// @brief Constructs empty callback, will result in never being called
    inline RPC_Request_Callback()
      : RPC_Request_Callback(nullptr, nullptr, nullptr) {  }

    /// @brief Constructs callback, will be called upon RPC response arrival originating
    /// from the original client side RPC request without any parameters
    /// @param methodName Name of the client side RPC we want to call on the cloud
    /// @param cb Callback method that will be called
    inline RPC_Request_Callback(const char *methodName, processFn cb)
      : RPC_Request_Callback(methodName, nullptr, cb) {  }

    /// @brief Constructs callback, will be called upon RPC response arrival originating
    /// from the original client side RPC request with additional parameters that should be passed to the method
    /// @param methodName Name of the client side RPC we want to call on the cloud
    /// @param parameteres Parameters that will be passed to the client side RPC,
    /// Optional, pass NULL if there are no argument for the given method
    /// @param cb Callback method that will be called
    inline RPC_Request_Callback(const char *methodName, const JsonArray *parameteres, processFn cb)
      : m_methodName(methodName), m_parameters(parameteres), m_request_id(0U), m_cb(cb) {  }

    /// @brief Calls the callback that was subscribed, when this class instance was initally created
    /// @tparam Logger Logging class that should be used to print messages
    /// @param data Received RPC response that include optional parameters for the method called on the cloud
    template<typename Logger>
    inline returnType Call_Callback(argumentType data) const {
      // Check if the callback is a nullptr,
      // meaning it has not been assigned any valid callback method.
      if (!m_cb) {
        Logger::log(RPC_REQUEST_CB_NULL);
        return returnType();
      }
      return m_cb(data);
    }

    /// @brief Gets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which RPC_Request_Callback
    /// is connected to which received RPC response
    /// @return Unique identifier connected to the request for client side rpc
    inline const uint32_t& Get_Request_ID() const {
      return m_request_id;
    }

    /// @brief Sets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which RPC_Request_Callback
    /// is connected to which received RPC response
    /// @param request_id Unique identifier connected to the request for client side rpc
    inline void Set_Request_ID(const uint32_t& request_id) {
      m_request_id = request_id;
    }

    /// @brief Gets the poiner to the underlying name that was passed,
    /// when this class instance was initally created
    /// @return Pointer to the passed methodName
    inline const char* Get_Name() const {
      return m_methodName;
    }

    /// @brief Gets the pointer to the underlying paramaters that were passed,
    /// when this class instance was initally created
    /// @return Pointer to the passed parameters
    inline const JsonArray* Get_Parameters() const {
      return m_parameters;
    }

  private:
    const char        *m_methodName;  // Method name
    const JsonArray   *m_parameters;  // Parameter json
    uint32_t          m_request_id;   // Id the request was called with
    processFn         m_cb;           // Callback to call
};

#endif // RPC_Request_Callback_h
