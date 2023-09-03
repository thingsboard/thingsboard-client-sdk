// Header include.
#include "Arduino_HTTP_Client.h"

Arduino_HTTP_Client::Arduino_HTTP_Client(Client& transport_client, const char *host, const uint16_t& port) :
    m_http_client(transport_client, host, port)
{
    // Nothing to do
}

void Arduino_HTTP_Client::set_keep_alive(const bool& keep_alive) {
    if (keep_alive) {
        m_http_client.connectionKeepAlive();
    }
}

int Arduino_HTTP_Client::connect(const char *host, const uint16_t& port) {
    return m_client.connect(host, port);
}

void Arduino_HTTP_Client::stop() {
    m_client.stop();
}

int Arduino_HTTP_Client::post(const char *url_path, const char *content_type, const char *request_body) {
    return m_client.post(url_path, content_type, request_body);
}

int Arduino_HTTP_Client::get_response_status_code() {
    return m_client.responseStatusCode();
}

int Arduino_HTTP_Client::get(const char *url_path) {
    return m_client.get(url_path);
}

String Arduino_HTTP_Client::get_response_body() {
    return m_client.responseBody();
}
