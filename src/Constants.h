#ifndef Constants_h
#define Constants_h

// Local includes.
#include "Configuration.h"

// Library includes.
#if THINGSBOARD_ENABLE_PSRAM || THINGSBOARD_ENABLE_DYNAMIC
#include <ArduinoJson.h>
#endif // THINGSBOARD_ENABLE_PSRAM || THINGSBOARD_ENABLE_DYNAMIC

#define Default_Endpoints_Amount 7
#define Default_Response_Amount 8
#define Default_Subscriptions_Amount 1
#define Default_Attributes_Amount 1
#define Default_RPC_Amount 0
#define Default_Request_RPC_Amount 2
#define Default_Payload_Size 64
#define Default_Max_Stack_Size 1024
#if THINGSBOARD_ENABLE_STREAM_UTILS
#define Default_Buffering_Size 64
#endif // THINGSBOARD_ENABLE_STREAM_UTILS
#if THINGSBOARD_ENABLE_DYNAMIC
#define Default_Max_Response_Size 0
#endif // THINGSBOARD_ENABLE_DYNAMIC


// Log messages.
#if !THINGSBOARD_ENABLE_DYNAMIC
char constexpr TOO_MANY_JSON_FIELDS[] = "Attempt to enter to many JSON fields into StaticJsonDocument (%u), increase (%s) (%u) accordingly";
#endif // !THINGSBOARD_ENABLE_DYNAMIC
char constexpr UNABLE_TO_SERIALIZE[] = "Unable to serialize key-value json";
char constexpr CONNECT_FAILED[] = "Connecting to server failed";
char constexpr UNABLE_TO_SERIALIZE_JSON[] = "Unable to serialize json data";
char constexpr UNABLE_TO_ALLOCATE_JSON[] = "Allocating memory for the JsonDocument failed, passed JsonDocument is NULL";
char constexpr JSON_SIZE_TO_SMALL[] = "JsonDocument too small to store all values. Ensure every key value pair gets JSON_OBJECT_SIZE(1) capacity + size required by value / key that is inserted";


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
