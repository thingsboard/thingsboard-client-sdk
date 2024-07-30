#ifndef API_Implementation_h
#define API_Implementation_h

// Local include.
#include "Callback.h"


// Log messages.
#if !THINGSBOARD_ENABLE_DYNAMIC
char constexpr MAX_SUBSCRIPTIONS_EXCEEDED[] = "Too many (%s) subscriptions, increase MaxSubscribtions or unsubscribe";
#endif // !THINGSBOARD_ENABLE_DYNAMIC
char constexpr SUBSCRIBE_TOPIC_FAILED[] = "Subscribing the given topic (%s) failed";
// RPC data keys.
char constexpr RPC_METHOD_KEY[] = "method";
char constexpr RPC_PARAMS_KEY[] = "params";
char constexpr RPC_EMPTY_PARAMS_VALUE[] = "{}";


/// @brief Base functionality required by all API implementation
class API_Implementation {
  public:
    /// @brief Constructor
    API_Implementation() = default;

    /// @brief Process callback that will be called upon response arrival
    /// and is responsible for handling the payload before serialization and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param payload Payload that was sent over the cloud and received over the given topic
    /// @param length Total length of the received payload
    virtual void Process_Response(char * const topic, uint8_t * const payload, unsigned int length) const {
        // Nothing to do
    }

    /// @brief Process callback that will be called upon response arrival
    /// and is responsible for handling the alredy serialized payload and calling the appropriate previously subscribed callbacks
    /// @param topic Previously subscribed topic, we got the response over
    /// @param data Payload sent by the server over our given topic, that contains our key value pairs
    virtual void Process_Json_Response(char * const topic, JsonObjectConst const & data) const = 0;

    /// @brief Returns a non-owning pointer to the respone topic string, that we should have received the actual data on.
    /// Used to check, which API Implementation needs to handle the current response to a previously sent request
    /// @return Response topic null-terminated string
    virtual const char const * Get_Response_Topic_String() const = 0;

    /// @brief Forwards the call to let the API clear up any ongoing subscriptions and stop receiving information over the previously subscribed topic
    /// @return Whether unsubscribing was successfull or not
    virtual bool Unsubscribe_Topic() = 0;

    /// @brief Forwards the call to let the API clear up any ongoing single-event subscriptions (Provision, Attribute Request, RPC Request)
    /// and simply resubscribes the topic for all permanent subscriptions (RPC, Shared Attribute Update)
    /// @return Whether resubscribing was successfull or not
    virtual bool Resubscribe_Topic() {
        return Unsubscribe_Topic();
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
    /// @param send_telemtry_callback Method which allows to send arbitrary Json payload, points to sendTelemetryJson per default
    /// @param send_callback Method which allows to send arbitrary Json payload, points to Send_Json per default
    /// @param subscribe_callback Method which allows to subscribe to arbitrary topics, points to m_client.subscribe per default
    /// @param unsubscribe_callback Method which allows to subscribe to arbitrary topics, points to m_client.unsubscribe per default
    /// @param get_size_callback Method which allows to send arbitrary Json payload, points to m_client.get_buffer_size per default
    void Set_Client_Callbacks(Callback<API_Implementation*, API_Implementation> subscribe_api_callback, Callback<bool, JsonDocument const & source, size_t const & jsonSize> send_telemtry_callback, Callback<bool, char const * const topic, JsonDocument const & source, size_t const & jsonSize> send_callback, Callback<bool, char const * const topic> subscribe_callback, Callback<bool, char const * const topic> unsubscribe_callback, Callback<uint16_t, void> get_size_callback) {
        m_subscribe_api_callback = subscribe_api_callback;
        m_send_telemtry_callback = send_telemtry_callback;
        m_send_callback = send_callback;
        m_subscribe_callback = subscribe_callback;
        m_unsubscribe_callback = unsubscribe_callback;
        m_get_size_callback = get_size_callback;
    }

  protected:
    Callback<API_Implementation*, API_Implementation>                                              m_subscribe_api_callback; // Subscribe API callback
    Callback<bool, JsonDocument const & source, size_t const & jsonSize>                           m_send_telemtry_callback; // Send Telemtry callback
    Callback<bool, char const * const topic, JsonDocument const & source, size_t const & jsonSize> m_send_callback;          // Send json callback
    Callback<bool, char const * const topic>                                                       m_subscribe_callback;     // Subscribe topic callback
    Callback<bool, char const * const topic>                                                       m_unsubscribe_callback;   // Unsubscribe topic callback
    Callback<uint16_t, void>                                                                       m_get_size_callback;      // Get client size callback
};

#endif // API_Implementation_h
