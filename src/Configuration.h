#ifndef Configuration_h
#define Configuration_h

// Include sdkconfig file it it exists to allow overwriting of some defines with the configuration entered in the Espressif IDF menuconfig.
// Only available when compiling for Espressif IDF, but allows to more easily change some configurations with a GUI instead of code.
#  ifdef __has_include
#    if __has_include(<sdkconfig.h>)
#      include <sdkconfig.h>
#    endif
#  endif

// Include the internal version of the ESP IDF contained in the esp_idf_version file, if it exists, to allow checking for supported ESP IDF version.
#  ifdef __has_include
#    if __has_include(<esp_idf_version.h>)
#      include <esp_idf_version.h>
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
#  ifndef THINGSBOARD_ENABLE_STL
#    ifdef __has_include
#      if __has_include(<string>) && __has_include(<functional>) && __has_include(<vector>) && __has_include(<iterator>)
#        define THINGSBOARD_ENABLE_STL 1
#      else
#        define THINGSBOARD_ENABLE_STL 0
#      endif
#    else
#      ifdef ARDUINO
#        define THINGSBOARD_ENABLE_STL 0
#      else
#        define THINGSBOARD_ENABLE_STL 1
#      endif
#    endif
#  endif

// Use advanced STL features if they are supported by the compiler (std::ranges::view, template constraints and concepts).
// Currently only the case for ESP IDF when using a major version following 5 and when using Arduino following a major version 3.
// Allows to improve performance significantly, because to filter arrays or vectors we do not have to make copies of them anymore.
#  ifndef THINGSBOARD_ENABLE_CXX20
#    if THINGSBOARD_ENABLE_STL && __cplusplus >= 202002L
#      define THINGSBOARD_ENABLE_CXX20 1
#    else
#      define THINGSBOARD_ENABLE_CXX20 0
#    endif
#  endif

// Use the esp_timer header internally for handling timeouts and callbacks, as long as the header exists, because it is more efficient than the Arduino Ticker implementation,
// because we can stop the timer without having to delete it, removing the need to create a new timer to restart it. Because instead we can simply stop and start again.
// Only exists following major version 3 minor version 0 on ESP32 (https://github.com/espressif/esp-idf/releases/tag/v3.0-rc1)and major version 3 minor version 1 on ESP8266 (https://github.com/espressif/ESP8266_RTOS_SDK/releases/tag/v3.1-rc1)
#  ifndef THINGSBOARD_USE_ESP_TIMER
#    ifdef __has_include
#      if __has_include(<esp_timer.h>)
#        define THINGSBOARD_USE_ESP_TIMER 1
#      else
#        define THINGSBOARD_USE_ESP_TIMER 0
#      endif
#    else
#      define THINGSBOARD_USE_ESP_TIMER 0
#    endif
#  endif

// Use the mqtt_client header internally for handling the sending and receiving of MQTT data, as long as the header exists,
// to allow users that do have the needed component to use the Espressif_MQTT_Client instead of only the Arduino_MQTT_Client.
// Only exists following major version 3 minor version 2 on ESP32 (https://github.com/espressif/esp-idf/releases/tag/v3.2) and major version 3 minor version 4 on ESP8266 (https://github.com/espressif/ESP8266_RTOS_SDK/releases/tag/v3.4).
#  ifndef THINGSBOARD_USE_ESP_MQTT
#    ifdef __has_include
#      if __has_include(<mqtt_client.h>)
#        define THINGSBOARD_USE_ESP_MQTT 1
#      else
#        define THINGSBOARD_USE_ESP_MQTT 0
#      endif
#    else
#      define THINGSBOARD_USE_ESP_MQTT 0
#    endif
#  endif

// Use the mbed_tls header internally for handling the creation of hashes from binary data, as long as the header exists,
// because if it is already included we do not need to rely on and incude external lbiraries like Seeed_mbedtls.h, which implements the same features.
// Only exists following major version 0 minor version 9 on ESP32 (https://github.com/espressif/esp-idf/releases/v0.9) and major version 3 minor version 3 on ESP8266 (https://github.com/espressif/ESP8266_RTOS_SDK/releases/tag/v3.3-rc1).
#  ifndef THINGSBOARD_USE_MBED_TLS
#    ifdef __has_include
#      if __has_include(<mbedtls/md.h>)
#        define THINGSBOARD_USE_MBED_TLS 1
#      else
#        define THINGSBOARD_USE_MBED_TLS 0
#      endif
#    else
#      define THINGSBOARD_USE_MBED_TLS 0
#    endif
#  endif

// Use the esp_ota_ops header internally for handling the writing of ota update data, as long as the header exists,
// to allow users that do have the needed component to use the Espressif_Updater instead of only the Arduino_ESP32_Updater.
// Only exists following major version 1 minor version 0 on ESP32 (https://github.com/espressif/esp-idf/releases/v0.9) and major version 3 minor version 0 on ESP8266 (https://github.com/espressif/ESP8266_RTOS_SDK/releases/tag/v3.0-rc1).
// Additionally, for all the expected API calls to be implemented atleast, major version 2 minor version 1 on ESP32 and major version 3 minor version 0 on ESP8266 is required.
#  ifndef THINGSBOARD_USE_ESP_PARTITION
#    ifdef __has_include
#      if __has_include(<esp_ota_ops.h>) && (!defined(ESP32) || ((ESP_IDF_VERSION_MAJOR == 2 && ESP_IDF_VERSION_MINOR >= 1) || ESP_IDF_VERSION_MAJOR > 2)) && (!defined(ESP8266) || ESP_IDF_VERSION_MAJOR >= 3)
#        define THINGSBOARD_USE_ESP_PARTITION 1
#      else
#        define THINGSBOARD_USE_ESP_PARTITION 0
#      endif
#    else
#      define THINGSBOARD_USE_ESP_PARTITION 0
#    endif
#  endif

// Enables the ThingsBoard class to be fully dynamic instead of requiring template arguments to statically allocate memory.
// If enabled the program might be slightly slower and all the memory will be placed onto the heap instead of the stack.
// See https://arduinojson.org/v6/api/dynamicjsondocument/ for the main difference in the underlying code.
// Can also optionally be configured via the ESP-IDF menuconfig, if that is the done the value is set to the value entered in the menuconfig,
// if the value is manually overriden tough with a #define before including ThingsBoard then the hardcoded value takes precendence.
#  ifndef THINGSBOARD_ENABLE_DYNAMIC
#    define THINGSBOARD_ENABLE_DYNAMIC CONFIG_THINGSBOARD_ENABLE_DYNAMIC
#  endif

// Enables the ThingsBoard class to print all received and sent messages and their topic, from and to the server,
// additionally some more debug messages will be printed. Requires more flash memory, and more calls to the console requiring more performance.
// Recommended to disable when building for release, should only be enabled to debug where a issue might stem from.
// Can also optionally be configured via the ESP-IDF menuconfig, if that is the done the value is set to the value entered in the menuconfig,
// if the value is manually overriden tough with a #define before including ThingsBoard then the hardcoded value takes precendence.
#  ifndef THINGSBOARD_ENABLE_DEBUG
#    define THINGSBOARD_ENABLE_DEBUG CONFIG_THINGSBOARD_ENABLE_DEBUG
#  endif

// Use the StreamUtils header internally for enabling the usage of an additonal library as a fallback, as long as the header exists,
// to allwo to directly serialize a json message that is sent to the cloud, if the size of that message would be bigger than the internal buffer size of the client.
// Allows sending much bigger messages than would otherwise be possible, and without the need to increase stack or heap requirements, but at the cost of increased send times.
// See https://arduinojson.org/v6/how-to/use-arduinojson-with-pubsubclient/#serializing-a-json-document-into-an-mqtt-message for the main difference in the underlying code.
// Option can only be enabled when using Arduino, because this feature relies on Arduino as it improves the underlying data streams to directly write the data into the MQTT Client,
// but writing each byte one by one, would be too slow, therefore the ArduinoStreamUtils (https://github.com/bblanchon/ArduinoStreamUtils) library is used to buffer those calls into bigger packets.
// This allows sending data that is very big without requiring to allocate that much memory, because it is sent in smaller packets.
// To support this feature, however the IMQTT_Client interface implementation, needs to additionally override the Print interface, because that is required by the wrapper class BufferingPrint.
#  ifndef THINGSBOARD_ENABLE_STREAM_UTILS
#    ifdef __has_include
#      if __has_include(<StreamUtils.h>)
#        define THINGSBOARD_ENABLE_STREAM_UTILS 1
#      else
#        define THINGSBOARD_ENABLE_STREAM_UTILS 0
#      endif
#    else
#      define THINGSBOARD_ENABLE_STREAM_UTILS 0
#    endif
#  endif

// Enables the ThingsBoard class to save the allocated memory of the DynamicJsonDocument into psram instead of onto the sram.
// Enabled by default if THINGSBOARD_ENABLE_DYNAMIC has been set and the esp_heap_caps header exists, because it requries DynamicJsonDocument to work.
// If enabled the program might be slightly slower, but all the memory will be placed onto psram instead of sram, meaning the sram can be allocated for other things.
// See https://arduinojson.org/v6/how-to/use-external-ram-on-esp32/ and https://arduinojson.org/v6/api/basicjsondocument/ for the main difference in the underlying code.
#  ifndef THINGSBOARD_ENABLE_PSRAM
#    ifdef __has_include
#      if THINGSBOARD_ENABLE_DYNAMIC && __has_include(<esp_heap_caps.h>)
#        define THINGSBOARD_ENABLE_PSRAM 1
#      else
#        define THINGSBOARD_ENABLE_PSRAM 0
#      endif
#    else
#      define THINGSBOARD_ENABLE_PSRAM 0
#    endif
#  endif

#endif // Configuration_h
