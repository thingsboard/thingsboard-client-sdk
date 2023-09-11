/*
  Espressif_MQTT_Client.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Espressif_MQTT_Client_h
#define Espressif_MQTT_Client_h

// Local include.
#include "Configuration.h"

#ifdef THINGSBOARD_USE_ESP_MQTT

// Local includes.
#include "IMQTT_Client.h"

// Library includes.
#include <mqtt_client.h>

/// @brief MQTT Client interface implementation that uses the offical ESP MQTT client from Espressif (https://github.com/espressif/esp-mqtt),
/// under the hood to establish and communicate over a MQTT connection.
/// Documentation about the specific use and caviates of the ESP MQTT client can be found here https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mqtt.html.
class Espressif_MQTT_Client : public IMQTT_Client {
  public:
    /// @brief Constructs a IMQTT_Client implementation which creates and empty esp_mqtt_client_config_t, which then has to be configured with the other methods in the class.
    Espressif_MQTT_Client();

    /// @brief Configured the server certificate, which allows to connected to the MQTT broker over a secure TLS / SSL conenction instead of the default unencrypted channel.
    /// The first option is recommended if relevant data is sent or if the client receives and handles Remote Procedure Calls or Shared Attribute Update Callbacks from the server,
    /// because using an unencrpyted connection, will allow 3rd parties to listen to the communication and impersonate the server sending payloads which might influence the device in unexpected ways.
    /// However if Over the Air udpates are enabled secure communication should definetly be enabled, because if that is not done a 3rd party might impersonate the server sending a malicious payload,
    /// which is then flashed onto the device instead of the real firmware. Which depeding on the payload might even be able to destroy the device or make it otherwise unusable.
    /// See https://stackoverflow.blog/2020/12/14/security-considerations-for-ota-software-updates-for-iot-gateway-devices/ for more information on the aforementioned security risk
    /// @param server_certificate_pem Null-terminated string containg the root certificate in PEM format, of the server we are attempting to send to and receive MQTT data from
    void set_server_certificate(const char *server_certificate_pem);

    void set_callback(function cb) override;

    bool set_buffer_size(const uint16_t& buffer_size) override;

    uint16_t get_buffer_size() override;

    void set_server(const char *domain, const uint16_t& port) override;

    bool connect(const char *client_id, const char *user_name, const char *password) override;

    void disconnect() override;

    bool loop() override;

    bool publish(const char *topic, const uint8_t *payload, const size_t& length) override;

    bool subscribe(const char *topic) override;

    bool unsubscribe(const char *topic) override;

    bool connected() override;

    /// @brief Event handler registered to receive MQTT events. Is called by the MQTT client event loop, whenever a new event occurs.
    /// @param handler_args User data registered to the event
    /// @param base Event base for the handler
    /// @param event_id The id for the received event
    /// @param event_data The data for the event, esp_mqtt_event_handle_t
    void mqtt_event_handler(void *handler_args, esp_event_base_t base, const esp_mqtt_event_id_t& event_id, void *event_data);

private:
    function m_received_data_callback;
    bool m_connected;
    esp_mqtt_client_config_t m_mqtt_configuration;
    esp_mqtt_client_handle_t m_mqtt_client;

    static Espressif_MQTT_Client *m_instance;

    static void static_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
};

#endif // THINGSBOARD_USE_ESP_MQTT

#endif // Espressif_MQTT_Client_h
