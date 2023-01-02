/*
  Telemetry.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Telemetry_h
#define Telemetry_h

// Library includes.
// Ensure ArduinoJson supports std::string type.
#define ARDUINOJSON_ENABLE_STD_STRING 1
#include <ArduinoJson.h>

// Telemetry record class, allows to store different data using common interface.
class Telemetry {
  public:
    inline Telemetry()
      : m_type(DataType::TYPE_NONE), m_key(NULL), m_value() { }

    // Constructs telemetry record from integer value.
    // EnableIf trick is required to overcome ambiguous float/integer conversion
    template <
      typename T,
      typename = ARDUINOJSON_NAMESPACE::enable_if<ARDUINOJSON_NAMESPACE::is_integral<T>::value>
      >
    inline Telemetry(const char *key, T val)
      : m_type(DataType::TYPE_INT), m_key(key), m_value()   {
      m_value.integer = val;
    }

    // Constructs telemetry record from boolean value.
    inline Telemetry(const char *key, bool val)
      : m_type(DataType::TYPE_BOOL), m_key(key), m_value()  {
      m_value.boolean = val;
    }

    // Constructs telemetry record from float value.
    inline Telemetry(const char *key, float val)
      : m_type(DataType::TYPE_REAL), m_key(key), m_value()  {
      m_value.real = val;
    }

    // Constructs telemetry record from string value.
    inline Telemetry(const char *key, const char *val)
      : m_type(DataType::TYPE_STR), m_key(key), m_value()   {
      m_value.str = val;
    }

    // Returnd true if the empty constructor was called
    // and no data was passed to the Telemetry instance istelf.
    inline const bool IsEmpty() const {
      return !m_key && m_type == DataType::TYPE_NONE;
    }

    // Serializes key-value pair in a generic way.
    const bool SerializeKeyValue(JsonVariant &jsonObj) const {
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
            // Nothing to do.
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
            // Nothing to do.
          break;
      }
      return false;
    }

  private:
    // Data container
    union Data {
      const char  *str;
      bool        boolean;
      int         integer;
      float       real;
    };

    // Data type inside a container
    enum class DataType: const uint8_t {
      TYPE_NONE,
      TYPE_BOOL,
      TYPE_INT,
      TYPE_REAL,
      TYPE_STR
    };

    DataType     m_type;  // Data type flag
    const char   *m_key;  // Data key
    Data         m_value; // Data value
};

// Convenient aliases
using Attribute = Telemetry;
using RPC_Response = Telemetry;

#endif // Telemetry_h
