// Header include.
#include "Shared_Attribute_Callback.h"

Shared_Attribute_Callback::Shared_Attribute_Callback() :
    Shared_Attribute_Callback(nullptr)
{
    // Nothing to do
}

Shared_Attribute_Callback::Shared_Attribute_Callback(processFn cb) :
    m_attributes(),
    m_cb(cb)
{
    // Nothing to do
}

#if !THINGSBOARD_ENABLE_STL

Shared_Attribute_Callback::Shared_Attribute_Callback(const char *attributes, processFn cb)
      : m_attributes(attributes), m_cb(cb) {  }

#endif // !THINGSBOARD_ENABLE_STL

void Shared_Attribute_Callback::Set_Callback(processFn cb) {
    m_cb = cb;
}

#if THINGSBOARD_ENABLE_STL

const std::vector<const char *>& Shared_Attribute_Callback::Get_Attributes() const {
    return m_attributes;
}

#else

const char* Shared_Attribute_Callback::Get_Attributes() const {
    return m_attributes;
}

void Shared_Attribute_Callback::Set_Attributes(const char *attributes) {
    m_attributes = attributes;
}

#endif // THINGSBOARD_ENABLE_STL
