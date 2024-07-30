#ifndef Server_Side_RPC_h
#define Server_Side_RPC_h

// Local includes.
#include "RPC_Callback.h"
#include "API_Implementation.h"


// Server side RPC topics.
char constexpr RPC_SUBSCRIBE_TOPIC[] = "v1/devices/me/rpc/request/+";
char constexpr RPC_REQUEST_TOPIC[] = "v1/devices/me/rpc/request";
char constexpr RPC_SEND_RESPONSE_TOPIC[] = "v1/devices/me/rpc/response/%u";
// Log messages.
char constexpr SERVER_RPC_METHOD_NULL[] = "Server-side RPC methodName is NULL";
char constexpr RPC_RESPONSE_OVERFLOWED[] = "Server-side RPC response overflowed, increase MaxRPC (%u)";
#if THINGSBOARD_ENABLE_DYNAMIC
char constexpr SERVER_SIDE_RPC_SUBSCRIPTIONS[] = "server-side RPC";
#endif // THINGSBOARD_ENABLE_DYNAMIC
#if THINGSBOARD_ENABLE_DEBUG
char constexpr NO_RPC_PARAMS_PASSED[] = "No parameters passed with RPC, passing null JSON";
char constexpr CALLING_RPC_CB[] = "Calling subscribed callback for rpc with methodname (%s)";
#endif // THINGSBOARD_ENABLE_DEBUG


/// @brief Handles the internal implementation of the ThingsBoard server side RPC API.
/// See https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc for more information
#if !THINGSBOARD_ENABLE_DYNAMIC
/// @tparam MaxSubscribtions Maximum amount of simultaneous server side rpc subscriptions.
/// Once the maximum amount has been reached it is not possible to increase the size, this is done because it allows to allcoate the memory on the stack instead of the heap, default = Default_Subscriptions_Amount (2)
/// @tparam MaxRPC Maximum amount of key-value pairs that will ever be sent in the subscribed callback method of an RPC_Callback, allows to use a StaticJsonDocument on the stack in the background.
/// If we simply use .to<JsonVariant>(); on the received document and use .set() to change the internal value then the size requirements are 0.
/// However if we attempt to send multiple key-value pairs, we have to adjust the size accordingly. See https://arduinojson.org/v6/assistant/ for more information on how to estimate the required size and divide the reulst by 16 to receive the required MaxRPC value, default = Default_RPC_Amount (0)
template<size_t MaxSubscribtions = Default_Subscriptions_Amount, size_t MaxRPC = Default_RPC_Amount>
#endif // !THINGSBOARD_ENABLE_DYNAMIC
class Server_Side_RPC : public API_Implementation {
  public:
    /// @brief Constructor
    Server_Side_RPC() = default;

    /// @brief Subscribes multiple server side RPC callbacks,
    /// that will be called if a request from the server for the method with the given name is received.
    /// See https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc for more information
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param first Iterator pointing to the first element in the data container
    /// @param last Iterator pointing to the end of the data container (last element + 1)
    /// @return Whether subscribing the given callbacks was successful or not
    template<typename InputIterator>
    bool RPC_Subscribe(InputIterator const & first, InputIterator const & last) {
#if !THINGSBOARD_ENABLE_DYNAMIC
        size_t const size = Helper::distance(first, last);
        if (m_rpc_callbacks.size() + size > m_rpc_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, SERVER_SIDE_RPC_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_subscribe_callback.Call_Callback(RPC_SUBSCRIBE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, RPC_SUBSCRIBE_TOPIC);
            return false;
        }

        // Push back complete vector into our local m_rpc_callbacks vector.
        m_rpc_callbacks.insert(m_rpc_callbacks.end(), first, last);
        return true;
    }

    /// @brief Subscribe one server side RPC callback,
    /// that will be called if a request from the server for the method with the given name is received.
    /// See https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc for more information
    /// @param callback Callback method that will be called
    /// @return Whether subscribing the given callback was successful or not
    bool RPC_Subscribe(RPC_Callback const & callback) {
#if !THINGSBOARD_ENABLE_DYNAMIC
        if (m_rpc_callbacks.size() + 1 > m_rpc_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, SERVER_SIDE_RPC_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_subscribe_callback.Call_Callback(RPC_SUBSCRIBE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, RPC_SUBSCRIBE_TOPIC);
            return false;
        }

        // Push back given callback into our local vector
        m_rpc_callbacks.push_back(callback);
        return true;
    }

    /// @brief Unsubcribes all server side RPC callbacks.
    /// See https://thingsboard.io/docs/user-guide/rpc/#server-side-rpc for more information
    /// @return Whether unsubcribing all the previously subscribed callbacks
    /// and from the rpc topic, was successful or not
    bool RPC_Unsubscribe() {
        m_rpc_callbacks.clear();
        return m_unsubscribe_callback.Call_Callback(RPC_SUBSCRIBE_TOPIC);
    }

    const char * const Get_Response_Topic_String() const override {
        return RPC_REQUEST_TOPIC;
    }

    bool Unsubscribe_Topic() override {
        return RPC_Unsubscribe();
    }

    bool Resubscribe_Topic() override {
        if (!m_rpc_callbacks.empty() && !m_client.subscribe(RPC_SUBSCRIBE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, RPC_SUBSCRIBE_TOPIC);
        }
        return true;
    }

    void Process_Response(char * const topic, JsonObjectConst const & data) const override {
        char const * const methodName = data[RPC_METHOD_KEY];

        if (methodName == nullptr) {
            Logger::println(SERVER_RPC_METHOD_NULL);
            return;
        }

        for (auto const & rpc : m_rpc_callbacks) {
            char const * const subscribedMethodName = rpc.Get_Name();
            if (Helper::stringIsNullorEmpty(subscribedMethodName)) {
              Logger::println(SERVER_RPC_METHOD_NULL);
              continue;
            }
            // Strncmp returns the ascii value difference of the ascii characters that are different,
            // meaning 0 is the same string and less and more than 0 is the difference in ascci values between the 2 chararacters.
            else if (strncmp(subscribedMethodName, methodName, strlen(subscribedMethodName)) != 0) {
              continue;
            }

            // Do not inform client, if parameter field is missing for some reason
            if (!data.containsKey(RPC_PARAMS_KEY)) {
#if THINGSBOARD_ENABLE_DEBUG
                Logger::println(NO_RPC_PARAMS_PASSED);
#endif // THINGSBOARD_ENABLE_DEBUG
            }

#if THINGSBOARD_ENABLE_DEBUG
            Logger::printfln(CALLING_RPC_CB, methodName);
#endif // THINGSBOARD_ENABLE_DEBUG

            JsonVariantConst const param = data[RPC_PARAMS_KEY];
#if THINGSBOARD_ENABLE_DYNAMIC
            size_t const & rpc_response_size = rpc.Get_Response_Size();
            // String are char const * and therefore stored as a pointer --> zero copy, meaning the size for the strings is 0 bytes,
            // Data structure size depends on the amount of key value pairs passed.
            // See https://arduinojson.org/v6/assistant/ for more information on the needed size for the JsonDocument
            TBJsonDocument jsonBuffer(rpc_response_size);
#else
            size_t constexpr rpc_response_size = MaxRPC;
            StaticJsonDocument<JSON_OBJECT_SIZE(MaxRPC)> jsonBuffer;
#endif // THINGSBOARD_ENABLE_DYNAMIC
            rpc.Call_Callback(param, jsonBuffer);

            if (jsonBuffer.isNull()) {
                // Message is ignored and not sent at all.
                break;
            }
            else if (jsonBuffer.overflowed()) {
                Logger::printfln(RPC_RESPONSE_OVERFLOWED, rpc_response_size);
                break;
            }

            size_t const request_id = Helper::parseRequestId(RPC_REQUEST_TOPIC, topic);
            char responseTopic[Helper::detectSize(RPC_SEND_RESPONSE_TOPIC, request_id)] = {};
            (void)snprintf(responseTopic, sizeof(responseTopic), RPC_SEND_RESPONSE_TOPIC, request_id);

            size_t const jsonSize = Helper::Measure_Json(jsonBuffer);
            (void)m_send_callback.Call_Callback(responseTopic, jsonBuffer, jsonSize);
            break;
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
    Vector<RPC_Callback>                  m_rpc_callbacks; // Server side RPC callbacks vector
#else
    Array<RPC_Callback, MaxSubscribtions> m_rpc_callbacks; // Server side RPC callbacks array
#endif // THINGSBOARD_ENABLE_DYNAMIC
};

#endif // Server_Side_RPC_h
