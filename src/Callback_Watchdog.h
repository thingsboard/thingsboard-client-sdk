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
#include <Ticker.h>

class Callback_Watchdog {
  public:
    /// @brief Constructor
    /// @param callback Callback method that will be called if the timeout time passes without being fed
    Callback_Watchdog(std::function<void(void)> callback);

    /// @brief Starts the watchdog timer once for the initally given timeout
    /// @param timeout_millis Amount of milliseconds until the feed method is excpected to have been called or the given callback method will be called
    void once(const uint32_t& timeout_millis);

    /// @brief Stops any currently ongoing watchdog timer
    void detach();

  private:
    std::function<void(void)> m_callback;
    Ticker m_ticker;

    static Callback_Watchdog *m_instance;

    /// @brief Static callback used to call the initally subscribed callback, if the internal watchdog has not been fed
    static void timerCallback();
};

#endif // THINGSBOARD_ENABLE_OTA

#endif // Argument_Cache_h
