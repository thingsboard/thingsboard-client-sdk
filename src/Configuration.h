#ifndef Configuration_h
#define Configuration_h

// Include sdkconfig file it it exists to allow overwriting of some defines with the configuration entered in the Espressif IDF menuconfig.
// Only available when compiling for Espressif IDF, but allows to more easily change some configurations with a GUI instead of code.
#  ifdef __has_include
#    if __has_include(<sdkconfig.h>)
#      include <sdkconfig.h>
#    endif
#  endif

// Enabled the usage of int64_t and double values with ArduinoJson. Making the JsonVariant store double and int64_t instead of float and int32_t.
// See https://arduinojson.org/v6/api/config/use_long_long/ for more information.
#define ARDUINOJSON_USE_LONG_LONG 1
#define ARDUINOJSON_USE_DOUBLE 1

// Enable the usage of the C++ STL library, depending on if needed STL base functionality is supported,
// allows to use c++ style function pointers as callbacks removing the need to store a static pointer to the instance of the class.
// Additionally it allows to store data in the vector class, which in case it does not exist we have to fall back to an own custom implementation
// of the vector class which is less efficient. Additionally if possible only c++ strings are used
// and if it does not exist we fall back to the Arduino String class.
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

// Enable the usage of OTA (Over the air) updates, only possible with STL base functionality, theoretically possible without STL support,
// but the code would have to be adjusted at compile time depending on if the C++ STL is supported or not and that has not been implemented for OTA yet.
#  ifndef THINGSBOARD_ENABLE_OTA
#    if THINGSBOARD_ENABLE_STL
#      define THINGSBOARD_ENABLE_OTA 1
#    else
#      define THINGSBOARD_ENABLE_OTA 0
#    endif
#  endif

// Use the esp_timer header internally for handling timeouts and callbacks, as long as the header exists, because it is more efficient than the Arduino Ticker implementation,
// because we can stop the timer without having to delete it, removing the need to create a new timer to restart it. Because instead we can simply stop and start again.
#  ifdef __has_include
#    if  __has_include(<esp_timer.h>)
#      ifndef THINGSBOARD_USE_ESP_TIMER
#        define THINGSBOARD_USE_ESP_TIMER 1
#      endif
#    else
#      ifndef THINGSBOARD_USE_ESP_TIMER
#        define THINGSBOARD_USE_ESP_TIMER 0
#      endif
#    endif
#  else
#    define THINGSBOARD_USE_ESP_TIMER 0
#  endif

// Use the mqtt_client header internally for handling the sending and receiving of MQTT data, as long as the header exists,
// to allow users that do have the needed component to use the Espressif_MQTT_Client instead of only the Arduino_MQTT_Client.
#  ifdef __has_include
#    if  __has_include(<mqtt_client.h>)
#      ifndef THINGSBOARD_USE_ESP_MQTT
#        define THINGSBOARD_USE_ESP_MQTT 1
#      endif
#    else
#      ifndef THINGSBOARD_USE_ESP_MQTT
#        define THINGSBOARD_USE_ESP_MQTT 0
#      endif
#    endif
#  else
#    define THINGSBOARD_USE_ESP_MQTT 0
#  endif

// Use the mbed_tls header internally for handling the creation of hashes from binary data, as long as the header exists,
// because if it is already included we do not need to rely on and incude external lbiraries like Seeed_mbedtls.h, which implements the same features.
#  ifdef __has_include
#    if  __has_include(<mbedtls/md.h>)
#      ifndef THINGSBOARD_USE_MBED_TLS
#        define THINGSBOARD_USE_MBED_TLS 1
#      endif
#    else
#      ifndef THINGSBOARD_USE_MBED_TLS
#        define THINGSBOARD_USE_MBED_TLS 0
#      endif
#    endif
#  else
#    define THINGSBOARD_USE_MBED_TLS 0
#  endif

// Use the esp_ota_ops header internally for handling the writing of ota update data, as long as the header exists,
// to allow users that do have the needed component to use the Espressif_Updater instead of only the Arduino_ESP32_Updater.
#  ifdef __has_include
#    if  __has_include(<esp_ota_ops.h>)
#      ifndef THINGSBOARD_USE_ESP_PARTITION
#        define THINGSBOARD_USE_ESP_PARTITION 1
#      endif
#    else
#      ifndef THINGSBOARD_USE_ESP_PARTITION
#        define THINGSBOARD_USE_ESP_PARTITION 0
#      endif
#    endif
#  else
#    define THINGSBOARD_USE_ESP_PARTITION 0
#  endif

// Use the pgmspace header internally for enalbing the usage of the PROGMEm header for constant variables, as long as the header exists,
// to allow variables to be placed into flash memory instead of sram, meaning the sram can be allocated for other things.
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
// Can also optionally be configured via the ESP-IDF menuconfig, if that is the done the value is set to the value entered in the menuconfig,
// if the value is manually overriden tough with a #define before including ThingsBoard then the hardcoded value takes precendence.
#  ifndef THINGSBOARD_ENABLE_DYNAMIC
#    ifndef CONFIG_THINGSBOARD_ENABLE_DYNAMIC
#      define THINGSBOARD_ENABLE_DYNAMIC 0
#    else
#      define THINGSBOARD_ENABLE_DYNAMIC CONFIG_THINGSBOARD_ENABLE_DYNAMIC
#    endif
#  endif

// Enables the ThingsBoard class to print all received and sent messages and their topic, from and to the server,
// additionally some more debug messages will be printed. Requires more flash memory, and more calls to the console requiring more performance.
// Recommended to disable when building for release, should only be enabled to debug where a issue might stem from.
// Can also optionally be configured via the ESP-IDF menuconfig, if that is the done the value is set to the value entered in the menuconfig,
// if the value is manually overriden tough with a #define before including ThingsBoard then the hardcoded value takes precendence.
#  ifndef THINGSBOARD_ENABLE_DEBUG
#    ifndef CONFIG_THINGSBOARD_ENABLE_DEBUG
#      define THINGSBOARD_ENABLE_DEBUG 0
#    else
#      define THINGSBOARD_ENABLE_DEBUG CONFIG_THINGSBOARD_ENABLE_DEBUG
#    endif
#  endif

// Enables the usage of an additonal library as a fallback, to directly serialize a json message that is sent to the cloud,
// if the size of that message would be bigger than the internal buffer size of the client.
// Allows sending much bigger messages than would otherwise be possible, and without the need to increase stack or heap requirements, but at the cost of increased send times.
// See https://arduinojson.org/v6/how-to/use-arduinojson-with-pubsubclient/#serializing-a-json-document-into-an-mqtt-message for the main difference in the underlying code.
// Option can only be enabled when using Arduino, because this feature relies on Arduino as it improves the underlying data streams to directly write the data into the MQTT Client,
// but writing each byte one by one, would be too slow, therefore the ArduinoStreamUtils (https://github.com/bblanchon/ArduinoStreamUtils) library is used to buffer those calls into bigger packets.
// This allows sending data that is very big without requiring to allocate that much memory, because it is sent in smaller packets.
// To support this feature, however this interface needs to additionally implement the Print interface, because that is required by the wrapper class BufferingPrint.
#  ifndef THINGSBOARD_ENABLE_STREAM_UTILS
#    define THINGSBOARD_ENABLE_STREAM_UTILS 0
#  endif

// Enables the ThingsBoard class to save the allocated memory of the DynamicJsonDocument into psram instead of onto the sram.
// Enabled by default if THINGSBOARD_ENABLE_DYNAMIC has been set and the esp_heap_caps header exists, because it requries DynamicJsonDocument to work.
// If enabled the program might be slightly slower, but all the memory will be placed onto psram instead of sram, meaning the sram can be allocated for other things.
// See https://arduinojson.org/v6/how-to/use-external-ram-on-esp32/ and https://arduinojson.org/v6/api/basicjsondocument/ for the main difference in the underlying code.
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
