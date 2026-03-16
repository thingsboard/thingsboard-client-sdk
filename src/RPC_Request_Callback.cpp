// Header include.
#include "RPC_Request_Callback.h"

RPC_Request_Callback::RPC_Request_Callback(char const * method_name, function callback, JsonArray const * parameters, uint64_t const & timeout_microseconds, Callback_Watchdog::function timeout_callback) :
    Callback(callback),
    m_method_name(method_name),
    m_parameters(parameters),
    m_request_id(0U),
    m_request_timeout(timeout_microseconds, timeout_callback)
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

Timeoutable_Request & RPC_Request_Callback::Get_Request_Timeout() {
    return m_request_timeout;
}
