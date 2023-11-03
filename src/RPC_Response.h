#ifndef RPC_RESPONSE_H
#define RPC_RESPONSE_H

// Local includes.
#include "Telemetry.h"


/// @brief RPC response expected to be sent by the user to the server once an RPC method has been called by the server,
/// is a simple wrapper around the Telemetry and JsonVariant class, which allow to easily serialize the response into a json string
class RPC_Response : public JsonVariant {
  public:
    /// @brief Constructor
    RPC_Response();

    /// @brief Constructor
    /// @param variant JsonVariant object the internal data should be copied from
    explicit RPC_Response(JsonVariant variant);

    /// @brief Constructor
    /// @param telemetry Telemetry object the internal data should be copied from
    explicit RPC_Response(Telemetry telemetry);

    /// @brief Constructor
    /// @tparam T Type of the passed value
    /// @param key Key of the key value pair we want to create
    /// @param value Value of the key value pair we want to create
    template <typename T>
    RPC_Response(const char *key, T value) :
        RPC_Response(Telemetry(key, value))
    {
        // Nothing to do
    }
};

#endif //RPC_RESPONSE_H
