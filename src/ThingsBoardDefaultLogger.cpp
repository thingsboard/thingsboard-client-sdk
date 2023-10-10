// Header include.
#include "ThingsBoardDefaultLogger.h"

// Local includes.
#include "Configuration.h"

// Library include.
#if THINGSBOARD_ENABLE_PROGMEM
#include <WString.h>
#endif // THINGSBOARD_ENABLE_PROGMEM
#include <stdio.h>


// Log messages.
#if THINGSBOARD_ENABLE_PROGMEM
constexpr char LOG_MESSAGE_FORMAT[] PROGMEM = "[TB] %s\n";
#else
constexpr char LOG_MESSAGE_FORMAT[] = "[TB] %s\n";
#endif // THINGSBOARD_ENABLE_PROGMEM


void ThingsBoardDefaultLogger::log(const char *msg) {
    printf(LOG_MESSAGE_FORMAT, msg);
}
