/*
  RPC_Request_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef RPC_Request_Callback_h
#define RPC_Request_Callback_h

// Local includes.
#include "Callback.h"

// Library includes.
#include <ArduinoJson.h>

/// @brief RPC request callback wrapper
class RPC_Request_Callback : public Callback<void, const JsonVariantConst&> {
  public:
    /// @brief Constructs empty callback, will result in never being called
    RPC_Request_Callback();

    /// @brief Constructs callback, will be called upon RPC response arrival originating
    /// from the original client side RPC request without any parameters
    /// @param methodName Name of the client side RPC we want to call on the cloud
    /// @param cb Callback method that will be called
    RPC_Request_Callback(const char *methodName, function cb);

    /// @brief Constructs callback, will be called upon RPC response arrival originating
    /// from the original client side RPC request with additional parameters that should be passed to the method
    /// @param methodName Name of the client side RPC we want to call on the cloud
    /// @param parameteres Parameters that will be passed to the client side RPC,
    /// Optional, pass NULL if there are no argument for the given method
    /// @param cb Callback method that will be called
    RPC_Request_Callback(const char *methodName, const JsonArray *parameteres, function cb);

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
    void Set_Parameters(const JsonArray *parameteres);

  private:
    const char        *m_methodName;  // Method name
    const JsonArray   *m_parameters;  // Parameter json
    uint32_t          m_request_id;   // Id the request was called with
};

#endif // RPC_Request_Callback_h
