#ifndef Arduino_MQTT_Client_h
#define Arduino_MQTT_Client_h

#ifdef ARDUINO

// Local includes.
#include "IMQTT_Client.h"

// Library include
#include <PubSubClient.h>


/// @brief MQTT Client interface implementation that uses the PubSubClient forked from ThingsBoard (https://github.com/thingsboard/pubsubclient),
/// under the hood to establish and communicate over a MQTT connection. The fork includes fixes to solve issues with using std::function callbacks for non ESP boards
class Arduino_MQTT_Client : public IMQTT_Client {
  public:
    /// @brief Constructs a IMQTT_Client implementation without a network client, meaning it has to be added later with the set_client() method
    Arduino_MQTT_Client() = default;

    /// @brief Constructs a IMQTT_Client implementation with the given network client
    /// @param transport_client Client that is used to send the actual payload via. MQTT, needs to implement the client interface,
    /// but the actual type of connection does not matter (Ethernet or WiFi)
    Arduino_MQTT_Client(Client & transport_client);

    /// @brief Sets the client has to be used if the empty constructor was used initally
    /// @param transport_client Client that is used to send the actual payload via. MQTT, needs to implement the client interface,
    /// but the actual type of connection does not matter (Ethernet or WiFi)
    void set_client(Client & transport_client);

    void set_data_callback(data_function cb) override;

    void set_connect_callback(connect_function cb) override;

    bool set_buffer_size(uint16_t const & buffer_size) override;

    uint16_t get_buffer_size() override;

    void set_server(char const * const domain, uint16_t const & port) override;

    bool connect(char const * const client_id, char const * const user_name, char const * const password) override;

    void disconnect() override;

    bool loop() override;

    bool publish(char const * const topic, uint8_t const * const payload, size_t const & length) override;

    bool subscribe(char const * const topic) override;

    bool unsubscribe(char const * const topic) override;

    bool connected() override;

#if THINGSBOARD_ENABLE_STREAM_UTILS

    bool begin_publish(char const * const topic, size_t const & length) override;

    bool end_publish() override;

    //----------------------------------------------------------------------------
    // Print interface
    //----------------------------------------------------------------------------

    size_t write(uint8_t payload_byte) override;

    size_t write(uint8_t const * const buffer, size_t const & size) override;

#endif // THINGSBOARD_ENABLE_STREAM_UTILS

  private:
    connect_function m_cb;      // Callback that will be called as soon as the mqtt client has connected
    PubSubClient m_mqtt_client; // Underlying MQTT client instance used to send data
};

#endif // ARDUINO

#endif // Arduino_MQTT_Client_h
