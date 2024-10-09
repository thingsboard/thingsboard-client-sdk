#ifndef RPC_Callback_h
#define RPC_Callback_h

// Local includes.
#include "Callback.h"
#include "Constants.h"


/// @brief Server-side RPC callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// Documentation about the specific use of Server-side RPC in ThingsBoard can be found here https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc
class RPC_Callback : public Callback<void, JsonVariantConst const &, JsonDocument &> {
  public:
    /// @brief Constructs empty callback, will result in never being called. Internals are simply default constructed as nullptr
    RPC_Callback() = default;

    /// @brief Constructs callback, will be called upon server-side RPC request arrival with the given method name
    /// @param method_name Name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @param callback Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    /// and should enter data into the JsonDocument, can be empty if the RPC widget does not expect any response.
    /// See https://arduinojson.org/v6/api/jsondocument/ for more information on how to enter data into a JsonDocument
#if THINGSBOARD_ENABLE_DYNAMIC
    /// @param response_size Internal size the JsonDocument should be able to hold to contain the response to the server side RPC call.
    /// Use JSON_OBJECT_SIZE() and pass the amount of key value pair to calculate the estimated size. See https://arduinojson.org/v6/assistant/ for more information on how to estimate the required size, default = Default_RPC_Amount (0)
    RPC_Callback(char const * method_name, function callback, size_t const & response_size = JSON_OBJECT_SIZE(Default_RPC_Amount))
#else
    RPC_Callback(char const * method_name, function callback)
#endif // THINGSBOARD_ENABLE_DYNAMIC
      : Callback(callback)
      , m_method_name(method_name)
#if THINGSBOARD_ENABLE_DYNAMIC
      , m_response_size(response_size)
#endif // THINGSBOARD_ENABLE_DYNAMIC
    {
        // Nothing to do
    }

    /// @brief Gets the poiner to the underlying name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @return Pointer to the passed method name
    char const * Get_Name() const {
        return m_method_name;
    }

    /// @brief Sets the poiner to the underlying name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @param method_name Pointer to the passed method name
    void Set_Name(char const * method_name) {
        m_method_name = method_name;
    }

#if THINGSBOARD_ENABLE_DYNAMIC
    /// @brief Gets the internal size the JsonDocument needs to have to contain the response to the server side RPC call.
    /// @return Internal JsonDocument size
    size_t const & Get_Response_Size() const {
        return m_response_size;
    }

    /// @brief Sets the internal size the JsonDocument needs to have to contain the response to the server side RPC call.
    /// Use JSON_OBJECT_SIZE() and pass the amount of key value pair to calculate the estimated size. See https://arduinojson.org/v6/assistant/ for more information on how to estimate the required size
    /// @param response_size Internal JsonDocument size
    void Set_Response_Size(size_t const & response_size) {
        m_response_size = response_size;
    }
#endif // THINGSBOARD_ENABLE_DYNAMIC

  private:
    char const *m_method_name = {};  // Method name
#if THINGSBOARD_ENABLE_DYNAMIC
    size_t     m_response_size = {}; // Required size to contain the response
#endif // THINGSBOARD_ENABLE_DYNAMIC
};

#endif // RPC_Callback_h
