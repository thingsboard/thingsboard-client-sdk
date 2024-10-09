#ifndef Callback_Watchdog_h
#define Callback_Watchdog_h

// Local includes.
#include "Callback.h"

// Library includes.
#if THINGSBOARD_USE_ESP_TIMER
#include <esp_timer.h>
#else
#include <arduino-timer.h>
#endif // THINGSBOARD_USE_ESP_TIMER


#if THINGSBOARD_USE_ESP_TIMER
constexpr char WATCHDOG_TIMER_NAME[] = "watchdog_timer";
#endif // THINGSBOARD_USE_ESP_TIMER


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
class Callback_Watchdog : public Callback<void> {
  public:
    /// @brief Constructs empty timeout timer callback, will result in never being called. Internals are simply default constructed as nullptr
    Callback_Watchdog() = default;

    /// @brief Constructs callback, will be called if the timeout time passes without detach() being called
    /// @param callback Callback method that will be called as soon as the internal software timers have processed that the given timeout time passed
    explicit Callback_Watchdog(function callback)
      : Callback(callback)
#if THINGSBOARD_USE_ESP_TIMER
      , m_oneshot_timer(nullptr)
#else
      , m_oneshot_timer()
#endif // THINGSBOARD_USE_ESP_TIMER
    {
        // Nothing to do
    }

#if THINGSBOARD_USE_ESP_TIMER
    /// @brief Destructor
    ~Callback_Watchdog() {
        // Timer only has to deleted at the end of the lifetime of this object, to ensure no memory leak occurs.
        // But besides that the same timer can simply be stopped and restarted without needing to delete and create the timer again everytime.
        (void)esp_timer_delete(m_oneshot_timer);
        m_oneshot_timer = nullptr;
    }
#endif // THINGSBOARD_USE_ESP_TIMER

    /// @brief Starts the watchdog timer once for the given timeout
    /// @param timeout_microseconds Amount of microseconds until the detach() method is excpected to have been called or the initally given callback method will be called
    void once(uint64_t const & timeout_microseconds) {
#if THINGSBOARD_USE_ESP_TIMER
        create_timer();
        (void)esp_timer_start_once(m_oneshot_timer, timeout_microseconds);
#else
        m_oneshot_timer.in(timeout_microseconds, &Callback_Watchdog::oneshot_timer_callback, this);
#endif // THINGSBOARD_USE_ESP_TIMER
    }

    /// @brief Stops the currently ongoing watchdog timer and ensures the callback is not called. Timer can simply be restarted with calling once() again
    void detach() {
#if THINGSBOARD_USE_ESP_TIMER
        (void)esp_timer_stop(m_oneshot_timer);
#else
        m_oneshot_timer.cancel();
#endif // THINGSBOARD_USE_ESP_TIMER
    }

#if !THINGSBOARD_USE_ESP_TIMER
    /// @brief Internally checks if the time already passed, has to be done because we are using a simple software timer.
    /// Indirectly called from the interal processing loop of this library, so we expect the user to recently often call the library loop() function.
    /// In the worst case the actuall call of the callback might be massively delayed compared to the original timer time
    void update() {
        m_oneshot_timer.tick<void>();
    }
#endif // !THINGSBOARD_USE_ESP_TIMER

  private:
#if THINGSBOARD_USE_ESP_TIMER
    /// @brief Creates and initally configures the timer, has to be done once before either esp_timer_start_once or esp_timer_stop is called
    /// It can not be created in the constructor, because that would possibly be called before we have executed the main app code, meaning the esp timer base is not initalized yet.
    /// This would result in an invalid configuration which would cause crashes when used in combination with once() or detach()
    void create_timer() {
        // Timer has already been created previously there is no need to create it again
        if (m_oneshot_timer != nullptr) {
            return;
        }

        const esp_timer_create_args_t oneshot_timer_args = {
            .callback = &oneshot_timer_callback,
            .arg = this,
            .dispatch_method = esp_timer_dispatch_t::ESP_TIMER_TASK,
            .name = WATCHDOG_TIMER_NAME,
            .skip_unhandled_events = false
        };

        const esp_err_t error = esp_timer_create(&oneshot_timer_args, &m_oneshot_timer);
        if (error != ESP_OK) {
            return;
        }
    }
#endif // THINGSBOARD_USE_ESP_TIMER

    /// @brief Static callback used to call the initally subscribed callback, if the internal watchdog has not been reset in time with detach()
#if THINGSBOARD_USE_ESP_TIMER
    static void
#else
    /// @return Whether we want to simply reset the internal time and start the timer once again, always false because we only use oneshot timers internally
    static  bool
#endif // THINGSBOARD_USE_ESP_TIMER
    oneshot_timer_callback(void *arg) {
        if (arg == nullptr) {
#if THINGSBOARD_USE_ESP_TIMER
            return;
#else
            return false;
#endif // THINGSBOARD_USE_ESP_TIMER
        }

        auto instance = static_cast<Callback_Watchdog *>(arg);
        instance->Call_Callback();
#if !THINGSBOARD_USE_ESP_TIMER
        return false;
#endif // !THINGSBOARD_USE_ESP_TIMER
    }

#if THINGSBOARD_USE_ESP_TIMER
    esp_timer_handle_t m_oneshot_timer = {}; // ESP Timer handle that is used to start and stop the oneshot timer
#else
    Timer<1, micros>   m_oneshot_timer = {}; // Ticker instance that handles the timer under the hood, if possible we directly use esp timer instead because it is more efficient
#endif // THINGSBOARD_USE_ESP_TIMER
};

#endif // Argument_Cache_h
