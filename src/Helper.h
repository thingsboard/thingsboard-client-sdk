#ifndef Helper_h
#define Helper_h

// Local includes.
#include "Configuration.h"

// Library include.
#include <stdint.h>
#include <ArduinoJson.h>
#if THINGSBOARD_ENABLE_STL
#include <iterator>
#endif // THINGSBOARD_ENABLE_STL


/// @brief Static helper class that includes some uniliterally used functionalities in multiple places, especially the ThingsBoardHttp and ThingsBoard implementations
class Helper {
  public:
    /// @brief Returns the length in characters needed for a given value with the given argument string to be displayed completly
    /// @param msg Formating message that the given argument will be inserted into
    /// @param ... Additional arguments that should be inserted into the message at the given points,
    /// see https://cplusplus.com/reference/cstdio/printf/ for more information on the possible arguments
    /// @return Length in characters, needed for the given message with the given values inserted to be displayed completly
    static uint8_t detectSize(const char *msg, ...);

    /// @brief Returns the amount of occurences of the given smybol in the given string
    /// @param str String that we want to check the symbol in
    /// @param symbol Symbols we want to search for
    /// @return Amount of occurences of the given symbol
    static size_t getOccurences(const char *str, char symbol);

    /// @brief Returns wheter the given string is either a nullptr or is an empty string,
    /// meaning it only contains a null terminator and no other characters
    /// @param str String that we want to check for emptiness
    /// @return Wheter the given string is a nullptr or empty
    static bool stringIsNullorEmpty(const char *str);

    /// @brief Returns the portion of the received topic after the base topic as an integer.
    /// Should contain the request id that the original request was sent with.
    /// Is used to know which received response is connected to which inital request.
    /// @param base_topic Base portion of the topic that does not contain any parameters,
    /// should not contain trailing '/' character because the implementation already adds +1 to exclude that character as well (v1/devices/me/attributes/response)
    /// @param received_topic Received topic that contains the base topic as well as the request id parameter (v1/devices/me/rpc/response/$request_id)
    /// @return Converted integral request id if possible or 0 if parsing as an integer failed.
    static size_t parseRequestId(const char* base_topic, const char* received_topic);

    /// @brief Calculates the total size of the string the serializeJson method would produce including the null end terminator.
    /// See https://arduinojson.org/v6/api/json/measurejson/ for more information on the underlying method used
    /// @tparam TSource Source class that should be used to serialize the json that is sent to the server
    /// @param source Data source containing our json key value pairs we want to measure
    /// @return Total size of the string produced by serializeJson + 1 byte for the string null terminator
    template <typename TSource>
    inline static size_t Measure_Json(const TSource& source) {
      return JSON_STRING_SIZE(measureJson(source));
    }

    /// @brief Removes the element with the given index using the given iterator, which allows to use data containers that do not have a random-access iterator.
    /// The user is also cautioned that this function only erases the element, and that if the element is itself a pointer,
    /// the pointed-to memory is not touched in any way. Managing the pointer is the user's responsibility.
    /// See https://stackoverflow.com/questions/875103/how-do-i-erase-an-element-from-stdvector-by-index for more information
    /// @tparam DataContainer Class which allows to pass any arbitrary data container that contains the cbegin() and erase() method
    /// @param container Data container holding the elements we want to remove an element from
    /// @param index Index we want to delete the element at
    template<class DataContainer>
    inline static void remove(DataContainer& container, const size_t& index) {
#if THINGSBOARD_ENABLE_STL
        auto iterator = container.cbegin();
        std::advance(iterator, index);
        container.erase(iterator);
#else
        container.erase(index);
#endif // THINGSBOARD_ENABLE_STL
    }

    /// @brief Calculates the distance between two iterators
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// When THINGSBOARD_ENABLE_STL is not enabled then this method will only calculate the correct distance for random access iterators.
    /// This is done because calculating the size for a random access iterator is O(1) where as a sequential iterator requires O(n) computational time
    /// @param first_itr Iterator pointing to the first element in the data container
    /// @param last_itr Iterator pointing to the end of the data container (last element + 1)
    /// @return Distance between the two iterators
    template<class InputIterator>
    inline static size_t distance(const InputIterator& first_itr, const InputIterator& last_itr) {
#if THINGSBOARD_ENABLE_STL
        return std::distance(first_itr, last_itr);
#else
        return last_itr - first_itr;
#endif // THINGSBOARD_ENABLE_STL
    }
};

#endif // Helper
