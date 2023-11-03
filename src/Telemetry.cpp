#include "Telemetry.h"

Telemetry::Telemetry() :
    m_type(DataType::TYPE_NONE),
    m_key(nullptr),
    m_value()
{
    // Nothing to do
}

Telemetry::Telemetry(const char *key, bool val)	:
    m_type(DataType::TYPE_BOOL),
    m_key(key),
    m_value()
{	
        m_value.boolean = val;	
}

Telemetry::Telemetry(const char *key, const char *value) :
    m_type(DataType::TYPE_STR),
    m_key(key),
    m_value()
{
    m_value.str = value;
}

bool Telemetry::IsEmpty() const {
    return !m_key && m_type == DataType::TYPE_NONE;
}
