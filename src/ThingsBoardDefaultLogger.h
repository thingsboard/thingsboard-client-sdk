#ifndef Thingsboard_Default_Logger_h
#define Thingsboard_Default_Logger_h

/// @brief Default logger class used by the ThingsBoard class to log messages into the console
class ThingsBoardDefaultLogger {
  public:
    /// @brief Logs the given message to the serial console
    /// Ensure to initalize the serial before calling this method
    /// @param msg Message we want to print into the console
    static void log(const char* msg);
};

#endif // Thingsboard_Default_Logger_h
