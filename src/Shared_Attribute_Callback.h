#ifndef Shared_Attribute_Callback_h
#define Shared_Attribute_Callback_h

// Local includes.
#include "Callback.h"
#include "Constants.h"


#if THINGSBOARD_ENABLE_PROGMEM
constexpr char ATT_CB_IS_NULL[] PROGMEM = "Shared attribute update callback is NULL";
#else
constexpr char ATT_CB_IS_NULL[] = "Shared attribute update callback is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM


/// @brief Shared attribute update callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// Documentation about the specific use of shared attribute update  in ThingsBoard can be found here https://thingsboard.io/docs/reference/mqtt-api/#subscribe-to-attribute-updates-from-the-server
/// @tparam MaxAttributes Maximum amount of attributes that will ever be requested with this instance of the class, allows to use an array on the stack in the background.
/// Be aware though the size set in this template and the size passed to the ThingsBoard MaxAttributes template need to be the same or some of the requested keys may be lost, default = 5
#if !THINGSBOARD_ENABLE_DYNAMIC
template <size_t MaxAttributes = Default_Attributes_Amount>
#endif // !THINGSBOARD_ENABLE_DYNAMIC
class Shared_Attribute_Callback : public Callback<void, const JsonObjectConst&>  {
  public:
    /// @brief Constructs empty callback, will result in never being called
    Shared_Attribute_Callback() :
        Shared_Attribute_Callback(nullptr)
    {
        // Nothing to do
    }

    /// @brief Constructs callback, will be called upon shared attribute update arrival,
    /// of any existing or new shared attribute on the given device
    /// @param cb Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    explicit Shared_Attribute_Callback(function cb) :
        Callback(cb, ATT_CB_IS_NULL),
        m_attributes()
    {
        // Nothing to do
    }

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
      , m_attributes(args...)
    {
        // Nothing to do
    }

    /// @brief Gets all the subscribed shared attributes that will result,
    /// in the subscribed method being called if any of those attributes values is changed by the cloud,
    /// with their current value they have been changed to
    /// @return Subscribed shared attributes
#if THINGSBOARD_ENABLE_DYNAMIC
    const Vector<const char *>& Get_Attributes() const {
#else
    const Array<const char *, MaxAttributes>& Get_Attributes() const {
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
    inline void Set_Attributes(Args... args) {
        m_attributes.assign(args...);
    }

  private:
#if THINGSBOARD_ENABLE_DYNAMIC
    Vector<const char *>                 m_attributes; // Shared attribute we want to subscribe to receive a message if they change
#else
    Array<const char *, MaxAttributes>   m_attributes; // Shared attribute we want to subscribe to receive a message if they change
#endif // THINGSBOARD_ENABLE_DYNAMIC
};

#endif // Shared_Attribute_Callback
