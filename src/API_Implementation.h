#ifndef API_Implementation_h
#define API_Implementation_h

// Local include.
#include "Callback.h"
#include "Constants.h"
#include "DefaultLogger.h"
#include "API_Process_Type.h"


// Log messages.
#if !THINGSBOARD_ENABLE_DYNAMIC
char constexpr MAX_SUBSCRIPTIONS_EXCEEDED[] = "Too many (%s) subscriptions, increase MaxSubscribtions or unsubscribe";
#endif // !THINGSBOARD_ENABLE_DYNAMIC
char constexpr SUBSCRIBE_TOPIC_FAILED[] = "Subscribing the given topic (%s) failed";
// RPC data keys.
char constexpr RPC_METHOD_KEY[] = "method";
char constexpr RPC_PARAMS_KEY[] = "params";
// Shared attribute update API topics.
char constexpr ATTRIBUTE_TOPIC[] = "v1/devices/me/attributes";
// Shared attribute request keys.
char constexpr SHARED_RESPONSE_KEY[] = "shared";


/// @brief Base functionality required by all API implementation
class API_Implementation {
  public:
    /// @brief Constructor
    API_Implementation() = default;

    /// @brief Returns the way the server response should be processed.
    /// Only ever uses one at the time, because the response is either unserialized data which we need to process as such (OTA Firmware Update)
    /// or actually JSON which needs to be serialized (everything else)
    /// @return How the API implementation should be passed the response
    virtual API_Process_Type Get_Process_Type() {
        return API_Process_Type::JSON;
    }

    /// @brief Process callback that will be called upon response arrival
    /// and is responsible for handling the payload before serialization and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
    virtual void Process_Response(char * const topic, uint8_t * payload, unsigned int length) {
        // Nothing to do
    }

    /// @brief Process callback that will be called upon response arrival
    /// and is responsible for handling the alredy serialized payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    virtual void Process_Json_Response(char * const topic, JsonObjectConst & data) {
        // Nothing to do
    }

    /// @brief Returns a non-owning pointer to the respone topic string, that we should have received the actual data on.
    /// Used to check, which API Implementation needs to handle the current response to a previously sent request
    /// @return Response topic null-terminated string
    virtual char const * Get_Response_Topic_String() const {
        return nullptr;
    }

    /// @brief Unsubcribes all callbacks, to clear up any ongoing subscriptions and stop receiving information over the previously subscribed topic
    /// @return Whether unsubcribing all the previously subscribed callbacks
    /// and from the previously subscribed topic, was successful or not
    virtual bool Unsubscribe()  {
        return true;
    }

    /// @brief Forwards the call to let the API clear up any ongoing single-event subscriptions (Provision, Attribute Request, RPC Request)
    /// and simply resubscribes the topic for all permanent subscriptions (RPC, Shared Attribute Update)
    /// @return Whether resubscribing was successfull or not
    virtual bool Resubscribe_Topic() {
        return Unsubscribe();
    }

#if !THINGSBOARD_USE_ESP_TIMER
    /// @brief Internal loop method to update inernal timers for API calls that can timeout.
    /// Only exists on boards that can not use the ESP Timer, because that one uses the FreeRTOS timer in the background instead
    /// and therefore does not require calling a loop method
    virtual void loop() {
        // Nothing to do
    }
#endif // !THINGSBOARD_USE_ESP_TIMER

    /// @brief Sets the underlying callbacks that are required for the different API Implementation to communicate with the cloud.
    /// Directly set by the used ThingsBoard client to its internal methods, therefore calling again and overriding
    /// as a user ist not recommended, unless you know what you are doing
    /// @param subscribe_api_callback Method which allows to subscribe additional API endpoints, points to Subscribe_API_Implementation per default
    /// @param send_telemtry_callback Method which allows to send arbitrary JSON payload, points to sendTelemetryJson per default
    /// @param send_callback Method which allows to send arbitrary JSON payload, points to Send_Json per default
    /// @param subscribe_callback Method which allows to subscribe to arbitrary topics, points to m_client.subscribe per default
    /// @param unsubscribe_callback Method which allows to unsubscribe from arbitrary topics, points to m_client.unsubscribe per default
    /// @param get_size_callback Method which allows to get the current underlying size of the buffer, points to m_client.get_buffer_size per default
    /// @param set_buffer_size_callback Method which allows to set the current underlying size of the buffer, points to m_client.set_buffer_size per default
    void Set_Client_Callbacks(Callback<void, API_Implementation &>::function subscribe_api_callback, Callback<bool, JsonDocument const &, size_t const &>::function send_telemtry_callback, Callback<bool, char const * const, JsonDocument const &, size_t const &>::function send_callback, Callback<bool, char const * const, char const * const>::function send_string_callback, Callback<bool, char const * const>::function subscribe_callback, Callback<bool, char const * const>::function unsubscribe_callback, Callback<uint16_t>::function get_size_callback, Callback<bool, uint16_t>::function set_buffer_size_callback) {
        m_subscribe_api_callback.Set_Callback(subscribe_api_callback);
        m_send_telemtry_callback.Set_Callback(send_telemtry_callback);
        m_send_callback.Set_Callback(send_callback);
        m_send_string_callback.Set_Callback(send_string_callback);
        m_subscribe_callback.Set_Callback(subscribe_callback);
        m_unsubscribe_callback.Set_Callback(unsubscribe_callback);
        m_get_size_callback.Set_Callback(get_size_callback);
        m_set_buffer_size.Set_Callback(set_buffer_size_callback);
    }

  protected:
    Callback<void, API_Implementation &>                                     m_subscribe_api_callback; // Subscribe API callback
    Callback<bool, JsonDocument const &, size_t const &>                     m_send_telemtry_callback; // Send Telemtry callback
    Callback<bool, char const * const, JsonDocument const &, size_t const &> m_send_callback;          // Send JSON callback
    Callback<bool, char const * const, char const * const>                   m_send_string_callback;   // Send JSON string callback
    Callback<bool, char const * const>                                       m_subscribe_callback;     // Subscribe topic callback
    Callback<bool, char const * const>                                       m_unsubscribe_callback;   // Unsubscribe topic callback
    Callback<uint16_t>                                                       m_get_size_callback;      // Get client size callback
    Callback<bool, uint16_t>                                                 m_set_buffer_size;        // Set client buffer size callback
};

#endif // API_Implementation_h
