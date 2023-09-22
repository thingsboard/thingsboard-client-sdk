#ifndef IUpdater_h
#define IUpdater_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

// Library include.
#include <stddef.h>
#include <stdint.h>


/// @brief Updater interface that contains the method that a class that can be used to flash given binary data onto a device has to implement
class IUpdater {
  public:
    /// @brief Initalizes the writing of the given data
    /// @param firmware_size Total size of the data that should be written, is done in multiple packets
    /// @return Whether initalizing the update was successful or not
    virtual bool begin(const size_t& firmware_size) = 0;
  
    /// @brief Writes the given amount of bytes of the packet data
    /// @param payload Firmware packet data that should be written
    /// @param total_bytes Amount of bytes in the current firmware packet data
    /// @return Total amount of bytes that were successfully written
    virtual size_t write(uint8_t* payload, const size_t& total_bytes) = 0;
  
    /// @brief Resets the writing of the given data so it can be restarted with begin
    virtual void reset() = 0;
  
    /// @brief Ends the update and returns wheter it was successfully completed
    /// @return Whether the complete amount of bytes initally given was successfully written or not
    virtual bool end() = 0;
};

#endif // THINGSBOARD_ENABLE_OTA

#endif // IUpdater_h
