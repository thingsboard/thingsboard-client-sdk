// Header include.
#include "OTA_Update_Callback.h"

#if THINGSBOARD_ENABLE_OTA

OTA_Update_Callback::OTA_Update_Callback() :
    OTA_Update_Callback(nullptr, nullptr, nullptr, nullptr, nullptr)
{
    // Nothing to do
}

OTA_Update_Callback::OTA_Update_Callback(function endCb, const char *currFwTitle, const char *currFwVersion, IUpdater *updater, const uint8_t &chunkRetries, const uint16_t &chunkSize, const uint64_t &timeout) :
    OTA_Update_Callback(nullptr, endCb, currFwTitle, currFwVersion, updater, chunkRetries, chunkSize, timeout)
{
    // Nothing to do
}

OTA_Update_Callback::OTA_Update_Callback(progressFn progressCb, function endCb, const char *currFwTitle, const char *currFwVersion, IUpdater *updater, const uint8_t &chunkRetries, const uint16_t &chunkSize, const uint64_t &timeout) :
    Callback(endCb, OTA_CB_IS_NULL),
    m_progressCb(progressCb),
    m_fwTitel(currFwTitle),
    m_fwVersion(currFwVersion),
    m_updater(updater),
    m_retries(chunkRetries),
    m_size(chunkSize),
    m_timeout(timeout)
{
    // Nothing to do
}

void OTA_Update_Callback::Set_Progress_Callback(progressFn progressCb) {
    m_progressCb = progressCb;
}

const char* OTA_Update_Callback::Get_Firmware_Title() const {
    return m_fwTitel;
}

void OTA_Update_Callback::Set_Firmware_Title(const char *currFwTitle) {
    m_fwTitel = currFwTitle;
}

const char* OTA_Update_Callback::Get_Firmware_Version() const {
    return m_fwVersion;
}

void OTA_Update_Callback::Set_Firmware_Version(const char *currFwVersion) {
    m_fwVersion = currFwVersion;
}

IUpdater* OTA_Update_Callback::Get_Updater() const {
    return m_updater;
}

void OTA_Update_Callback::Set_Updater(IUpdater* updater) {
    m_updater = updater;
}

const uint8_t& OTA_Update_Callback::Get_Chunk_Retries() const {
  return m_retries;
}

void OTA_Update_Callback::Set_Chunk_Retries(const uint8_t &chunkRetries) {
    m_retries = chunkRetries;
}

const uint16_t& OTA_Update_Callback::Get_Chunk_Size() const {
  return m_size;
}

void OTA_Update_Callback::Set_Chunk_Size(const uint16_t &chunkSize) {
    m_size = chunkSize;
}

const uint64_t& OTA_Update_Callback::Get_Timeout() const {
    return m_timeout;
}

void OTA_Update_Callback::Set_Timeout(const uint64_t &timeout_microseconds) {
    m_timeout = timeout_microseconds;
}

#endif // THINGSBOARD_ENABLE_OTA
