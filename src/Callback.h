#ifndef Callback_h
#define Callback_h

// Local includes.
#include "Configuration.h"
#if !THINGSBOARD_ENABLE_STL && THINGSBOARD_ENABLE_DYNAMIC
#include "Vector.h"
#else
#include "Array.h"
#endif // !THINGSBOARD_ENABLE_STL && THINGSBOARD_ENABLE_DYNAMIC

// Library includes.
#include <ArduinoJson.h>
#if THINGSBOARD_ENABLE_STL
#include <functional>
#include <vector>
#endif // THINGSBOARD_ENABLE_STL


#if THINGSBOARD_ENABLE_STL && THINGSBOARD_ENABLE_DYNAMIC
/// @brief Vector signature, makes it possible to use the Vector name everywhere instead of having to differentiate between C++ STL support or not
template<typename T>
using Vector = std::vector<T>;
#endif // THINGSBOARD_ENABLE_STL && THINGSBOARD_ENABLE_DYNAMIC


/// @brief General purpose safe callback wrapper. Expects either c-style or c++ style function pointer,
/// depending on if the C++ STL has been implemented on the given device or not.
/// Simply wraps that function pointer and before calling it ensures it actually exists
/// @tparam return_typ Type the given callback method should return
/// @tparam argument_types Types the given callback method should receive
template<typename return_typ, typename... argument_types>
class Callback {
  public:
    /// @brief Callback signature
#if THINGSBOARD_ENABLE_STL
    using function = std::function<return_typ(argument_types... arguments)>;
#else
    using function = return_typ (*)(argument_types... arguments);
#endif // THINGSBOARD_ENABLE_STL

    /// @brief Constructs empty callback, will result in never being called. Internals are simply default constructed as nullptr
    Callback() = default;

    /// @brief Constructor
    /// @param callback Callback method that will be called upon data arrival with the given data that was received serialized into the given arguemnt types.
    /// If nullptr is passed the callback will never be called and return with a defaulted instance of the requested return variable
    explicit Callback(function callback)
      : m_callback(callback)
    {
        // Nothing to do
    }

    /// @brief Calls the callback that was subscribed, when this class instance was initally created.
    /// If the default constructor was used or a nullptr was passed instead of a valid function pointer,
    /// this method will check beforehand and simply return with a defaulted instance of the requested return variable
    /// @param ...arguments Optional additional arguments that are simply formwarded to the subscribed callback if it exists
    /// @return Argument returned by the previously subscribed callback or if none or nullptr is subscribed
    /// we instead return a defaulted instance of the requested return variable
    return_typ Call_Callback(argument_types const &... arguments) const {
        if (!m_callback) {
          return return_typ();
        }
        return m_callback(arguments...);
    }

    /// @brief Sets the callback method that will be called upon data arrival with the given data that was received serialized into the given argument types,
    /// used to change the callback initally passed or to set the callback if it was not passed as an argument initally
    /// @param callback Callback method that will be called upon data arrival with the given data that was received serialized into the given argument types
    void Set_Callback(function callback) {
        m_callback = callback;
    }

  private:
    function   m_callback = {}; // Callback to call
};

#endif // Callback_h
