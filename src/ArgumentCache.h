/*
  ArgumentCache.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef ArgumentCache_h
#define ArgumentCache_h

// Local include
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

// Library includes
#include <tuple>

/// @brief Cache arguments passed to a method so it can be recalled later with the same arguments
/// @tparam Args All arguments that a method was called with and should be called, is a variadic template (allowing an undefined amount of template arguments).
template <class... Args>
class ArgumentCache {
  public:
    /// @brief Constructor
    ArgumentCache(Args... args) :
        m_argument(std::forward<Args>(args)...)
    {
        // Nothing to do.
    }

    const std::tuple<Args...>& getArguments() const {
        return m_argument;
    }

private:
    const std::tuple<Args...> m_argument;
};

#endif // THINGSBOARD_ENABLE_OTA

#endif // ArgumentCache_h
