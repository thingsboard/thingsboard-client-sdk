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

const Vector<const char *>& Shared_Attribute_Callback::Get_Attributes() const {
    return m_attributes;
}
