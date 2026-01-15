#ifndef MQTT_Connection_State_h
#define MQTT_Connection_State_h

// Library include.
#include <stdint.h>


/// @brief Possible states the MQTT broker connection can be in
/// @note Intermediate states connecting and disconnecting, are for the @ref Espressif_MQTT_Client which is non blocking.
/// Meaning calling disconnect will not immediately disconnect from the cloud but instead require a while. In comparsion @ref Arduino_MQTT_Client is blocking, meaning we block until we disconnected or connected
enum class MQTT_Connection_State : uint8_t {
    DISCONNECTED, ///< Not yet connected or force disconnected from the MQTT broker. Loosing connection unexpectedly results in the ERROR state instead
    CONNECTING, ///< Received request to connect to the MQTT broker and connection is currently ongoing. Once successfull will be in CONNECTED state or alternatively in ERROR state if the connection failed
    CONNECTED, ///< Device has successfully connected to the MQTT broker. Should stay in this state until force disconnect is received or until error occurs in the connection to the MQTT broker
    DISCONNECTING, ///< Received request to force disconnect from the MQTT broker. Should always result in DISCONNECTED state once processed
    ERROR ///< State for failure to connect to the MQTT broker or loosing connection unexpectedly, can be deciphered in more detail using @ref MQTT_Connection_Error
};

#endif // MQTT_Connection_State_h
