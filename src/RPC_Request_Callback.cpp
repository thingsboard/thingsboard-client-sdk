// Header include.
#include "RPC_Request_Callback.h"

#if THINGSBOARD_ENABLE_PROGMEM
char constexpr RPC_REQUEST_CB_NULL[] PROGMEM = "Client-side RPC request callback is NULL";
#else
char constexpr RPC_REQUEST_CB_NULL[] = "Client-side RPC request callback is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM

RPC_Request_Callback::RPC_Request_Callback(char const * const methodName, function callback) :
    RPC_Request_Callback(methodName, nullptr, callback)
{
    // Nothing to do
}

RPC_Request_Callback::RPC_Request_Callback(char const * const methodName, JsonArray const * const parameteres, function callback) :
    Callback(callback, RPC_REQUEST_CB_NULL),
    m_methodName(methodName),
    m_parameters(parameteres),
    m_request_id(0U)
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

void RPC_Request_Callback::Set_Parameters(JsonArray const * const parameteres) {
    m_parameters = parameteres;
}
