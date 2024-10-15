// Header include.
#include "Telemetry.h"

Telemetry::Telemetry()
  : m_type(DataType::TYPE_NONE)
  , m_key(nullptr)
  , m_value()
{
    // Nothing to do
}

Telemetry::Telemetry(char const * key, bool value)
  : m_type(DataType::TYPE_BOOL)
  , m_key(key)
  , m_value()
{
    m_value.boolean = value;
}

Telemetry::Telemetry(char const * key, char const * value)
  : m_type(DataType::TYPE_STR)
  , m_key(key)
  , m_value()
{
    m_value.str = value;
}

bool Telemetry::IsEmpty() const {
    return (m_key == nullptr) && m_type == DataType::TYPE_NONE;
}
