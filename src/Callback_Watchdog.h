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
#include <Arduino.h>
#include <functional>
#include <Ticker.h>

class Callback_Watchdog {
  public:
    /// @brief Constructor
    Callback_Watchdog()
      : m_timeout_millis(0U)
      , m_callback(nullptr)
      , m_ticker()
      , m_last_feed_millis(0)
    {
        m_instance = this;
    }

    /// @brief Starts the watchdog timer
    /// @param timeout_millis Amount of milliseconds until the feed method is excpected to have been called or the given callback method will be called
    /// @param callback Callback method that will be called if the timeout time passes without being fed
    inline void start(const uint32_t& timeout_millis, std::function<void(void)> callback) {
        m_timeout_millis = timeout_millis;
        m_callback = callback;
        m_ticker.attach_ms(m_timeout_millis, &Callback_Watchdog::timerCallback);
        m_last_feed_millis = millis();
    }

    /// @brief Resets the watchdog timer expected to be called before the initally given timeout time is expended
    inline void feed() {
        m_last_feed_millis = millis();
    }

  private:
    uint32_t m_timeout_millis;
    std::function<void(void)> m_callback;
    Ticker m_ticker;
    uint32_t m_last_feed_millis;

    static Callback_Watchdog *m_instance;

    /// @brief Static callback used to call the initally subscribed callback, if the internal watchdog has not been fed
    inline static void timerCallback() {
        if (m_instance == nullptr) {
            return;
        }

        if (millis() - m_instance->m_last_feed_millis < m_instance->m_timeout_millis) {
            return;
        }

        m_instance->m_callback();
    }
};

Callback_Watchdog *Callback_Watchdog::m_instance = nullptr;

#endif // THINGSBOARD_ENABLE_OTA

#endif // Argument_Cache_h
