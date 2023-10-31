#ifndef Thingsboard_Default_Logger_h
#define Thingsboard_Default_Logger_h

// Local includes.
#include "ILogger.h"

/// @brief Default logger class used by the ThingsBoard class to log messages into the console
class DefaultLogger : public ILogger {
  public:
    int log(const char *message) const override;

    int logf(const char *format, ...) const override;
};

#endif // Thingsboard_Default_Logger_h
