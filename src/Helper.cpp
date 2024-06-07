// Header include.
#include "Helper.h"

// Local includes.
#include "Constants.h"

// Library includes.
#include <string.h>

size_t Helper::getOccurences(char const * const str, char symbol) {
    size_t count = 0;
    if (str == nullptr) {
      return count;
    }
    for (size_t i = 0; i < strlen(str); ++i) {
      if (str[i] != symbol) {
        continue;
      }
      count++;
    }
    return count;
}

bool Helper::stringIsNullorEmpty(char const * const str) {
    return str == nullptr || str[0] == '\0';
}

size_t Helper::parseRequestId(char const * const base_topic, char const * const received_topic) {
    // Remove the not needed part of the received topic string, which is everything before the request id,
    // therefore we ignore the section before that which is the base topic + an additional "/" character, that seperates the topic from the request id.
    // Meaning the index we attempt to parse is the length of the base topic + 1 for the additonal "/" character
    size_t const index = strlen(base_topic) + 1U;
    return atoi(received_topic + index);
}
