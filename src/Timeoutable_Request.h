#ifndef Timeoutable_Request_h
#define Timeoutable_Request_h

// Local includes.
#include "Callback_Watchdog.h"


/// @brief General purpose request callback that can timeout if the response to the request is not received
/// in the expected amount of time and the internal watchdog times out
class Timeoutable_Request {
  public:
    /// @brief Constructs request timeout callback
    /// @note Will be called when no response to the request was received in the expected amount of time, causing the internal watchdog to time out.
    /// To achieve this behaviour the internal timer can be started and stopped, and simply calls the subscribed callback if the timer is not stopped before it times out
    /// @param timeout_microseconds Amount of microseconds until a response should have been received from the server, counted from the moment the request is sent.
    /// If a response is not received in the timeout time, the timeout callback method will be called to inform the user that the request has failed.
    /// If the value is 0 the timer will not be started and therefore never call the timeout callback method, default = 0
    /// @param timeout_callback Callback method that will be called upon request timeout (did not receive a response in the given timeout time). Can happen if the requested method does not exist on the cloud,
    /// or if the connection could not be established. A nullptr means even if a timeout occured the callback is simply ignored and the user not informed, default = nullptr
    Timeoutable_Request(uint64_t const & timeout_microseconds = 0U, Callback_Watchdog::function timeout_callback = nullptr);

    /// @brief Gets the amount of microseconds until we expect to have received a response
    /// @return Timeout time until timeout callback is called
    uint64_t const & Get_Timeout() const;

    /// @brief Sets the amount of microseconds until we expect to have received a response
    /// @note The value of 0, means the timeout timer is never started and therefore the timeout callback never called
    /// @param timeout_microseconds Timeout time until timeout callback is called
    void Set_Timeout(uint64_t const & timeout_microseconds);

#if !THINGSBOARD_USE_ESP_TIMER
    /// @brief Updates the internal timeout timer
    void Update_Timeout_Timer();
#endif // !THINGSBOARD_USE_ESP_TIMER

    /// @brief Starts the internal timeout timer if we actually received a configured valid timeout time and a valid callback.
    /// @note The timeout time is valid if it is not 0 and the callback is valid if it is not a nullptr.
    /// Is not mean to be called explicitly by the user, because it is instead called when necessary by internal methods that handle the class instance
    void Start_Timeout_Timer();

    /// @brief Stops the internal timeout timer, is called as soon as an answer is received from the cloud.
    /// If this method is not called in time the initally subscribed callback will be used to inform the user of the timeout instead
    /// @note Is not mean to be called explicitly by the user, because it is instead called when necessary by internal methods that handle the class instance
    void Stop_Timeout_Timer();

    /// @brief Sets the callback method that will be called upon request timeout (did not receive a response from the cloud in the given timeout time)
    /// @param timeout_callback Callback function that will be called
    void Set_Timeout_Callback(Callback_Watchdog::function timeout_callback);

  private:
    uint64_t          m_timeout_microseconds = {}; // Timeout time until we expect response to request
    Callback_Watchdog m_timeout_callback = {};     // Handles callback that will be called if request times out
};

#endif // Timeoutable_Request_h
