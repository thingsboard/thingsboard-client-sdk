#ifndef RPC_Callback_h
#define RPC_Callback_h

// Local includes.
#include "Callback.h"
#include "RPC_Response.h"


// JSON variant const (read only twice as small as JSON variant), is used to communicate server-side RPC parameters to the client
using RPC_Data = const JsonVariantConst;


/// @brief Server-side RPC callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// Documentation about the specific use of Server-side RPC in ThingsBoard can be found here https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc
class RPC_Callback : public Callback<RPC_Response, RPC_Data&> {
  public:
    /// @brief Constructs empty callback, will result in never being called
    RPC_Callback();

    /// @brief Constructs callback, will be called upon server-side RPC request arrival with the given methodName
    /// @param methodName Name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @param cb Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    /// and should return a RPC_Response, the RPC_Response can be empty if the RPC widget does not expect any response
    RPC_Callback(const char *methodName, function cb);

    /// @brief Gets the poiner to the underlying name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @return Pointer to the passed methodName
    const char* Get_Name() const;

    /// @brief Sets the poiner to the underlying name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @param methodName Pointer to the passed methodName
    void Set_Name(const char *methodName);

  private:
    const char  *m_methodName;  // Method name
};

#endif // RPC_Callback_h
