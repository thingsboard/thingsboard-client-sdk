/*
  Callback_Watchdog.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Callback_Watchdog_h
#define Callback_Watchdog_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

// Library includes.
#include <functional>
#if !THINGSBOARD_USE_ESP_TIMER
#include <Ticker.h>
#endif // !THINGSBOARD_USE_ESP_TIMER


/// @brief Wrapper class around the esp_timer, allowing to start and stop timers. Is meant to be started with once(),
/// which will then call the registered callback after timeout if the detach() method has not been called before the time passed.
/// Resulting in behaviour similair to a watchdog but without as high of an accuracy and without restarting the device,
/// allowing to fail and handle the error case silently
class Callback_Watchdog {
  public:
    /// @brief Constructor
    /// @param callback Callback method that will be called if the timeout time passes without detach() being called
    Callback_Watchdog(std::function<void(void)> callback);

    /// @brief Destructor
    ~Callback_Watchdog();

    /// @brief Starts the watchdog timer once for the given timeout
    /// @param timeout_microseconds Amount of microseconds until the detach() method is excpected to have been called or the given callback method will be called
    void once(const uint64_t& timeout_microseconds);

    /// @brief Stops the currently ongoing watchdog timer and ensures the callback is not called. Timer can simply be restarted with calling once() again.
    void detach();

  private:
    std::function<void(void)> m_callback;
#if THINGSBOARD_USE_ESP_TIMER
    void *m_oneshot_timer;
#else
    Ticker m_oneshot_timer;
#endif // THINGSBOARD_USE_ESP_TIMER

    static Callback_Watchdog *m_instance;

    /// @brief Static callback used to call the initally subscribed callback, if the internal watchdog has not been fed with detach()
#if THINGSBOARD_USE_ESP_TIMER
    /// @param arg Possible argument passed to the timer callback
    static void oneshot_timer_callback(void *arg);
#else
    static void oneshot_timer_callback();
#endif // THINGSBOARD_USE_ESP_TIMER
};

#endif // THINGSBOARD_ENABLE_OTA

#endif // Argument_Cache_h
