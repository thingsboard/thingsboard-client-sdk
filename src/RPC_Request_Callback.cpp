// Header include.
#include "RPC_Request_Callback.h"

RPC_Request_Callback::RPC_Request_Callback(char const * method_name, function received_callback, JsonArray const * parameters, uint64_t const & timeout_microseconds, Callback_Watchdog::function timeout_callback) :
    Callback(received_callback),
    m_method_name(method_name),
    m_parameters(parameters),
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
    return m_method_name;
}

void RPC_Request_Callback::Set_Name(char const * method_name) {
    m_method_name = method_name;
}

JsonArray const * RPC_Request_Callback::Get_Parameters() const {
    return m_parameters;
}

void RPC_Request_Callback::Set_Parameters(JsonArray const * parameters) {
    m_parameters = parameters;
}

uint64_t const & RPC_Request_Callback::Get_Timeout() const {
    return m_timeout_microseconds;
}

void RPC_Request_Callback::Set_Timeout(uint64_t const & timeout_microseconds) {
    m_timeout_microseconds = timeout_microseconds;
}

#if !THINGSBOARD_USE_ESP_TIMER
void RPC_Request_Callback::Update_Timeout_Timer() {
    m_timeout_callback.update();
}
#endif // !THINGSBOARD_USE_ESP_TIMER

void RPC_Request_Callback::Start_Timeout_Timer() {
    if (m_timeout_microseconds == 0U) {
        return;
    }
    m_timeout_callback.once(m_timeout_microseconds);
}

void RPC_Request_Callback::Stop_Timeout_Timer() {
    m_timeout_callback.detach();
}

void RPC_Request_Callback::Set_Timeout_Callback(Callback_Watchdog::function timeout_callback) {
    m_timeout_callback = Callback_Watchdog(timeout_callback);
}
