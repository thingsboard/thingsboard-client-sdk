// Header include.
#include "RPC_Request_Callback.h"

RPC_Request_Callback::RPC_Request_Callback() :
    RPC_Request_Callback(nullptr, nullptr, nullptr)
{
    // Nothing to do
}

RPC_Request_Callback::RPC_Request_Callback(const char *methodName, processFn cb) :
    RPC_Request_Callback(methodName, nullptr, cb)
{
    // Nothing to do
}

RPC_Request_Callback::RPC_Request_Callback(const char *methodName, const JsonArray *parameteres, processFn cb) :
    m_methodName(methodName),
    m_parameters(parameteres),
    m_request_id(0U),
    m_cb(cb)
{
    // Nothing to do
}

void RPC_Request_Callback::Set_Callback(processFn cb) {
    m_cb = cb;
}

const uint32_t& RPC_Request_Callback::Get_Request_ID() const {
    return m_request_id;
}

void RPC_Request_Callback::Set_Request_ID(const uint32_t &request_id) {
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

void RPC_Request_Callback::Get_Parameters(const JsonArray *parameteres) {
    m_parameters = parameteres;
}
