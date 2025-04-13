#ifndef Constants_h
#define Constants_h

// Local includes.
#include "Configuration.h"

// Library includes.
#include <stdint.h>
#if THINGSBOARD_ENABLE_PSRAM || THINGSBOARD_ENABLE_DYNAMIC
#include <ArduinoJson.h>
#endif // THINGSBOARD_ENABLE_PSRAM || THINGSBOARD_ENABLE_DYNAMIC

uint8_t constexpr DEFAULT_ENDPOINT_AMOUNT = 7U;
uint8_t constexpr DEFAULT_RESPONSE_AMOUNT = 8U;
uint8_t constexpr DEFAULT_SUBSCRIPTION_AMOUNT = 1U;
uint8_t constexpr DEFAULT_ATTRIBUTES_AMOUNT = 1U;
uint8_t constexpr DEFAULT_RPC_AMOUNT = 0U;
uint8_t constexpr DEFAULT_Request_RPC_AMOUNT = 2U;
uint8_t constexpr DEFAULT_PAYLOAD_SIZE = 64U;
uint16_t constexpr DEFAULT_MAX_STACK_SIZE = 1024U;
#if THINGSBOARD_ENABLE_STREAM_UTILS
uint8_t constexpr DEFAULT_BUFFERING_SIZE = 64U;
#endif // THINGSBOARD_ENABLE_STREAM_UTILS
#if THINGSBOARD_ENABLE_DYNAMIC
uint8_t constexpr DEFAULT_MAX_RESPONSE_SIZE = 0U;
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
