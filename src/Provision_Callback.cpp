// Header include.
#include "Provision_Callback.h"

constexpr char ACCESS_TOKEN_CRED_TYPE[] = "ACCESS_TOKEN";
constexpr char MQTT_BASIC_CRED_TYPE[] = "MQTT_BASIC";
constexpr char X509_CERTIFICATE_CRED_TYPE[] = "X509_CERTIFICATE";

Provision_Callback::Provision_Callback(Access_Token, function callback, char const * provision_device_key, char const * provision_device_secret, char const * device_name, uint64_t const & timeout_microseconds, Callback_Watchdog::function timeout_callback)
  : Callback(callback)
  , m_device_key(provision_device_key)
  , m_device_secret(provision_device_secret)
  , m_device_name(device_name)
  , m_access_token(nullptr)
  , m_cred_username(nullptr)
  , m_cred_password(nullptr)
  , m_cred_client_id(nullptr)
  , m_hash(nullptr)
  , m_credentials_type(nullptr)
  , m_request_timeout(timeout_microseconds, timeout_callback)
{
    // Nothing to do
}

Provision_Callback::Provision_Callback(Device_Access_Token, function callback, char const * provision_device_key, char const * provision_device_secret, char const * access_token, char const * device_name, uint64_t const & timeout_microseconds, Callback_Watchdog::function timeout_callback)
  : Callback(callback)
  , m_device_key(provision_device_key)
  , m_device_secret(provision_device_secret)
  , m_device_name(device_name)
  , m_access_token(access_token)
  , m_cred_username(nullptr)
  , m_cred_password(nullptr)
  , m_cred_client_id(nullptr)
  , m_hash(nullptr)
  , m_credentials_type(ACCESS_TOKEN_CRED_TYPE)
  , m_request_timeout(timeout_microseconds, timeout_callback)
{
    // Nothing to do
}

Provision_Callback::Provision_Callback(Basic_MQTT_Credentials, function callback, char const * provision_device_key, char const * provision_device_secret, char const * username, char const * password, char const * client_id, char const * device_name, uint64_t const & timeout_microseconds, Callback_Watchdog::function timeout_callback)
  : Callback(callback)
  , m_device_key(provision_device_key)
  , m_device_secret(provision_device_secret)
  , m_device_name(device_name)
  , m_access_token(nullptr)
  , m_cred_username(username)
  , m_cred_password(password)
  , m_cred_client_id(client_id)
  , m_hash(nullptr)
  , m_credentials_type(MQTT_BASIC_CRED_TYPE)
  , m_request_timeout(timeout_microseconds, timeout_callback)
{
    // Nothing to do
}

Provision_Callback::Provision_Callback(X509_Certificate, function callback, char const * provision_device_key, char const * provision_device_secret, char const * hash, char const * device_name, uint64_t const & timeout_microseconds, Callback_Watchdog::function timeout_callback)
  : Callback(callback)
  , m_device_key(provision_device_key)
  , m_device_secret(provision_device_secret)
  , m_device_name(device_name)
  , m_access_token(nullptr)
  , m_cred_username(nullptr)
  , m_cred_password(nullptr)
  , m_cred_client_id(nullptr)
  , m_hash(hash)
  , m_credentials_type(X509_CERTIFICATE_CRED_TYPE)
  , m_request_timeout(timeout_microseconds, timeout_callback)
{
    // Nothing to do
}

const char* Provision_Callback::Get_Device_Key() const {
    return m_device_key;
}

void Provision_Callback::Set_Device_Key(char const * provision_device_key) {
    m_device_key = provision_device_key;
}

const char* Provision_Callback::Get_Device_Secret() const {
    return m_device_secret;
}

void Provision_Callback::Set_Device_Secret(char const * provision_device_secret) {
    m_device_secret = provision_device_secret;
}

const char* Provision_Callback::Get_Device_Name() const {
    return m_device_name;
}

void Provision_Callback::Set_Device_Name(char const * device_name) {
    m_device_name = device_name;
}

const char* Provision_Callback::Get_Device_Access_Token() const {
    return m_access_token;
}

void Provision_Callback::Set_Device_Access_Token(char const * access_token) {
    m_access_token = access_token;
}

const char* Provision_Callback::Get_Credentials_Username() const {
    return m_cred_username;
}

void Provision_Callback::Set_Credentials_Username(char const * username) {
    m_cred_username = username;
}

const char* Provision_Callback::Get_Credentials_Password() const {
    return m_cred_password;
}

void Provision_Callback::Set_Credentials_Password(char const * password) {
    m_cred_password = password;
}

const char* Provision_Callback::Get_Credentials_Client_ID() const {
    return m_cred_client_id;
}

void Provision_Callback::Set_Credentials_Client_ID(char const * client_id) {
    m_cred_client_id = client_id;
}

const char* Provision_Callback::Get_Certificate_Hash() const {
    return m_hash;
}

void Provision_Callback::Set_Certificate_Hash(char const * hash) {
    m_hash = hash;
}

const char* Provision_Callback::Get_Credentials_Type() const {
    return m_credentials_type;
}

Timeoutable_Request & Provision_Callback::Get_Request_Timeout() {
    return m_request_timeout;
}
