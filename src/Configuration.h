/*
  Configuration.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Configuration_h
#define Configuration_h

// Enabled the usage of int64_t and double values with ArduinoJson. Making the JsonVariant store double and int64_t instead of float and int32_t.
// See https://arduinojson.org/v6/api/config/use_long_long/ for more information
#define ARDUINOJSON_USE_LONG_LONG 1
#define ARDUINOJSON_USE_DOUBLE 1

// Enable the usage of the STL library, depending on if needed STL base functionality is supported
#  ifdef __has_include
#    if __has_include(<string>) && __has_include(<functional>) && __has_include(<vector>) && __has_include(<iterator>)
#      ifndef THINGSBOARD_ENABLE_STL
#        define THINGSBOARD_ENABLE_STL 1
#      endif
#    else
#      ifndef THINGSBOARD_ENABLE_STL
#        define THINGSBOARD_ENABLE_STL 0
#      endif
#    endif
#  else
#    ifdef ARDUINO
#      ifndef THINGSBOARD_ENABLE_STL
#        define THINGSBOARD_ENABLE_STL 0
#      endif
#    else
#      ifndef THINGSBOARD_ENABLE_STL
#        define THINGSBOARD_ENABLE_STL 1
#      endif
#    endif
#  endif

// Enable the usage of OTA (Over the air) updates, only possible with STL base functionality
#  ifndef THINGSBOARD_ENABLE_OTA
#    if THINGSBOARD_ENABLE_STL
#      define THINGSBOARD_ENABLE_OTA 1
#    else
#      define THINGSBOARD_ENABLE_OTA 0
#    endif
#  endif

// Enable the usage of the PROGMEM header for constants variables (variables are placed into flash memory instead of sram).
#  ifdef __has_include
#    if  __has_include(<pgmspace.h>)
#      ifndef THINGSBOARD_ENABLE_PROGMEM
#        define THINGSBOARD_ENABLE_PROGMEM 1
#      endif
#    else
#      ifndef THINGSBOARD_ENABLE_PROGMEM
#        define THINGSBOARD_ENABLE_PROGMEM 0
#      endif
#    endif
#  else
#    define THINGSBOARD_ENABLE_PROGMEM 0
#  endif

// Enables the ThingsBoard class to be fully dynamic instead of requiring template arguments to statically allocate memory.
// If enabled the program might be slightly slower and all the memory will be placed onto the heap instead of the stack.
// See https://arduinojson.org/v6/api/dynamicjsondocument/ for the main difference in the underlying code.
#  ifndef THINGSBOARD_ENABLE_DYNAMIC
#    define THINGSBOARD_ENABLE_DYNAMIC 0
#  endif

// Enables the ThingsBoard class to print all received and sent messages and their topic, from and to the server,
// additionally some more debug messages will be printed. Requires more flash memory, and more Serial calls requiring more performance.
// Recommended to disable when building for release.
#  ifndef THINGSBOARD_ENABLE_DEBUG
#    define THINGSBOARD_ENABLE_DEBUG 0
#  endif

// Enables the usage of an additonal library as a fallback, to directly serialize a json message that is sent to the cloud,
// if the size of that message would be bigger than the internal buffer size of the client.
// Allows sending much bigger messages than would otherwise be possible, and without much decrease stack or heap requirements, but at the cost of increased send times.
// See https://arduinojson.org/v6/how-to/use-arduinojson-with-pubsubclient/#serializing-a-json-document-into-an-mqtt-message for the main difference int he underlying code.
#  ifndef THINGSBOARD_ENABLE_STREAM_UTILS
#    define THINGSBOARD_ENABLE_STREAM_UTILS 0
#  endif

// Enables the ThingsBoard class to save the allocated memory of the DynamicJsonDocument into psram instead of onto the sram.
// Enabled by default if THINGSBOARD_ENABLE_DYNAMIC has been set, because it requries DynamicJsonDocument to work.
// If enabled the program might be slightly slower and all the memory will be placed onto psram instead of sram.
// See https://arduinojson.org/v6/api/basicjsondocument/ for the main difference in the underlying code.
#  ifdef __has_include
#    if  THINGSBOARD_ENABLE_DYNAMIC && __has_include(<esp_heap_caps.h>)
#      ifndef THINGSBOARD_ENABLE_PSRAM
#        define THINGSBOARD_ENABLE_PSRAM 1
#      endif
#    else
#      ifndef THINGSBOARD_ENABLE_PSRAM
#        define THINGSBOARD_ENABLE_PSRAM 0
#      endif
#    endif
#  else
#    define THINGSBOARD_ENABLE_PSRAM 0
#  endif

#endif // Configuration_h
