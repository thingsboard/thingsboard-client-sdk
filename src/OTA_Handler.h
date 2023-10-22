#ifndef OTA_Handler_h
#define OTA_Handler_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

// Local include.
#include "Callback_Watchdog.h"
#include "HashGenerator.h"
#include "Helper.h"
#include "ILogger.h"
#include "OTA_Update_Callback.h"
#include "OTA_Failure_Response.h"


// Firmware data keys.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char FW_STATE_FAILED[] PROGMEM = "FAILED";
#else
constexpr char FW_STATE_FAILED[] = "FAILED";
#endif // THINGSBOARD_ENABLE_PROGMEM


/// @brief Handles the complete processing of received binary firmware data, including flashing it onto the device,
/// creating a hash of the received data and in the end ensuring that the complete OTA firmware was flashes successfully and that the hash is the one we initally received
class OTA_Handler {
  public:
    /// @brief Constructor
    /// @param logger Logger implementation that should be used to print messages generated by internal processes
    /// @param publish_callback Callback that is used to request the firmware chunk of the firmware binary with the given chunk number
    /// @param send_fw_state_callback Callback that is used to send information about the current state of the over the air update
    /// @param finish_callback Callback that is called once the update has been finished and the user should be informed of the failure or success of the over the air update
    OTA_Handler(const ILogger& logger, std::function<bool(const size_t&)> publish_callback, std::function<bool(const char *, const char *)> send_fw_state_callback, std::function<bool(void)> finish_callback);

    /// @brief Starts the firmware update with requesting the first firmware packet and initalizes the underlying needed components
    /// @param fw_callback Callback method that contains configuration information, about the over the air update
    /// @param fw_size Complete size of the firmware binary that will be downloaded and flashed onto this device
    /// @param fw_algorithm String of the algorithm type used to hash the firmware binary
    /// @param fw_checksum Checksum of the complete firmware binary, should be the same as the actually written data in the end
    /// @param fw_checksum_algorithm Algorithm type used to hash the firmware binary
    void Start_Firmware_Update(const OTA_Update_Callback& fw_callback, const size_t& fw_size, const std::string& fw_algorithm, const std::string& fw_checksum, const mbedtls_md_type_t& fw_checksum_algorithm);

    /// @brief Stops the firmware update completly and informs that user that the update has failed because it has been aborted, ongoing communication is discarded.
    /// Be aware the written partition is not erased so the already written binary firmware data still remains in the flash partition,
    /// shouldn't really matter, because if we start the update process again the partition will be overwritten anyway and a partially written firmware will not be bootable
    void Stop_Firmware_Update();

    /// @brief Uses the given firmware packet data and process it. Starting with writing the given amount of bytes of the packet data into flash memory and
    /// into a hash function that will be used to compare the expected complete binary file and the actually received binary file
    /// @param current_chunk Index of the chunk we recieved the binary data for
    /// @param payload Firmware packet data of the current chunk
    /// @param total_bytes Amount of bytes in the current firmware packet data
    void Process_Firmware_Packet(const size_t& current_chunk, uint8_t *payload, const size_t& total_bytes);

  private:
    const OTA_Update_Callback *m_fw_callback;                                 // Callback method that contains configuration information, about the over the air update
    const ILogger& m_logger;                                                  // Logging instance used to print messages 
    std::function<bool(const size_t&)> m_publish_callback;                    // Callback that is used to request the firmware chunk of the firmware binary with the given chunk number
    std::function<bool(const char *, const char *)> m_send_fw_state_callback; // Callback that is used to send information about the current state of the over the air update
    std::function<bool(void)> m_finish_callback;                              // Callback that is called once the update has been finished and the user should be informed of the failure or success of the over the air update
    size_t m_fw_size;                                                         // Total size of the firmware binary we will receive. Allows for a binary size of up to theoretically 4 GB
    std::string m_fw_algorithm;                                               // String of the algorithm type used to hash the firmware binary
    std::string m_fw_checksum;                                                // Checksum of the complete firmware binary, should be the same as the actually written data in the end
    mbedtls_md_type_t m_fw_checksum_algorithm;                                // Algorithm type used to hash the firmware binary
    IUpdater *m_fw_updater;                                                   // Interface implementation that writes received firmware binary data onto the given device
    HashGenerator m_hash;                                                     // Class instance that allows to generate a hash from received firmware binary data
    size_t m_total_chunks;                                                    // Total amount of chunks that need to be received to get the complete firmware binary
    size_t m_requested_chunks;                                                // Amount of successfully requested and received firmware binary chunks
    uint8_t m_retries;                                                        // Amount of request retries we attempt for each chunk, increasing makes the connection more stable
    Callback_Watchdog m_watchdog;                                             // Class instances that allows to timeout if we do not receive a response for a requested chunk in the given time

    /// @brief Restarts or starts the firmware update and its needed components and then requests the first firmware chunk
    void Request_First_Firmware_Packet();

    /// @brief Requests the next firmware chunk of the OTA firmware if there are any left
    /// and starts the timer that ensures we request the same chunk again if we have not received a response yet
    void Request_Next_Firmware_Packet();

    /// @brief Completes the firmware update, which consists of checking the complete hash of the firmware binary if the initally received value,
    /// both should be the same and if that is not the case that means that we received invalid firmware binary data and have to restart the update.
    /// If checking the hash was successfull we attempt to finish flashing the ota partition and then inform the user that the update was successfull
    void Finish_Firmware_Update();

    /// @brief Handles errors with the received failure response so that the firmware update can regenerate from any possible issue.
    /// Will only execute the given failure response as long as there are still retries remaining, if there are not any further issue will cause the update to be aborted
    /// @param failure_response Possible response to a failure that the method should handle
    void Handle_Failure(const OTA_Failure_Response& failure_response);

    /// @brief Callback that will be called if we did not receive the firmware chunk response in the given timeout time
    void Handle_Request_Timeout();
};

#endif // THINGSBOARD_ENABLE_OTA

#endif // OTA_Handler_h
