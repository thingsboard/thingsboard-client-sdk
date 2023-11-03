// Header include.
#include "RPC_Response.h"

RPC_Response::RPC_Response() :
    JsonDocument()
{
    // Nothing to do
}

RPC_Response::RPC_Response(JsonDocument document) :
    JsonDocument(document)
{
    // Nothing to do
}

RPC_Response::RPC_Response(Telemetry telemetry) {
    StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonBuffer;
    if (!telemetry.SerializeKeyValue(jsonBuffer)) {
        return;
    }
    this->set(jsonBuffer);
}
