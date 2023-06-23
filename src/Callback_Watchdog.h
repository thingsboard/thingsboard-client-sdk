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
    /// @param timeout_millis Amount of milliseconds until the feed method is excpected to have been called or the given callback method will be called
    /// @param callback Callback method that will be called if the timeout time passes without being fed
    Callback_Watchdog(const uint32_t& timeout_millis, std::function<void(void)> callback)
      : m_timeout_millis(timeout_millis)
      , m_callback(callback)
      , m_ticker()
    {
        m_instance = this;
    }

    /// @brief Starts the watchdog timer once for the initally given timeout
    inline void once() {
        m_ticker.once_ms(m_timeout_millis, &Callback_Watchdog::timerCallback);
    }

    /// @brief Stops any currently ongoing watchdog timer
    inline void detach() {
        m_ticker.detach();
    }

  private:
    uint32_t m_timeout_millis;
    std::function<void(void)> m_callback;
    Ticker m_ticker;

    static Callback_Watchdog *m_instance;

    /// @brief Static callback used to call the initally subscribed callback, if the internal watchdog has not been fed
    inline static void timerCallback() {
        if (m_instance == nullptr) {
            return;
        }

        m_instance->m_callback();
    }
};

Callback_Watchdog *Callback_Watchdog::m_instance = nullptr;

#endif // THINGSBOARD_ENABLE_OTA

#endif // Argument_Cache_h
