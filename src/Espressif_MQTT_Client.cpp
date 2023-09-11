// Header include.
#include "Espressif_MQTT_Client.h"

#ifdef THINGSBOARD_USE_ESP_MQTT

// Library include
#include <esp_log.h>

constexpr char TAG[] = "MQTT";
constexpr int MQTT_FAILURE_CODE = -1;

Espressif_MQTT_Client *Espressif_MQTT_Client::m_instance = nullptr;

Espressif_MQTT_Client::Espressif_MQTT_Client() :
    m_received_data_callback(nullptr),
    m_connected(false),
    m_mqtt_configuration(),
    m_mqtt_client()
{
    m_instance = this;
}

void Espressif_MQTT_Client::set_server_certificate(const char *server_certificate_pem) {
    // Because PEM format is expected for the server certificate we do not need to set the certificate_len,
    // because the PEM format expects a null-terminated string.
    m_mqtt_configuration.broker.verification.certificate = server_certificate_pem;
}

void Espressif_MQTT_Client::set_callback(function cb) {
    m_received_data_callback = cb;
}

bool Espressif_MQTT_Client::set_buffer_size(const uint16_t& buffer_size) {
    m_mqtt_configuration.buffer.size = buffer_size;
    return true;
}

uint16_t Espressif_MQTT_Client::get_buffer_size() {
    return m_mqtt_configuration.buffer.size;
}

void Espressif_MQTT_Client::set_server(const char *domain, const uint16_t& port) {
    m_mqtt_configuration.broker.address.hostname = domain;
    m_mqtt_configuration.broker.address.port = port;
    // Decide transport depending on if a certificate was passed, because the set_server() method is called in the connect method meaning if the certificate has not been set yet,
    // it is to late as we attempt to establish the connection in the connect() method which is called directly after this one.
    const bool transport_over_sll = m_mqtt_configuration.broker.verification.certificate != nullptr;
    const esp_mqtt_transport_t transport = (transport_over_sll ? esp_mqtt_transport_t::MQTT_TRANSPORT_OVER_SSL : esp_mqtt_transport_t::MQTT_TRANSPORT_OVER_TCP);
    m_mqtt_configuration.broker.address.transport = transport;
}

bool Espressif_MQTT_Client::connect(const char *client_id, const char *user_name, const char *password) {
    m_mqtt_configuration.credentials.client_id = client_id;
    m_mqtt_configuration.credentials.username = user_name;
    m_mqtt_configuration.credentials.authentication.password = password;

    // The client is first initalized once the connect has actually been called, this is done because the passed setting are required for the client inizialitation structure,
    // additionally before we attempt to connect with the client we have to ensure it is configued by then.
    m_mqtt_client = esp_mqtt_client_init(&m_mqtt_configuration);

    // The last argument may be used to pass data to the event handler, in this example mqtt_event_handler.
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(m_mqtt_client, esp_mqtt_event_id_t::MQTT_EVENT_ANY, Espressif_MQTT_Client::static_mqtt_event_handler, nullptr));

    ESP_ERROR_CHECK(esp_mqtt_client_start(m_mqtt_client));
    return true;
}

void Espressif_MQTT_Client::disconnect() {
    ESP_ERROR_CHECK(esp_mqtt_client_disconnect(m_mqtt_client));
}

bool Espressif_MQTT_Client::loop() {
    // Unused because the esp mqtt client uses its own task to handle receiving and sending of data, therefore we do not need to do anything in the loop method.
    // Because the loop method is meant for clients that do not have their own process method but instead rely on the upper level code calling a loop method to provide processsing time.
    return m_connected;
}

bool Espressif_MQTT_Client::publish(const char *topic, const uint8_t *payload, const size_t& length) {
    // The blocking version esp_mqtt_client_publish() it is sent directly from the users task context.
    // This way is used to send messages to the cloud, because like that no internal buffer has to be used to store the message until it should be sent,
    // because all messages are sent with QoS level 0. If this is not wanted esp_mqtt_client_enqueue() could be used with store = true,
    // to ensure the sending is done in the mqtt event context instead of the users task context.
    // Allows to use the publish method without having to worry about any CPU overhead, so it can even be used in callbacks or high priority tasks, without starving other tasks.
    const int message_id = esp_mqtt_client_publish(m_mqtt_client, topic, reinterpret_cast<const char*>(payload), length, 0U, 0U);
    return message_id != MQTT_FAILURE_CODE;
}

bool Espressif_MQTT_Client::subscribe(const char *topic) {
    const int message_id = esp_mqtt_client_subscribe(m_mqtt_client, topic, 0U);
    return message_id != MQTT_FAILURE_CODE;
}

bool Espressif_MQTT_Client::unsubscribe(const char *topic) {
    const int message_id = esp_mqtt_client_unsubscribe(m_mqtt_client, topic);
    return message_id != MQTT_FAILURE_CODE;
}

bool Espressif_MQTT_Client::connected() {
    return m_connected;
}

void Espressif_MQTT_Client::mqtt_event_handler(void *handler_args, esp_event_base_t base, const esp_mqtt_event_id_t& event_id, void *event_data) {

    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%i", base, static_cast<std::underlying_type<esp_mqtt_event_id_t>::type>(event_id));
    esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch (event_id) {
        case esp_mqtt_event_id_t::MQTT_EVENT_CONNECTED:
            m_connected = true;
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            ESP_LOGI(TAG, " The client has successfully established a connection to the broker. The client is now ready to send and receive data");
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_DISCONNECTED:
            m_connected = false;
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            ESP_LOGI(TAG, "The client has aborted the connection due to being unable to read or write data, e.g., because the server is unavailable");
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            ESP_LOGI(TAG, "The broker has acknowledged the client’s subscribe request. The event data contains the message ID of the subscribe message");
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            ESP_LOGI(TAG, "The broker has acknowledged the client’s unsubscribe request. The event data contains the message ID of the unsubscribe message");
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            ESP_LOGI(TAG, "The broker has acknowledged the client’s publish message. This is only posted for QoS level 1 and 2, as level 0 does not use acknowledgements. The event data contains the message ID of the publish message.");
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            ESP_LOGI(TAG, "The client has received a publish message. The event data contains: message ID, name of the topic it was published to, received data and its length. For data that exceeds the internal buffer, multiple MQTT_EVENT_DATA events are posted and current_data_offset and total_data_len from event data updated to keep track of the fragmented message");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            if (m_received_data_callback != nullptr) {
                m_received_data_callback(event->topic, reinterpret_cast<uint8_t*>(event->data), event->data_len);
            }
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            ESP_LOGI(TAG, "The client has encountered an error. The field error_handle in the event data contains error_type that can be used to identify the error. The type of error determines which parts of the error_handle struct is filled");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
                ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
                ESP_LOGI(TAG, "Last captured errno : %d (%s)",  event->error_handle->esp_transport_sock_errno,
                         strerror(event->error_handle->esp_transport_sock_errno));
            } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
                ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
            } else {
                ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
            }
            break;
        case esp_mqtt_event_id_t::MQTT_EVENT_BEFORE_CONNECT:
            ESP_LOGI(TAG, "MQTT_EVENT_BEFORE_CONNECT");
            ESP_LOGI(TAG, "The client is initialized and about to start connecting to the broker");
            break;
        default:
            ESP_LOGI(TAG, "Other MQTT event id:%i", static_cast<std::underlying_type<esp_mqtt_event_id_t>::type>(event_id));
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
