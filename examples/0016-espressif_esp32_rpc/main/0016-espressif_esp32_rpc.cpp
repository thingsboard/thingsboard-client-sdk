#include <esp_netif.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <string.h>

// Whether the given script is using encryption or not,
// generally recommended as it increases security (communication with the server is not in clear text anymore),
// it does come with an overhead tough as having an encrypted session requires a lot of memory,
// which might not be avaialable on lower end devices.
#define ENCRYPTED false

#include <Espressif_MQTT_Client.h>
#include <Server_Side_RPC.h>
#include <ThingsBoard.h>


// See https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/memory-types.html#drom-data-stored-in-flash
// for more information about the aforementioned feature
constexpr char WIFI_SSID[] = "YOUR_WIFI_SSID";
constexpr char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
constexpr char TOKEN[] = "YOUR_DEVICE_ACCESS_TOKEN";

// Thingsboard we want to establish a connection to
constexpr char THINGSBOARD_SERVER[] = "demo.thingsboard.io";

// MQTT port used to communicate with the server, 1883 is the default unencrypted MQTT port,
// whereas 8883 would be the default encrypted SSL MQTT port
#if ENCRYPTED
constexpr uint16_t THINGSBOARD_PORT = 8883U;
#else
constexpr uint16_t THINGSBOARD_PORT = 1883U;
#endif

// Maximum size packets will ever be sent or received by the underlying MQTT client,
// if the size is to small messages might not be sent or received messages will be discarded
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

constexpr const char RPC_JSON_METHOD[] = "example_json";
constexpr char RPC_TEMPERATURE_METHOD[] = "example_set_temperature";
constexpr char RPC_SWITCH_METHOD[] = "example_set_switch";
constexpr char RPC_TEMPERATURE_KEY[] = "temp";
constexpr char RPC_SWITCH_KEY[] = "switch";
constexpr char RPC_RESPONSE_KEY[] = "example_response";
constexpr uint8_t MAX_RPC_SUBSCRIPTIONS = 3U;
constexpr uint8_t MAX_RPC_RESPONSE = 5U;

// Initalize the Mqtt client instance
Espressif_MQTT_Client<> mqttClient;
// Initialize used apis
Server_Side_RPC<MAX_RPC_SUBSCRIPTIONS, MAX_RPC_RESPONSE> rpc;
const std::array<IAPI_Implementation*, 1U> apis = {
    &rpc
};
// Initialize ThingsBoard instance with the maximum needed buffer size
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE, Default_Max_Stack_Size, apis);

// Status for successfully connecting to the given WiFi
bool wifi_connected = false;
// Statuses for subscribing to rpc
bool subscribed = false;


/// @brief Callback method that is called if we got an ip address from the connected WiFi meaning we successfully established a connection
/// @param event_handler_arg User data registered to the event
/// @param event_base Event base for the handler
/// @param event_id The id for the received event
/// @param event_data The data for the event, esp_event_handler_t
void on_got_ip(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    wifi_connected = true;
}

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {
    const wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

    esp_netif_config_t netif_config = ESP_NETIF_DEFAULT_WIFI_STA();
    esp_netif_t *netif = esp_netif_new(&netif_config);
    assert(netif);

    ESP_ERROR_CHECK(esp_netif_attach_wifi_station(netif));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ip_event_t::IP_EVENT_STA_GOT_IP, &on_got_ip, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_default_wifi_sta_handlers());
    ESP_ERROR_CHECK(esp_wifi_set_storage(wifi_storage_t::WIFI_STORAGE_RAM));

    wifi_config_t wifi_config;
    memset(&wifi_config, 0, sizeof(wifi_config));
    strncpy(reinterpret_cast<char*>(wifi_config.sta.ssid), WIFI_SSID, strlen(WIFI_SSID) + 1);
    strncpy(reinterpret_cast<char*>(wifi_config.sta.password), WIFI_PASSWORD, strlen(WIFI_PASSWORD) + 1);

    ESP_LOGI("MAIN", "Connecting to %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(wifi_mode_t::WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(wifi_interface_t::WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}

/// @brief Processes function for RPC call "example_json"
/// JsonVariantConst is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// @param data Data containing the rpc data that was called and its current value
/// @param response Data containgin the response value, any number, string or json, that should be sent to the cloud. Useful for getMethods
void processGetJson(const JsonVariantConst &data, JsonDocument &response) {
    ESP_LOGI("RPC Example", "Received the json RPC method");

    // Size of the response document needs to be configured to the size of the innerDoc + 1.
    StaticJsonDocument<JSON_OBJECT_SIZE(4)> innerDoc;
    innerDoc["string"] = "exampleResponseString";
    innerDoc["int"] = 5;
    innerDoc["float"] = 5.0f;
    innerDoc["bool"] = true;
    response["json_data"] = innerDoc;
}

/// @brief Processes function for RPC call "example_set_temperature"
/// JsonVariantConst is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// @param data Data containing the rpc data that was called and its current value
/// @param response Data containgin the response value, any number, string or json, that should be sent to the cloud. Useful for getMethods
void processTemperatureChange(const JsonVariantConst &data, JsonDocument &response) {
    const float example_temperature = data[RPC_TEMPERATURE_KEY];
    ESP_LOGI("RPC Example", "Received the set temperature RPC method: %f", example_temperature);

    // Ensure to only pass values do not store by copy, or if they do increase the MaxRPC template parameter accordingly to ensure that the value can be deserialized.
    // See https://arduinojson.org/v6/api/jsondocument/add/ for more information on which variables cause a copy to be created
    response["string"] = "exampleResponseString";
    response["int"] = 5;
    response["float"] = 5.0f;
    response["double"] = 10.0;
    response["bool"] = true;
}

/// @brief Processes function for RPC call "example_set_switch"
/// JsonVariantConst is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// @param data Data containing the rpc data that was called and its current value
/// @param response Data containgin the response value, any number, string or json, that should be sent to the cloud. Useful for getMethods
void processSwitchChange(const JsonVariantConst &data, JsonDocument &response) {
    // Process data
    const bool switch_state = data[RPC_SWITCH_KEY];
    ESP_LOGI("RPC Example", "Received the set switch method: %d", switch_state);
    response.set(22.02);
}

extern "C" void app_main(void) {
    ESP_LOGI("MAIN", "[APP] Startup..");
    ESP_LOGI("MAIN", "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI("MAIN", "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    InitWiFi();

#if ENCRYPTED
    mqttClient.set_server_certificate(ROOT_CERT);
#endif // ENCRYPTED

    for (;;) {
        // Wait until we connected to WiFi
        if (!wifi_connected) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        if (!tb.connected()) {
            tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT);
        }

        if (!subscribed) {
            const std::array<RPC_Callback, MAX_RPC_SUBSCRIPTIONS> callbacks = {
              // Requires additional memory in the JsonDocument for the JsonDocument that will be copied into the response
              RPC_Callback{ RPC_JSON_METHOD,           processGetJson },
              // Requires additional memory in the JsonDocument for 5 key-value pairs that do not copy their value into the JsonDocument itself
              RPC_Callback{ RPC_TEMPERATURE_METHOD,    processTemperatureChange },
               // Internal size can be 0, because if we use the JsonDocument as a JsonVariant and then set the value we do not require additional memory
              RPC_Callback{ RPC_SWITCH_METHOD,         processSwitchChange }
            };
            // Perform a subscription. All consequent data processing will happen in
            // processTemperatureChange() and processSwitchChange() functions,
            // as denoted by callbacks array.
            subscribed = rpc.RPC_Subscribe(callbacks.begin(), callbacks.end());
        }

        tb.loop();
        
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
