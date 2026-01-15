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
/// Be aware though the size set in this template and the size passed to the ThingsBoard MaxAttributes template need to be the same or the value in this class lower,
/// if not some of the requested keys may be lost, default = DEFAULT_ATTRIBUTES_AMOUNT (1)
template <size_t MaxAttributes = DEFAULT_ATTRIBUTES_AMOUNT>
#endif // !THINGSBOARD_ENABLE_DYNAMIC
class Shared_Attribute_Callback : public Callback<void, JsonObjectConst const &>  {
#if THINGSBOARD_ENABLE_DYNAMIC
    using CString_Container = Container<char const *>;
#else
    using CString_Container = Container<char const *, MaxAttributes>;
#endif // THINGSBOARD_ENABLE_DYNAMIC

  public:
    /// @brief Constructs empty callback, will result in never being called. Internals are simply default constructed as nullptr
    Shared_Attribute_Callback() = default;

    /// @brief Constructs callback, will be called upon shared attribute update arrival, where atleast one of the given multiple shared attributes subscribed was updated by the cloud
    /// @note If the update does not include any of the given shared attributes the callback is not called.
    /// @note Directly forwards the given arguments to the overloaded Container constructor,
    /// meaning all combinatons of arguments that would initalize an std::vector can be used to call this constructor.
    /// See possible std::vector constructors here https://en.cppreference.com/w/cpp/container/vector/vector, for the possible passable parameters.
    /// The possibilites mainly consist out of the fill constructor, where a number n and a value is given and then the value is copied into that many elements,
    /// alternatively if no value is given the default constructed value is copied n times instead,
    /// or the range constructor where we can pass an interator to the start and to the end of the data container (last element + 1)
    /// to copy every element in between thoose iterators, in the same order as in the original data container.
    /// The last option is a copy constructor where we pass another container and all the values of that container will be copied into our buffer
    /// @tparam ...Args Holds the multiple arguments that will simply be forwarded to the container constructor and therefore allow to use every overloaded constructor without having to explicitly implement them
    /// @param callback callback method that will be called upon data arrival with the given data that was received.
    /// If nullptr is passed the callback will never be called and instead return with a defaulted instance of the requested return variable
    /// @param ...args Attributes to suscribe, that will be forwarded into the overloaded Container constructor see https://en.cppreference.com/w/cpp/container/vector/vector for more information
    template<typename... Args>
    Shared_Attribute_Callback(function callback, Args const &... args)
      : Callback(callback)
      , m_attributes(args...)
    {
        // Nothing to do
    }

    ~Shared_Attribute_Callback() override = default;

    /// @brief Gets all the subscribed shared attributes that will result in the callback method being called if any of those attributes values is changed by the cloud
    /// @return Subscribed shared attributes
    CString_Container const & Get_Attributes() const {
        return m_attributes;
    }

    /// @brief Sets all the subscribed shared attributes that will result in the callback method being called if any of those attributes values is changed by the cloud
    /// @note Directly forwards the given arguments to the overloaded Container constructor,
    /// meaning all combinatons of arguments that would initalize an std::vector can be used to call this constructor.
    /// See possible std::vector constructors here https://en.cppreference.com/w/cpp/container/vector/vector, for the possible passable parameters.
    /// The possibilites mainly consist out of the fill constructor, where a number n and a value is given and then the value is copied into that many elements,
    /// alternatively if no value is given the default constructed value is copied n times instead,
    /// or the range constructor where we can pass an interator to the start and to the end of the data container (last element + 1)
    /// to copy every element in between thoose iterators, in the same order as in the original data container.
    /// The last option is a copy constructor where we pass another container and all the values of that container will be copied into our buffer
    /// @tparam ...Args Holds the multiple arguments that will simply be forwarded to the Container assign method and therefore allow to use every overloaded Container assign without having to implement them
    /// @param ...args Attributes to subscribe, that will be forwarded into the overloaded Container assign method see https://en.cppreference.com/w/cpp/container/vector/assign for more information
    template<typename... Args>
    void Set_Attributes(Args const &... args) {
        m_attributes.assign(args...);
    }

  private:
    CString_Container m_attributes = {}; // Shared attribute we want to subscribe to receive a message if they change
};

#endif // Shared_Attribute_Callback
