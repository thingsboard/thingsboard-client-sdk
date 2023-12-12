#ifndef RPC_Callback_h
#define RPC_Callback_h

// Local includes.
#include "Callback.h"
#include "RPC_Response.h"


/// @brief Server-side RPC callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// Documentation about the specific use of Server-side RPC in ThingsBoard can be found here https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc
class RPC_Callback : public Callback<RPC_Response, JsonVariantConst const &> {
  public:
    /// @brief Constructs empty callback, will result in never being called
    RPC_Callback();

    /// @brief Constructs callback, will be called upon server-side RPC request arrival with the given methodName
    /// @param methodName Name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @param cb Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    /// and should return a RPC_Response, the RPC_Response can be empty if the RPC widget does not expect any response
    RPC_Callback(char const * const methodName, function cb);

    /// @brief Gets the poiner to the underlying name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @return Pointer to the passed methodName
    char const * Get_Name() const;

    /// @brief Sets the poiner to the underlying name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @param methodName Pointer to the passed methodName
    void Set_Name(char const * const methodName);

  private:
    char const *m_methodName;  // Method name
};

#endif // RPC_Callback_h
