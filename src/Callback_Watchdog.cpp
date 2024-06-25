// Header include.
#include "Callback_Watchdog.h"

#if THINGSBOARD_ENABLE_OTA

#if THINGSBOARD_USE_ESP_TIMER
// Library includes.
#include <esp_timer.h>

constexpr char WATCHDOG_TIMER_NAME[] = "watchdog_timer";
#endif // THINGSBOARD_USE_ESP_TIMER

Callback_Watchdog::Callback_Watchdog(std::function<void(void)> callback)
  : m_callback(callback)
#if THINGSBOARD_USE_ESP_TIMER
  , m_oneshot_timer(nullptr)
#else
  , m_oneshot_timer()
#endif // THINGSBOARD_USE_ESP_TIMER
{
    // Nothing to do
}

Callback_Watchdog::~Callback_Watchdog() {
#if THINGSBOARD_USE_ESP_TIMER
    // Timer only has to deleted at the end of the lifetime of this object, to ensure no memory leak occurs.
    // But besides that the same timer can simply be stopped and restarted without needing to delete and create the timer again everytime.
    (void)esp_timer_delete(static_cast<esp_timer_handle_t>(m_oneshot_timer));
    m_oneshot_timer = nullptr;
#endif // THINGSBOARD_USE_ESP_TIMER
}

void Callback_Watchdog::once(uint64_t const & timeout_microseconds) {
#if THINGSBOARD_USE_ESP_TIMER
    create_timer();
    (void)esp_timer_start_once(static_cast<esp_timer_handle_t>(m_oneshot_timer), timeout_microseconds);
#else
    m_oneshot_timer.in(timeout_microseconds, &Callback_Watchdog::oneshot_timer_callback, this);
#endif // THINGSBOARD_USE_ESP_TIMER
}

void Callback_Watchdog::detach() {
#if THINGSBOARD_USE_ESP_TIMER
    (void)esp_timer_stop(static_cast<esp_timer_handle_t>(m_oneshot_timer));
#else
    m_oneshot_timer.cancel();
#endif // THINGSBOARD_USE_ESP_TIMER
}

#if !THINGSBOARD_USE_ESP_TIMER
void Callback_Watchdog::update() {
    m_oneshot_timer.tick();
}
#endif // !THINGSBOARD_USE_ESP_TIMER

#if THINGSBOARD_USE_ESP_TIMER
void Callback_Watchdog::create_timer() {
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

    // Temporary handle is used, because it allows using a void* as the actual oneshot_timer,
    // allowing us to only include the esp_timer header in the defintion (.cpp) file,
    // instead of also needing to declare it in the declaration (.h) header file
    esp_timer_handle_t temp_handle;
    const esp_err_t error = esp_timer_create(&oneshot_timer_args, &temp_handle);
    if (error != ESP_OK) {
        return;
    }

    m_oneshot_timer = temp_handle;
}
#endif // THINGSBOARD_USE_ESP_TIMER

#if THINGSBOARD_USE_ESP_TIMER
void
#else
bool
#endif // THINGSBOARD_USE_ESP_TIMER
Callback_Watchdog::oneshot_timer_callback(void *arg) {
    if (arg == nullptr) {
        return;
    }
    auto instance = static_cast<Callback_Watchdog *>(arg);
    instance->m_callback();
#if !THINGSBOARD_USE_ESP_TIMER
    // Returning true will simply reset the internal time and start the timer once again,
    // where as returning false will remove the timer internally so it can be added again
    return false;
#endif // !THINGSBOARD_USE_ESP_TIMER
}

#endif // THINGSBOARD_ENABLE_OTA
