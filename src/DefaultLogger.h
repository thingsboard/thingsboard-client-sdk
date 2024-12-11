#ifndef Default_Logger_h
#define Default_Logger_h

// Local includes.
#include "Helper.h"


// Log messages.
char constexpr FAILED_MESSAGE[] = "Invalid arguments passed to format specifiers (%) in Logger::printfln";
char constexpr LOG_MESSAGE_FORMAT[] = "[TB] %s\n";


/// @brief Default logger class used by the ThingsBoard class to log messages into the console output
class DefaultLogger {
  public:
    /// @brief Prints the given format message containing format specifiers (subsequences beginning with %) as well as a new line character at the end of the message.
    /// If no additional arguments are given it simply prints the given format messsage
    /// @tparam ...Args Additional arguments formatted and inserted in the resulting string replacing their respective specifiers.
    /// See https://cplusplus.com/reference/cstdio/printf/ for more information on the possible format specifiers and the corresponding argument type
    /// @param format Formatting message that the given arguments will be inserted into
    /// @param ...args Arguments that will be formatted and inserted into the resulting string, replacing their respective specifiers
    /// @return Either the written amount of characters or an error indicator (being a negative number) if one occured
    template<typename ...Args>
    static int printfln(char const * format, Args const &... args) {
        // Check if variadic template contains any actual additional arguments that should be inserted into format message.
        // If it does, use constexpr if statement for C++20 supported devices to optimize away the branch instruction at compile time
        // or alternatively runtime for devices that do not support C++20
#if THINGSBOARD_ENABLE_CXX20
        if constexpr (sizeof...(Args) == 0U) {
#else
        if (sizeof...(Args) == 0U) {
#endif // THINGSBOARD_ENABLE_CXX20
            return printf(LOG_MESSAGE_FORMAT, format);
        }
        else {
            int const size = Helper::detectSize(format, args...);
            char arguments[size] = {};
            int const written_characters = snprintf(arguments, size, format, args...);
            // Written characters is expected to be one less, because of the null termination character
            bool const result = (written_characters == (size - 1));
            return printf(LOG_MESSAGE_FORMAT, result ? arguments : FAILED_MESSAGE);
        }
    }
};

#endif // Default_Logger_h
