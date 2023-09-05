// Header include.
#include "Callback_Watchdog.h"

#if THINGSBOARD_ENABLE_OTA

// Library includes.
#include <esp_timer.h>
#include <Arduino.h>

#if THINGSBOARD_ENABLE_PROGMEM
constexpr char WATCHDOG_TIMER_NAME[] PROGMEM = "watchdog_timer";
#else
constexpr char WATCHDOG_TIMER_NAME[] = "watchdog_timer";
#endif // THINGSBOARD_ENABLE_PROGMEM

Callback_Watchdog *Callback_Watchdog::m_instance = nullptr;

Callback_Watchdog::Callback_Watchdog(std::function<void(void)> callback) :
    m_callback(callback),
#if THINGSBOARD_USE_ESP_TIMER
    m_oneshot_timer(nullptr)
#else
    m_oneshot_timer()
#endif // THINGSBOARD_USE_ESP_TIMER
{
    m_instance = this;
#if THINGSBOARD_USE_ESP_TIMER
    const esp_timer_create_args_t oneshot_timer_args = {
            .callback = &oneshot_timer_callback,
            .name = WATCHDOG_TIMER_NAME
    };
    // Temporary handle is used, because it allows using a void* as the actual oneshot_timer,
    // allowing us to only include the esp_timer header in the defintion (.cpp) file,
    // instead of also needing to declare it in the declaration (.h) header file
    esp_timer_handle_t temp_handle;
    (void)esp_timer_create(&oneshot_timer_args, &temp_handle);
    m_oneshot_timer = temp_handle;
#endif // THINGSBOARD_USE_ESP_TIMER
}

Callback_Watchdog::~Callback_Watchdog() {
#if THINGSBOARD_USE_ESP_TIMER
    // Timer only has to deleted at the end of the lifetime of this object, to ensure no memory leak occurs.
    // But besides that the same timer can simply be stopped and restarted without needing to delete and create the timer again everytime.
    (void)esp_timer_delete(static_cast<esp_timer_handle_t>(m_oneshot_timer));
#else
    m_oneshot_timer.detach();
#endif // THINGSBOARD_USE_ESP_TIMER
    m_instance = nullptr;
}

void Callback_Watchdog::once(const uint64_t& timeout_microseconds) {
#if THINGSBOARD_USE_ESP_TIMER
    (void)esp_timer_start_once(static_cast<esp_timer_handle_t>(m_oneshot_timer), timeout_microseconds);
#else
    const uint32_t timeout_millis = timeout_microseconds / 1000U;
    m_oneshot_timer.once_ms(timeout_millis, &Callback_Watchdog::oneshot_timer_callback);
#endif // THINGSBOARD_USE_ESP_TIMER
}

void Callback_Watchdog::detach() {
#if THINGSBOARD_USE_ESP_TIMER
    (void)esp_timer_stop(static_cast<esp_timer_handle_t>(m_oneshot_timer));
#else
    m_oneshot_timer.detach();
#endif // THINGSBOARD_USE_ESP_TIMER
}

#if THINGSBOARD_USE_ESP_TIMER
void Callback_Watchdog::oneshot_timer_callback(void *arg) {
#else
void Callback_Watchdog::oneshot_timer_callback() {
#endif // THINGSBOARD_USE_ESP_TIMER
    if (m_instance == nullptr) {
        return;
    }

    m_instance->m_callback();
}

#endif // THINGSBOARD_ENABLE_OTA
