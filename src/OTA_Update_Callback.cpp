// Header include.
#include "OTA_Update_Callback.h"

OTA_Update_Callback::OTA_Update_Callback(char const * current_fw_title, char const * current_fw_version, IUpdater * updater, function finished_callback, Callback<void, size_t const &, size_t const &>::function progress_callback, Callback<void>::function update_starting_callback, uint8_t chunk_retries, uint16_t chunk_size, uint64_t const & timeout_microseconds)
  : Callback(finished_callback)
  , m_current_fw_title(current_fw_title)
  , m_current_fw_version(current_fw_version)
  , m_updater(updater)
  , m_progress_callback(progress_callback)
  , m_update_starting_callback(update_starting_callback)
  , m_chunk_retries(chunk_retries)
  , m_chunk_size(chunk_size)
  , m_timeout_microseconds(timeout_microseconds)
{
    // Nothing to do
}

char const * OTA_Update_Callback::Get_Firmware_Title() const {
    return m_current_fw_title;
}

void OTA_Update_Callback::Set_Firmware_Title(char const * current_fw_title) {
    m_current_fw_title = current_fw_title;
}

char const * OTA_Update_Callback::Get_Firmware_Version() const {
    return m_current_fw_version;
}

void OTA_Update_Callback::Set_Firmware_Version(char const * current_fw_version) {
    m_current_fw_version = current_fw_version;
}

IUpdater * OTA_Update_Callback::Get_Updater() const {
    return m_updater;
}

void OTA_Update_Callback::Set_Updater(IUpdater* updater) {
    m_updater = updater;
}

size_t const & OTA_Update_Callback::Get_Request_ID() const {
    return m_request_id;
}

void OTA_Update_Callback::Set_Request_ID(size_t const & request_id) {
    m_request_id = request_id;
}

void OTA_Update_Callback::Call_Progress_Callback(size_t const & current, size_t const & total) const {
    return m_progress_callback.Call_Callback(current, total);
}

void OTA_Update_Callback::Set_Progress_Callback(Callback<void, size_t const &, size_t const &>::function progress_callback) {
    m_progress_callback.Set_Callback(progress_callback);
}

void OTA_Update_Callback::Call_Update_Starting_Callback() const {
    return m_update_starting_callback.Call_Callback();
}

void OTA_Update_Callback::Set_Update_Starting_Callback(Callback<void>::function update_starting_callback) {
    m_update_starting_callback.Set_Callback(update_starting_callback);
}

uint8_t OTA_Update_Callback::Get_Chunk_Retries() const {
  return m_chunk_retries;
}

void OTA_Update_Callback::Set_Chunk_Retries(uint8_t chunk_retries) {
    m_chunk_retries = chunk_retries;
}

uint16_t OTA_Update_Callback::Get_Chunk_Size() const {
  return m_chunk_size;
}

void OTA_Update_Callback::Set_Chunk_Size(uint16_t chunk_size) {
    m_chunk_size = chunk_size;
}

uint64_t const & OTA_Update_Callback::Get_Timeout() const {
    return m_timeout_microseconds;
}

void OTA_Update_Callback::Set_Timeout(const uint64_t & timeout_microseconds) {
    m_timeout_microseconds = timeout_microseconds;
}
