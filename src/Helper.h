#ifndef Helper_h
#define Helper_h

// Local includes.
#include "Configuration.h"

// Library include.
#include <ArduinoJson.h>
#if THINGSBOARD_ENABLE_STL
#include <iterator>
#endif // THINGSBOARD_ENABLE_STL
#include <stdint.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>


/// @brief Static helper class that includes some functionalities used in multiple places throughout the library, especially the ThingsBoardHttp and ThingsBoard implementations
class Helper {
  public:
    /// @brief Returns the total amount of bytes needed to store the formatted string with null termination, that will be created if the given format string and the arguments are passed to snprintf
    /// @note This is achieved by using the snprintf without a buffer that the string should be copied into and with the buffer size of 0.
    /// This works because the result returned by snprintf is what would have been written if the passed buffer would have been large enough not counting null character,
    /// or if an error occured while creating the string a negative number is returned instead. This value is then simply incremented by 1 and returned.
    /// Additionally to ensure no negative number is ever returned even if an error occured an assert is called beforehand to ensure the size value is bigger than 0
    /// @tparam ...Args Holds the multiple arguments that will simply be forwarded to the snprintf method, allowing any arbitrary amount of combinations without having to rely on va_list
    /// @param format Non owning pointer to the formatting message that the given arguments will be inserted into.
    /// Does not need to be kept alive, because the formatting message is only used for the scope of the method itself
    /// @param ...args Arguments that will be forwarded into the snprintf method see https://cplusplus.com/reference/cstdio/snprintf/ for more information
    /// @return Amount of bytes in characters, needed for the formatted string with the given arguments inserted, to be displayed completly including null termination
    template<typename... Args>
    static size_t Calculate_Print_Size(char const * format, Args const &... args) {
        const int result = snprintf(nullptr, 0U, format, args...) + 1U;
        assert(result > 0);
        return static_cast<size_t>(result);
    }

    /// @brief Returns the amount of occurences of the given smybol in the given byte payload
    /// @param bytes Non owning pointer to the byte payload that we want to check the symbol for.
    /// Does not need to be kept alive, because the byte payload is only used for the scope of the method itself
    /// @param symbol Symbol we want to search for
    /// @param length Length of the byte payload, meaning if we reach the given length and have not found any occurence of the symbol we return 0.
    /// Ensure to never pass a length that is longer than the actualy payload, because this will cause this method to read outside of the bounds of the buffer
    /// @return Amount of occurences of the given symbol
    static size_t Calculate_Symbol_Occurences(uint8_t const * bytes, char symbol, uint32_t length);

    /// @brief Returns wheter the given string is either a nullptr or is an empty string,
    /// meaning it only contains a null terminator and no other characters
    /// @param str Non owning poitner to the string that we want to check for emptiness
    /// @return Wheter the given string is a nullptr or empty
    static bool String_IsNull_Or_Empty(char const * str);

    /// @brief Splits the topic at the given position and extracts the request id parameter from the remaining string
    /// @note Should contain the request id that the original request was sent with. Is used to know which received response is connected to which inital request,
    /// so that the correct request can be informed that a response has been received.
    /// To achieve this the function removes the not needed part of the received topic string, which is everything before the request id
    /// and then simply call the atoi function on the remaning topic string.
    /// @param received_topic Non owning pointer to the received topic that contains the base topic as well as the request id parameter (v1/devices/me/rpc/response/$request_id).
    /// Does not need to be kept alive, because the received topic is only used for the scope of the method itself
    /// @param end_position Number indicating the amount of characters that have to be incremented to reach the position where the $request_id lies in the received topic.
    /// Most of the time it can simply be the value returned by calling strlen() on the base version of the topic. So for example on (v1/devices/me/rpc/response/) instead of the received topic (v1/devices/me/rpc/response/42)
    /// @return Converted integral request id if possible or 0 if parsing as an integer failed
    static size_t Split_Topic_Into_Request_ID(char const * received_topic, size_t const & end_position);

    /// @brief Calculates the total size of the string the serializeJson method would produce including the null end terminator.
    /// @note Be aware that null terminator will later not be serialized in the serializeJson method,
    /// meaning the returned written amount of bytes is the return value of this method - 1.
    /// See https://arduinojson.org/v6/api/json/measurejson/ for more information on the underlying method used
    /// @tparam TSource Source class that should be used to serialize the json that is sent to the server
    /// @param source Source containing our json key-value pairs that we want to measure the resulting size of the serializeJson call for
    /// @return Total size required for the string that would be produced by serializeJson + 1 byte for the string null terminator
    template <typename TSource>
    static size_t Measure_Json(TSource const & source) {
        return measureJson(source) + 1U;
    }

    /// @brief Calculates the distance between two iterators
    /// @tparam InputIterator Class that allows for forward incrementable access to data
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param first Iterator pointing to the first element in the data container
    /// @param last Iterator pointing to the end of the data container (last element + 1)
    /// @return Distance between the two iterators
    template<typename InputIterator>
    static size_t distance(InputIterator const & first, InputIterator const & last) {
#if THINGSBOARD_ENABLE_STL
        return std::distance(first, last);
#else
        // Subtracting last by the first is only a valid way to calculate the distance if we can guarantee that the given iterators are random access,
        // to keep compatibility with code that supports the STL we allow InputIterators, therefore we have to implement the size calculation the more inneficient O(n) way instead.
        // This allows the edge case where an end-user uses this method themselves in the code with their own implemented list data type.
        size_t size = 0U;
        for (auto it = first; it != last; ++it, ++size) {}
        return size;
#endif // THINGSBOARD_ENABLE_STL
    }
};

#endif // Helper
