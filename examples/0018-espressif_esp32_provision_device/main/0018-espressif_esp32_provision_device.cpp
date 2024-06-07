#include <esp_netif.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <string.h>

// Whether the given script is using encryption or not,
// generally recommended as it increases security (communication with the server is not in clear text anymore),
// it does come with an overhead tough as having an encrypted session requires a lot of memory,
// which might not be avaialable on lower end devices.
#define ENCRYPTED false

#include <Espressif_MQTT_Client.h>
#include <ThingsBoard.h>

#define TAG "Provision Device"

// Examples using arduino used PROGMEM to save constants into flash memory,
// this is not needed when using Espressif IDF because per default
// all read only variables will be saved into DROM (flash memory).
// See https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/memory-types.html#drom-data-stored-in-flash
// for more information about the aforementioned feature
constexpr char WIFI_SSID[] = "YOUR_WIFI_SSID";
constexpr char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
constexpr char TOKEN[] = "YOUR_DEVICE_ACCESS_TOKEN";
constexpr char PROVISION_DEVICE_KEY[] = "YOUR_PROVISION_DEVICE_KEY";
constexpr char PROVISION_DEVICE_SECRET[] = "YOUR_PROVISION_DEVICE_SECRET";
constexpr char DEVICE_NAME[] = "";

constexpr char CREDENTIALS_TYPE[] = "credentialsType";
constexpr char CREDENTIALS_VALUE[] = "credentialsValue";
constexpr char CLIENT_ID[] = "clientId";
constexpr char CLIENT_PASSWORD[] = "password";
constexpr char CLIENT_USERNAME[] = "userName";
constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char HUMIDITY_KEY[] = "humidity";
constexpr char ACCESS_TOKEN_CRED_TYPE[] = "ACCESS_TOKEN";
constexpr char MQTT_BASIC_CRED_TYPE[] = "MQTT_BASIC";
constexpr char X509_CERTIFICATE_CRED_TYPE[] = "X509_CERTIFICATE";

constexpr char THINGSBOARD_SERVER[] = "demo.thingsboard.io";

#if ENCRYPTED
constexpr uint16_t THINGSBOARD_PORT = 8883U;
#else
constexpr uint16_t THINGSBOARD_PORT = 1883U;
#endif

constexpr uint16_t MAX_MESSAGE_SIZE = 256U;

#if ENCRYPTED
// See https://comodosslstore.com/resources/what-is-a-root-ca-certificate-and-how-do-i-download-it/
// on how to get the root certificate of the server we want to communicate with,
// this is needed to establish a secure connection and changes depending on the website.
constexpr char ROOT_CERT[] = R"(-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)";
#endif

// Initalize the Mqtt client instance
Espressif_MQTT_Client mqttClient;
// Initialize ThingsBoard instance with the maximum needed buffer size
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

typedef struct {
    std::string client_id;
    std::string username;
    std::string password;
} credentials_t;

credentials_t credentials;
bool provisionRequestSent = false;
bool provisionResponseProcessed = false;

/**
 * @brief Event handler for the IP_EVENT_STA_GOT_IP event.
 *
 * This function is called when the WiFi station (STA) gets an IP address.
 * It logs a message indicating that an IP address has been obtained.
 *
 * @param arg        User-defined argument (not used in this handler).
 * @param event_base Base ID of the event (should be IP_EVENT for this handler).
 * @param event_id   ID of the event (should be IP_EVENT_STA_GOT_IP for this handler).
 * @param event_data Event-specific data (not used in this handler).
 *
 * @return void
 */
static void on_got_ip(void* arg, esp_event_base_t event_base,
                      int32_t event_id, void* event_data) {
    ESP_LOGI(TAG, "Got IP address");
}

/**
 * @brief Initialize WiFi and connect to the specified access point.
 *
 * This function initializes the WiFi stack, creates a default station (STA) interface,
 * configures it with the SSID and password defined in the `WIFI_SSID` and `WIFI_PASSWORD`
 * macros, and connects to the access point. It also registers an event handler for the
 * `IP_EVENT_STA_GOT_IP` event to handle IP acquisition.
 *
 * @note This function assumes that `WIFI_SSID` and `WIFI_PASSWORD` are defined as
 *       macros containing the SSID and password of the target WiFi network.
 *
 * @return void
 */
void InitWiFi() {
    // Initialize the TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());
    
    // Create default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Create default WiFi STA (Station) interface
    esp_netif_create_default_wifi_sta();

    // Initialize WiFi with default configuration
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Register event handler for IP_EVENT_STA_GOT_IP
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_got_ip, NULL));

    // Configure WiFi connection settings
    wifi_config_t wifi_config = {};
    strcpy((char*)wifi_config.sta.ssid, WIFI_SSID);
    strcpy((char*)wifi_config.sta.password, WIFI_PASSWORD);

    // Set WiFi mode to STA (Station)
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Set WiFi configuration
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // Start WiFi
    ESP_ERROR_CHECK(esp_wifi_start());

    // Connect to the AP
    ESP_ERROR_CHECK(esp_wifi_connect());
}

/**
 * @brief Process the provisioning response received from the server.
 *
 * This function processes the provisioning response contained in the `Provision_Data` object.
 * It serializes the response to JSON, logs the response, checks for success status, and 
 * extracts the credentials based on the `credentialsType`. It supports `ACCESS_TOKEN` and 
 * `MQTT_BASIC` credential types. If the device is provisioned successfully, it disconnects 
 * from the current cloud client and sets the `provisionResponseProcessed` flag to true.
 *
 * @param data A reference to the `Provision_Data` object containing the provisioning response.
 *
 * @return void
 */
void processProvisionResponse(const Provision_Data &data) {
    // Measure the size of the JSON data
    const size_t jsonSize = Helper::Measure_Json(data);
    
    // Buffer to hold the serialized JSON
    char buffer[jsonSize];
    
    // Serialize the JSON data
    serializeJson(data, buffer, jsonSize);
    
    // Log the received provision response
    ESP_LOGI(TAG, "Received device provision response: %s", buffer);

    // Check if the provisioning was successful
    if (strcmp(data["status"], "SUCCESS") != 0) {
        ESP_LOGE(TAG, "Provision response contains the error: %s", data["errorMsg"].as<const char*>());
        return;
    }

    // Process credentials based on the type
    if (strcmp(data["credentialsType"], "ACCESS_TOKEN") == 0) {
        credentials.client_id = "";
        credentials.username = data["credentialsValue"].as<std::string>();
        credentials.password = "";
    } else if (strcmp(data["credentialsType"], "MQTT_BASIC") == 0) {
        auto credentials_value = data["credentialsValue"].as<JsonObjectConst>();
        credentials.client_id = credentials_value["clientId"].as<std::string>();
        credentials.username = credentials_value["userName"].as<std::string>();
        credentials.password = credentials_value["password"].as<std::string>();
    } else {
        ESP_LOGE(TAG, "Unexpected provision credentialsType: %s", data["credentialsType"].as<const char*>());
        return;
    }

    // Disconnect from the cloud client connected to the provision account
    // because the device has been provisioned and can reconnect with new credentials
    if (tb.connected()) {
        tb.disconnect();
    }

    // Set the provision response processed flag to true
    provisionResponseProcessed = true;
}

/**
 * @brief Provision a device by connecting to the ThingsBoard server.
 *
 * This function provisions a new device by connecting to the ThingsBoard server using
 * a provision account. If the device name (`DEVICE_NAME`) is empty, it generates a 
 * device name using the device's MAC address. It sends a provisioning request and waits
 * for the provisioning response to be processed. Once the device is provisioned, it 
 * disconnects from the provision account and reconnects using the newly generated credentials.
 *
 * @param pvParameters A pointer to the parameters passed to the task (not used in this function).
 *
 * @return void
 */
void provision_device(void *pvParameters) {
    // Get the device name, using MAC address if DEVICE_NAME is empty
    std::string dev_name = DEVICE_NAME;
    if (dev_name.empty()) {
        uint8_t mac[6];
        esp_wifi_get_mac(WIFI_IF_STA, mac);
        char mac_str[18];
        sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        dev_name = mac_str;
    }

    // Connect to the ThingsBoard server as a client wanting to provision a new device
    if (!tb.connect(THINGSBOARD_SERVER, "provision", THINGSBOARD_PORT)) {
        ESP_LOGE(TAG, "Failed to connect to ThingsBoard server with provision account");
        return;
    }

    // Prepare and send the provision request
    Provision_Callback provisionCallback(Access_Token(), &processProvisionResponse, PROVISION_DEVICE_KEY, PROVISION_DEVICE_SECRET, dev_name.c_str());
    provisionRequestSent = tb.Provision_Request(provisionCallback);

    // Wait for the provisioning response to be processed
    while (!provisionResponseProcessed) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        tb.loop();
    }

    // Disconnect from the cloud client connected to the provision account
    // because the device has been provisioned and can reconnect with new credentials
    if (tb.connected()) {
        tb.disconnect();
    }

    // Reconnect to the ThingsBoard server using the new credentials
    tb.connect(THINGSBOARD_SERVER, credentials.username.c_str(), THINGSBOARD_PORT, credentials.client_id.c_str(), credentials.password.c_str());

    // Run the ThingsBoard loop
    tb.loop();
}


extern "C" void app_main(void) {
    InitWiFi();

#if ENCRYPTED
    mqttClient.set_server_certificate(ROOT_CERT);
#endif

    xTaskCreate(&provision_device, "provision_device_task", 1024 * 8, NULL, 5, NULL);
}
