#ifndef Espressif_MQTT_Client_h
#define Espressif_MQTT_Client_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_USE_ESP_MQTT

// Local includes.
#include "IMQTT_Client.h"

// Library includes.
#include <mqtt_client.h>


/// @brief MQTT Client interface implementation that uses the offical ESP MQTT client from Espressif (https://github.com/espressif/esp-mqtt),
/// under the hood to establish and communicate over a MQTT connection. This component works with both Espressif IDF v4.X and v5.X, meaning it is version idependent, this is the case
/// because depending on the used version the implementation automatically adjusts to still initalize the client correctly.
/// Documentation about the specific use and caviates of the ESP MQTT client can be found here https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mqtt.html
class Espressif_MQTT_Client : public IMQTT_Client {
  public:
    /// @brief Constructs a IMQTT_Client implementation which creates and empty esp_mqtt_client_config_t, which then has to be configured with the other methods in the class
    Espressif_MQTT_Client();

    /// @brief Destructor
    ~Espressif_MQTT_Client();

    /// @brief Configured the server certificate, which allows to connect to the MQTT broker over a secure TLS / SSL conenction instead of the default unencrypted channel.
    /// Has to be called before initally calling connect() on the client to ensure the certificate is set before the connection is established, if that is not done the connection will not be encrypted.
    /// Encryption is recommended if relevant data is sent or if the client receives and handles Remote Procedure Calls or Shared Attribute Update Callbacks from the server,
    /// because using an unencrpyted connection, will allow 3rd parties to listen to the communication and impersonate the server sending payloads which might influence the device in unexpected ways.
    /// However if Over the Air udpates are enabled secure communication should definetly be enabled, because if that is not done a 3rd party might impersonate the server sending a malicious payload,
    /// which is then flashed onto the device instead of the real firmware. Which depeding on the payload might even be able to destroy the device or make it otherwise unusable.
    /// See https://stackoverflow.blog/2020/12/14/security-considerations-for-ota-software-updates-for-iot-gateway-devices/ for more information on the aforementioned security risk
    /// @param server_certificate_pem Null-terminated string containg the root certificate in PEM format, of the server we are attempting to send to and receive MQTT data from
    /// @return Whether chaing the internal server certificate was successful or not, ensure to disconnect and reconnect to actually apply the change
    bool set_server_certificate(const char *server_certificate_pem);

    /// @brief Sets the keep alive timeout in seconds, if the value is 0 then the default of 120 seconds is used instead to disable the keep alive mechanism use set_disable_keep_alive() instead.
    /// The default timeout value ThingsBoard expectes to receive any message including a keep alive to not show the device as inactive can be found here https://thingsboard.io/docs/user-guide/install/config/#mqtt-server-parameters
    /// under the transport.sessions.inactivity_timeout section and is 300 seconds. Meaning a value bigger than 300 seconds with the default config defeats the purpose of the keep alive alltogether
    /// @param keep_alive_timeout_seconds Timeout until we send another PINGREQ control packet to the broker to establish that we are still connected
    /// @return Whether changing the internal keep alive timeout was successful or not
    bool set_keep_alive_timeout(const uint16_t& keep_alive_timeout_seconds); 
    
    /// @brief Whether to disable or enable the keep alive mechanism, meaning we do not send any PINGREQ control packets to the broker anymore, meaning if we do not send other packet the device will be marked as inactive.
    /// The default value is false meaning the keep alive mechanism is enabled.
    /// The default timeout value ThingsBoard expectes to receive any message including a keep alive to not show the device as inactive can be found here https://thingsboard.io/docs/user-guide/install/config/#mqtt-server-parameters
    /// under the transport.sessions.inactivity_timeout section and is 300 seconds. Meaning if we disable the keep alive mechanism and do not send another packet atleast every 300 seconds with the default config
    /// then the device will change states between active and inactive depending on when we send the packets
    /// @param disable_keep_alive Whether to enable or disable the internal keep alive mechanism, which sends PINGREQ control packets every keep alive timeout seconds, configurable in set_keep_alive_timeout()
    /// @return Whether enabling or disabling the internal keep alive mechanism was successful or not
    bool set_disable_keep_alive(const bool& disable_keep_alive);

    /// @brief Wheter to disable or enable that the MQTT client will reconnect to the server automatically if it errors or disconnects. The default is false meaning we will automatically reconnect
    /// @param disable_auto_reconnect Whether to automatically reconnect if the the client errors or disconnects
    /// @return Whether enabling or disabling the internal auto reconnect mechanism was successful or not
    bool set_disable_auto_reconnect(const bool& disable_auto_reconnect);

    /// @brief Sets the priority and stack size of the MQTT task running in the background, that is handling the receiving and sending of any outstanding MQTT messages to or from the broker.
    /// The default value for the priority is 5 and can also be changed in the ESP IDF menuconfig, whereas the default value for the stack size is 6144 bytes and can also be changed in the ESP IDF menuconfig
    /// @param priority Task priority with which the MQTT task should run, higher priority means it takes more precedence over other tasks, making it more important
    /// @param stack_size Task stack size meaning how much stack size the MQTT task can allocate before the device crashes with a StackOverflow, might need to be increased
    /// if the user allocates a lot of memory on the stack in a request callback method, because those functions are dispatched from the MQTT event task
    /// @return Whether changing the internal MQTT task configurations were successfull or not
    bool set_mqtt_task_configuration(const uint8_t& priority, const uint16_t& stack_size);

    /// @brief Sets the amount of time in milliseconds that we wait before we automatically reconnect to the MQTT broker if the connection has been lost. The default value is 10 seconds.
    /// Will be ignored if set_disable_auto_reconnect() has been set to true, because we will not reconnect automatically anymore, instead that would have to be done by the user themselves
    /// @param reconnect_timeout_milliseconds Time in milliseconds until we automatically reconnect to the MQTT broker
    /// @return Whether changing the internal reconnect timeout was successfull or not
    bool set_reconnect_timeout(const uint16_t& reconnect_timeout_milliseconds);

    /// @brief Sets the amount of time in millseconds that we wait until we expect a netowrk operation on the MQTT client to have successfully finished. The defalt value is 10 seconds.
    /// If that is not the case then the network operation will be aborted, might be useful to increase for devices that perform other high priority tasks and do not have enough CPU resources,
    /// to send bigger messages to the MQTT broker in time, which can cause disconnects and the sent message being discarded
    /// @param network_timeout_milliseconds Time in milliseconds that we wait until we abort the network operation if it has not completed yet
    /// @return Whether changing the internal network timeout was successfull or not
    bool set_network_timeout(const uint16_t& network_timeout_milliseconds);

    /// @brief Sets whether to enqueue published messages or not, enqueueing has to save them in the out buffer, meaning the internal buffer size might need to be increased,
    /// but the MQTT client can in exchange send the publish messages once the main MQTT task is running again, instead of blocking in the task that has called the publish method.
    /// This furthermore, allows to use nearly all internal ThingsBoard calls without having to worry about blocking the task the method was called for,
    /// or having to worry about CPU overhead.
    /// If enqueueing the messages to be published fails once this options has been enabled, then the internal buffer size might need to be increased. Ensure to call set_buffer_size() with a bigger value
    /// and check if enqueueing the messages to be published works successfully again
    /// @param enqueue_messages Whether to enqueue published messages or not, where setting the value to true means that the messages are enqueued and therefor non blocking on the called from task
    void set_enqueue_messages(const bool& enqueue_messages);

    void set_callback(function callback) override;

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

private:
    function m_received_data_callback;             // Callback that will be called as soon as the mqtt client receives any data
    bool m_connected;                              // Whether the client has received the connected or disconnected event
    bool m_enqueue_messages;                       // Whether we enqueue messages making nearly all ThingsBoard calls non blocking or wheter we publish instead
    esp_mqtt_client_config_t m_mqtt_configuration; // Configuration of the underlying mqtt client, saved as a private variable to allow changes after inital configuration with the same options for all non changed settings
    esp_mqtt_client_handle_t m_mqtt_client;        // Handle to the underlying mqtt client, used to establish the communication

    static Espressif_MQTT_Client *m_instance;      // Instance to the created class, will be set once the constructor has been called and reset once the destructor has been called, used to call private member method from static callback

    /// @brief Is internally used to allow changes to the underlying configuration of the esp_mqtt_client_handle_t after it has connected,
    /// to for example increase the buffer size or increase the timeouts or stack size, allows to change the underlying client configuration,
    /// without the need to completly disconnect and reconnect the client
    /// @return Whether updating the configuration with the changed settings was successfull or not
    bool update_configuration();

    /// @brief Event handler registered to receive MQTT events. Is called by the MQTT client event loop, whenever a new event occurs
    /// @param handler_args User data registered to the event
    /// @param base Event base for the handler
    /// @param event_id The id for the received event
    /// @param event_data The data for the event, esp_mqtt_event_handle_t
    void mqtt_event_handler(void *handler_args, esp_event_base_t base, const esp_mqtt_event_id_t& event_id, void *event_data);

    static void static_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
};

#endif // THINGSBOARD_USE_ESP_MQTT

#endif // Espressif_MQTT_Client_h
