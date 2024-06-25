#ifndef Callback_Watchdog_h
#define Callback_Watchdog_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

// Library includes.
#include <functional>
#if !THINGSBOARD_USE_ESP_TIMER
#include <arduino-timer.h>
#endif // !THINGSBOARD_USE_ESP_TIMER


/// @brief Wrapper class which allows to start a timer and if it is not stopped in the given time then the callback that was passed will be called,
/// which informs the user of the failure to stop the timer in time, meaning a timeout has occured.
/// The class wraps around either the Arduino timer class from Arduino (https://github.com/contrem/arduino-timer) or the offical ESP Timer implementation from Espressif (https://github.com/espressif/esp-idf/tree/master/examples/system/esp_timer), the latter takes precendence if it exists.
/// This is done because it uses FreeRTOS to start the actual timer in the background, which removes the need for a Hardware Timer with Interrupts but still achieve the advantage of accurate timings and no need for active polling.
/// For all other use cases where the esp timer does not exists we instead use the Arduino timer as a fallback, because is is a simple software timer with active polling that works on all Arduino based devices,
/// because it simply uses the millis() method per default but can be configured over template arguments to use other methods that return the current time.
/// The class instance is meant to be started with once() which will then call the registered callback after the timeout has passed.
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
    void once(uint64_t const & timeout_microseconds);

    /// @brief Stops the currently ongoing watchdog timer and ensures the callback is not called. Timer can simply be restarted with calling once() again
    void detach();

#if !THINGSBOARD_USE_ESP_TIMER
    /// @brief Internally checks if the time already passed, has to be done because we are using a simple software timer.
    /// Indirectly called from the interal processing loop of this library, so we expect the user to recently often call the library loop() function.
    /// In the worst case the actuall call of the callback might be massively delayed compared to the original timer time
    void update();
#endif // !THINGSBOARD_USE_ESP_TIMER

  private:
    std::function<void(void)> m_callback; // Callback that should be called if the watchdog timeout expired without calling detach first
#if THINGSBOARD_USE_ESP_TIMER
    void *m_oneshot_timer;                // ESP Timer handle that is used to start and stop the oneshot timer
#else
    Timer<1, micros> m_oneshot_timer;     // Ticker instance that handles the timer under the hood, if possible we directly use esp timer instead because it is more efficient
#endif // THINGSBOARD_USE_ESP_TIMER

    static Callback_Watchdog *m_instance; // Instance to the created class, will be set once the constructor has been called and reset once the destructor has been called, used to call private member method from static callback

#if THINGSBOARD_USE_ESP_TIMER
    /// @brief Creates and initally configures the timer, has to be done once before either esp_timer_start_once or esp_timer_stop is called
    /// It can not be created in the constructor, because that would possibly be called before we have executed the main app code, meaning the esp timer base is not initalized yet.
    /// This would result in an invalid configuration which would cause crashes when used in combination with once() or detach()
    void create_timer();
#endif // THINGSBOARD_USE_ESP_TIMER

    /// @brief Static callback used to call the initally subscribed callback, if the internal watchdog has not been reset in time with detach()
    static void oneshot_timer_callback(void *arg);
};

#endif // THINGSBOARD_ENABLE_OTA

#endif // Argument_Cache_h
