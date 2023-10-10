// Header include.
#include "Espressif_MQTT_Client.h"

#if THINGSBOARD_USE_ESP_MQTT

// The error integer -1 means a general failure while handling the mqtt client,
// where as -2 means that the outbox is filled and the message can therefore not be sent.
// Therefore we have to check if the value is smaller or equal to the MQTT_FAILURE_MESSAGE_ID,
// to ensure other errors are indentified as well
constexpr int MQTT_FAILURE_MESSAGE_ID = -1;

Espressif_MQTT_Client *Espressif_MQTT_Client::m_instance = nullptr;

Espressif_MQTT_Client::Espressif_MQTT_Client() :
    m_received_data_callback(nullptr),
    m_connected(false),
    m_enqueue_messages(false),
    m_mqtt_configuration(),
    m_mqtt_client(nullptr)
{
    m_instance = this;
}

Espressif_MQTT_Client::~Espressif_MQTT_Client() {
    m_instance = nullptr;
    (void)esp_mqtt_client_destroy(m_mqtt_client);
}

bool Espressif_MQTT_Client::set_server_certificate(const char *server_certificate_pem) {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely.
    // Because PEM format is expected for the server certificate we do not need to set the certificate_len,
    // because the PEM format expects a null-terminated string.
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.cert_pem = server_certificate_pem;
#else
    m_mqtt_configuration.broker.verification.certificate = server_certificate_pem;
#endif // ESP_IDF_VERSION_MAJOR < 5
    return update_configuration();
}

bool Espressif_MQTT_Client::set_keep_alive_timeout(const uint16_t& keep_alive_timeout_seconds) {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.keepalive = keep_alive_timeout_seconds;
#else
    m_mqtt_configuration.session.keepalive = keep_alive_timeout_seconds;
#endif // ESP_IDF_VERSION_MAJOR < 5
    return update_configuration();
}

bool Espressif_MQTT_Client::set_disable_keep_alive(const bool& disable_keep_alive) {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.disable_keepalive = disable_keep_alive;
#else
    m_mqtt_configuration.session.disable_keepalive = disable_keep_alive;
#endif // ESP_IDF_VERSION_MAJOR < 5
    return update_configuration();
}

bool Espressif_MQTT_Client::set_disable_auto_reconnect(const bool& disable_auto_reconnect) {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.disable_auto_reconnect = disable_auto_reconnect;
#else
    m_mqtt_configuration.network.disable_auto_reconnect = disable_auto_reconnect;
#endif // ESP_IDF_VERSION_MAJOR < 5
    return update_configuration();
}

bool Espressif_MQTT_Client::set_mqtt_task_configuration(const uint8_t& priority, const uint16_t& stack_size) {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.task_prio = priority;
    m_mqtt_configuration.task_stack = stack_size;
#else
    m_mqtt_configuration.task.priority = priority;
    m_mqtt_configuration.task.stack_size = stack_size;
#endif // ESP_IDF_VERSION_MAJOR < 5
    return update_configuration();
}

bool Espressif_MQTT_Client::set_reconnect_timeout(const uint16_t& reconnect_timeout_milliseconds) {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.reconnect_timeout_ms = reconnect_timeout_milliseconds;
#else
    m_mqtt_configuration.network.reconnect_timeout_ms = reconnect_timeout_milliseconds;
#endif // ESP_IDF_VERSION_MAJOR < 5
    return update_configuration();
}

bool Espressif_MQTT_Client::set_network_timeout(const uint16_t& network_timeout_milliseconds) {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.network_timeout_ms = network_timeout_milliseconds;
#else
    m_mqtt_configuration.network.timeout_ms = network_timeout_milliseconds;
#endif // ESP_IDF_VERSION_MAJOR < 5
    return update_configuration();
}

void Espressif_MQTT_Client::set_enqueue_messages(const bool& enqueue_messages) {
    m_enqueue_messages = enqueue_messages;
}

void Espressif_MQTT_Client::set_callback(function callback) {
    m_received_data_callback = callback;
}

bool Espressif_MQTT_Client::set_buffer_size(const uint16_t& buffer_size) {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.buffer_size = buffer_size;
#else
    m_mqtt_configuration.buffer.size = buffer_size;
#endif // ESP_IDF_VERSION_MAJOR < 5

    // Calls esp_mqtt_set_config(), which should adjust the underlying mqtt client to the changed values.
    // Which it does but not for the buffer_size, this results in the buffer size only being able to be changed when initally creating the mqtt client.
    // If the mqtt client is reinitalized this causes disconnected and reconnects tough and the connection becomes unstable.
    // Therefore this workaround can also not be used. Instead we expect the esp_mqtt_set_config(), to do what the name implies and therefore still call it
    // and created an issue revolving around the aformentioned problem so it might get fixed in future version of the esp_mqtt client.
    // See https://github.com/espressif/esp-mqtt/issues/267 for more information on the issue 
    return update_configuration();
}

uint16_t Espressif_MQTT_Client::get_buffer_size() {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    return m_mqtt_configuration.buffer_size;
#else
    return m_mqtt_configuration.buffer.size;
#endif // ESP_IDF_VERSION_MAJOR < 5
}

void Espressif_MQTT_Client::set_server(const char *domain, const uint16_t& port) {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.host = domain;
    m_mqtt_configuration.port = port;
    // Decide transport depending on if a certificate was passed, because the set_server() method is called in the connect method meaning if the certificate has not been set yet,
    // it is to late as we attempt to establish the connection in the connect() method which is called directly after this one.
    const bool transport_over_sll = m_mqtt_configuration.cert_pem != nullptr;
#else
    m_mqtt_configuration.broker.address.hostname = domain;
    m_mqtt_configuration.broker.address.port = port;
    // Decide transport depending on if a certificate was passed, because the set_server() method is called in the connect method meaning if the certificate has not been set yet,
    // it is to late as we attempt to establish the connection in the connect() method which is called directly after this one.
    const bool transport_over_sll = m_mqtt_configuration.broker.verification.certificate != nullptr;
#endif // ESP_IDF_VERSION_MAJOR < 5

    const esp_mqtt_transport_t transport = (transport_over_sll ? esp_mqtt_transport_t::MQTT_TRANSPORT_OVER_SSL : esp_mqtt_transport_t::MQTT_TRANSPORT_OVER_TCP);

    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.transport = transport;
#else
    m_mqtt_configuration.broker.address.transport = transport;
#endif // ESP_IDF_VERSION_MAJOR < 5
}

bool Espressif_MQTT_Client::connect(const char *client_id, const char *user_name, const char *password) {
    // ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the complete esp_mqtt_client_config_t structure changed completely
#if ESP_IDF_VERSION_MAJOR < 5
    m_mqtt_configuration.client_id = client_id;
    m_mqtt_configuration.username = user_name;
    m_mqtt_configuration.password = password;
#else
    m_mqtt_configuration.credentials.client_id = client_id;
    m_mqtt_configuration.credentials.username = user_name;
    m_mqtt_configuration.credentials.authentication.password = password;
#endif // ESP_IDF_VERSION_MAJOR < 5
    // Update configuration is called to ensure that if we connected previously and call connect again with other credentials,
    // then we also update the client_id, username and password we connect with. Especially important for the provisioning workflow to work correctly
    update_configuration();

    // Check wheter the client has been initalzed before already, it it has we do not want to reinitalize,
    // but simply force reconnection with the client because it has lost that connection
    if (m_mqtt_client != nullptr) {
        const esp_err_t error = esp_mqtt_client_reconnect(m_mqtt_client);
        return error == ESP_OK;
    }

    // The client is first initalized once the connect has actually been called, this is done because the passed setting are required for the client inizialitation structure,
    // additionally before we attempt to connect with the client we have to ensure it is configued by then.
    m_mqtt_client = esp_mqtt_client_init(&m_mqtt_configuration);

    // The last argument may be used to pass data to the event handler, here that would be the static_mqtt_event_handler. But for our use case this is not needed,
    // because the static_mqtt_event_handler calls a private method on this class again anyway, meaning we already have access to all private member variables that are required
    esp_err_t error = esp_mqtt_client_register_event(m_mqtt_client, esp_mqtt_event_id_t::MQTT_EVENT_ANY, Espressif_MQTT_Client::static_mqtt_event_handler, nullptr);

    if (error != ESP_OK) {
        return false;
    }

    error = esp_mqtt_client_start(m_mqtt_client);
    return error == ESP_OK;
}

void Espressif_MQTT_Client::disconnect() {
    (void)esp_mqtt_client_disconnect(m_mqtt_client);
}

bool Espressif_MQTT_Client::loop() {
    // Unused because the esp mqtt client uses its own task to handle receiving and sending of data, therefore we do not need to do anything in the loop method.
    // Because the loop method is meant for clients that do not have their own process method but instead rely on the upper level code calling a loop method to provide processsing time.
    return m_connected;
}

bool Espressif_MQTT_Client::publish(const char *topic, const uint8_t *payload, const size_t& length) {
    int message_id = MQTT_FAILURE_MESSAGE_ID;

    if (m_enqueue_messages) {
        message_id = esp_mqtt_client_enqueue(m_mqtt_client, topic, reinterpret_cast<const char*>(payload), length, 0U, 0U, true);
        return message_id > MQTT_FAILURE_MESSAGE_ID;
    }

    // The blocking version esp_mqtt_client_publish() it is sent directly from the users task context.
    // This way is used to send messages to the cloud, because like that no internal buffer has to be used to store the message until it should be sent,
    // because all messages are sent with QoS level 0. If this is not wanted esp_mqtt_client_enqueue() could be used with store = true,
    // to ensure the sending is done in the mqtt event context instead of the users task context.
    // Allows to use the publish method without having to worry about any CPU overhead, so it can even be used in callbacks or high priority tasks, without starving other tasks,
    // but compared to the other method esp_mqtt_client_enqueue() requires to save the message in the outbox, which increases the memory requirements for the internal buffer size
    message_id = esp_mqtt_client_publish(m_mqtt_client, topic, reinterpret_cast<const char*>(payload), length, 0U, 0U);
    return message_id > MQTT_FAILURE_MESSAGE_ID;
}

bool Espressif_MQTT_Client::subscribe(const char *topic) {
    const int message_id = esp_mqtt_client_subscribe(m_mqtt_client, topic, 0U);
    return message_id > MQTT_FAILURE_MESSAGE_ID;
}

bool Espressif_MQTT_Client::unsubscribe(const char *topic) {
    const int message_id = esp_mqtt_client_unsubscribe(m_mqtt_client, topic);
    return message_id > MQTT_FAILURE_MESSAGE_ID;
}

bool Espressif_MQTT_Client::connected() {
    return m_connected;
}

bool Espressif_MQTT_Client::update_configuration() {
    // Check if the client has been initalized, because if it did not the value should still be nullptr
    // and updating the config makes no sense because the changed settings will be applied anyway when the client is first intialized
    if (m_mqtt_client == nullptr) {
        return true;
    }

    const esp_err_t error = esp_mqtt_set_config(m_mqtt_client, &m_mqtt_configuration);
    return error == ESP_OK;
}

void Espressif_MQTT_Client::mqtt_event_handler(void *handler_args, esp_event_base_t base, const esp_mqtt_event_id_t& event_id, void *event_data) {
    const esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);

    switch (event_id) {
        case esp_mqtt_event_id_t::MQTT_EVENT_CONNECTED:
            m_connected = true;
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_DISCONNECTED:
            m_connected = false;
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_SUBSCRIBED:
            // Nothing to do
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_UNSUBSCRIBED:
            // Nothing to do
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_PUBLISHED:
            // Nothing to do
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_DATA:
            // Check wheter the given message has not bee received completly, but instead would be received in multiple chunks,
            // if it were we discard the message because receiving a message over multiple chunks is currently not supported
            if (event->data_len != event->total_data_len) {
                break;
            }

            if (m_received_data_callback != nullptr) {
                m_received_data_callback(event->topic, reinterpret_cast<uint8_t*>(event->data), event->data_len);
            }
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_ERROR:
            // Nothing to do
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_BEFORE_CONNECT:
            // Nothing to do
            break;
        default:
            // Nothing to do
            break;
    }
}

void Espressif_MQTT_Client::static_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    if (m_instance == nullptr) {
        return;
    }

    m_instance->mqtt_event_handler(handler_args, base, static_cast<esp_mqtt_event_id_t>(event_id), event_data);
}

#endif // THINGSBOARD_USE_ESP_MQTT
