// Header include.
#include "RPC_Callback.h"

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char RPC_CB_NULL[] PROGMEM = "Server-side RPC callback is NULL";
#else
constexpr char RPC_CB_NULL[] = "Server-side RPC callback is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM

RPC_Callback::RPC_Callback() :
    RPC_Callback(nullptr, nullptr)
{
    // Nothing to do
}

RPC_Callback::RPC_Callback(const char *methodName, function cb) :
    Callback(cb, RPC_CB_NULL),
    m_methodName(methodName)
{
    // Nothing to do
}
const char* RPC_Callback::Get_Name() const {
    return m_methodName;
}

void RPC_Callback::Set_Name(const char *methodName) {
    m_methodName = methodName;
}
