#ifndef OTA_Update_Callback_h
#define OTA_Update_Callback_h

// Local includes.
#include "Callback.h"
#include "IUpdater.h"
#include "Timeoutable_Request.h"


// OTA default values.
uint8_t constexpr CHUNK_RETRIES = 12U;
uint16_t constexpr CHUNK_SIZE = (4U * 1024U);
uint64_t constexpr REQUEST_TIMEOUT_MS = (5U * 1000U * 1000U);


/// @brief Over the air firmware update callback wrapper
/// @note Contains the needed configuration settings to create the request that should be sent to the server.
/// Documentation about the specific use of Over the air updates in ThingsBoard can be found here https://thingsboard.io/docs/user-guide/ota-updates/
class OTA_Update_Callback : public Callback<void, bool const &> {
  public:
    /// @brief Constructs empty callbacks, will result in never being called. Internals are simply default constructed as nullptr
    OTA_Update_Callback() = default;

    /// @brief Constructs callbacks that will be called to inform about the OTA firmware update process
    /// @param current_fw_title Non owning pointer to the firmware title the device has choosen, is used to only allow updates with the same given title, other updates will be canceled.
    /// Additionally it has to be kept alive by the user for the runtime of the OTA Update process
    /// @param current_fw_version Non owning pointer to the firmware version the device is currently on, is used to only allow updates with a different version, other updates will be canceled.
    /// Additionally it has to be kept alive by the user for the runtime of the OTA Update process
    /// @param updater Non owning pointer to the updater implementation that writes the given firmware data.
    /// Additionally it has to be kept alive by the user for the runtime of the OTA Update process
    /// @param finished_callback End callback method that will be called as soon as the OTA firmware update, either finished successfully or failed.
    /// Is meant to allow to either restart the device if the udpate was successfull or to restart any stopped services before the update started in the subscribed update_starting_callback
    /// @param progress_callback Progress callback method that will be called every time our current progress of downloading the complete firmware data changed,
    /// meaning it will be called if the amount of already downloaded chunks increased.
    /// Is meant to allow to display a progress bar or print the current progress of the update into the console with the currently already downloaded amount of chunks and the total amount of chunks, default = nullptr
    /// @param update_starting_callback Update starting callback method that will be called as soon as the shared attribute firmware keys have been received and processed
    /// and the moment before we subscribe the necessary topics for the OTA firmware update.
    /// Is meant to give a moment were any additional processes or communication with the cloud can be stopped to ensure the update process runs as smooth as possible.
    /// To ensure that calling the ThingsBoardSized::Cleanup_Subscriptions() method can be used which stops any receiving of data over MQTT besides the one for the OTA firmware update,
    /// if this method is used ensure to call all subscribe methods again so they can be resubscribed, in the method passed to the finished_callback if the update failed and we do not restart the device, default = nullptr
    /// @param chunk_retries Amount of retries the OTA firmware update has to download each seperate chunk with a given size,
    /// before the complete download is stopped and registered as failed, default = CHUNK_RETRIES
    /// @param chunk_size Size of the chunks that the firmware binary data will be split into,
    /// increased chunk size might speed up the process by a little bit, but requires more heap memory,
    // because the whole chunk is saved into the heap before it can be processed and is then erased again after it has been used, default = CHUNK_SIZE
    /// @param timeout Maximum amount of time in microseconds for the OTA firmware update for each seperate chunk,
    /// until that chunk counts as a timeout, retries is then subtraced by one and the download is retried, default = REQUEST_TIMEOUT_MS
    OTA_Update_Callback(char const * current_fw_title, char const * current_fw_version, IUpdater * updater, function finished_callback, Callback<void, size_t const &, size_t const &>::function progress_callback = nullptr, Callback<void>::function update_starting_callback = nullptr, uint8_t chunk_retries = CHUNK_RETRIES, uint16_t chunk_size = CHUNK_SIZE, uint64_t const & timeout_microseconds = REQUEST_TIMEOUT_MS);

    ~OTA_Update_Callback() override = default;

    /// @brief Gets the current firmware title
    /// @note Used to decide if an OTA firmware update is already installed and therefore should not be downloaded,
    /// this is only done if the title of the update and the current firmware title are the same because if they are not then this firmware is meant for another device type
    /// @return Non owning pointer to the current firmware title of the device.
    /// Owned by the user that passed it originally in the constructor or with the @ref Set_Firmware_Title method
    char const * Get_Firmware_Title() const;

    /// @brief Sets the current firmware title
    /// @note Used to decide if an OTA firmware update is already installed and therefore should not be downloaded,
    /// this is only done if the title of the update and the current firmware title are the same because if they are not then this firmware is meant for another device type
    /// @param current_fw_title Non owning pointer to the current firmware title of the device.
    /// Additionally it has to be kept alive by the user for the runtime of the OTA Update process
    void Set_Firmware_Title(const char * current_fw_title);

    /// @brief Gets the current firmware version
    /// @note Used to decide if an OTA firmware update is already installed and therefore should not be downloaded,
    /// this is only done if the version of the update and the current firmware version are different, because if they are not then we would download the same firmware as is already on the device
    /// @return Non owning pointer to the current firmware version of the device.
    /// Owned by the user that passed it originally in the constructor or with the @ref Set_Firmware_Version method
    char const * Get_Firmware_Version() const;

    /// @brief Sets the current firmware version
    /// @note Used to decide if an OTA firmware update is already installed and therefore should not be downloaded,
    /// this is only done if the version of the update and the current firmware version are different, because if they are not then we would download the same firmware as is already on the device
    /// @param current_fw_version Non owning pointer to the current firmware version of the device.
    /// Additionally it has to be kept alive by the user for the runtime of the OTA Update process
    void Set_Firmware_Version(const char *current_fw_version);

    /// @brief Gets the updater implementation, used to write the actual firmware data into the needed memory location,
    /// so it can be used to reboot the given device with that new flashed firmware
    /// @return Non owning pointer to the updater implementation that writes the given firmware data.
    /// Owned by the user that passed it originally in the constructor or with the @ref Set_Updater method
    IUpdater * Get_Updater() const;

    /// @brief Sets the updater implementation, used to write the actual firmware data into the needed memory location,
    /// so it can be used to reboot the given device with that new flashed firmware
    /// @param updater Updater implementation that writes the given firmware data.
    /// Additionally it has to be kept alive by the user for the runtime of the OTA Update process
    void Set_Updater(IUpdater * updater);

    /// @brief Gets the unique request identifier that is connected to the original request
    /// @note Will be later used to verifiy which @ref OTA_Update_Callback is connected to which received OTA firmware update
    /// @return Unique identifier connected to the request to start the OTA firmware update
    size_t const & Get_Request_ID() const;

    /// @brief Sets the unique request identifier that is connected to the original request
    /// @note Will be later used to verifiy which @ref OTA_Update_Callback is connected to which received OTA firmware update.
    /// Not meant for external use, because the value is overwritten by internal method calls anyway once the class instance has been passed as a parameter anyway.
    /// This is the case because only the internal methods knows the current request id that this callback will be attached too
    /// @param request_id Unique identifier connected to the request to start the OTA firmware update
    void Set_Request_ID(size_t const & request_id);

    /// @brief Calls the progress callback that was subscribed, when this class instance was initally created
    /// @param current Already received and processsed amount of chunks
    /// @param total Total amount of chunks we need to receive and process until the update has completed
    void Call_Progress_Callback(size_t const & current, size_t const & total) const;

    /// @brief Sets the progress callback method
    /// @note Is meant to allow to display a progress bar or print the current progress of the update. With the currently already downloaded and processed amount of chunks and the total amount of chunks to process
    /// @param progress_callback Progress callback method that will be called every time our current progress of downloading and processing the complete firmware data changed,
    /// meaning it will be called if the amount of already downloaded chunks increased
    void Set_Progress_Callback(Callback<void, size_t const &, size_t const &>::function progress_callback);

    /// @brief Calls the update starting callback that was subscribed, when this class instance was initally created
    void Call_Update_Starting_Callback() const;

    /// @brief Sets the update starting callback method
    /// @note Is meant to give a moment were any additional processes or communication with the cloud can be stopped to ensure the update process runs as smooth as possible.
    /// To ensure that calling the ThingsBoardSized::Cleanup_Subscriptions() method can be used which stops any receiving of data over MQTT besides the one for the OTA firmware update,
    /// if this method is used ensure to call all subscribe methods again so they can be resubscribed, in the method passed to the finished_callback if the update failed and we do not restart the device
    /// @param update_starting_callback Update starting callback method that will be called as soon as the shared attribute firmware keys have been received and processed
    /// and the moment before we subscribe the necessary topics for the OTA firmware update
    void Set_Update_Starting_Callback(Callback<void>::function update_starting_callback);

    /// @brief Gets the amount of times we attempt to download each chunk of the OTA firmware binary file
    /// @note If the download fails because it times out, the write of the chunk data with the updater implementation fails then
    /// the retries are decreased by 1 until we hit 0, if that is the case then we instead stop the OTA firmware update completely
    /// @return Amount of retries for each single chunk before we abort the update
    uint8_t Get_Chunk_Retries() const;

    /// @brief Sets the amount of times we attempt to download each chunk of the OTA firmware binary file
    /// @note If the download fails because it times out, the write of the chunk data with the updater implementation fails then
    /// the retries are decreased by 1 until we hit 0, if that is the case then we instead stop the OTA firmware update completely
    /// @param chunk_retries Amount of retries for each single chunk before we abort the update
    void Set_Chunk_Retries(uint8_t chunk_retries);

    /// @brief Gets the size of the chunks that the firmware binary data will be split into
    /// @note Increased chunk size might speed up the process, but requires more heap memory,
    /// because the whole chunk is saved into the heap before it can be processed and is then overwriten after it has been used by the next chunk.
    /// This means the size passed to this method or with the constructor needs to be small enough to be allocated on the heap at runtime
    /// @return Size of each single chunk to be downloaded
    uint16_t Get_Chunk_Size() const;

    /// @brief Sets the size of the chunks that the firmware binary data will be split into
    /// @note Increased chunk size might speed up the process, but requires more heap memory,
    /// because the whole chunk is saved into the heap before it can be processed and is then overwriten after it has been used by the next chunk
    /// This means the size passed to this method or with the constructor needs to be small enough to be allocated on the heap at runtime
    /// @param chunk_size Size of each single chunk to be downloaded
    void Set_Chunk_Size(uint16_t chunk_size);

    /// @brief Gets the request timeout callback
    /// @note Will be called when no response to the request was received in the expected amount of time, causing the internal watchdog to time out.
    /// To achieve this behaviour the internal timer can be started and stopped, and simply calls the subscribed callback if the timer is not stopped before it times out
    /// @return Request timeout callback
    Timeoutable_Request & Get_Request_Timeout();

  private:
    char const                                     *m_current_fw_title = {};        // Current firmware title of device
    char const                                     *m_current_fw_version = {};      // Current firmware version of device
    IUpdater                                       *m_updater = {};                 // Updater implementation used to write firmware data
    size_t                                         m_request_id = {};               // Id the request was called with
    Callback<void, size_t const &, size_t const &> m_progress_callback = {};        // Callback called when amount of downloaded chunks increased
    Callback<void>                                 m_update_starting_callback = {}; // Callback called when update is about to start (moment before topic subscription)
    uint8_t                                        m_chunk_retries = {};            // Maximum amount of retries for a single chunk to be downloaded and flashed successfully
    uint16_t                                       m_chunk_size = {};               // Size of chunks the firmware data will be split into
    Timeoutable_Request                            m_request_timeout = {};          // Handles callback that will be called if request times out
};

#endif // OTA_Update_Callback_h
