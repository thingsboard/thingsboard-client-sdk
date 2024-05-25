#ifndef Constants_h
#define Constants_h

// Local includes.
#include "Configuration.h"

// Library includes.
#if THINGSBOARD_ENABLE_PROGMEM
#include <pgmspace.h>
#endif // THINGSBOARD_ENABLE_PROGMEM
#if THINGSBOARD_ENABLE_PSRAM || THINGSBOARD_ENABLE_DYNAMIC
#include <ArduinoJson.h>
#endif // THINGSBOARD_ENABLE_PSRAM || THINGSBOARD_ENABLE_DYNAMIC

#define Default_Fields_Amount 8
#define Default_Subscriptions_Amount 2
#define Default_Attributes_Amount 5
#define Default_RPC_Amount 0
#define Default_Payload 64
#define Default_Max_Stack_Size 1024
#if THINGSBOARD_ENABLE_STREAM_UTILS
#define Default_Buffering_Size 64
#endif // THINGSBOARD_ENABLE_STREAM_UTILS


// Log messages.
#if THINGSBOARD_ENABLE_PROGMEM
char constexpr UNABLE_TO_SERIALIZE[] PROGMEM = "Unable to serialize key-value json";
#if !THINGSBOARD_ENABLE_DYNAMIC
char constexpr TOO_MANY_JSON_FIELDS[] PROGMEM = "Too many JSON fields passed (%u), increase MaxFieldsAmount (%u) accordingly";
#endif // !THINGSBOARD_ENABLE_DYNAMIC
char constexpr CONNECT_FAILED[] PROGMEM = "Connecting to server failed";
char constexpr UNABLE_TO_SERIALIZE_JSON[] PROGMEM = "Unable to serialize json data";
char constexpr UNABLE_TO_ALLOCATE_JSON[] PROGMEM = "Allocating memory for the JsonDocument failed, passed JsonObject or JsonVariant is NULL";
#else
char constexpr UNABLE_TO_SERIALIZE[] = "Unable to serialize key-value json";
#if !THINGSBOARD_ENABLE_DYNAMIC
char constexpr TOO_MANY_JSON_FIELDS[] = "Too many JSON fields passed (%u), increase MaxFieldsAmount (%u) accordingly";
#endif // !THINGSBOARD_ENABLE_DYNAMIC
char constexpr CONNECT_FAILED[] = "Connecting to server failed";
char constexpr UNABLE_TO_SERIALIZE_JSON[] = "Unable to serialize json data";
char constexpr UNABLE_TO_ALLOCATE_JSON[] = "Allocating memory for the JsonDocument failed, passed JsonObject or JsonVariant is NULL";
#endif // THINGSBOARD_ENABLE_PROGMEM


#if THINGSBOARD_ENABLE_PSRAM
#include <esp_heap_caps.h>

struct SpiRamAllocator {
  void* allocate(size_t size) {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
  }

  void deallocate(void* pointer) {
    heap_caps_free(pointer);
  }

  void* reallocate(void* ptr, size_t new_size) {
    return heap_caps_realloc(ptr, new_size, MALLOC_CAP_SPIRAM);
  }
};

using TBJsonDocument = BasicJsonDocument<SpiRamAllocator>;
#elif THINGSBOARD_ENABLE_DYNAMIC
using TBJsonDocument = DynamicJsonDocument;
#endif

#endif // Constants_h
