#ifndef Callback_h
#define Callback_h

// Local includes.
#include "Configuration.h"
#include "ILogger.h"

// Library includes.
#if THINGSBOARD_ENABLE_STL
#include <functional>
#endif // THINGSBOARD_ENABLE_STL


/// @brief General purpose callback wrapper. Expects either c-style or c++ style function pointer,
/// depending on if the C++ STL has been implemented on the given device or not
/// @tparam returnType Type the given callback method should return
/// @tparam argumentTypes Types the given callback method should receive
template<typename returnType, typename... argumentTypes>
class Callback {
  public:
    /// @brief Callback signature
#if THINGSBOARD_ENABLE_STL
    using function = std::function<returnType(argumentTypes... arguments)>;
#else
    using function = returnType (*)(argumentTypes... arguments);
#endif // THINGSBOARD_ENABLE_STL

    /// @brief Constructs base callback, will be called upon specific arrival of json message
    /// where the requested data was sent by the cloud and received by the client
    /// @param callback Callback method that will be called upon data arrival with the given data that was received serialized into the given arguemnt types
    /// @param message Message that is logged if the callback given initally is a nullptr and can therefore not be called,
    /// used to ensure users are informed that the initalization of the child class is invalid
    inline Callback(function callback, const char *message)
      : m_callback(callback)
      , m_message(message)
    {
        // Nothing to do
    }

    /// @brief Calls the callback that was subscribed, when this class instance was initally created
    /// @param logger Logger implementation that should be used to print messages generated by internal processes
    /// @param arguments Received client-side or shared attribute request data that include
    /// the client-side or shared attributes that were requested and their current values
    inline returnType Call_Callback(const ILogger& logger, argumentTypes... arguments) const {
        // Check if the callback is a nullptr,
        // meaning it has not been assigned any valid callback method
        if (!m_callback) {
          logger.print(m_message);
          return returnType();
        }
        return m_callback(arguments...);
    }

    /// @brief Sets the callback method that will be called upon data arrival with the given data that was received serialized into the given argument types,
    /// used to change the callback initally passed or to set the callback if it was not passed as an argument initally
    /// @param callback Callback method that will be called upon data arrival with the given data that was received serialized into the given argument types
    inline void Set_Callback(function callback) {
        m_callback = callback;
    }

  private:
    function   m_callback; // Callback to call
    const char *m_message; // Message to print if callback is a nullptr
};

#endif // Callback_h
