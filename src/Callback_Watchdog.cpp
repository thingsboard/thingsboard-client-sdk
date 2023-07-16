// Header include.
#include "Callback_Watchdog.h"

Callback_Watchdog *Callback_Watchdog::m_instance = nullptr;

Callback_Watchdog::Callback_Watchdog(std::function<void(void)> callback) :
    m_callback(callback),
    m_ticker()
{
    m_instance = this;
}

void Callback_Watchdog::once(const uint32_t& timeout_millis) {
    m_ticker.once_ms(timeout_millis, &Callback_Watchdog::timerCallback);
}

void Callback_Watchdog::detach() {
    m_ticker.detach();
}

void Callback_Watchdog::timerCallback() {
    if (m_instance == nullptr) {
        return;
    }

    m_instance->m_callback();
}
