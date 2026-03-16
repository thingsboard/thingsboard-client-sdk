#ifndef IMQTT_Client_h
#define IMQTT_Client_h

// Local include.
#include "Callback.h"
#include "DefaultLogger.h"
#include "MQTT_Connection_State.h"
#include "MQTT_Connection_Error.h"

// Library include.
#if THINGSBOARD_ENABLE_STREAM_UTILS
#include <Print.h>
#endif // THINGSBOARD_ENABLE_STREAM_UTILS


/// @brief MQTT Client interface that contains the method that a class that can be used to send and receive data over an MQTT connection should implement
/// @note Seperates the specific implementation used from the ThingsBoard client, allows to use different clients depending on different needs.
/// In this case the main use case of the seperation is to both support Espressif IDF and Arduino with the following libraries as recommendations.
/// The default MQTT Client for Arduino is the PubSubClient forked from ThingsBoard (https://github.com/thingsboard/pubsubclient),
/// it includes fixes to solve issues with using std::function callbacks for non ESP boards.
/// For Espressif IDF however the default MQTT Client is the esp-mqtt (https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mqtt.html) component.
/// The aforementioned recommendations are already implemented in the library and can can simply be used and included when using the library, for Arduino the Arduino_MQTT_Client can simply be included
/// and for Espressif IDF the Espressif_MQTT_Client can simply be included, the implementations have been tested and should be compatible when used in conjunction with the ThingsBoard client.
/// Alternatively when using Arduino it is possible to enable support for THINGSBOARD_ENABLE_STREAM_UTILS by importing the ArduinoStreamUtils (https://github.com/bblanchon/ArduinoStreamUtils) in the project.
/// This feature allows to improve the underlying data streams by directly writing the data into the MQTT Client instead of into an output buffer,
/// but writing each byte one by one, would be too slow, therefore the ArduinoStreamUtils (https://github.com/bblanchon/ArduinoStreamUtils) library is used to buffer those calls into bigger packets.
/// This allows sending data that is very big without requiring to allocate that much memory, because it is sent in smaller packets.
/// To support this feature, however this interface needs to additionally implement the Print interface, because that is required by the wrapper class BufferingPrint, which only exists on Arduino
/// This then allows to send arbitrary size payloads if that is done the internal buffer of the MQTT Client implementation
/// can theoretically set the value as big as the buffering_size passed to the constructor + enough memory to hold the topic and MQTT Header ~= 20 bytes.
/// This will mean though that all messages are sent over the StreamUtils library as long as they are bigger than the internal send buffer size,
/// which needs more time than sending a message directly but has the advantage of requiring less memory
#if THINGSBOARD_ENABLE_STREAM_UTILS
class IMQTT_Client : public Print {
#else
class IMQTT_Client {
#endif // THINGSBOARD_ENABLE_STREAM_UTILS
  public:
    /// @copydoc Callback::~Callback
    virtual ~IMQTT_Client() {}

    /// @brief Sets the callback that is called, if any message is received by the MQTT broker
    /// @note The callback is called with the topic string that the message was received over,
    /// as well as the payload data and the size of that payload data. Directly set by the used ThingsBoard client to its internal methods,
    /// therefore calling again and overriding as a user ist not recommended, unless you know what you are doing
    /// @param callback Method that should be called on received MQTT response
    virtual void set_data_callback(Callback<void, char *, uint8_t *, unsigned int>::function callback) = 0;

    /// @brief Sets the callback that is called, if we have successfully established a connection with the MQTT broker
    /// @note Directly set by the used ThingsBoard client to its internal method, therefore calling again and overriding as a user ist not recommended, unless you know what you are doing.
    /// If receiving information once the device has connected is wanted by the user it is recommended to use @ref subscribe_connection_state_changed_callback method instead
    /// @param callback Method that should be called on established MQTT connection
    virtual void set_connect_callback(Callback<void>::function callback) = 0;

    /// @brief Changes the size of the buffer for sent and received MQTT messages
    /// @note The value can not be bigger than uint16_t because the maximum message size received
    /// or sent by MQTT can never be bigger than 64K, because it relies on TCP and the TCP size limit also uses a uint16_t internally for the size parameter
    /// @param receive_buffer_size Maximum amount of data that can be received via MQTT at once,
    /// expected behaviour is that, if bigger packets are received they are discarded and a warning is printed to the console.
    /// Should be big enough to hold the biggest response that is expected to be ever received by the device at once
    /// @param send_buffer_size Maximum amount of data that can be sent via MQTT at once,
    /// expected behaviour is that, if we attempt to send data that is bigger, it will simply not be sent and a message is printed to the console instead.
    /// Should be big enough to hold the biggest request that is expected to be ever sent by the device at once.
    /// Alternatively when using Arduino it is possible to enable support for THINGSBOARD_ENABLE_STREAM_UTILS by importing the ArduinoStreamUtils (https://github.com/bblanchon/ArduinoStreamUtils) in the project.
    /// This feature allows to improve the underlying data streams by directly writing the data into the MQTT Client instead of into an output buffer,
    /// but writing each byte one by one, would be too slow, therefore the ArduinoStreamUtils (https://github.com/bblanchon/ArduinoStreamUtils) library is used to buffer those calls into bigger packets.
    /// This allows sending data that is very big without requiring to allocate that much memory, because it is sent in smaller packets.
    /// To support this feature, however this interface needs to additionally implement the Print interface, because that is required by the wrapper class BufferingPrint, which only exists on Arduino
    /// This then allows to send arbitrary size payloads if that is done the internal buffer of the MQTT Client implementation
    /// can theoretically set the value as big as the buffering_size passed to the constructor + enough memory to hold the topic and MQTT Header ~= 20 bytes.
    /// This will mean though that all messages are sent over the StreamUtils library as long as they are bigger than the internal send buffer size,
    /// which needs more time than sending a message directly but has the advantage of requiring less memory
    /// @return Whether allocating the needed memory for the given buffer sizes was successful or not
    virtual bool set_buffer_size(uint16_t receive_buffer_size, uint16_t send_buffer_size) = 0;

    /// @brief Gets the previously set size of the internal buffer size meant for incoming MQTT data
    /// @return Internal size of the buffer
    virtual uint16_t get_receive_buffer_size() = 0;

    /// @brief Gets the previously set size of the internal buffer size meant for outgoing MQTT data
    /// @return Internal size of the buffer
    virtual uint16_t get_send_buffer_size() = 0;

    /// @brief Configures the server and port that the client should connect with over MQTT
    /// @note Should be called atleast once before calling connect() so the target server and port to connect too have been configured
    /// @param domain Non owning pointer to server instance name the client should connect too.
    /// Additionally it has to be kept alive by the user for the runtime of the MQTT client connection
    /// @param port Port that will be used to establish a connection and send / receive data.
    /// Should be either 1883 for unencrypted MQTT or 8883 for MQTT with TLS/SSL encryption.
    /// The latter is recommended if relevant data is sent or if the client receives and handles Remote Procedure Calls or Shared Attribute Update Callbacks from the server,
    /// because using an unencrpyted connection, will allow 3rd parties to listen to the communication and impersonate the server sending payloads which might influence the device in unexpected ways.
    /// However if Over the Air udpates are enabled secure communication should definetly be enabled, because if that is not done a 3rd party might impersonate the server sending a malicious payload,
    /// which is then flashed onto the device instead of the real firmware. Which depeding on the payload might even be able to destroy the device or make it otherwise unusable.
    /// See https://stackoverflow.blog/2020/12/14/security-considerations-for-ota-software-updates-for-iot-gateway-devices/ for more information on the aforementioned security risk
    virtual void set_server(char const * domain, uint16_t port) = 0;

    /// @brief Connects to the previously with set_server configured server instance and port with the given credentials
    /// @param client_id Non owning pointer to client identification code, that allows to differentiate which MQTT device is sending the traffic to the MQTT broker.
    /// Additionally it has to be kept alive by the user for the runtime of the MQTT client connection
    /// @param user_name Non owning pointer to client username that is used to authenticate, who is connecting over MQTT.
    /// Additionally it has to be kept alive by the user for the runtime of the MQTT client connection
    /// @param password Non owning pointer to client password that is used to authenticate, who is connecting over MQTT.
    /// Additionally it has to be kept alive by the user for the runtime of the MQTT client connection
    /// @return Whether the client could establish the connection successfully or not
    virtual bool connect(char const * client_id, char const * user_name, char const * password) = 0;

    /// @brief Force disconnects from the previously connected server and should release all used resources.
    /// @note Be aware that @ref Espressif_MQTT_Client automatically reconnects, as long as @ref Espressif_MQTT_Client::set_disable_auto_reconnect was not set to true
    virtual void disconnect() = 0;

    /// @brief Receives / sends any outstanding messages from and to the MQTT broker
    /// @note Only required if the MQTT client is blocking and does not use a seperate task to process messages.
    /// This is the case for @ref Arduino_MQTT_Client, which requires calls to this method to actually process the received and sent data.
    /// For the @ref Espressif_MQTT_Client however, this method simply does nothing and never needs to be called,
    /// because received and set data is processed using a seperate FreeRTOS task
    /// @return Whether sending or receiving the oustanding the messages was successful or not.
    /// Returns false if an internal error occured or the connection has been lost.
    /// Exact state can be read from @ref get_connection_state and @ref get_last_connection_error
    virtual bool loop() = 0;

    /// @brief Sends the given payload over the previously established connection
    /// @param topic Non owning pointer to topic that the message is sent over, where different MQTT topics expect a different kind of payload.
    /// Does not need to kept alive as the function copies the data into the outgoing MQTT buffer to publish the given payload
    /// @param payload Payload containg the data that should be sent
    /// @param length Length of the payload in bytes
    /// @return Whether publishing the payload on the given topic was successful or not
    virtual bool publish(char const * topic, uint8_t const * payload, size_t const & length) = 0;

    /// @brief Subscribes to MQTT message on the given topic, which will cause an internal callback to be called for each message received on that topic from the server,
    /// it should then, call the previously configured callback with set_data_callback() with the received data
    /// @param topic Non owning pointer to topic we want to receive a notification about if messages are sent by the server.
    /// Does not need to kept alive as the function copies the data into the outgoing MQTT buffer to subscribe to the given topic
    /// @return Wheter subscribing the given topic was possible or not, should return false and a warning should be printed,
    /// if the connection has been lost or the topic does not exist
    virtual bool subscribe(char const * topic) = 0;
  
    /// @brief Unsubscribes to previously subscribed MQTT messages of the given topic
    /// @param topic Non owning pointer to topic we want to stop receiving a notification about if messages are sent by the server
    /// Does not need to kept alive as the function copies the data into the outgoing MQTT buffer to unsubscribe from the given topic
    /// @return Wheter unsubscribing the given topic was possible or not, should return false and a warning should be printed,
    /// if the connection has been lost or the topic was not previously subscribed
    virtual bool unsubscribe(char const * topic) = 0;

    /// @brief Returns our current connection status to MQTT, true meaning we are connected,
    /// false meaning we have been disconnected or have not established a connection yet
    /// @return Whether the client is currently connected or not
    virtual bool connected() = 0;

    /// @brief Get the current connection state to the server includes possible intermediate states between connecting and disconnecting
    /// @note Only the @ref Espressif_MQTT_Client ever returns the intermediate states, because the implementation is non blocking.
    /// Meaning calling disconnect will not immediately disconnect from the cloud but instead require a while. In comparsion @ref Arduino_MQTT_Client is blocking, meaning we block until we disconnected or connected.
    /// If the ERROR state is returned, the reason for the failed connection can be deciphered more clearly using @ref get_last_connection_error
    /// @return The current state of the connection to the MQTT broker
    virtual MQTT_Connection_State get_connection_state() const = 0;

    /// @brief Allows to deciper the reason for a failure, while attempting to establish a connection to the MQTT broker
    /// @note Shows the reason for the last failure to connect or the current one, if @ref get_connection_state returns the ERROR state
    /// @return The last error that occured while attempting to establish a connection to MQTT
    virtual MQTT_Connection_Error get_last_connection_error() const = 0;

    /// @brief Sets the callback that is called, whenever the underlying state of our connection with the MQTT broker changes
    /// @note Is called when we for example attempt to connect to the MQTT broker, or once the underlying client has connected or failed to connect.
    /// Passes the current connection state, also accessible with @ref get_connection_state
    /// and the last error that occured while trying to connect, also accessible with @ref get_last_connection_error as additional information
    /// @param callback Method that should be called on state changes to our MQTT connection 
    virtual void subscribe_connection_state_changed_callback(Callback<void, MQTT_Connection_State, MQTT_Connection_Error>::function callback) = 0;

#if THINGSBOARD_ENABLE_STREAM_UTILS

    /// @brief Start to publish a message over a given topic, without being restricted to the internal buffer size
    /// @note Allows for arbitrarily large payloads to be sent without them having to be copied into a new buffer and held in memory.
    /// To use this feature first call begin_publish(), followed by multiple calls to write() and then ending with a call to end_publish()
    /// @param topic Non owning pointer to topic that the message is sent over, where different MQTT topics expect a different kind of payload.
    /// Does not need to kept alive as the function copies the data into the outgoing MQTT buffer to begin publishing to the given topic
    /// @param length Length of the payload in bytes
    /// @return Whether starting to publish on the given topic was successful or not
    virtual bool begin_publish(char const * topic, size_t const & length) = 0;

    /// @brief Finishes any publish message started with begin_publish()
    /// @return Whether the complete packet was sent successfully or not
    virtual bool end_publish() = 0;

    //----------------------------------------------------------------------------
    // Print interface
    //----------------------------------------------------------------------------

    /// @brief Sends a single byte of payload to be published, is meant to be used after having called begin_publish()
    /// @note Once the complete payload has been written ensure to call end_publish() to send any remaining bytes.
    /// Because payload bytes are sent one by one this method is extremly inefficient,
    /// if possible package the payload into bigger chunks and use the write() method with a non-owning pointer to an array instead
    /// @param payload_byte Byte containing part of the payload that should be sent
    /// @return The amount of bytes successfully written
    virtual size_t write(uint8_t payload_byte) = 0;

    /// @brief Sends a buffer containing multiple bytes of payload to be published, is meant to be used after having calling begin_publish()
    /// Once the complete payload has been written ensure to call end_publish() to send any remaining bytes
    /// @param buffer Non owning pointer to a buffer containing part of the payload that should be sent.
    /// Does not need to kept alive as the function copies the bytes into the outgoing MQTT buffer
    /// @param size Amount of bytes contained in the buffer that should be sent
    /// @return The amount of bytes successfully written
    virtual size_t write(uint8_t const * buffer, size_t const & size) = 0;

#endif // THINGSBOARD_ENABLE_STREAM_UTILS
};

#endif // IMQTT_Client_h
