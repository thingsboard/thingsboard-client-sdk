// Header include.
#include "Helper.h"

// Library includes.
#if THINGSBOARD_ENABLE_STL
#include <algorithm>
#endif // THINGSBOARD_ENABLE_STL
#include <string.h>

size_t Helper::Calculate_Symbol_Occurences(uint8_t const * bytes, char symbol, uint32_t length) {
    size_t count = 0;
    if (bytes == nullptr) {
        return count;
    }
#if THINGSBOARD_ENABLE_STL
    count = std::count(bytes, bytes + length, symbol);
#else
    for (size_t i = 0; i < length; ++i) {
        if (bytes[i] == symbol) {
            count++;
        }
    }
#endif // THINGSBOARD_ENABLE_STL
    return count;
}

bool Helper::String_IsNull_Or_Empty(char const * str) {
    return str == nullptr || str[0] == '\0';
}

size_t Helper::Split_Topic_Into_Request_ID(char const * received_topic, size_t const & end_position) {
    return atoi(received_topic + end_position);
}
