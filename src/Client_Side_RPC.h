#ifndef Client_Side_RPC_h
#define Client_Side_RPC_h

// Local includes.
#include "RPC_Request_Callback.h"
#include "API_Implementation.h"


// Client side RPC topics.
char constexpr RPC_RESPONSE_SUBSCRIBE_TOPIC[] = "v1/devices/me/rpc/response/+";
char constexpr RPC_RESPONSE_TOPIC[] = "v1/devices/me/rpc/response";
char constexpr RPC_SEND_REQUEST_TOPIC[] = "v1/devices/me/rpc/request/%u";
// Log messages.
char constexpr CLIENT_RPC_METHOD_NULL[] = "Client-side RPC methodName is NULL";
#if THINGSBOARD_ENABLE_DYNAMIC
char constexpr CLIENT_SIDE_RPC_SUBSCRIPTIONS[] = "client-side RPC";
#endif // THINGSBOARD_ENABLE_DYNAMIC
char constexpr RPC_EMPTY_PARAMS_VALUE[] = "{}";


/// @brief Handles the internal implementation of the ThingsBoard client side RPC API.
/// See https://thingsboard.io/docs/user-guide/rpc/#client-side-rpc for more information
#if !THINGSBOARD_ENABLE_DYNAMIC
/// @tparam MaxSubscribtions Maximum amount of simultaneous client side rpc requests.
/// Once the maximum amount has been reached it is not possible to increase the size, this is done because it allows to allcoate the memory on the stack instead of the heap, default = Default_Subscriptions_Amount (2)
template<size_t MaxSubscribtions = Default_Subscriptions_Amount>
#endif // !THINGSBOARD_ENABLE_DYNAMIC
class Client_Side_RPC : public API_Implementation {
  public:
    /// @brief Constructor
    Client_Side_RPC() = default;

    /// @brief Requests one client-side RPC callback,
    /// that will be called if a response from the server for the method with the given name is received.
    /// See https://thingsboard.io/docs/user-guide/rpc/#client-side-rpc for more information
    /// @param callback Callback method that will be called
    /// @return Whether requesting the given callback was successful or not
    bool RPC_Request(RPC_Request_Callback const & callback) {
        char const * methodName = callback.Get_Name();

        if (Helper::stringIsNullorEmpty(methodName)) {
            Logger::println(CLIENT_RPC_METHOD_NULL);
            return false;
        }
        RPC_Request_Callback * registeredCallback = nullptr;
        if (!RPC_Request_Subscribe(callback, registeredCallback)) {
            return false;
        }
        else if (registeredCallback == nullptr) {
            return false;
        }

        JsonArray const * const parameters = callback.Get_Parameters();

#if THINGSBOARD_ENABLE_DYNAMIC
        // String are const char* and therefore stored as a pointer --> zero copy, meaning the size for the strings is 0 bytes,
        // Data structure size depends on the amount of key value pairs passed + the default methodName and params key needed for the request.
        // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
        TBJsonDocument requestBuffer(JSON_OBJECT_SIZE(parameters != nullptr ? parameters->size() + 2U : 2U));
#else
        // Ensure to have enough size for the infinite amount of possible parameters that could be sent to the cloud,
        // therefore we set the size to the MaxFieldsAmount instead of JSON_OBJECT_SIZE(1), which will result in a JsonDocument with a size of 16 bytes
        StaticJsonDocument<JSON_OBJECT_SIZE(MaxFieldsAmount)> requestBuffer;
#endif // THINGSBOARD_ENABLE_DYNAMIC

        requestBuffer[RPC_METHOD_KEY] = methodName;

        if (parameters != nullptr && !parameters->isNull()) {
            requestBuffer[RPC_PARAMS_KEY] = *parameters;
        }
        else {
            requestBuffer[RPC_PARAMS_KEY] = RPC_EMPTY_PARAMS_VALUE;
        }

        m_request_id++;
        registeredCallback->Set_Request_ID(m_request_id);
        registeredCallback->Start_Timeout_Timer();

        char topic[Helper::detectSize(RPC_SEND_REQUEST_TOPIC, m_request_id)] = {};
        (void)snprintf(topic, sizeof(topic), RPC_SEND_REQUEST_TOPIC, m_request_id);

        size_t const objectSize = Helper::Measure_Json(requestBuffer);
        return m_send_callback.Call_Callback(topic, requestBuffer, objectSize);
    }

    /// @brief Subscribes to the client-side RPC response topic,
    /// that will be called if a reponse from the server for the method with the given name is received.
    /// See https://thingsboard.io/docs/user-guide/rpc/#client-side-rpc for more information
    /// @param callback Callback method that will be called
    /// @param registeredCallback Editable pointer to a reference of the local version that was copied from the passed callback
    /// @return Whether requesting the given callback was successful or not
    bool RPC_Request_Subscribe(RPC_Request_Callback const & callback, RPC_Request_Callback * & registeredCallback) {
#if !THINGSBOARD_ENABLE_DYNAMIC
        if (m_rpc_request_callbacks.size() + 1 > m_rpc_request_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, CLIENT_SIDE_RPC_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_subscribe_callback.Call_Callback(RPC_RESPONSE_SUBSCRIBE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, RPC_RESPONSE_SUBSCRIBE_TOPIC);
            return false;
        }

        // Push back given callback into our local vector
        m_rpc_request_callbacks.push_back(callback);
        registeredCallback = &m_rpc_request_callbacks.back();
        return true;
    }

    /// @brief Unsubscribes all client-side RPC request callbacks
    /// @return Whether unsubcribing the previously subscribed callbacks
    /// and from the client-side RPC response topic, was successful or not
    bool RPC_Request_Unsubscribe() {
        m_rpc_request_callbacks.clear();
        return m_unsubscribe_callback.Call_Callback(RPC_RESPONSE_SUBSCRIBE_TOPIC);
    }

    const char * const Get_Response_Topic_String() const override {
        return RPC_RESPONSE_TOPIC;
    }

    bool Unsubscribe_Topic() override {
        return RPC_Request_Unsubscribe();
    }

#if !THINGSBOARD_USE_ESP_TIMER
    void loop() override {
        for (auto & rpc_request : m_rpc_request_callbacks) {
            rpc_request.Update_Timeout_Timer();
        }
    }
#endif // !THINGSBOARD_USE_ESP_TIMER

    void Process_Json_Response(char * const topic, JsonObjectConst const & data) const override {
        size_t const request_id = Helper::parseRequestId(RPC_RESPONSE_TOPIC, topic);

        for (auto it = m_rpc_request_callbacks.begin(); it != m_rpc_request_callbacks.end(); ++it) {
            auto & rpc_request = *it;

            if (rpc_request.Get_Request_ID() != request_id) {
                continue;
            }
            rpc_request.Stop_Timeout_Timer();
            rpc_request.Call_Callback(data);

            // Delete callback because the changes have been requested and the callback is no longer needed
            Helper::remove(m_rpc_request_callbacks, it);
            break;
        }

        // Attempt to unsubscribe from the shared attribute request topic,
        // if we are not waiting for any further responses with shared attributes from the server.
        // Will be resubscribed if another request is sent anyway
        if (m_rpc_request_callbacks.empty()) {
            (void)RPC_Request_Unsubscribe();
        }
    }

    // Vectors or array (depends on wheter if THINGSBOARD_ENABLE_DYNAMIC is set to 1 or 0), hold copy of the actual passed data, this is to ensure they stay valid,
    // even if the user only temporarily created the object before the method was called.
    // This can be done because all Callback methods mostly consists of pointers to actual object so copying them
    // does not require a huge memory overhead and is acceptable especially in comparsion to possible problems that could
    // arise if references were used and the end user does not take care to ensure the Callbacks live on for the entirety
    // of its usage, which will lead to dangling references and undefined behaviour.
    // Therefore copy-by-value has been choosen as for this specific use case it is more advantageous,
    // especially because at most we copy internal vectors or array, that will only ever contain a few pointers
#if THINGSBOARD_ENABLE_DYNAMIC
    Vector<RPC_Callback>                  m_rpc_request_callbacks; // Server side RPC callbacks vector
#else
    Array<RPC_Callback, MaxSubscribtions> m_rpc_request_callbacks; // Server side RPC callbacks array
#endif // THINGSBOARD_ENABLE_DYNAMIC
    static size_t                         m_request_id;            // Allows nearly 4.3 million requests before wrapping back to 0, static so we actually keep track of the current request id even if we use multiple instances
};

#endif // Client_Side_RPC_h
