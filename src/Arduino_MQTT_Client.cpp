// Header include.
#include "Arduino_MQTT_Client.h"

#ifdef ARDUINO

Arduino_MQTT_Client::Arduino_MQTT_Client() :
    m_mqtt_client()
{
    // Nothing to do
}

Arduino_MQTT_Client::Arduino_MQTT_Client(Client& transport_client) :
    m_mqtt_client(transport_client)
{
    // Nothing to do
}

void Arduino_MQTT_Client::set_client(Client& transport_client) {
    m_mqtt_client.setClient(transport_client);
}

void Arduino_MQTT_Client::set_callback(function cb) {
    m_mqtt_client.setCallback(cb);
}

bool Arduino_MQTT_Client::set_buffer_size(const uint16_t& buffer_size) {
    return m_mqtt_client.setBufferSize(buffer_size);
}

uint16_t Arduino_MQTT_Client::get_buffer_size() {
    return m_mqtt_client.getBufferSize();
}

void Arduino_MQTT_Client::set_server(const char *domain, const uint16_t& port) {
    m_mqtt_client.setServer(domain, port);
}

bool Arduino_MQTT_Client::connect(const char *client_id, const char *user_name, const char *password) {
    return m_mqtt_client.connect(client_id, user_name, password);
}

void Arduino_MQTT_Client::disconnect() {
    m_mqtt_client.disconnect();
}

bool Arduino_MQTT_Client::loop() {
    return m_mqtt_client.loop();
}

bool Arduino_MQTT_Client::publish(const char *topic, const uint8_t *payload, const size_t& length) {
    return m_mqtt_client.publish(topic, payload, length, false);
}

bool Arduino_MQTT_Client::subscribe(const char *topic) {
    return m_mqtt_client.subscribe(topic);
}

bool Arduino_MQTT_Client::unsubscribe(const char *topic) {
    return m_mqtt_client.unsubscribe(topic);
}

bool Arduino_MQTT_Client::connected() {
    return m_mqtt_client.connected();
}

#if THINGSBOARD_ENABLE_STREAM_UTILS

bool Arduino_MQTT_Client::begin_publish(const char *topic, const size_t& length) {
    return m_mqtt_client.beginPublish(topic, length, false);
}

bool Arduino_MQTT_Client::end_publish() {
    return m_mqtt_client.endPublish();
}

size_t Arduino_MQTT_Client::write(uint8_t payload_byte) {
    return m_mqtt_client.write(payload_byte);
}

size_t Arduino_MQTT_Client::write(const uint8_t *buffer, size_t size) {
    return m_mqtt_client.write(buffer, size);
}

#endif // THINGSBOARD_ENABLE_STREAM_UTILS

#endif // ARDUINO
