// Header include.
#include "Shared_Attribute_Callback.h"

Shared_Attribute_Callback::Shared_Attribute_Callback() :
    Shared_Attribute_Callback(nullptr)
{
    // Nothing to do
}

Shared_Attribute_Callback::Shared_Attribute_Callback(function cb) :
    Callback(cb, ATT_CB_IS_NULL),
    m_attributes()
{
    // Nothing to do
}

#if THINGSBOARD_ENABLE_STL

const std::vector<const char *>& Shared_Attribute_Callback::Get_Attributes() const {
    return m_attributes;
}

#else

Shared_Attribute_Callback::Shared_Attribute_Callback(const char *attributes, function cb) :
    Callback(cb, ATT_CB_IS_NULL),
    m_attributes(attributes)
{
    // Nothing to do
}

const char* Shared_Attribute_Callback::Get_Attributes() const {
    return m_attributes;
}

void Shared_Attribute_Callback::Set_Attributes(const char *attributes) {
    m_attributes = attributes;
}

#endif // THINGSBOARD_ENABLE_STL
