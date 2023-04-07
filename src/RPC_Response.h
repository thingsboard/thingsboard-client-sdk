/*
  RPC_Response.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef RPC_RESPONSE_H
#define RPC_RESPONSE_H

#include <Telemetry.h>

class RPC_Response : public JsonVariant {
public:

    RPC_Response()
      : JsonVariant() {
    }

    RPC_Response(JsonVariant variant)
      : JsonVariant(variant) {
    }

    RPC_Response(Telemetry telemetry) {
        StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonBuffer;
        const JsonVariant object = jsonBuffer.to<JsonVariant>();
        if (!telemetry.SerializeKeyValue(object)) {
            return;
        }
        this->set(object);
    }

    template <typename T1, typename T2>
    RPC_Response(T1 type, const char *key, T2 value)
      : RPC_Response(Telemetry(type, key, value)) {
    }
};

#endif //RPC_RESPONSE_H
