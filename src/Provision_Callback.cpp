// Header include.
#include "Provision_Callback.h"

#if THINGSBOARD_ENABLE_PROGMEM
constexpr char PROVISION_CB_IS_NULL[] PROGMEM = "Provisioning callback is NULL";
constexpr char ACCESS_TOKEN_CRED_TYPE[] PROGMEM = "ACCESS_TOKEN";
constexpr char MQTT_BASIC_CRED_TYPE[] PROGMEM = "MQTT_BASIC";
constexpr char X509_CERTIFICATE_CRED_TYPE[] PROGMEM = "X509_CERTIFICATE";
#else
constexpr char PROVISION_CB_IS_NULL[] = "Provisioning callback is NULL";
constexpr char ACCESS_TOKEN_CRED_TYPE[] = "ACCESS_TOKEN";
constexpr char MQTT_BASIC_CRED_TYPE[] = "MQTT_BASIC";
constexpr char X509_CERTIFICATE_CRED_TYPE[] = "X509_CERTIFICATE";
#endif // THINGSBOARD_ENABLE_PROGMEM

Provision_Callback::Provision_Callback(Access_Token, function callback, char const * const provisionDeviceKey, char const * const provisionDeviceSecret, char const * const deviceName)
  : Callback(callback, PROVISION_CB_IS_NULL)
  , m_deviceKey(provisionDeviceKey)
  , m_deviceSecret(provisionDeviceSecret)
  , m_deviceName(deviceName)
  , m_accessToken(nullptr)
  , m_credUsername(nullptr)
  , m_credPassword(nullptr)
  , m_credClientID(nullptr)
  , m_hash(nullptr)
  , m_credentialsType(nullptr)
{
    // Nothing to do
}

Provision_Callback::Provision_Callback(Device_Access_Token, function callback, char const * const provisionDeviceKey, char const * const provisionDeviceSecret, char const * const accessToken, char const * const deviceName)
  : Callback(callback, PROVISION_CB_IS_NULL)
  , m_deviceKey(provisionDeviceKey)
  , m_deviceSecret(provisionDeviceSecret)
  , m_deviceName(deviceName)
  , m_accessToken(accessToken)
  , m_credUsername(nullptr)
  , m_credPassword(nullptr)
  , m_credClientID(nullptr)
  , m_hash(nullptr)
  , m_credentialsType(ACCESS_TOKEN_CRED_TYPE)
{
    // Nothing to do
}

Provision_Callback::Provision_Callback(Basic_MQTT_Credentials, function callback, char const * const provisionDeviceKey, char const * const provisionDeviceSecret, char const * const username, char const * const password, char const * const clientID, char const * const deviceName)
  : Callback(callback, PROVISION_CB_IS_NULL)
  , m_deviceKey(provisionDeviceKey)
  , m_deviceSecret(provisionDeviceSecret)
  , m_deviceName(deviceName)
  , m_accessToken(nullptr)
  , m_credUsername(username)
  , m_credPassword(password)
  , m_credClientID(clientID)
  , m_hash(nullptr)
  , m_credentialsType(MQTT_BASIC_CRED_TYPE)
{
    // Nothing to do
}

Provision_Callback::Provision_Callback(X509_Certificate, function callback, char const * const provisionDeviceKey, char const * const provisionDeviceSecret, char const * const hash, char const * const deviceName)
  : Callback(callback, PROVISION_CB_IS_NULL)
  , m_deviceKey(provisionDeviceKey)
  , m_deviceSecret(provisionDeviceSecret)
  , m_deviceName(deviceName)
  , m_accessToken(nullptr)
  , m_credUsername(nullptr)
  , m_credPassword(nullptr)
  , m_credClientID(nullptr)
  , m_hash(hash)
  , m_credentialsType(X509_CERTIFICATE_CRED_TYPE)
{
    // Nothing to do
}

const char* Provision_Callback::Get_Device_Key() const {
    return m_deviceKey;
}

void Provision_Callback::Set_Device_Key(char const * const provisionDeviceKey) {
    m_deviceKey = provisionDeviceKey;
}

const char* Provision_Callback::Get_Device_Secret() const {
    return m_deviceSecret;
}

void Provision_Callback::Set_Device_Secret(char const * const provisionDeviceSecret) {
    m_deviceSecret = provisionDeviceSecret;
}

const char* Provision_Callback::Get_Device_Name() const {
    return m_deviceName;
}

void Provision_Callback::Set_Device_Name(char const * const deviceName) {
    m_deviceName = deviceName;
}

const char* Provision_Callback::Get_Device_Access_Token() const {
    return m_accessToken;
}

void Provision_Callback::Set_Device_Access_Token(char const * const accessToken) {
    m_accessToken = accessToken;
}

const char* Provision_Callback::Get_Credentials_Username() const {
    return m_credUsername;
}

void Provision_Callback::Set_Credentials_Username(char const * const username) {
    m_credUsername = username;
}

const char* Provision_Callback::Get_Credentials_Password() const {
    return m_credPassword;
}

void Provision_Callback::Set_Credentials_Password(char const * const password) {
    m_credPassword = password;
}

const char* Provision_Callback::Get_Credentials_Client_ID() const {
    return m_credClientID;
}

void Provision_Callback::Set_Credentials_Client_ID(char const * const clientID) {
    m_credClientID = clientID;
}

const char* Provision_Callback::Get_Certificate_Hash() const {
    return m_hash;
}

void Provision_Callback::Set_Certificate_Hash(char const * const hash) {
    m_hash = hash;
}

const char* Provision_Callback::Get_Credentials_Type() const {
    return m_credentialsType;
}
