// Header include.
#include "RPC_Callback.h"

RPC_Callback::RPC_Callback() :
    RPC_Callback(nullptr, nullptr)
{
    // Nothing to do
}

RPC_Callback::RPC_Callback(const char *methodName, processFn cb) :
    m_methodName(methodName),
    m_cb(cb)
{
    // Nothing to do
}

void RPC_Callback::Set_Callback(processFn cb) {
    m_cb = cb;
}

const char* RPC_Callback::Get_Name() const {
    return m_methodName;
}

void RPC_Callback::Set_Name(const char *methodName) {
    m_methodName = methodName;
}
