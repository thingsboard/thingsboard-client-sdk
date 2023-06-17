/*
  Helper.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Helper_h
#define Helper_h

// Library include.
#include <stdint.h>

/// @brief Static helper class that includes some uniliterally used functionalities in multiple places
class Helper {
  public:
    /// @brief Returns the length in characters needed for a given value with the given argument string to be displayed completly
    /// @param msg Formating message that the given argument will be inserted into
    /// @param ... Additional arguments that should be inserted into the message at the given points,
    /// see https://cplusplus.com/reference/cstdio/printf/ for more information on the possible arguments
    /// @return Length in characters, needed for the given message with the given values inserted to be displayed completly
    static uint8_t detectSize(const char *msg, ...);

    /// @brief Returns the amount of occurences of the given smybol in the given string
    /// @param str String that we want to check the symbol in
    /// @param symbol Symbols we want to search for
    /// @return Amount of occurences of the given symbol
    static uint32_t getOccurences(const char *str, char symbol);
};

#endif // Helper
