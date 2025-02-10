#ifndef MQTT_Connection_Error_h
#define MQTT_Connection_Error_h

// Library include.
#include <stdint.h>


/// @brief Possible error states the MQTT broker connection can be in, if connecting the device to the MQTT broker failed or the device lost connection to the MQTT broker.
enum class MQTT_Connection_Error : uint8_t {
    NONE, ///< Connection accepted
    REFUSE_PROTOCOL, ///< Connection failed because of wrong protocol
    REFUSE_ID_REJECTED, ///< Connection failed because of rejected ID
    REFUSE_SERVER_UNAVAILABLE, ///< Connection failed because of unavailable server
    REFUSE_BAD_USERNAME, ///< Connection failed because of wrong user
    REFUSE_NOT_AUTHORIZED ///< Connection failed because of wrong username or password
};

#endif // MQTT_Connection_Error_h
