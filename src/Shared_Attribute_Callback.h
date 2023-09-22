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
// JSON object const (read only twice as small as JSON object), is used to communicate Shared Attributes to the client
using Shared_Attribute_Data = const JsonObjectConst;


/// @brief Shared attribute update callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// Documentation about the specific use of shared attribute update  in ThingsBoard can be found here https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server
class Shared_Attribute_Callback : public Callback<void, const Shared_Attribute_Data&>  {
  public:
    /// @brief Constructs empty callback, will result in never being called
    Shared_Attribute_Callback();

    /// @brief Constructs callback, will be called upon shared attribute update arrival,
    /// of any existing or new shared attribute on the given device
    /// @param cb Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    explicit Shared_Attribute_Callback(function cb);

#if THINGSBOARD_ENABLE_STL

    /// @brief Constructs callback, will be called upon shared attribute update arrival,
    /// where atleast one of the given multiple shared attributes passed was updated by the cloud.
    /// If the update does not include any of the given shared attributes the callback is not called.
    /// Directly forwards the given arguments to the overloaded vector constructor,
    /// meaning all combinatons of arguments that would initalize a vector can be used to call this constructor.
    /// See possible vector constructors https://en.cppreference.com/w/cpp/container/vector/vector, for the possible passed parameters.
    /// The possibilites mainly consist out of the fill constructor, where a value and a number n is given and then that many elements of the value will be copied into
    /// or be created with their default value, or out of the range constructor where we can pass an interator the start of another data container
    /// and to the end of the data container (last element + 1)
    /// and then every element between those iteratos will be copied, in the same order as in the original data container.
    /// The last option is a copy constructor where we pass a vector and the values of that vector will be copied into our buffer
    /// @tparam ...Args Holds the multiple arguments that will simply be forwarded to the vector constructor and therefore allow to use every overloaded vector constructor without having to implement them
    /// @param callback Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    /// @param ...args Arguments that will be forwarded into the overloaded vector constructor see https://en.cppreference.com/w/cpp/container/vector/vector for more information
    template<typename... Args>
    inline Shared_Attribute_Callback(function callback, Args... args)
      : Callback(callback, ATT_CB_IS_NULL)
      , m_attributes(std::forward<Args>(args)...)
    {
        // Nothing to do
    }

    /// @brief Gets all the subscribed shared attributes that will result,
    /// in the subscribed method being called if any of those attributes values is changed by the cloud,
    /// with their current value they have been changed to
    /// @return Subscribed shared attributes
    const std::vector<const char *>& Get_Attributes() const;

    /// @brief Sets all the subscribed shared attributes that will result,
    /// in the subscribed method being called if any of those attributes values is changed by the cloud,
    /// with their current value they have been changed to.
    /// Directly forwards the given arguments to the overloaded vector assign method,
    /// meaning all combinatons of arguments that can call the assign method on a vector can be used to call this method.
    /// See possible overloaded vector assign methods https://en.cppreference.com/w/cpp/container/vector/assign, for the possible passed parameters.
    /// The possibilites mainly consist out of the fill assign method, where a value and a number n is given and then that many elements of the value will be copied into
    /// or be created with their default value, or out of the range assign method where we can pass an interator the start of another data container
    /// and to the end of the data container (last element + 1)
    /// and then every element between those iteratos will be copied, in the same order as in the original data container.
    /// The last option is a copy assign method where we pass a vector and the values of that vector will be copied into our buffer
    /// @tparam ...Args Holds the multiple arguments that will simply be forwarded to the vector assign method and therefore allow to use every overloaded vector assign without having to implement them
    /// @param ...args Arguments that will be forwarded into the overloaded vector assign method see https://en.cppreference.com/w/cpp/container/vector/assign for more information
    template<typename... Args>
    inline void Set_Attributes(Args... args) {
        m_attributes.assign(std::forward<Args>(args)...);
    }

#else

    /// @brief Constructs callback, will be called upon upon shared attribute update arrival,
    /// where atleast one of the given multiple shared attributes passed was updated by the cloud.
    /// If the update does not include any of the given shared attributes the callback is not called
    /// @param attributes Comma seperated string containing all attributes we want to subscribe (test1, test2, ...)
    /// @param cb Callback method that will be called
    Shared_Attribute_Callback(const char *attributes, function cb);

    /// @brief Gets the string containing all the requested client-side or shared attributes that will result,
    /// in the subscribed method being called if any of those attributes values is changed by the cloud,
    /// with their current value they have been changed to
    /// @return Subscribed shared attributes
    const char* Get_Attributes() const;

    /// @brief Sets the string containing all the requested client-side or shared attributes that will result,
    /// in the subscribed method being called if any of those attributes values is changed by the cloud,
    /// with their current value they have been changed to
    /// @param attributes Subscribed shared attributes
    void Set_Attributes(const char *attributes);

#endif // THINGSBOARD_ENABLE_STL

  private:
#if THINGSBOARD_ENABLE_STL
    std::vector<const char *>      m_attributes;    // Shared attribute we want to subscribe to receive a message if they change
#else
    const char                     *m_attributes;   // Shared attribute we want to subscribe to receive a message if they change
#endif // THINGSBOARD_ENABLE_STL
};

#endif // Shared_Attribute_Callback
