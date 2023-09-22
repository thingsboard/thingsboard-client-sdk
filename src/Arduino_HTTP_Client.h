#ifndef Arduino_HTTP_Client_h
#define Arduino_HTTP_Client_h

#ifdef ARDUINO

// Library includes.
#include <ArduinoHttpClient.h>

// Local includes.
#include "IHTTP_Client.h"


/// @brief HTTP Client interface implementation that uses the ArduinoHttpClient (https://github.com/arduino-libraries/ArduinoHttpClient),
/// under the hood to establish and communicate over an HTTP connection
class Arduino_HTTP_Client : public IHTTP_Client {
  public:
    /// @brief Constructs a IHTTP_Client implementation with the given network client
    /// @param transport_client Client that is used to send the actual payload via. HTTP, needs to implement the client interface,
    /// but the actual type of connection does not matter (Ethernet or WiFi)
    /// @param host Server instance name the client should connect too
    /// @param port Port that will be used to establish a connection and send / receive data
    /// Should be either 80 for unencrypted HTTP or 443 for HTTPS with encryption.
    /// The latter is recommended if relevant data is sent or if the client receives and handles requests from the server,
    /// because using an unencrpyted connection, will allow 3rd parties to listen to the communication and impersonate the server sending payloads which might influence the device in unexpected ways
    Arduino_HTTP_Client(Client& transport_client, const char *host, const uint16_t& port);

    void set_keep_alive(const bool& keep_alive) override;

    int connect(const char *host, const uint16_t& port) override;

    void stop() override;

    int post(const char *url_path, const char *content_type, const char *request_body) override;

    int get_response_status_code() override;

    int get(const char *url_path) override;

#if THINGSBOARD_ENABLE_STL
    std::string get_response_body() override;
#else
    String get_response_body() override;
#endif // THINGSBOARD_ENABLE_STL

  private:
    HttpClient m_http_client; // Underlying HTTP client instance used to send data
};

#endif // ARDUINO

#endif // Arduino_HTTP_Client_h
