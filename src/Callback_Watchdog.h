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


/// @brief Wrapper class which allows to start a timer and if it is not stopped in the given time then the callback that was passed will be called,
/// which informs the user of the failure to stop the timer in time, meaning a timeout has occured.
/// The class wraps around either the Arduino Ticker class from Arduino (https://github.com/sstaub/Ticker) or the offical ESP Timer implementation from Espressif (https://github.com/espressif/esp-idf/tree/2bc1f2f574/examples/system/esp_timer), the latter takes precendence if it exists.
/// This is done because the usage is more efficient, under the hood we simply start and then stop and if needed start a oneshot timer again.
/// When using the esp timer directly the same timer can be reused to stop and start and therefore does not need to create a new timer everytime,
/// the Ticker in comparsion automatically deletes a timer once it has been stopped and we therefore need to create it again once the timer is started.
/// This causes a lot of overhead therefore if the esp timer exists we do not use the Ticker class but instead the esp timer directly.
/// For all other use cases where the esp timer does not exists we instead use the Ticker as a fallback, because in that case its implementation is completly different anyway.
/// The class instance is meant to be started with once() which will then call the registered callback after the timeout has passed,
/// if the detach() method has not been called yet.
/// This results in behaviour similair to a esp task watchdog but without as high of an accuracy and without restarting the device,
/// allowing to let it fail and handle the error case silently by the user in the callback method.
/// Documentation about the specific use and caviates of the ESP Timer implementation can be found here https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_timer.html
class Callback_Watchdog {
  public:
    /// @brief Constructor
    /// @param callback Callback method that will be called if the timeout time passes without detach() being called
    Callback_Watchdog(std::function<void(void)> callback);

    /// @brief Destructor
    ~Callback_Watchdog();

    /// @brief Starts the watchdog timer once for the given timeout
    /// @param timeout_microseconds Amount of microseconds until the detach() method is excpected to have been called or the initally given callback method will be called
    void once(const uint64_t& timeout_microseconds);

    /// @brief Stops the currently ongoing watchdog timer and ensures the callback is not called. Timer can simply be restarted with calling once() again.
    void detach();

  private:
    std::function<void(void)> m_callback;
#if THINGSBOARD_USE_ESP_TIMER
    void *m_oneshot_timer;                // ESP Timer handle that is used to start and stop the oneshot timer
#else
    Ticker m_oneshot_timer;               // Ticker instance that handles the timer under the hood, if possible we directly use esp timer instead because it is more efficient
#endif // THINGSBOARD_USE_ESP_TIMER

    static Callback_Watchdog *m_instance; // Instance to the created class, will be set once the constructor has been called and reset once the destructor has been called, used to call private member method from static callback

#if THINGSBOARD_USE_ESP_TIMER

    /// @brief Creates and initally configures the timer, has to be done once before either esp_timer_start_once or esp_timer_stop is called
    /// It can not be created in the constructor, because that would possibly be called before we have executed the main app code, meaning the esp timer base is not initalized yet.
    /// This would result in an invalid configuration which would cause crashes when used in combination with once() or detach()
    void create_timer();
    
#endif // THINGSBOARD_USE_ESP_TIMER

    /// @brief Static callback used to call the initally subscribed callback, if the internal watchdog has not been reset in time with detach()
#if THINGSBOARD_USE_ESP_TIMER
    /// @param arg Possible argument passed to the timer callback is always nullptr, because we simply forward the call to an internal member method instead
    static void oneshot_timer_callback(void *arg);
#else
    static void oneshot_timer_callback();
#endif // THINGSBOARD_USE_ESP_TIMER
};

#endif // THINGSBOARD_ENABLE_OTA

#endif // Argument_Cache_h
