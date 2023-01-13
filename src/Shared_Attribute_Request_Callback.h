/*
  Shared_Attribute_Request_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Shared_Attribute_Request_Callback_h
#define Shared_Attribute_Request_Callback_h

// Library includes.
// Ensure ArduinoJson supports std::string type.
#define ARDUINOJSON_ENABLE_STD_STRING 1
#include <ArduinoJson.h>
#include <functional>
#include <vector>

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
constexpr char ATT_REQUEST_CB_IS_NULL[] PROGMEM = "Shared attribute request callback is NULL";

// Convenient aliases
// JSON object const (read only twice as small as JSON object), is used to communicate Shared Attributes and Provision Data to the client
using Shared_Attribute_Data = const JsonObjectConst;

/// @brief Shared attributes request callback wrapper
class Shared_Attribute_Request_Callback {
  public:
    /// @brief Shared attributes callback signature
    using returnType = void;
    using argumentType = const Shared_Attribute_Data&;
    using processFn = std::function<returnType(argumentType data)>;

    /// @brief Constructs empty callback, will result in never being called
    inline Shared_Attribute_Request_Callback()
      : m_att(), m_request_id(0U), m_cb(nullptr) {  }

    /// @brief Constructs callback, will be called upon shared attribute request arrival
    /// where the given multiple requested shared attributes were sent by the cloud and received by the client
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array
    /// @param first_itr Iterator pointing to the first element in the data container
    /// @param last_itr Iterator pointing to the end of the data container (last element + 1)
    /// @param cb Callback method that will be called
    template<class InputIterator>
    inline Shared_Attribute_Request_Callback(const InputIterator& first_itr, const InputIterator& last_itr, processFn cb)
      : m_att(first_itr, last_itr), m_request_id(0U), m_cb(cb) {  }

    /// @brief Calls the callback that was subscribed, when this class instance was initally created
    /// @tparam Logger Logging class that should be used to print messages
    /// @param data Received shared attribute request data that include
    /// the shared attributes that were requested and their current values
    template<typename Logger>
    inline returnType Call_Callback(argumentType data) const {
      // Check if the callback is a nullptr,
      // meaning it has not been assigned any valid callback method.
      if (!m_cb) {
        Logger::log(ATT_REQUEST_CB_IS_NULL);
        return returnType();
      }
      return m_cb(data);
    }

    /// @brief Gets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which Shared_Attribute_Request_Callback
    /// is connected to which received shared attributes
    /// @return Unique identifier connected to the requested shared attributes
    inline const uint32_t& Get_Request_ID() const {
      return m_request_id;
    }

    /// @brief Sets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which Shared_Attribute_Request_Callback
    /// is connected to which received shared attributes
    /// @param request_id Unqiue identifier of the request for shared attributes
    inline void Set_Request_ID(const uint32_t& request_id) {
      m_request_id = request_id;
    }

    /// @brief Gets all the requested shared attributes that will result,
    /// in the subscribed method being called when the response with their current value
    // is sent from the cloud and received by the client,
    /// passed when this class instance was initally created
    /// @return Requested shared attributes
    inline const std::vector<const char *>& Get_Attributes() const {
      return m_att;
    }

  private:
    std::vector<const char *>      m_att;          // Attribute we want to request
    uint32_t                       m_request_id;   // Id the request was called with
    processFn                      m_cb;           // Callback to call
};

#endif // Shared_Attribute_Request_Callback
