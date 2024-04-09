#pragma once

#include "Callback.h"
#include "RPC_Response.h"

// JSON variant const (read only twice as small as JSON variant), is used to communicate server-side
// RPC parameters to the client
using RPC_Data = const JsonVariantConst;

class Gateway_RPC_Callback : public Callback<RPC_Response, RPC_Data&> {
  public:
  Gateway_RPC_Callback();
  Gateway_RPC_Callback(function cb);

  bool isCallbackSet() const;
};