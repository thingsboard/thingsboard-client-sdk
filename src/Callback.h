#ifndef Callback_h
#define Callback_h

// Local includes.
#include "Configuration.h"

// Library includes.
#include <ArduinoJson.h>
#if THINGSBOARD_ENABLE_STL
#include <functional>
#include <vector>
#endif // THINGSBOARD_ENABLE_STL


#if THINGSBOARD_ENABLE_STL && THINGSBOARD_ENABLE_DYNAMIC
/// @brief Container signature, makes it possible to use the Container name everywhere instead of having to differentiate between C++ STL support or not
template<typename T>
using Container = std::vector<T>;
#else
#include "Container.h"
#endif // THINGSBOARD_ENABLE_STL && THINGSBOARD_ENABLE_DYNAMIC


/// @brief General purpose safe callback wrapper. Expects either c-style or c++ style function pointer,
/// depending on if the C++ STL has been implemented on the given device or not.
/// Simply wraps that function pointer and before calling it ensures it is valid
/// @tparam return_type Type the given callback method should return
/// @tparam argument_types Types the given callback method should receive
template<typename return_type, typename... argument_types>
class Callback {
  public:
    /// @brief Callback signature
#if THINGSBOARD_ENABLE_STL
    using function = std::function<return_type(argument_types... arguments)>;
#else
    using function = return_type (*)(argument_types... arguments);
#endif // THINGSBOARD_ENABLE_STL

    /// @brief Constructs empty callback, will result in never being called. Internals are simply default constructed as nullptr
    Callback() = default;

    /// @brief Constructor
    /// @param callback Callback method that will be called upon data arrival with the given data that was received.
    /// If nullptr is passed the callback will never be called and instead return with a defaulted instance of the requested return variable
    explicit Callback(function callback)
      : m_callback(callback)
    {
        // Nothing to do
    }

    /// @brief Virtual default destructor, created to ensure that if a pointer to this class is used and deleted, we will also call the derived base class destructor
    /// @note Deleting a base class destructor that does not have a virtual destructor is undefined behaviour, because the derived class destructor originally instantiated with new is never called.
    /// This can cause potential memory leaks, because derived classes can not clean up their internal members as expected and instead simply leak them
    virtual ~Callback() = default;

    /// @brief Calls the callback that was subscribed, when this class instance was initally created
    /// @note If the default constructor was used or a nullptr was passed instead of a valid function pointer,
    /// this method will simply return a defaulted instance of the requested return variable instead
    /// @param ...arguments Optional additional arguments that are simply formwarded to the subscribed callback if it exists
    /// @return Argument returned by the previously subscribed callback or if none or nullptr is subscribed
    /// we instead return a defaulted instance of the requested return variable
    return_type Call_Callback(argument_types const &... arguments) const {
        if (!m_callback) {
          return return_type();
        }
        return m_callback(arguments...);
    }

    /// @brief Sets the callback method that will be called upon data arrival with the given data that was received.
    /// If nullptr is passed the callback will never be called and instead return with a defaulted instance of the requested return variable
    /// @param callback callback method that will be called upon data arrival with the given data that was received
    void Set_Callback(function callback) {
        m_callback = callback;
    }

  private:
    function   m_callback = {}; // Callback to call
};

#endif // Callback_h
