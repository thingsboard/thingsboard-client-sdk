// Header include.
#include "RPC_Callback.h"

#if THINGSBOARD_ENABLE_PROGMEM
char constexpr RPC_CB_NULL[] PROGMEM = "Server-side RPC callback is NULL";
#else
char constexpr RPC_CB_NULL[] = "Server-side RPC callback is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM

RPC_Callback::RPC_Callback()
  : RPC_Callback(nullptr, nullptr)
{
    // Nothing to do
}

RPC_Callback::RPC_Callback(char const * const methodName, function cb)
  : Callback(cb, RPC_CB_NULL)
  , m_methodName(methodName)
{
    // Nothing to do
}
const char* RPC_Callback::Get_Name() const {
    return m_methodName;
}

void RPC_Callback::Set_Name(char const * const methodName) {
    m_methodName = methodName;
}
