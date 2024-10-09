#ifndef Attribute_Request_Callback_h
#define Attribute_Request_Callback_h

// Local includes.
#include "Callback_Watchdog.h"
#if !THINGSBOARD_ENABLE_DYNAMIC
#include "Constants.h"
#endif // !THINGSBOARD_ENABLE_DYNAMIC


/// @brief Client-side or shared attributes request callback wrapper,
/// contains the needed configuration settings to create the request that should be sent to the server.
/// Which attribute scope will be requested from either client-side or shared, is decided depending on which method the class instance is passed to as an argument.
/// If that method is the Client_Attributes_Request() then the passed attributes are requested and if they exist are received from the client scope,
/// but if that method is the Shared_Attributes_Request() then the passed attributes are requested and if they exist are received from the shared scope instead.
/// To achieve that some internal member variables get set automatically by those methods, the first one being a string to differentiate which attribute scope was requested
/// and the second being the id of the mqtt request, where the response by the server will use the same id, which makes it easy to know which method intially requested the data and should now receive it.
/// Documentation about the specific use of Requesting client-side or shared scope atrributes in ThingsBoard can be found here https://thingsboard.io/docs/reference/mqtt-api/#request-attribute-values-from-the-server
#if !THINGSBOARD_ENABLE_DYNAMIC
/// @tparam MaxAttributes Maximum amount of attributes that will ever be requested with this instance of the class, allows to use an array on the stack in the background.
/// Be aware though the size set in this template and the size passed to the ThingsBoard MaxAttributes template need to be the same or the value in this class lower, if not some of the requested keys may be lost, default = Default_Attributes_Amount (5)
template <size_t MaxAttributes = Default_Attributes_Amount>
#endif // !THINGSBOARD_ENABLE_DYNAMIC
class Attribute_Request_Callback : public Callback<void, JsonObjectConst const &> {
  public:
    /// @brief Constructs empty callback, will result in never being called. Internals are simply default constructed as nullptr
    Attribute_Request_Callback() = default;

    /// @brief Constructs callback, will be called upon client-side or shared attribute request arrival
    /// where the given multiple requested client-side or shared attributes were sent by the cloud and received by the client.
    /// Directly forwards the given arguments to the overloaded vector constructor,
    /// meaning all combinatons of arguments that would initalize a vector can be used to call this constructor.
    /// See possible vector constructors https://en.cppreference.com/w/cpp/container/vector/vector, for the possible passed parameters.
    /// The possibilites mainly consist out of the fill constructor, where a value and a number n is given and then that many elements of the value will be copied into
    /// or be created with their default value, or out of the range constructor where we can pass an interator the start of another data container
    /// and to the end of the data container (last element + 1)
    /// and then every element between those iteratos will be copied, in the same order as in the original data container.
    /// The last option is a copy constructor where we pass a vector and the values of that vector will be copied into our buffer
    /// @tparam ...Args Holds the multiple arguments that will simply be forwarded to the vector constructor and therefore allow to use every overloaded vector constructor without having to implement them
    /// @param callback Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    /// @param timeout_microseconds Optional amount of microseconds until we expect to have received a response and if we didn't, we call the previously subscribed callback.
    /// If the value is 0 we will not start the timer and therefore never call the timeout callback method, default = 0
    /// @param timeout_callback Optional callback method that will be called upon request timeout (did not receive a response in the given timeout time). Can happen if the requested method does not exist on the cloud,
    /// or if the connection could not be established, default = nullptr
    /// @param ...args Arguments that will be forwarded into the overloaded vector constructor see https://en.cppreference.com/w/cpp/container/vector/vector for more information
    template<typename... Args>
    Attribute_Request_Callback(function callback, uint64_t const & timeout_microseconds = 0U, Callback_Watchdog::function timeout_callback = nullptr, Args const &... args)
      : Callback(callback)
      , m_attributes(args...)
      , m_request_id(0U)
      , m_attribute_key(nullptr)
      , m_timeout_microseconds(timeout_microseconds)
      , m_timeout_callback(timeout_callback)
    {
        // Nothing to do
    }

    /// @brief Gets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which Attribute_Request_Callback
    /// is connected to which received client-side or shared attributes
    /// @return Unique identifier connected to the requested client-side or shared attributes
    size_t const & Get_Request_ID() const {
        return m_request_id;
    }

    /// @brief Sets the unique request identifier that is connected to the original request,
    /// and will be later used to verifiy which Attribute_Request_Callback
    /// is connected to which received client-side or shared attributes.
    /// Not meant for external use, because the value is overwritten by the ThingsBoard class once the class instance has been passed as a parameter anyway,
    /// this is the case because only the ThingsBoard class knows the current request id that this callback will be attached too. 
    /// @param request_id Unqiue identifier of the request for client-side or shared attributes
    void Set_Request_ID(size_t const & request_id) {
        m_request_id = request_id;
    }

    /// @brief Gets the response key of the key-value pair,
    /// that we expect the client-side or shared attribute payload json data to be contained in
    /// @return Key that the data is saved into,
    /// "client" for client-side attributes and "shared" for shared scope attributes
    char const * Get_Attribute_Key() const {
        return m_attribute_key;
    }

    /// @brief Sets the response key of the key-value pair,
    /// that we expect the client-side or shared attribute payload json data to be contained in
    /// Not meant for external use, because the value is overwritten by the ThingsBoard class once the class instance has been passed as a parameter anyway,
    /// this is the case because the json key changes depending on if we request client-side or shared scope attributes
    /// and which type we requests depends on which method the class instance is passed as a parameter to
    /// @param attribute_key Key that the data is saved into,
    /// "client" for client-side attributes and "shared" for shared scope attributes
    void Set_Attribute_Key(char const * attribute_key) {
        m_attribute_key = attribute_key;
    }

    /// @brief Gets all the requested client-side or shared attributes that will result,
    /// in the subscribed method being called when the response with their current value
    /// is sent from the cloud and received by the client
    /// @return Requested client-side or shared attributes
#if THINGSBOARD_ENABLE_DYNAMIC
    const Vector<char const *>& Get_Attributes() const {
#else
    const Array<char const *, MaxAttributes>& Get_Attributes() const {
#endif // THINGSBOARD_ENABLE_DYNAMIC
        return m_attributes;
    }

    /// @brief Sets all the requested client-side or shared attributes that will result,
    /// in the subscribed method being called when the response with their current value
    /// is sent from the cloud and received by the client.
    /// Directly forwards the given arguments to the overloaded vector assign method,
    /// meaning all combinatons of arguments that can call the assign method on a vector can be used to call this method.
    /// See possible overloaded vector assign methods https://en.cppreference.com/w/cpp/container/vector/assign, for the possible passed parameters.
    /// The possibilites mainly consist out of the fill assign method, where a value and a number n is given and then that many elements of the value will be copied into
    /// or be created with their default value, or out of the range assign method where we can pass an interator the start of another data container
    /// and to the end of the data container (last element + 1)
    /// and then every element between those iteratos will be copied, in the same order as in the original data container.
    /// The last option is a copy assign method where we pass a vector and the values of that vector will be copied into our buffer
    /// @tparam ...Args Holds the multiple arguments that will simply be forwarded to the vector assign method and therefore allow to use every overloaded vector assign without having to implement them
    /// @param ...args Arguments that will be forwarded into the overloaded vector assign method see https://en.cppreference.com/w/cpp/container/vector/assign for more information
    template<typename... Args>
    void Set_Attributes(Args const &... args) {
        m_attributes.assign(args...);
    }

    /// @brief Gets the amount of microseconds until we expect to have received a response
    /// @return Timeout time until timeout callback is called
    uint64_t const & Get_Timeout() const {
        return m_timeout_microseconds;
    }

    /// @brief Sets the amount of microseconds until we expect to have received a response
    /// @param timeout_microseconds Timeout time until timeout callback is called
    void Set_Timeout(uint64_t const & timeout_microseconds) {
        m_timeout_microseconds = timeout_microseconds;
    }

#if !THINGSBOARD_USE_ESP_TIMER
    /// @brief Updates the internal timeout timer
    void Update_Timeout_Timer() {
        m_timeout_callback.update();
    }
#endif // !THINGSBOARD_USE_ESP_TIMER

    /// @brief Starts the internal timeout timer if we actually received a configured valid timeout time and a valid callback.
    /// Is called as soon as the request is actually sent
    void Start_Timeout_Timer() {
        if (m_timeout_microseconds == 0U) {
            return;
        }
        m_timeout_callback.once(m_timeout_microseconds);
    }

    /// @brief Stops the internal timeout timer, is called as soon as an answer is received from the cloud
    /// if it isn't we call the previously subscribed callback instead
    void Stop_Timeout_Timer() {
        m_timeout_callback.detach();
    }

    /// @brief Sets the callback method that will be called upon request timeout (did not receive a response in the given timeout time)
    /// @param timeout_callback Callback function that will be called
    void Set_Timeout_Callback(Callback_Watchdog::function timeout_callback) {
        m_timeout_callback.Set_Callback(timeout_callback);
    }

  private:
#if THINGSBOARD_ENABLE_DYNAMIC
    Vector<char const *>               m_attributes = {};           // Attribute we want to request
#else
    Array<char const *, MaxAttributes> m_attributes = {};           // Attribute we want to request
#endif // THINGSBOARD_ENABLE_DYNAMIC
    size_t                             m_request_id = {};           // Id the request was called with
    char const                         *m_attribute_key = {};       // Attribute key that we wil receive the response on ("client" or "shared")
    uint64_t                           m_timeout_microseconds = {}; // Timeout time until we expect response to request
    Callback_Watchdog                  m_timeout_callback = {};     // Handles callback that will be called if request times out
};

#endif // Attribute_Request_Callback_h
