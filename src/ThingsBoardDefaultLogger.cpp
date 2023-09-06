// Header include.
#include "ThingsBoardDefaultLogger.h"

// Local includes.
#include "Configuration.h"

// Library include.
#if THINGSBOARD_ENABLE_PROGMEM
#include <WString.h>
#endif // THINGSBOARD_ENABLE_PROGMEM
#include <cstdio>

void ThingsBoardDefaultLogger::log(const char *msg) {
#if THINGSBOARD_ENABLE_PROGMEM
    printf(F("[TB] %s \r\n"), msg);
#else
    printf("[TB] %s \r\n", msg);
#endif // THINGSBOARD_ENABLE_PROGMEM
}
