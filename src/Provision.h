#ifndef Provision_h
#define Provision_h

// Local includes.
#include "Provision_Callback.h"
#include "API_Implementation.h"


// Provision topics.
char constexpr PROV_RESPONSE_TOPIC[] = "/provision/response";
char constexpr PROV_REQUEST_TOPIC[] = "/provision/request";
// Provision data keys.
char constexpr DEVICE_NAME_KEY[] = "deviceName";
char constexpr PROV_DEVICE_KEY[] = "provisionDeviceKey";
char constexpr PROV_DEVICE_SECRET_KEY[] = "provisionDeviceSecret";
char constexpr PROV_CRED_TYPE_KEY[] = "credentialsType";
char constexpr PROV_TOKEN[] = "token";
char constexpr PROV_CRED_USERNAME[] = "username";
char constexpr PROV_CRED_PASSWORD[] = "password";
char constexpr PROV_CRED_CLIENT_ID[] = "clientId";
char constexpr PROV_CRED_HASH[] = "hash";


/// @brief Handles the internal implementation of the ThingsBoard provision API.
/// See https://thingsboard.io/docs/user-guide/device-provisioning/ for more information
class Provision : public API_Implementation {
  public:
    /// @brief Constructor
    Provision() = default;

    /// @brief Sends provisioning request for a new device, meaning we want to create a device that we can then connect over,
    /// where the given provision device key / secret decide which device profile is used to create the given device with.
    /// Optionally a device name can be passed or be left empty (cloud will use a random string as the name instead).
    /// The cloud then sends back json data containing our credentials, that will call the given callback, if creating the device was successful.
    /// The data contained in that callbackcan then be used to disconnect and reconnect to the ThingsBoard server as our newly created device.
    /// See https://thingsboard.io/docs/user-guide/device-provisioning/ for more information
    /// @param callback Callback method that will be called upon data arrival with the given data that was received serialized into a JsonDocument
    /// @return Whether sending the provisioning request was successful or not
    bool Provision_Request(Provision_Callback const & callback) {
        char const * provisionDeviceKey = callback.Get_Device_Key();
        char const * provisionDeviceSecret = callback.Get_Device_Secret();

        if (Helper::stringIsNullorEmpty(provisionDeviceKey) || Helper::stringIsNullorEmpty(provisionDeviceSecret)) {
            return false;
        }
        else if (!Provision_Subscribe(callback)) {
            return false;
        }

        StaticJsonDocument<JSON_OBJECT_SIZE(9)> requestBuffer;
        char const * deviceName = callback.Get_Device_Name();
        char const * accessToken = callback.Get_Device_Access_Token();
        char const * credUsername = callback.Get_Credentials_Username();
        char const * credPassword = callback.Get_Credentials_Password();
        char const * credClientID = callback.Get_Credentials_Client_ID();
        char const * hash = callback.Get_Certificate_Hash();
        char const * credentialsType = callback.Get_Credentials_Type();

        // Deciding which underlying provisioning method is restricted, by the Provision_Callback class.
        // Meaning only the key-value pairs that are needed for the given provisioning method are set,
        // resulting in the rest not being sent and therefore the provisioning request having the correct formatting
        if (!Helper::stringIsNullorEmpty(deviceName)) {
            requestBuffer[DEVICE_NAME_KEY] = deviceName;
        }
        if (!Helper::stringIsNullorEmpty(accessToken)) {
            requestBuffer[PROV_TOKEN] = accessToken;
        }
        if (!Helper::stringIsNullorEmpty(credUsername)) {
            requestBuffer[PROV_CRED_USERNAME] = credUsername;
        }
        if (!Helper::stringIsNullorEmpty(credPassword)) {
            requestBuffer[PROV_CRED_PASSWORD] = credPassword;
        }
        if (!Helper::stringIsNullorEmpty(credClientID)) {
            requestBuffer[PROV_CRED_CLIENT_ID] = credClientID;
        }
        if (!Helper::stringIsNullorEmpty(hash)) {
            requestBuffer[PROV_CRED_HASH] = hash;
        }
        if (!Helper::stringIsNullorEmpty(credentialsType)) {
            requestBuffer[PROV_CRED_TYPE_KEY] = credentialsType;
        }
        requestBuffer[PROV_DEVICE_KEY] = provisionDeviceKey;
        requestBuffer[PROV_DEVICE_SECRET_KEY] = provisionDeviceSecret;

        size_t const objectSize = Helper::Measure_Json(requestBuffer);
        return Send_Json(PROV_REQUEST_TOPIC, requestBuffer, objectSize);
    }

    /// @brief Subscribes one provision callback,
    /// that will be called if a provision response from the server is received
    /// @param callback Callback method that will be called
    /// @return Whether requesting the given callback was successful or not
    bool Provision_Subscribe(Provision_Callback const & callback) {
        if (!m_client.subscribe(PROV_RESPONSE_TOPIC)) {
            Logger::printfln(SUBSCRIBE_TOPIC_FAILED, PROV_RESPONSE_TOPIC);
            return false;
        }
        m_provision_callback = callback;
        return true;
    }

    /// @brief Unsubcribes the provision callback
    /// @return Whether unsubcribing the previously subscribed callback
    /// and from the provision response topic, was successful or not
    bool Provision_Unsubscribe() {
        m_provision_callback = Provision_Callback();
        return m_client.unsubscribe(PROV_RESPONSE_TOPIC);
    }

    char const * Get_Response_Topic_String() const override {
        return PROV_RESPONSE_TOPIC;
    }

    bool Unsubscribe_Topic() override {
        return Provision_Unsubscribe();
    }

    void Process_Json_Response(char * const topic, JsonObjectConst const & data) const override {
        m_provision_callback.Call_Callback(data);
        // Unsubscribe from the provision response topic,
        // Will be resubscribed if another request is sent anyway
        (void)Provision_Unsubscribe();
    }

    Provision_Callback m_provision_callback; // Provision response callback
};

#endif // Provision_h
