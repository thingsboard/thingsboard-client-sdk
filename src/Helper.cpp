// Header include.
#include "Helper.h"

// Local includes.
#include "Constants.h"

// Library includes.
#include <string.h>

size_t Helper::getOccurences(uint8_t const * bytes, char symbol, unsigned int length) {
    size_t count = 0;
    if (bytes == nullptr) {
        return count;
    }
    for (size_t i = 0; i < length; ++i) {
        if (bytes[i] == symbol) {
            count++;
        }
    }
    return count;
}

bool Helper::stringIsNullorEmpty(char const * str) {
    return str == nullptr || str[0] == '\0';
}

size_t Helper::parseRequestId(char const * base_topic, char const * received_topic) {
    // Remove the not needed part of the received topic string, which is everything before the request id,
    // therefore we ignore the section before that which is the base topic, that seperates the topic from the request id.
    // Meaning the index we attempt to parse at, is simply the length of the base topic
    return atoi(received_topic + strlen(base_topic));
}
