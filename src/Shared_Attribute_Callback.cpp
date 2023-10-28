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

#if THINGSBOARD_ENABLE_DYNAMIC
const Vector<const char *>& Shared_Attribute_Callback::Get_Attributes() const {
#else
const Array<const char *, 2U>& Shared_Attribute_Callback::Get_Attributes() const {
#endif // THINGSBOARD_ENABLE_DYNAMIC
    return m_attributes;
}
