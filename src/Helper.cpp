// Header include.
#include "Helper.h"

// Local includes.
#include "Constants.h"

// Library includes.
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

uint8_t Helper::detectSize(const char *msg, ...) {
      va_list args;
      va_start(args, msg);
      // Result is what would have been written if the passed buffer would have been large enough not counting null character,
      // or if an error occured while creating the string a negative number is returned instead. TO ensure this will not crash the system
      // when creating an array with negative size we assert beforehand with a clear error message.
      const int32_t result = vsnprintf_P(nullptr, 0U, msg, args) + 1U;
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
