#ifndef Shared_Attribute_Update_h
#define Shared_Attribute_Update_h

// Local include.
#include "Shared_Attribute_Callback.h"
#include "IAPI_Implementation.h"


// Shared attribute update API topics.
char constexpr ATTRIBUTE_TOPIC[] = "v1/devices/me/attributes";


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
class Server_Side_RPC : public IAPI_Implementation {
  public:
    /// @brief Constructor
    Server_Side_RPC() = default;

    /// @brief Subscribes multiple shared attribute callbacks,
    /// that will be called if the key-value pair from the server for the given shared attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server for more information
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param first Iterator pointing to the first element in the data container
    /// @param last Iterator pointing to the end of the data container (last element + 1)
    /// @return Whether subscribing the given callbacks was successful or not
    template<typename InputIterator>
    bool Shared_Attributes_Subscribe(InputIterator const & first, InputIterator const & last) {
#if !THINGSBOARD_ENABLE_DYNAMIC
        size_t const size = Helper::distance(first, last);
        if (m_shared_attribute_update_callbacks.size() + size > m_shared_attribute_update_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, SHARED_ATTRIBUTE_UPDATE_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_subscribe_callback.Call_Callback(ATTRIBUTE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, ATTRIBUTE_TOPIC);
            return false;
        }

        // Push back complete vector into our local m_shared_attribute_update_callbacks vector.
        m_shared_attribute_update_callbacks.insert(m_shared_attribute_update_callbacks.end(), first, last);
        return true;
    }

    /// @brief Subscribe one shared attribute callback,
    /// that will be called if the key-value pair from the server for the given shared attributes is received.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server for more information
    /// @param callback Callback method that will be called
    /// @return Whether subscribing the given callback was successful or not
#if THINGSBOARD_ENABLE_DYNAMIC
    bool Shared_Attributes_Subscribe(Shared_Attribute_Callback const & callback) {
#else
    bool Shared_Attributes_Subscribe(Shared_Attribute_Callback<MaxAttributes> const & callback) {
#endif // THINGSBOARD_ENABLE_DYNAMIC
#if !THINGSBOARD_ENABLE_DYNAMIC
        if (m_shared_attribute_update_callbacks.size() + 1U > m_shared_attribute_update_callbacks.capacity()) {
            Logger::printfln(MAX_SUBSCRIPTIONS_EXCEEDED, SHARED_ATTRIBUTE_UPDATE_SUBSCRIPTIONS);
            return false;
        }
#endif // !THINGSBOARD_ENABLE_DYNAMIC
        if (!m_subscribe_callback.Call_Callback(ATTRIBUTE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, ATTRIBUTE_TOPIC);
            return false;
        }

        // Push back given callback into our local vector
        m_shared_attribute_update_callbacks.push_back(callback);
        return true;
    }

    /// @brief Unsubcribes all shared attribute callbacks.
    /// See https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server for more information
    /// @return Whether unsubcribing all the previously subscribed callbacks
    /// and from the attribute topic, was successful or not
    bool Shared_Attributes_Unsubscribe() {
        m_shared_attribute_update_callbacks.clear();
        return m_unsubscribe_callback.Call_Callback(ATTRIBUTE_TOPIC);
    }

    const char * const get_response_topic_string() const override {
        return ATTRIBUTE_TOPIC;
    }

    void process_response(char * const topic, JsonObjectConst const & data) const override {
        if (!data) {
#if THINGSBOARD_ENABLE_DEBUG
            Logger::println(NOT_FOUND_ATT_UPDATE);
#endif // THINGSBOARD_ENABLE_DEBUG
            return;
        }

        if (data.containsKey(SHARED_RESPONSE_KEY)) {
            data = data[SHARED_RESPONSE_KEY];
        }

        for (auto const & shared_attribute : m_shared_attribute_update_callbacks) {
            if (shared_attribute.Get_Attributes().empty()) {
#if THINGSBOARD_ENABLE_DEBUG
                Logger::println(ATT_CB_NO_KEYS);
#endif // THINGSBOARD_ENABLE_DEBUG
                // No specifc keys were subscribed so we call the callback anyway, assumed to be subscribed to any update
                shared_attribute.Call_Callback(data);
                continue;
            }

            char const * requested_att = nullptr;

            for (auto const & att : shared_attribute.Get_Attributes()) {
                if (Helper::stringIsNullorEmpty(att)) {
#if THINGSBOARD_ENABLE_DEBUG
                    Logger::println(ATT_IS_NULL);
#endif // THINGSBOARD_ENABLE_DEBUG
                    continue;
                }
                // Check if the request contained any of our requested keys and
                // break early if the key was requested from this callback.
                if (data.containsKey(att)) {
                    requested_att = att;
                    break;
                }
            }

            // Check if this callback did not request any keys that were in this response,
            // if there were not we simply continue with the next subscribed callback.
            if (requested_att == nullptr) {
#if THINGSBOARD_ENABLE_DEBUG
                Logger::println(ATT_NO_CHANGE);
#endif // THINGSBOARD_ENABLE_DEBUG
                continue;
            }

#if THINGSBOARD_ENABLE_DEBUG
            Logger::printfln(CALLING_ATT_CB, requested_att);
#endif // THINGSBOARD_ENABLE_DEBUG
            shared_attribute.Call_Callback(data);
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
    Vector<Shared_Attribute_Callback>                                 m_shared_attribute_update_callbacks; // Shared attribute update callbacks vector
#else
    Array<Shared_Attribute_Callback<MaxAttributes>, MaxSubscribtions> m_shared_attribute_update_callbacks; // Shared attribute update callbacks array
#endif // THINGSBOARD_ENABLE_DYNAMIC
};

#endif // Shared_Attribute_Update_h
