/*
  RPC_Request_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef RPC_Request_Callback_h
#define RPC_Request_Callback_h

// Local includes.
#include "Configuration.h"

// Library includes.
#include <ArduinoJson.h>
#if THINGSBOARD_ENABLE_STL
#include <functional>
#endif // THINGSBOARD_ENABLE_STL

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char RPC_REQUEST_CB_NULL[] PROGMEM = "Client-side RPC request callback is NULL";
#else
constexpr char RPC_REQUEST_CB_NULL[] = "Client-side RPC request callback is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM

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
    RPC_Request_Callback();

    /// @brief Constructs callback, will be called upon RPC response arrival originating
    /// from the original client side RPC request without any parameters
    /// @param methodName Name of the client side RPC we want to call on the cloud
    /// @param cb Callback method that will be called
    RPC_Request_Callback(const char *methodName, processFn cb);

    /// @brief Constructs callback, will be called upon RPC response arrival originating
    /// from the original client side RPC request with additional parameters that should be passed to the method
    /// @param methodName Name of the client side RPC we want to call on the cloud
    /// @param parameteres Parameters that will be passed to the client side RPC,
    /// Optional, pass NULL if there are no argument for the given method
    /// @param cb Callback method that will be called
    RPC_Request_Callback(const char *methodName, const JsonArray *parameteres, processFn cb);

    /// @brief Calls the callback that was subscribed
    /// @tparam Logger Logging class that should be used to print messages
    /// @param data Received RPC response that include optional parameters for the method called on the cloud
    template<typename Logger>
    inline returnType Call_Callback(argumentType data) const {
        // Check if the callback is a nullptr,
        // meaning it has not been assigned any valid callback method
        if (!m_cb) {
          Logger::log(RPC_REQUEST_CB_NULL);
          return returnType();
        }
        return m_cb(data);
    }

    /// @brief Sets the callback method that will be called
    /// @param cb Callback method that will be called
    void Set_Callback(processFn cb);

    /// @brief Gets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which RPC_Request_Callback
    /// is connected to which received RPC response
    /// @return Unique identifier connected to the request for client side rpc
    const uint32_t& Get_Request_ID() const;

    /// @brief Sets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which RPC_Request_Callback
    /// is connected to which received RPC response
    /// @param request_id Unique identifier connected to the request for client side rpc
    void Set_Request_ID(const uint32_t &request_id);

    /// @brief Gets the poiner to the underlying name
    /// @return Pointer to the passed methodName
    const char* Get_Name() const;

    /// @brief Sets the poiner to the underlying name
    /// @param methodName Pointer to the passed methodName
    void Set_Name(const char *methodName);

    /// @brief Gets the pointer to the underlying paramaters
    /// @return Pointer to the passed parameters
    const JsonArray* Get_Parameters() const;

    /// @brief Sets the pointer to the underlying paramaters
    /// @param parameteres Pointer to the passed parameters
    void Get_Parameters(const JsonArray *parameteres);

  private:
    const char        *m_methodName;  // Method name
    const JsonArray   *m_parameters;  // Parameter json
    uint32_t          m_request_id;   // Id the request was called with
    processFn         m_cb;           // Callback to call
};

#endif // RPC_Request_Callback_h
