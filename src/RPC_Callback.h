#ifndef RPC_Callback_h
#define RPC_Callback_h

// Local includes.
#include "Callback.h"
#include "Constants.h"


#if THINGSBOARD_ENABLE_PROGMEM
char constexpr RPC_CB_NULL[] PROGMEM = "Server-side RPC callback is NULL";
#else
char constexpr RPC_CB_NULL[] = "Server-side RPC callback is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM


/// @brief Server-side RPC callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// Documentation about the specific use of Server-side RPC in ThingsBoard can be found here https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc
#if !THINGSBOARD_ENABLE_DYNAMIC
/// @tparam MaxRPC Maximum amount of key-value pairs that will ever be sent with this instance of the class, allows to use a StaticJsonDocument on the stack in the background.
/// Be aware though the size set in this template and the size passed to the ThingsBoard MaxRPC template need to be the same or the value in this class lower, if not some of the requested keys may be lost, default = Default_RPC_Amount (0)
template <size_t MaxRPC = Default_RPC_Amount>
#endif // !THINGSBOARD_ENABLE_DYNAMIC
class RPC_Callback : public Callback<void, JsonVariantConst const &, JsonDocument &> {
  public:
    /// @brief Constructs empty callback, will result in never being called. Internals are simply default constructed as nullptr
    RPC_Callback() = default;

    /// @brief Constructs callback, will be called upon server-side RPC request arrival with the given methodName
    /// @param methodName Name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @param cb Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    /// and should enter data into the JsonDocument, can be empty if the RPC widget does not expect any response.
    /// See https://arduinojson.org/v6/api/jsondocument/ for more information on how to enter data into a JsonDocument
#if THINGSBOARD_ENABLE_DYNAMIC
    /// @param responseSize Maximum amount of internal size the JsonDocument should be able to hold to contain the response to the server side RPC call.
    /// Use JSON_OBJECT_SIZE() and pass the amount of key value pair to calculate the estimated size. See https://arduinojson.org/v6/assistant/ for more information on how to estimate the required size, default = Default_RPC_Amount (0)
    RPC_Callback(char const * const methodName, function cb, size_t const & responseSize = Default_RPC_Amount)
#else
    RPC_Callback(char const * const methodName, function cb)
#endif //THINGSBOARD_ENABLE_DYNAMIC
      : Callback(cb, RPC_CB_NULL)
#if THINGSBOARD_ENABLE_DYNAMIC
      , m_document(responseSize)
#else
      , m_document()
#endif //THINGSBOARD_ENABLE_DYNAMIC
      , m_methodName(methodName)
    {
        // Nothing to do
    }

    /// @brief Gets the poiner to the underlying name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @return Pointer to the passed methodName
    char const * Get_Name() const {
        return m_methodName;
    }

    /// @brief Sets the poiner to the underlying name we expect to be sent via. server-side RPC so that this method callback will be called
    /// @param methodName Pointer to the passed methodName
    void Set_Name(char const * const methodName) {
        m_methodName = methodName;
    }

    /// @brief Gets the reference to the underlying document, which will be sent as a response to the server side RPC, if it is not empty
    /// @return Reference to the underlying document
    JsonDocument & Get_Json_Document() {
        return m_document;
    }

  private:
#if THINGSBOARD_ENABLE_DYNAMIC
    DynamicJsonDocument m_document;                          // Dynamic json document, containing possible response
#else
    StaticJsonDocument<JSON_OBJECT_SIZE(MaxRPC)> m_document; // Static json document, containing possible response
#endif // THINGSBOARD_ENABLE_DYNAMIC
    char const *m_methodName;                                // Method name
};

#endif // RPC_Callback_h
