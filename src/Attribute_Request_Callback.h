#ifndef Attribute_Request_Callback_h
#define Attribute_Request_Callback_h

// Local includes.
#include "Timeoutable_Request.h"
#if !THINGSBOARD_ENABLE_DYNAMIC
#include "Constants.h"
#endif // !THINGSBOARD_ENABLE_DYNAMIC


/// @brief Client-side or shared attributes request callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// @note Which attribute scope will be requested from either client-side or shared, is decided depending on which method the class instance is passed to as an argument.
/// If that method is the Client_Attributes_Request() then the passed attributes are requested and if they exist are received from the client scope,
/// but if that method is the Shared_Attributes_Request() then the passed attributes are requested and if they exist are received from the shared scope instead.
/// To achieve that some internal member variables get set automatically by those methods, the first one being a string to differentiate which attribute scope was requested
/// and the second being the ID of the MQTT request, where the response by the server will use the same ID, which makes it easy to know which method intially requested the data and should now receive the response.
/// Documentation about the specific use of Requesting client-side or shared scope atrributes in ThingsBoard can be found here https://thingsboard.io/docs/reference/mqtt-api/#request-attribute-values-from-the-server
#if !THINGSBOARD_ENABLE_DYNAMIC
/// @tparam MaxAttributes Maximum amount of attributes that will ever be requested with this instance of the class, allows to use an array on the stack in the background.
/// Be aware though the size set in this template and the size passed to the Attribute_Request class MaxAttributes template need to be the same or higher than the value in this class,
/// if not some of the requested keys may be lost and the requests only partially fulfilled, default = DEFAULT_ATTRIBUTES_AMOUNT (1)
template <size_t MaxAttributes = DEFAULT_ATTRIBUTES_AMOUNT>
#endif // !THINGSBOARD_ENABLE_DYNAMIC
class Attribute_Request_Callback : public Callback<void, JsonObjectConst const &> {
#if THINGSBOARD_ENABLE_DYNAMIC
    using CString_Container = Container<char const *>;
#else
    using CString_Container = Container<char const *, MaxAttributes>;
#endif // THINGSBOARD_ENABLE_DYNAMIC

  public:
    /// @brief Constructs empty callback, will result in never being called. Internals are simply default constructed as nullptr
    Attribute_Request_Callback() = default;

    /// @brief Constructs callback, will be called upon client-side or shared attribute request arrival
    /// where the given multiple requested client-side or shared attributes were sent by the cloud and received by the client
    /// @note Directly forwards the given arguments to the overloaded Container constructor,
    /// meaning all combinatons of arguments that would initalize an std::vector can be used to call this constructor.
    /// See possible std::vector constructors here https://en.cppreference.com/w/cpp/container/vector/vector, for the possible passable parameters.
    /// The possibilites mainly consist out of the fill constructor, where a number n and a value is given and then the value is copied into that many elements,
    /// alternatively if no value is given the default constructed value is copied n times instead,
    /// or the range constructor where we can pass an interator to the start and to the end of the data container (last element + 1)
    /// to copy every element in between thoose iterators, in the same order as in the original data container.
    /// The last option is a copy constructor where we pass another container and all the values of that container will be copied into our buffer
    /// @tparam ...Args Holds the multiple arguments that will simply be forwarded to the container constructor and therefore allow to use every overloaded constructor without having to explicitly implement them
    /// @param callback Callback method that will be called upon data arrival with the given data that was received.
    /// If nullptr is passed the callback will never be called and instead return with a defaulted instance of the requested return variable
    /// @param timeout_microseconds Optional amount of microseconds until a response should have been received from the server, counted from the moment the request is sent.
    /// If a response is not received in the timeout time, the timeout callback method will be called to inform the user that the request has failed.
    /// If the value is 0 the timer will not be started and therefore never call the timeout callback method, default = 0
    /// @param timeout_callback Optional callback method that will be called upon request timeout (did not receive a response in the given timeout time). Can happen if the requested method does not exist on the cloud,
    /// or if the connection could not be established. A nullptr means even if a timeout occured the callback is simply ignored and the user not informed, default = nullptr
    /// @param ...args Attributes to requests, that will be forwarded into the overloaded Container constructor see https://en.cppreference.com/w/cpp/container/vector/vector for more information
    template<typename... Args>
    Attribute_Request_Callback(function callback, uint64_t const & timeout_microseconds = 0U, Callback_Watchdog::function timeout_callback = nullptr, Args const &... args)
      : Callback(callback)
      , m_attributes(args...)
      , m_request_id(0U)
      , m_attribute_key(nullptr)
      , m_request_timeout(timeout_microseconds, timeout_callback)
    {
        // Nothing to do
    }

    ~Attribute_Request_Callback() override = default;

    /// @brief Gets the unique request identifier that is connected to the original request
    /// @note Will be later used to verifiy which @ref Attribute_Request_Callback is connected to which received client-side or shared attributes
    /// @return Unique identifier connected to the requested client-side or shared attribute response
    size_t const & Get_Request_ID() const {
        return m_request_id;
    }

    /// @brief Sets the unique request identifier that is connected to the original request
    /// @note Will be later used to verifiy which @ref Attribute_Request_Callback is connected to which received client-side or shared attributes.
    /// Not meant for external use, because the value is overwritten by internal method calls anyway once the class instance has been passed as a parameter anyway.
    /// This is the case because only the internal methods knows the current request id that this callback will be attached too
    /// @param request_id Unqiue identifier of the request for client-side or shared attributes
    void Set_Request_ID(size_t const & request_id) {
        m_request_id = request_id;
    }

    /// @brief Gets the response key of the json array key-value pair,
    /// that we expect the client-side or shared attribute payload json data to be contained in
    /// @return JSON array key that the data is saved into, "client" for client-side attributes and "shared" for shared scope attributes
    char const * Get_Attribute_Key() const {
        return m_attribute_key;
    }

    /// @brief Sets the response key of the json array key-value pair,
    /// that we expect the client-side or shared attribute payload json data to be contained in
    /// @note Not meant for external use, because the value is overwritten by internal method calls anyway once the class instance has been passed as a parameter anyway.
    /// This is the case because the json key changes depending on if we request client-side or shared scope attributes
    /// and which type we requests depends on which method the class instance is passed too as an argument
    /// @param attribute_key Json array key that the data is saved into,
    /// "client" for client-side attributes and "shared" for shared scope attributes
    void Set_Attribute_Key(char const * attribute_key) {
        m_attribute_key = attribute_key;
    }

    /// @brief Gets all the requested client-side or shared attributes, that will be transformed into the necessary payload format
    /// @note Simply transforms the attributes into one comma-seperated c-string containing all the requested attributes.
    /// That c-string is then used to requests thoose attributes from the cloud and receive their values as a response.
    /// @return Requested client-side or shared attributes
    CString_Container const & Get_Attributes() const {
        return m_attributes;
    }

    /// @brief Sets all the requested client-side or shared attributes, that will be transformed into the necessary payload format
    /// @note Simply transforms the attributes into one comma-seperated c-string containing all the requested attributes.
    /// That c-string is then used to requests thoose attributes from the cloud and receive their values as a response.
    /// Directly forwards the given arguments to the overloaded container assign method which mirrors the std::vector interface,
    /// meaning all combinations of arguments that can call the assign method on a std::vector can be used to call this method.
    /// See possible overloaded std::vector assign methods https://en.cppreference.com/w/cpp/container/vector/assign, for the possible passable parameters.
    /// The possibilites mainly consist out of the fill assign method, where a value and a number n is given and then that many elements of the value will be copied into
    /// or be created with their default value, or out of the range assign method where we can pass an interator the start of another data container
    /// and to the end of the data container (last element + 1)
    /// and then every element between those iteratos will be copied, in the same order as in the original data container.
    /// The last option is a copy assign method where we pass a container and all values of that container will be copied into our buffer
    /// @tparam ...Args Holds the multiple arguments that will simply be forwarded to the Container assign method and therefore allow to use every overloaded Container assign without having to implement them
    /// @param ...args Attributes to requests, that will be forwarded into the overloaded Container assign method see https://en.cppreference.com/w/cpp/container/vector/assign for more information
    template<typename... Args>
    void Set_Attributes(Args const &... args) {
        m_attributes.assign(args...);
    }

    /// @brief Gets the request timeout callback
    /// @note Will be called when no response to the request was received in the expected amount of time, causing the internal watchdog to time out.
    /// To achieve this behaviour the internal timer can be started and stopped, and simply calls the subscribed callback if the timer is not stopped before it times out
    /// @return Request timeout callback
    Timeoutable_Request & Get_Request_Timeout() {
        return m_request_timeout;
    }

  private:
    CString_Container   m_attributes = {};      // Attribute we want to request
    size_t              m_request_id = {};      // Id the request was called with
    char const          *m_attribute_key = {};  // Attribute key that we wil receive the response on ("client" or "shared")
    Timeoutable_Request m_request_timeout = {}; // Handles callback that will be called if request times out
};

#endif // Attribute_Request_Callback_h
