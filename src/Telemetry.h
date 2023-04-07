/*
  Telemetry.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Telemetry_h
#define Telemetry_h

// Library includes.
#include <ArduinoJson.h>

// Struct dispatch tags, to differentiate between constructors,
// solves the issue of not being able to distuinguish float, integer and the boolean constructor.
struct Float{};
struct Bool{};
struct Int{};
struct CString{};

/// @brief Telemetry record class, allows to store different data using a common interface.
class Telemetry {
  public:
    /// @brief Creates an empty Telemetry record containg neither a key nor value
    inline Telemetry()
      : m_type(DataType::TYPE_NONE), m_key(NULL), m_value() { }

    /// @brief Constructs telemetry record from integer value
    /// @param key Key of the key value pair we want to create
    /// @param val Value of the key value pair we want to create
    inline Telemetry(Int, const char *key, int val)
            : m_type(DataType::TYPE_INT), m_key(key), m_value()   {
        m_value.integer = val;
    }

    /// @brief Constructs telemetry record from boolean value
    /// @param key Key of the key value pair we want to create
    /// @param val Value of the key value pair we want to create
    inline Telemetry(Bool, const char *key, bool val)
            : m_type(DataType::TYPE_BOOL), m_key(key), m_value()  {
        m_value.boolean = val;
    }

    /// @brief Constructs telemetry record from float value
    /// @param key Key of the key value pair we want to create
    /// @param val Value of the key value pair we want to create
    inline Telemetry(Float, const char *key, float val)
      : m_type(DataType::TYPE_REAL), m_key(key), m_value()  {
      m_value.real = val;
    }

    /// @brief Constructs telemetry record from string value
    /// @param key Key of the key value pair we want to create
    /// @param val Value of the key value pair we want to create
    inline Telemetry(CString, const char *key, const char *val)
            : m_type(DataType::TYPE_STR), m_key(key), m_value()   {
        m_value.str = val;
    }

    /// @brief Whether this record is empty or not
    /// @return Whether there is any data in this record or not
    inline bool IsEmpty() const {
      return !m_key && m_type == DataType::TYPE_NONE;
    }

    /// @brief Serializes the key-value pair depending on the constructor used
    /// @param jsonObj Object the value will be copied into with the given key
    /// @return Whether serializing was successfull or not
    inline bool SerializeKeyValue(const JsonVariant &jsonObj) const {
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

#endif // Telemetry_h
