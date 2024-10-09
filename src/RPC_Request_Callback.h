#ifndef RPC_Request_Callback_h
#define RPC_Request_Callback_h

// Local includes.
#include "Callback_Watchdog.h"


/// @brief Client-side RPC callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// Documentation about the specific use of client-side RPC in ThingsBoard can be found here https://thingsboard.io/docs/user-guide/rpc/#client-side-rpc
class RPC_Request_Callback : public Callback<void, JsonDocument const &> {
  public:
    /// @brief Constructs empty callback, will result in never being called. Internals are simply default constructed as nullptr
    RPC_Request_Callback() = default;

    /// @brief Constructs callback, will be called upon RPC response arrival originating
    /// from the original client side RPC request with any optional additional parameters that should be passed to the method
    /// @param method_name Name of the client side RPC method we want to call on the cloud
    /// @param received_callback Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    /// @param parameters Optional parameters that will be passed with the client side RPC call, use nullptr if there are no arguments for the given method, default = nullptr
    /// @param timeout_microseconds Optional amount of microseconds until we expect to have received a response and if we didn't, we call the previously subscribed callback.
    /// If the value is 0 we will not start the timer and therefore never call the timeout callback method, default = 0
    /// @param timeout_callback Optional callback method that will be called upon request timeout (did not receive a response in the given timeout time). Can happen if the requested method does not exist on the cloud,
    /// or if the connection could not be established, default = nullptr
    RPC_Request_Callback(char const * method_name, function received_callback, JsonArray const * parameters = nullptr, uint64_t const & timeout_microseconds = 0U, Callback_Watchdog::function timeout_callback = nullptr);

    /// @brief Gets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which RPC_Request_Callback
    /// is connected to which received client-side RPC response
    /// @return Unique identifier connected to the request for client side rpc
    size_t const & Get_Request_ID() const;

    /// @brief Sets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which RPC_Request_Callback
    /// is connected to which received client-side RPC response
    /// @param request_id Unique identifier connected to the request for client side rpc
    void Set_Request_ID(size_t const & request_id);

    /// @brief Gets the poiner to the underlying name of the client side RPC method we want to call on the cloud
    /// @return Pointer to the passed method name
    char const * Get_Name() const;

    /// @brief Sets the poiner to the underlying name of the client side RPC method we want to call on the cloud
    /// @param method_name Pointer to the passed method name
    void Set_Name(char const * method_name);

    /// @brief Gets the pointer to the underlying paramaters we want to call the client side RPC method on the cloud with
    /// @return Pointer to the passed parameters
    JsonArray const * Get_Parameters() const;

    /// @brief Sets the pointer to the underlying paramaters we want to call the client side RPC method on the cloud with
    /// @param parameters Pointer to the passed parameters
    void Set_Parameters(JsonArray const * parameters);

    /// @brief Gets the amount of microseconds until we expect to have received a response
    /// @return Timeout time until timeout callback is called
    uint64_t const & Get_Timeout() const;

    /// @brief Sets the amount of microseconds until we expect to have received a response
    /// @param timeout_microseconds Timeout time until timeout callback is called
    void Set_Timeout(uint64_t const & timeout_microseconds);

#if !THINGSBOARD_USE_ESP_TIMER
    /// @brief Updates the internal timeout timer
    void Update_Timeout_Timer();
#endif // !THINGSBOARD_USE_ESP_TIMER

    /// @brief Starts the internal timeout timer if we actually received a configured valid timeout time and a valid callback.
    /// Is called as soon as the request is actually sent
    void Start_Timeout_Timer();

    /// @brief Stops the internal timeout timer, is called as soon as an answer is received from the cloud
    /// if it isn't we call the previously subscribed callback instead
    void Stop_Timeout_Timer();

    /// @brief Sets the callback method that will be called upon request timeout (did not receive a response in the given timeout time)
    /// @param timeout_callback Callback function that will be called
    void Set_Timeout_Callback(Callback_Watchdog::function timeout_callback);

  private:
    char const                    *m_method_name = {};          // Method name
    JsonArray const               *m_parameters = {};          // Parameter json
    size_t                        m_request_id = {};           // Id the request was called with
    uint64_t                      m_timeout_microseconds = {}; // Timeout time until we expect response to request
    Callback_Watchdog             m_timeout_callback = {};     // Handles callback that will be called if request times out
};

#endif // RPC_Request_Callback_h
