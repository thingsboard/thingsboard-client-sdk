#ifndef RPC_Request_Callback_h
#define RPC_Request_Callback_h

// Local includes.
#include "Timeoutable_Request.h"


/// @brief Client-side RPC callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// Documentation about the specific use of client-side RPC in ThingsBoard can be found here https://thingsboard.io/docs/user-guide/rpc/#client-side-rpc
class RPC_Request_Callback : public Callback<void, JsonDocument const &> {
  public:
    /// @brief Constructs empty callback, will result in never being called. Internals are simply default constructed as nullptr
    RPC_Request_Callback() = default;

    /// @brief Constructs callback, will be called upon RPC response arrival originating
    /// from the original client side RPC request with any optional additional parameters that should be passed to the method
    /// @param method_name Non owning pointer to the name of the client-side RPC method we want to call on the cloud so that this method callback will be executed.
    /// Additionally it has to be kept alive by the user until the @ref RPC_Request method has been called with this instance as the argument, because that method copies the data into the outgoing MQTT buffer to create the client-side RPC request
    /// @param callback callback method that will be called upon data arrival with the given data that was received.
    /// If nullptr is passed the callback will never be called and instead return with a defaulted instance of the requested return variable
    /// @param parameters Optional Non-owning pointer to the paramaters we want to call the client-side RPC method with. Use nullptr if the client-side RPC method expects no arguments.
    /// Additionally it has to be kept alive by the user until the @ref RPC_Request method has been called with this instance as the argument, because that method copies the data into the outgoing MQTT buffer to create the client-side RPC request, default = nullptr
    /// @param timeout_microseconds Optional amount of microseconds until a response should have been received from the server, counted from the moment the request is sent.
    /// If a response is not received in the timeout time, the timeout callback method will be called to inform the user that the request has failed.
    /// If the value is 0 the timer will not be started and therefore never call the timeout callback method, default = 0
    /// @param timeout_callback Optional callback method that will be called upon request timeout (did not receive a response in the given timeout time). Can happen if the requested method does not exist on the cloud,
    /// or if the connection could not be established. A nullptr means even if a timeout occured the callback is simply ignored and the user not informed, default = nullptr
    RPC_Request_Callback(char const * method_name, function callback, JsonArray const * parameters = nullptr, uint64_t const & timeout_microseconds = 0U, Callback_Watchdog::function timeout_callback = nullptr);

    ~RPC_Request_Callback() override = default;

    /// @brief Gets the unique request identifier that is connected to the original request
    /// @note Will be later used to verifiy which @ref RPC_Request_Callback is connected to which received client-side RPC response
    /// @return Unique identifier connected to the requested client-side or shared attribute response
    size_t const & Get_Request_ID() const;

    /// @brief Sets the unique request identifier that is connected to the original request
    /// @note Will be later used to verifiy which @ref RPC_Request_Callback is connected to which received client-side RPC response.
    /// Not meant for external use, because the value is overwritten by internal method calls anyway once the class instance has been passed as a parameter anyway.
    /// This is the case because only the internal methods knows the current request id that this callback will be attached too
    /// @param request_id Unqiue identifier of the request for client-side RPC
    void Set_Request_ID(size_t const & request_id);

    /// @brief Gets the name of the client-side RPC method we want to call on the cloud so that this method callback will be executed
    /// @return Non owning pointer to the name of the client-side RPC method.
    /// Owned by the user that passed it originally in the constructor or with the @ref Set_Name method
    char const * Get_Name() const;

    /// @brief Sets the name of the client side RPC method we want to call on the cloud so that this method callback will be executed
    /// @param method_name Non owning pointer to the name of the client-side RPC method.
    /// Additionally it has to be kept alive by the user until the @ref RPC_Request method has been called with this instance as the argument, because that method copies the data into the outgoing MQTT buffer to create the client-side RPC request
    void Set_Name(char const * method_name);

    /// @brief Gets the paramaters we want to call the client-side RPC method on the cloud with
    /// @return Non-owning pointer to the paramaters we want to call the client-side RPC method with.
    /// Owned by the user that passed it originally in the constructor or with the @ref Set_Parameters method
    JsonArray const * Get_Parameters() const;

    /// @brief Sets the paramaters we want to call the client-side RPC method with on the cloud with
    /// @note The value of nullptr, means the client-side RPC method is called with no parameters
    /// @param parameters Non-owning pointer to the paramaters we want to call the client-side RPC method with
    /// Additionally it has to be kept alive by the user until the @ref RPC_Request method has been called with this instance as the argument, because that method copies the data into the outgoing MQTT buffer to create the client-side RPC request
    void Set_Parameters(JsonArray const * parameters);

    /// @brief Gets the request timeout callback
    /// @note Will be called when no response to the request was received in the expected amount of time, causing the internal watchdog to time out.
    /// To achieve this behaviour the internal timer can be started and stopped, and simply calls the subscribed callback if the timer is not stopped before it times out
    /// @return Request timeout callback
    Timeoutable_Request & Get_Request_Timeout();

  private:
    char const          *m_method_name = {};    // Method name
    JsonArray const     *m_parameters = {};     // Parameter json
    size_t              m_request_id = {};      // Id the request was called with
    Timeoutable_Request m_request_timeout = {}; // Handles callback that will be called if request times out
};

#endif // RPC_Request_Callback_h
