// Header include.
#include "Helper.h"

// Local includes.
#include "Constants.h"

// Library includes.
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int32_t Helper::detectSize(const char *msg, ...) {
      va_list args;
      va_start(args, msg);
      // Result is what would have been written if the passed buffer would have been large enough not counting null character,
      // or if an error occured while creating the string a negative number is returned instead. TO ensure this will not crash the system
      // when creating an array with negative size we assert beforehand with a clear error message.
      const int32_t result = vsnprintf(nullptr, 0U, msg, args) + 1U;
      assert(result >= 0);
      va_end(args);
      return result;
}

size_t Helper::getOccurences(const char *str, char symbol) {
    size_t count = 0;
    if (str == nullptr) {
      return count;
    }
    for (size_t i = 0; i < strlen(str); i++) {
      if (str[i] != symbol) {
        continue;
      }
      count++;
    }
    return count;
}

bool Helper::stringIsNullorEmpty(const char *str) {
    return str == nullptr || str[0] == '\0';
}

size_t Helper::parseRequestId(const char* base_topic, const char* received_topic) {
    // Remove the not needed part of the received topic string, which is everything before the request id,
    // therefore we ignore the section before that which is the base topic + an additional "/" character, that seperates the topic from the request id.
    // Meaning the index we attempt to parse is the length of the base topic + 1 for the additonal "/" character
    const size_t index = strlen(base_topic) + 1U;
    return atoi(received_topic + index);
}
