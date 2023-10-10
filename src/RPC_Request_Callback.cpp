// Header include.
#include "RPC_Request_Callback.h"

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char RPC_REQUEST_CB_NULL[] PROGMEM = "Client-side RPC request callback is NULL";
#else
constexpr char RPC_REQUEST_CB_NULL[] = "Client-side RPC request callback is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM

RPC_Request_Callback::RPC_Request_Callback() :
    RPC_Request_Callback(nullptr, nullptr, nullptr)
{
    // Nothing to do
}

RPC_Request_Callback::RPC_Request_Callback(const char *methodName, function callback) :
    RPC_Request_Callback(methodName, nullptr, callback)
{
    // Nothing to do
}

RPC_Request_Callback::RPC_Request_Callback(const char *methodName, const JsonArray *parameteres, function callback) :
    Callback(callback, RPC_REQUEST_CB_NULL),
    m_methodName(methodName),
    m_parameters(parameteres),
    m_request_id(0U)
{
    // Nothing to do
}

const size_t& RPC_Request_Callback::Get_Request_ID() const {
    return m_request_id;
}

void RPC_Request_Callback::Set_Request_ID(const size_t &request_id) {
    m_request_id = request_id;
}

const char* RPC_Request_Callback::Get_Name() const {
    return m_methodName;
}

void RPC_Request_Callback::Set_Name(const char *methodName) {
    m_methodName = methodName;
}

const JsonArray* RPC_Request_Callback::Get_Parameters() const {
    return m_parameters;
}

void RPC_Request_Callback::Set_Parameters(const JsonArray *parameteres) {
    m_parameters = parameteres;
}
