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
    /// @brief Prints the given format message containing format specifiers (subsequences beginning with %) as well as a new line character at the end of the message
    /// @tparam ...Args Additional arguments formatted and inserted in the resulting string replacing their respective specifiers.
    /// See https://cplusplus.com/reference/cstdio/printf/ for more information on the possible format specifiers and the corresponding argument type
    /// @param format Formatting message that the given arguments will be inserted into
    /// @param ...args Arguments that will be formatted and inserted into the resulting string, replacing their respective specifiers
    /// @return Either the written amount of characters or an error indicator (being a negative number) if one occured
    template<typename ...Args>
    static int printfln(char const * format, Args const &... args) {
        int const size = Helper::detectSize(format, args...);
        char arguments[size] = {};
        int const written_characters = snprintf(arguments, size, format, args...);
        // Written characters is expected to be one less, because of the null termination character
        bool const result = (written_characters == (size - 1));
        return println(result ? arguments : FAILED_MESSAGE);
    }

    /// @brief Prints the given message containing a new line character at the end of the message, but no format specifiers (subsequences beginning with %)
    /// @param message Message that should be simply printed to the console, without any additional arguments, use peintfln() for that
    /// @return Either the written amount of characters or an error indicator (being a negative number) if one occured
    static int println(char const * message) {
        return printf(LOG_MESSAGE_FORMAT, message);
    }
};

#endif // Default_Logger_h
