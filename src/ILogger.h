#ifndef ILogger_h
#define ILogger_h

/// @brief Logger interface that contains the method that a class that can print log messages has to implement.
/// Allows to safe the given log messages into a file or simply print them onto the console output, or do both
class ILogger {
  public:
    /// @brief Prints the given format message containing format specifiers (subsequences beginning with %) as well as a new line character at the end of the message.
    /// The additional arguments are formatted and inserted in the resulting string replacing their respective specifiers.
    /// See https://cplusplus.com/reference/cstdio/printf/ for more information on the possible format specifiers
    /// @param format Formatting message that the given arguments will be inserted into
    /// @param ... Arguments that will be formatted and inserted into the resulting string, replacing their respective specifiers
    /// @return Either the written amount of characters or an error indicator (being a negative number) if one occured
    virtual int printfln(const char *format, ...) const = 0;

    /// @brief Prints the given message containing no format specifiers (subsequences beginning with %) as well as a new line character at the end of the message
    /// @param message Message that should be simply printed to the console, without any additional arguments
    /// @return Either the written amount of characters or an error indicator (being a negative number) if one occured
    virtual int println(const char *message) const = 0;
};

#endif // ILogger_h
