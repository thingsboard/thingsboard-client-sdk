// Header include.
#include "RPC_Response.h"

RPC_Response::RPC_Response()
  : JsonVariant()
{
    // Nothing to do
}

RPC_Response::RPC_Response(JsonVariant const & variant)
  : JsonVariant(variant)
{
    // Nothing to do
}

RPC_Response::RPC_Response(Telemetry const & telemetry) {
    StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonBuffer;
    const JsonVariant object = jsonBuffer.to<JsonVariant>();
    if (!telemetry.SerializeKeyValue(object)) {
        return;
    }
    this->set(object);
}
