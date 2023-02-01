/*
  OTA_Update_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef OTA_Update_Callback_h
#define OTA_Update_Callback_h

#if defined(ESP8266) || defined(ESP32)

// Library includes.
#include <functional>

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
constexpr char OTA_CB_IS_NULL[] PROGMEM = "OTA update callback is NULL";

// OTA default values.
constexpr uint8_t CHUNK_RETRIES PROGMEM = 5U;
constexpr uint16_t CHUNK_SIZE PROGMEM = 4096U;
constexpr uint16_t REQUEST_TIMEOUT PROGMEM = 3000U;

/// @brief OTA firmware update callback wrapper
class OTA_Update_Callback {
  public:
    /// @brief OTA firmware update callback signature
    using returnType = void;
    using progressArgumentType = const uint32_t&;
    using endArgumentType = const bool&;
    using progressFn = std::function<returnType(progressArgumentType current, progressArgumentType total)>;
    using endFn = std::function<returnType(endArgumentType data)>;

    /// @brief Constructs empty callback, will result in never being called
    inline OTA_Update_Callback()
      : OTA_Update_Callback(nullptr, nullptr, nullptr, nullptr) {  }

    /// @brief Constructs callbacks that will be called when the OTA firmware data,
    /// has been completly sent by the cloud, received by the client and written to the flash partition
    /// @param endCb End callback method that will be called as soon as the OTA firmware update, either finished successfully or failed
    /// @param currFwTitle Firmware title the device has choosen, is used to only allow updates with the same given title, other updates will be canceled
    /// @param currFwVersion Firmware version the device is currently on, is used to decide to inform the cloud about the previous version we updatet from
    /// @param chunkRetries Amount of retries the OTA firmware update has to download each seperate chunk with a given size,
    /// before the complete download is stopped and registered as failed
    /// @param chunkSize Maximum size of OTA firmware update for each seperate chunk that should be downloaded,
    /// increased chunkSize might speed up the process by a little bit, but requires more heap memory,
    // because the whole chunk is saved into the heap before it can be processed and is then cleared again
    /// @param timeout Maximum amount time for the OTA firmware update for each seperate chunk,
    /// until that chunk counts as a timeout, retries is then subtraced by one and the download is retried
    inline OTA_Update_Callback(endFn endCb, const char *currFwTitle, const char *currFwVersion, const uint8_t& chunkRetries = CHUNK_RETRIES, const uint16_t& chunkSize = CHUNK_SIZE, const uint16_t& timeout = REQUEST_TIMEOUT)
      : OTA_Update_Callback(nullptr, endCb, currFwTitle, currFwVersion, chunkRetries, chunkSize, timeout) {  }

    /// @brief Constructs callbacks that will be called when the OTA firmware data,
    /// has been completly sent by the cloud, received by the client and written to the flash partition as well as callback
    // that will be called every time the current progress of the firmware update changes
    /// @param progressCb Progress callback method that will be called every time our current progress of downloading the complete firmware data changed,
    /// this makes it possible to display a progress bar or signal easily how far we are in the current downloading process
    /// @param endCb End callback method that will be called as soon as the OTA firmware update, either finished successfully or failed
    /// @param currFwTitle Firmware title the device has choosen, is used to only allow updates with the same given title, other updates will be canceled
    /// @param currFwVersion Firmware version the device is currently on, is used to decide to inform the cloud about the previous version we updatet from
    /// @param chunkRetries Amount of retries the OTA firmware update has to download each seperate chunk with a given size,
    /// before the complete download is stopped and registered as failed
    /// @param chunkSize Maximum size of OTA firmware update for each seperate chunk that should be downloaded,
    /// increased chunkSize might speed up the process by a little bit, but requires more heap memory,
    // because the whole chunk is saved into the heap before it can be processed and is then cleared again
    /// @param timeout Maximum amount time for the OTA firmware update for each seperate chunk,
    /// until that chunk counts as a timeout, retries is then subtraced by one and the download is retried
    inline OTA_Update_Callback(progressFn progressCb, endFn endCb, const char *currFwTitle, const char *currFwVersion, const uint8_t& chunkRetries = CHUNK_RETRIES, const uint16_t& chunkSize = CHUNK_SIZE, const uint16_t& timeout = REQUEST_TIMEOUT)
      : m_progressCb(progressCb), m_endCb(endCb), m_fwTitel(currFwTitle), m_fwVersion(currFwVersion), m_retries(chunkRetries), m_size(chunkSize), m_timeout(timeout) {  }

    /// @brief Calls the progress callback that was subscribed, when this class instance was initally created
    /// @tparam Logger Logging class that should be used to print messages
    /// @param current Received data that shows the current progress
    /// @param total Received data that shows the total progress until the update is completed
    template<typename Logger>
    inline returnType Call_Progress_Callback(progressArgumentType current, progressArgumentType total) const {
      // Check if the callback is a nullptr,
      // meaning it has not been assigned any valid callback method.
      if (!m_progressCb) {
        Logger::log(OTA_CB_IS_NULL);
        return returnType();
      }
      return m_progressCb(current, total);
    }

    /// @brief Calls the end callback that was subscribed, when this class instance was initally created
    /// @tparam Logger Logging class that should be used to print messages
    /// @param data Received data that shows, wheter the update was successfull or not
    template<typename Logger>
    inline returnType Call_End_Callback(endArgumentType data) const {
      // Check if the callback is a nullptr,
      // meaning it has not been assigned any valid callback method.
      if (!m_endCb) {
        Logger::log(OTA_CB_IS_NULL);
        return returnType();
      }
      return m_endCb(data);
    }

    /// @brief Gets the current firmware title, used to decide if an OTA firmware update is actually installed,
    /// this is only done if the title of the update and the current firmware title are the same
    /// @return Current firmware title of the device
    inline const char* Get_Firmware_Title() const {
      return m_fwTitel;
    }

    /// @brief Gets the current firmware version, used to decide if an OTA firmware update is actually installed,
    /// this is only done if the version of the update and the current firmware title are different
    /// @return Current firmware version of the device
    inline const char* Get_Firmware_Version() const {
      return m_fwVersion;
    }

    /// @brief Gets the amount of times we attempt to download each chunk of the OTA firmware binary file,
    /// if the download fails because it times out, doesn't let itself write into flash memory, ...
    /// the retries are decreased by 1 until we hit 0, if that is the case then we instead stop the OTA firmware update completly
    /// @return Amount of retries for each single chunk to be downloaded successfully
    inline const uint8_t& Get_Chunk_Retries() const {
      return m_retries;
    }

    /// @brief Gets the size a single chunk of the OTA firmware binary file we attempt to download should have
    /// @return Size of each single chunk to be downloaded
    inline const uint16_t& Get_Chunk_Size() const {
      return m_size;
    }

    /// @brief Gets the time we wait until we decleare a single chunk we attempted to download as a timeout
    /// @return Gets the timeout time for each single chunk to be downloaded
    inline const uint16_t& Get_Timeout() const {
      return m_timeout;
    }

  private:
    progressFn      m_progressCb;    // Progress callback to call
    endFn           m_endCb;         // End callback to call
    const char      *m_fwTitel;      // Current firmware title of device
    const char      *m_fwVersion;    // Current firmware version of device
    const uint8_t   m_retries;       // Maximum amount of retries
    const uint16_t  m_size;          // Maximum size of the chuncks we are downloading
    const uint16_t  m_timeout;       // How long we maximum wait for each chunck to arrive
};

#endif // defined(ESP8266) || defined(ESP32)

#endif // OTA_Update_Callback_h
