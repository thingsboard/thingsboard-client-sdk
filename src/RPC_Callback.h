/*
  RPC_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef RPC_Callback_h
#define RPC_Callback_h

// Local includes.
#include "Callback.h"
#include "RPC_Response.h"

// JSON variant const (read only twice as small as JSON variant), is used to communicate RPC parameters to the client
using RPC_Data = const JsonVariantConst;

/// @brief RPC callback wrapper
class RPC_Callback : public Callback<RPC_Response, RPC_Data&> {
  public:
    /// @brief Constructs empty callback, will result in never being called
    RPC_Callback();

    /// @brief Constructs callback, will be called upon RPC request arrival with the given methodName
    /// @param methodName Name we expect to be sent via. RPC so this callback will be called
    /// @param cb Callback method that will be called and should return a response if excpected
    RPC_Callback(const char *methodName, function cb);

    /// @brief Gets the poiner to the underlying name
    /// @return Pointer to the passed methodName
    const char* Get_Name() const;

    /// @brief Sets the poiner to the underlying name
    /// @param methodName Pointer to the passed methodName
    void Set_Name(const char *methodName);

  private:
    const char  *m_methodName;  // Method name
};

#endif // RPC_Callback_h
