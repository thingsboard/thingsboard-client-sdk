#include "Gateway_RPC_Callback.h"

#if THINGSBOARD_ENABLE_PROGMEM
constexpr char GATEWAY_RPC_CB_NULL[] PROGMEM = "Gateway RPC callback is NULL";
#else
constexpr char GATEWAY_RPC_CB_NULL[] = "Gateway RPC callback is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM

Gateway_RPC_Callback::Gateway_RPC_Callback()
    : Gateway_RPC_Callback(nullptr) {
  // Nothing to do
}

Gateway_RPC_Callback::Gateway_RPC_Callback(function cb)
    : Callback(cb, GATEWAY_RPC_CB_NULL) {
  // Nothing to do
}

bool Gateway_RPC_Callback::isCallbackSet() const { return m_callback != nullptr; }