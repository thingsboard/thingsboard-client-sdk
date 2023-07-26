/*
  Shared_Attribute_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Shared_Attribute_Callback_h
#define Shared_Attribute_Callback_h

// Local includes.
#include "Callback.h"

// Library includes.
#include <ArduinoJson.h>
#if THINGSBOARD_ENABLE_STL
#include <vector>
#endif // THINGSBOARD_ENABLE_STL

/// ---------------------------------
/// Constant strings in flash memory.
/// ---------------------------------
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char ATT_CB_IS_NULL[] PROGMEM = "Shared attribute update callback is NULL";
#else
constexpr char ATT_CB_IS_NULL[] = "Shared attribute update callback is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM

// Convenient aliases
// JSON object const (read only twice as small as JSON object), is used to communicate Shared Attributes and Provision Data to the client
using Shared_Attribute_Data = const JsonObjectConst;

/// @brief Shared attributes callback wrapper
class Shared_Attribute_Callback : public Callback<void, const Shared_Attribute_Data&>  {
  public:
    /// @brief Constructs empty callback, will result in never being called
    Shared_Attribute_Callback();

    /// @brief Constructs callback, will be called upon shared attribute update arrival,
    /// of any existing or new shared attribute on the given device
    /// @param cb Callback method that will be called
    explicit Shared_Attribute_Callback(function cb);

#if THINGSBOARD_ENABLE_STL

    /// @brief Constructs callback, will be called upon shared attribute update arrival,
    /// where atleast one of the given multiple shared attributes passed was updated by the cloud.
    /// If the update does not include any of the given shared attributes the callback is not called
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array
    /// @param first_itr Iterator pointing to the first element in the data container
    /// @param last_itr Iterator pointing to the end of the data container (last element + 1)
    /// @param cb Callback method that will be called
    template<class InputIterator>
    Shared_Attribute_Callback(const InputIterator &first_itr, const InputIterator &last_itr, function cb)
      : Callback(cb, ATT_CB_IS_NULL)
      , m_attributes(first_itr, last_itr)
      {
          // Nothing to do
      }

    /// @brief Gets all the subscribed shared attributes that will result,
    /// in the subscribed method being called if changed by the cloud
    /// @return Subscribed shared attributes
    const std::vector<const char *>& Get_Attributes() const;

    /// @brief Sets all the subscribed shared attributes that will result,
    /// in the subscribed method being called if changed by the cloud
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array
    /// @param first_itr Iterator pointing to the first element in the data container
    /// @param last_itr Iterator pointing to the end of the data container (last element + 1)
    template<class InputIterator>
    inline void Set_Attributes(const InputIterator &first_itr, const InputIterator &last_itr) {
        m_attributes.assign(first_itr, last_itr);
    }

#else

    /// @brief Constructs callback, will be called upon upon shared attribute update arrival,
    /// where atleast one of the given multiple shared attributes passed was updated by the cloud.
    /// If the update does not include any of the given shared attributes the callback is not called
    /// @param attributes Comma seperated string containing all attributes we want to subscribe (test1, test2, ...)
    /// @param cb Callback method that will be called
    Shared_Attribute_Callback(const char *attributes, function cb);

    /// @brief Gets the string containing all the requested client-side or shared attributes that will result,
    /// in the subscribed method being called when the response with their current value
    // is sent from the cloud and received by the client
    /// @return Requested client-side or shared attributes
    const char* Get_Attributes() const;

    /// @brief Sets the string containing all the requested client-side or shared attributes that will result,
    /// in the subscribed method being called when the response with their current value
    // is sent from the cloud and received by the client
    /// @param attributes Requested client-side or shared attributes
    void Set_Attributes(const char *attributes);

#endif // THINGSBOARD_ENABLE_STL

  private:
#if THINGSBOARD_ENABLE_STL
    std::vector<const char *>      m_attributes;    // Attribute we want to request
#else
    const char                     *m_attributes;   // Attribute we want to request
#endif // THINGSBOARD_ENABLE_STL
};

#endif // Shared_Attribute_Callback
