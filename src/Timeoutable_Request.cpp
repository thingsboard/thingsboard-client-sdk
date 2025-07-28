// Header include.
#include "Timeoutable_Request.h"

Timeoutable_Request::Timeoutable_Request(uint64_t const & timeout_microseconds, Callback_Watchdog::function timeout_callback)
  : m_timeout_microseconds(timeout_microseconds)
  , m_timeout_callback(timeout_callback)
{
    // Nothing to do
}

uint64_t const & Timeoutable_Request::Get_Timeout() const {
    return m_timeout_microseconds;
}

void Timeoutable_Request::Set_Timeout(uint64_t const & timeout_microseconds) {
    m_timeout_microseconds = timeout_microseconds;
}

#if !THINGSBOARD_USE_ESP_TIMER
void Timeoutable_Request::Update_Timeout_Timer() {
    m_timeout_callback.update();
}
#endif // !THINGSBOARD_USE_ESP_TIMER

void Timeoutable_Request::Start_Timeout_Timer() {
    if (m_timeout_microseconds == 0U) {
        return;
    }
    m_timeout_callback.once(m_timeout_microseconds);
}

void Timeoutable_Request::Stop_Timeout_Timer() {
    m_timeout_callback.detach();
}

void Timeoutable_Request::Set_Timeout_Callback(Callback_Watchdog::function timeout_callback) {
    m_timeout_callback.Set_Callback(timeout_callback);
}
