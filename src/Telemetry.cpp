#include "Telemetry.h"

Telemetry::Telemetry() :
    m_type(DataType::TYPE_NONE),
    m_key(NULL),
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

bool Telemetry::SerializeKeyValue(const JsonVariant &jsonObj) const {
    if (m_key) {
        switch (m_type) {
            case DataType::TYPE_BOOL:	
                jsonObj[m_key] = m_value.boolean;	
                return true;	
                break;
            case DataType::TYPE_INT:
                jsonObj[m_key] = m_value.integer;
                return true;
                break;
            case DataType::TYPE_REAL:
                jsonObj[m_key] = m_value.real;
                return true;
                break;
            case DataType::TYPE_STR:
                jsonObj[m_key] = m_value.str;
                return true;
                break;
            default:
                // Nothing to do
                break;
        }
        return false;
    }

    switch (m_type) {
        case DataType::TYPE_BOOL:	
            return jsonObj.set(m_value.boolean);	
            break;
        case DataType::TYPE_INT:
            return jsonObj.set(m_value.integer);
            break;
        case DataType::TYPE_REAL:
            return jsonObj.set(m_value.real);
            break;
        case DataType::TYPE_STR:
            return jsonObj.set(m_value.str);
            break;
        default:
            // Nothing to do
            break;
    }
    return false;
}
