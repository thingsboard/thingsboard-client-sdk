// Header include.
#include "Attribute_Request_Callback.h"

Attribute_Request_Callback::Attribute_Request_Callback() :
    Callback(nullptr, ATT_REQUEST_CB_IS_NULL),
    m_attributes(),
    m_request_id(0U),
    m_attribute_key(nullptr)
{
    // Nothing to do
}

#if !THINGSBOARD_ENABLE_STL

Attribute_Request_Callback::Attribute_Request_Callback(const char *attributes, function callback) :
    Callback(callback, ATT_REQUEST_CB_IS_NULL),
    m_attributes(attributes),
    m_request_id(0U),
    m_attribute_key(nullptr)
{
    // Nothing to do
}

#endif // !THINGSBOARD_ENABLE_STL

const size_t& Attribute_Request_Callback::Get_Request_ID() const {
    return m_request_id;
}

void Attribute_Request_Callback::Set_Request_ID(const size_t &request_id) {
    m_request_id = request_id;
}

const char* Attribute_Request_Callback::Get_Attribute_Key() const {
    return m_attribute_key;
}

void Attribute_Request_Callback::Set_Attribute_Key(const char *attribute_key) {
    m_attribute_key = attribute_key;
}

#if THINGSBOARD_ENABLE_STL

const std::vector<const char *>& Attribute_Request_Callback::Get_Attributes() const {
    return m_attributes;
}

#else

const char* Attribute_Request_Callback::Get_Attributes() const {
    return m_attributes;
}

void Attribute_Request_Callback::Set_Attributes(const char *attributes) {
    m_attributes = attributes;
}

#endif // THINGSBOARD_ENABLE_STL
