#ifndef Default_Logger_h
#define Default_Logger_h

// Local includes.
#include "ILogger.h"

/// @brief Default logger class used by the ThingsBoard class to log messages into the console
class DefaultLogger : public ILogger {
  public:
    virtual int logf(const char *format, ...) const override;
};

#endif // Default_Logger_h
