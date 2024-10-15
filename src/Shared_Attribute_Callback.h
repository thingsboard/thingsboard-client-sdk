#ifndef Shared_Attribute_Callback_h
#define Shared_Attribute_Callback_h

// Local includes.
#include "Callback.h"
#if !THINGSBOARD_ENABLE_DYNAMIC
#include "Constants.h"
#endif // !THINGSBOARD_ENABLE_DYNAMIC


/// @brief Shared attribute update callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// Documentation about the specific use of shared attribute update  in ThingsBoard can be found here https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server
#if !THINGSBOARD_ENABLE_DYNAMIC
/// @tparam MaxAttributes Maximum amount of attributes that will ever be requested with this instance of the class, allows to use an array on the stack in the background.
/// Be aware though the size set in this template and the size passed to the ThingsBoard MaxAttributes template need to be the same or the value in this class lower, if not some of the requested keys may be lost, default = Default_Attributes_Amount (5)
template <size_t MaxAttributes = Default_Attributes_Amount>
#endif // !THINGSBOARD_ENABLE_DYNAMIC
class Shared_Attribute_Callback : public Callback<void, JsonObjectConst const &>  {
  public:
    /// @brief Constructs empty callback, will result in never being called. Internals are simply default constructed as nullptr
    Shared_Attribute_Callback() = default;

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
    Shared_Attribute_Callback(function callback, Args const &... args)
      : Callback(callback)
      , m_attributes(args...)
    {
        // Nothing to do
    }

    /// @brief Gets all the subscribed shared attributes that will result,
    /// in the subscribed method being called if any of those attributes values is changed by the cloud,
    /// with their current value they have been changed to
    /// @return Subscribed shared attributes
#if THINGSBOARD_ENABLE_DYNAMIC
    Vector<char const *> const & Get_Attributes() const {
#else
    Array<char const *, MaxAttributes> const & Get_Attributes() const {
#endif // THINGSBOARD_ENABLE_DYNAMIC
        return m_attributes;
    }

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
    void Set_Attributes(Args const &... args) {
        m_attributes.assign(args...);
    }

  private:
#if THINGSBOARD_ENABLE_DYNAMIC
    Vector<char const *>                 m_attributes = {}; // Shared attribute we want to subscribe to receive a message if they change
#else
    Array<char const *, MaxAttributes>   m_attributes = {}; // Shared attribute we want to subscribe to receive a message if they change
#endif // THINGSBOARD_ENABLE_DYNAMIC
};

#endif // Shared_Attribute_Callback
