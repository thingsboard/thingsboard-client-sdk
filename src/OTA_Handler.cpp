// Header include.
#include "OTA_Handler.h"

#if THINGSBOARD_ENABLE_OTA

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
// Firmware data keys.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char FW_STATE_DOWNLOADING[] PROGMEM = "DOWNLOADING";
constexpr char FW_STATE_DOWNLOADED[] PROGMEM = "DOWNLOADED";
constexpr char FW_STATE_UPDATING[] PROGMEM = "UPDATING";
constexpr char FW_STATE_FAILED[] PROGMEM = "FAILED";
#else
constexpr char FW_STATE_DOWNLOADING[] = "DOWNLOADING";
constexpr char FW_STATE_DOWNLOADED[] = "DOWNLOADED";
constexpr char FW_STATE_UPDATING[] = "UPDATING";
constexpr char FW_STATE_FAILED[] = "FAILED";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Log messages.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char UNABLE_TO_REQUEST_CHUNCKS[] PROGMEM = "Unable to request firmware chunk";
constexpr char RECEIVED_UNEXPECTED_CHUNK[] PROGMEM = "Received chunk (%u), not the same as requested chunk (%u)";
constexpr char ERROR_UPDATE_BEGIN[] = "Failed to initalize flash updater, ensure that the partition scheme has two app sections";
constexpr char ERROR_UPDATE_WRITE[] PROGMEM = "Only wrote (%u) bytes of binary data instead of expected (%u)";
constexpr char UPDATING_HASH_FAILED[] PROGMEM = "Updating hash failed";
constexpr char ERROR_UPDATE_END[] PROGMEM = "Error (%u) during flash updater not all bytes written";
constexpr char CHKS_VER_FAILED[] PROGMEM = "Checksum verification failed";
constexpr char FW_UPDATE_ABORTED[] PROGMEM = "Firmware update aborted";
#if THINGSBOARD_ENABLE_DEBUG
constexpr char FW_CHUNK[] PROGMEM = "Receive chunk (%u), with size (%u) bytes";
constexpr char HASH_ACTUAL[] PROGMEM = "(%s) actual checksum: (%s)";
constexpr char HASH_EXPECTED[] PROGMEM = "(%s) expected checksum: (%s)";
constexpr char CHKS_VER_SUCCESS[] PROGMEM = "Checksum is the same as expected";
constexpr char FW_UPDATE_SUCCESS[] PROGMEM = "Update success";
#endif // THINGSBOARD_ENABLE_DEBUG
#else
constexpr char UNABLE_TO_REQUEST_CHUNCKS[] = "Unable to request firmware chunk";
constexpr char RECEIVED_UNEXPECTED_CHUNK[] = "Received chunk (%u), not the same as requested chunk (%u)";
constexpr char ERROR_UPDATE_BEGIN[] = "Failed to initalize flash updater, ensure that the partition scheme has two app sections";
constexpr char ERROR_UPDATE_WRITE[] = "Only wrote (%u) bytes of binary data instead of expected (%u)";
constexpr char UPDATING_HASH_FAILED[] = "Updating hash failed";
constexpr char ERROR_UPDATE_END[] = "Error during flash updater not all bytes written";
constexpr char CHKS_VER_FAILED[] = "Checksum verification failed";
constexpr char FW_UPDATE_ABORTED[] = "Firmware update aborted";
#if THINGSBOARD_ENABLE_DEBUG
constexpr char FW_CHUNK[] = "Receive chunk (%u), with size (%u) bytes";
constexpr char HASH_ACTUAL[] = "(%s) actual checksum: (%s)";
constexpr char HASH_EXPECTED[] = "(%s) expected checksum: (%s)";
constexpr char CHKS_VER_SUCCESS[] = "Checksum is the same as expected";
constexpr char FW_UPDATE_SUCCESS[] = "Update success";
#endif // THINGSBOARD_ENABLE_DEBUG
#endif // THINGSBOARD_ENABLE_PROGMEM

OTA_Handler::OTA_Handler(const ILogger& logger, std::function<bool(const size_t&)> publish_callback, std::function<bool(const char *, const char *)> send_fw_state_callback, std::function<bool(void)> finish_callback)
    : m_fw_callback(nullptr)
    , m_logger(logger)
    , m_publish_callback(publish_callback)
    , m_send_fw_state_callback(send_fw_state_callback)
    , m_finish_callback(finish_callback)
    , m_fw_size(0U)
    , m_fw_algorithm()
    , m_fw_checksum()
    , m_fw_checksum_algorithm()
    , m_hash()
    , m_total_chunks(0U)
    , m_requested_chunks(0U)
    , m_retries(0U)
    , m_watchdog(std::bind(&OTA_Handler::Handle_Request_Timeout, this))
{
  // Nothing to do
}

void OTA_Handler::Start_Firmware_Update(const OTA_Update_Callback& fw_callback, const size_t& fw_size, const std::string& fw_algorithm, const std::string& fw_checksum, const mbedtls_md_type_t& fw_checksum_algorithm) {
    m_fw_callback = &fw_callback;
    m_fw_size = fw_size;
    m_total_chunks = (m_fw_size / m_fw_callback->Get_Chunk_Size()) + 1U;
    m_fw_algorithm = fw_algorithm;
    m_fw_checksum = fw_checksum;
    m_fw_checksum_algorithm = fw_checksum_algorithm;
    m_fw_updater = m_fw_callback->Get_Updater();

    if (!m_publish_callback || !m_send_fw_state_callback || !m_finish_callback || !m_fw_updater) {
      m_logger.print(OTA_CB_IS_NULL);
      (void)m_send_fw_state_callback(FW_STATE_FAILED, OTA_CB_IS_NULL);
        return Handle_Failure(OTA_Failure_Response::RETRY_NOTHING);
    }
    Request_First_Firmware_Packet();
}

void OTA_Handler::Stop_Firmware_Update() {
    m_watchdog.detach();
    m_fw_updater->reset();
    m_logger.print(FW_UPDATE_ABORTED);
    (void)m_send_fw_state_callback(FW_STATE_FAILED, FW_UPDATE_ABORTED);
    Handle_Failure(OTA_Failure_Response::RETRY_NOTHING);
    m_fw_callback = nullptr;
}

void OTA_Handler::Process_Firmware_Packet(const size_t& current_chunk, uint8_t *payload, const size_t& total_bytes) {
    (void)m_send_fw_state_callback(FW_STATE_DOWNLOADING, nullptr);

    if (current_chunk != m_requested_chunks) {
      m_logger.printf(RECEIVED_UNEXPECTED_CHUNK, current_chunk, m_requested_chunks);
      return;
    }

    m_watchdog.detach();
#if THINGSBOARD_ENABLE_DEBUG
    m_logger.printf(FW_CHUNK, current_chunk, total_bytes);
#endif // THINGSBOARD_ENABLE_DEBUG

    if (current_chunk == 0U) {
        // Initialize Flash
        if (!m_fw_updater->begin(m_fw_size)) {
          m_logger.print(ERROR_UPDATE_BEGIN);
          (void)m_send_fw_state_callback(FW_STATE_FAILED, ERROR_UPDATE_BEGIN);
          return Handle_Failure(OTA_Failure_Response::RETRY_UPDATE);
        }
    }

    // Write received binary data to flash partition
    const size_t written_bytes = m_fw_updater->write(payload, total_bytes);
    if (written_bytes != total_bytes) {
        char message[Helper::detectSize(ERROR_UPDATE_WRITE, written_bytes, total_bytes)];
        snprintf(message, sizeof(message), ERROR_UPDATE_WRITE, written_bytes, total_bytes);
        m_logger.print(message);
        (void)m_send_fw_state_callback(FW_STATE_FAILED, message);
        return Handle_Failure(OTA_Failure_Response::RETRY_UPDATE);
    }

    // Update value only if writing to flash was a success
    if (!m_hash.update(payload, total_bytes)) {
        m_logger.print(UPDATING_HASH_FAILED);
        (void)m_send_fw_state_callback(FW_STATE_FAILED, UPDATING_HASH_FAILED);
        return Handle_Failure(OTA_Failure_Response::RETRY_UPDATE);
    }

    m_requested_chunks = current_chunk + 1;
    m_fw_callback->Call_Progress_Callback(m_logger, m_requested_chunks, m_total_chunks);

    // Ensure to check if the update was cancelled during the progress callback,
    // if it was the callback variable was reset and there is no need to request the next firmware packet
    if (m_fw_callback == nullptr) {
      return;
    }

    // Reset retries as the current chunk has been downloaded and handled successfully
    m_retries = m_fw_callback->Get_Chunk_Retries();
    Request_Next_Firmware_Packet();
}

void OTA_Handler::Request_First_Firmware_Packet() {
    m_requested_chunks = 0U;
    m_retries = m_fw_callback->Get_Chunk_Retries();
    m_hash.start(m_fw_checksum_algorithm);
    m_watchdog.detach();
    m_fw_updater->reset();
    Request_Next_Firmware_Packet();
}

void OTA_Handler::Request_Next_Firmware_Packet() {
    // Check if we have already requested and handled the last remaining chunk
    if (m_requested_chunks >= m_total_chunks) {
        Finish_Firmware_Update();   
        return;
    }

    if (!m_publish_callback(m_requested_chunks)) {
      m_logger.print(UNABLE_TO_REQUEST_CHUNCKS);
      (void)m_send_fw_state_callback(FW_STATE_FAILED, UNABLE_TO_REQUEST_CHUNCKS);
    }

    // Watchdog gets started no matter if publishing request was successful or not in hopes,
    // that after the given timeout the callback calls this method again and can then publish the request successfully.
    m_watchdog.once(m_fw_callback->Get_Timeout());
}

void OTA_Handler::Finish_Firmware_Update() {
    (void)m_send_fw_state_callback(FW_STATE_DOWNLOADED, nullptr);

    const std::string calculated_hash = m_hash.get_hash_string();
#if THINGSBOARD_ENABLE_DEBUG
    m_logger.printf(HASH_ACTUAL, m_fw_algorithm.c_str(), calculated_hash.c_str());
    m_logger.printf(HASH_EXPECTED, m_fw_algorithm.c_str(), m_fw_checksum.c_str());
#endif // THINGSBOARD_ENABLE_DEBUG

    // Check if the initally received checksum is the same as the one we calculated from the received binary data,
    // if not we assume the binary data has been changed or not completly downloaded --> Firmware update failed
    if (m_fw_checksum.compare(calculated_hash) != 0) {
        m_logger.print(CHKS_VER_FAILED);
        (void)m_send_fw_state_callback(FW_STATE_FAILED, CHKS_VER_FAILED);
        return Handle_Failure(OTA_Failure_Response::RETRY_UPDATE);
    }

#if THINGSBOARD_ENABLE_DEBUG
    m_logger.print(CHKS_VER_SUCCESS);
#endif // THINGSBOARD_ENABLE_DEBUG

    if (!m_fw_updater->end()) {
        m_logger.print(ERROR_UPDATE_END);
        (void)m_send_fw_state_callback(FW_STATE_FAILED, ERROR_UPDATE_END);
        return Handle_Failure(OTA_Failure_Response::RETRY_UPDATE);
    }

#if THINGSBOARD_ENABLE_DEBUG
    m_logger.print(FW_UPDATE_SUCCESS);
#endif // THINGSBOARD_ENABLE_DEBUG
    (void)m_send_fw_state_callback(FW_STATE_UPDATING, nullptr);

    m_fw_callback->Call_Callback(m_logger, true);
    (void)m_finish_callback();
}

void OTA_Handler::Handle_Failure(const OTA_Failure_Response& failure_response) {
  if (m_retries <= 0) {
      m_fw_callback->Call_Callback(m_logger, false);
      (void)m_finish_callback();
      return;
  }

  // Decrease the amount of retries of downloads for the current chunk,
  // reset as soon as the next chunk has been received and handled successfully
  m_retries--;

  switch (failure_response) {
    case OTA_Failure_Response::RETRY_CHUNK:
      Request_Next_Firmware_Packet();
      break;
    case OTA_Failure_Response::RETRY_UPDATE:
      Request_First_Firmware_Packet();
      break;
    case OTA_Failure_Response::RETRY_NOTHING:
      m_fw_callback->Call_Callback(m_logger, false);
      (void)m_finish_callback();
      break;
    default:
      // Nothing to do
      break;
  }
}

void OTA_Handler::Handle_Request_Timeout() {
    Handle_Failure(OTA_Failure_Response::RETRY_CHUNK);
}

#endif // THINGSBOARD_ENABLE_OTA
