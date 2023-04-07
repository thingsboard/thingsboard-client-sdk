//
// Created by imbeacon on 07.04.23.
//

#ifndef RPC_RESPONSE_H
#define RPC_RESPONSE_H

#include <Telemetry.h>

class RPC_Response : public JsonVariant {
public:

    RPC_Response() {
        JsonVariant();
    }

    RPC_Response(Telemetry telemetry) {
        StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
        telemetry.SerializeKeyValue(doc);
        this->set(doc.as<JsonObject>());
    }

    template <typename T1, typename T2>
    RPC_Response(T1 key, T2 value) {
        RPC_Response(Telemetry(key, value));
    }
};

#endif //RPC_RESPONSE_H
