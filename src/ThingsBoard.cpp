/*
  ThingsBoard.cpp - Library for sending data to the ThingsBoard.
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
  https://thingsboard.io/
*/

#include "ThingsBoard.h"

/*----------------------------------------------------------------------------*/

const bool Telemetry::SerializeKeyValue(JsonVariant &jsonObj) const {
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
