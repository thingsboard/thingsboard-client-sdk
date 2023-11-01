#ifndef ILogger_h
#define ILogger_h

/// @brief Logger interface that contains the method that a class that can print log messages has to implement.
/// Allows to safe the given log messages into a file or simply print them onto the console output, or do both
class ILogger {
  public:
    /// @brief Prints the given format message containing format specifiers (subsequences beginning with %), the additional arguments following format are formatted and inserted in the resulting string replacing their respective specifiers.
    /// See https://cplusplus.com/reference/cstdio/printf/ for more information on the possible format specifiers
    /// @param format Format message the additional arguments will be formatedd into
    /// @param  ... Additional arguments that will be formatted and inserted into the resulting string, replacing their respective specifiers
    /// @return Either the written amount of characters or an error indicater (being a negative number) if one occured
    virtual int log(const char *format, ...) const = 0;
};

#endif // ILogger_h
