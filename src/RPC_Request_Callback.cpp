// Header include.
#include "RPC_Request_Callback.h"

RPC_Request_Callback::RPC_Request_Callback(char const * const methodName, function received_callback, JsonArray const * const parameters, uint64_t const & timeout_microseconds, Callback_Watchdog::function timeout_callback) :
    Callback(callback),
    m_methodName(methodName),
    m_parameters(parameteres),
    m_request_id(0U),
    m_timeout_microseconds(timeout_microseconds),
    m_timeout_callback(timeout_callback)
{
    // Nothing to do
}

size_t const & RPC_Request_Callback::Get_Request_ID() const {
    return m_request_id;
}

void RPC_Request_Callback::Set_Request_ID(size_t const & request_id) {
    m_request_id = request_id;
}

char const * RPC_Request_Callback::Get_Name() const {
    return m_methodName;
}

void RPC_Request_Callback::Set_Name(char const * methodName) {
    m_methodName = methodName;
}

JsonArray const * RPC_Request_Callback::Get_Parameters() const {
    return m_parameters;
}

void RPC_Request_Callback::Set_Parameters(JsonArray const * const parameters) {
    m_parameters = parameters;
}

uint64_t const & RPC_Request_Callback::Get_Timeout() const {
    return m_timeout_microseconds;
}

void RPC_Request_Callback::Set_Timeout(uint64_t const & timeout_microseconds) {
    m_timeout_microseconds = timeout_microseconds;
}

#if !THINGSBOARD_USE_ESP_TIMER
void Update_Timeout_Timer() {
    m_timeout_callback.update();
}
#endif // !THINGSBOARD_USE_ESP_TIMER

void Start_Timeout_Timer() {
    if (m_timeout_microseconds == 0U) {
        return;
    }
    m_timeout_callback.once(m_timeout_microseconds);
}

void Stop_Timeout_Timer() {
    m_timeout_callback.detach();
}

void RPC_Request_Callback::Set_Timeout_Callback(Callback_Watchdog::function timeout_callback) {
    m_timeout_callback = Callback_Watchdog(timeout_callback);
}
