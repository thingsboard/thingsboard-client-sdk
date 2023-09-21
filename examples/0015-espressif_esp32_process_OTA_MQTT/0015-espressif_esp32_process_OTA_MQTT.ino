#include <esp_netif.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <nvs_flash.h>


// Whether the given script is using encryption or not,
// generally recommended as it increases security (communication with the server is not in clear text anymore),
// it does come with an overhead tough as having an encrypted session requires a lot of memory,
// which might not be avaialable on lower end devices.
#define ENCRYPTED false


#include <Espressif_Updater.h>
#include <Espressif_MQTT_Client.h>
#include <ThingsBoard.h>


// Firmware title and version used to compare with remote version, to check if an update is needed.
// Title needs to be the same and version needs to be different --> downgrading is possible
constexpr char CURRENT_FIRMWARE_TITLE[] = "ESP32";
constexpr char CURRENT_FIRMWARE_VERSION[] = "1.0.0";

// Maximum amount of retries we attempt to download each firmware chunck over MQTT
constexpr uint8_t FIRMWARE_FAILURE_RETRIES = 12U;
// Size of each firmware chunck downloaded over MQTT,
// increased packet size, might increase download speed
constexpr uint16_t FIRMWARE_PACKET_SIZE = 4096U;

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

// Thingsboard we want to establish a connection too
constexpr char THINGSBOARD_SERVER[] = "demo.thingsboard.io";

// MQTT port used to communicate with the server, 1883 is the default unencrypted MQTT port,
// whereas 8883 would be the default encrypted SSL MQTT port
#if ENCRYPTED
constexpr uint16_t THINGSBOARD_PORT = 8883U;
#else
constexpr uint16_t THINGSBOARD_PORT = 1883U;
#endif

// Maximum size packets will ever be sent or received by the underlying MQTT client,
// if the size is to small messages might not be sent or received messages will be discarded.
// The Espressif_MQTT_Client, currently has an issue with the underlying library used, where it is not possible
// to change the buffer size once the client has been initalized, meaning the buffer size can only be set before calling connect(),
// for the first time. Therefore when using the OTA update mechanism it is required to increase the buffer size to the size of the received firmware packets
// and a little bit more for the topic we received the message on.
// This has to be done at least until the issue https://github.com/espressif/esp-mqtt/issues/267 has been fixed in the esp-mqtt client,
// or if an older version of the esp-mqtt client is used that does not include the possible fixes to the aforementioned issue yet.
constexpr uint16_t MAX_MESSAGE_SIZE = FIRMWARE_PACKET_SIZE + 50U;

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
// Initalize the Updater client instance used to flash binary to flash memory
Espressif_Updater updater;


// Status for successfully connecting to the given WiFi
bool wifi_connected = false;
// Statuses for updating
bool currentFWSent = false;
bool updateRequestSent = false;


/// @brief Updated callback that will be called as soon as the firmware update finishes
/// @param success Either true (update successful) or false (update failed)
void updatedCallback(const bool& success) {
  if (success) {
    ESP_LOGI("MAIN", "Done, Reboot now");
    esp_restart();
    return;
  }
  ESP_LOGI("MAIN", "Downloading firmware failed");
}

/// @brief Progress callback that will be called every time we downloaded a new chunk successfully
/// @param currentChunk 
/// @param totalChuncks 
void progressCallback(const size_t& currentChunk, const size_t& totalChuncks) {
  ESP_LOGI("MAIN", "Downwloading firmware progress %.2f%%", static_cast<float>(currentChunk * 100U) / totalChuncks);
}

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

extern "C" void app_main() {
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

        if (!currentFWSent) {
            // Firmware state send at the start of the firmware, to inform the cloud about the current firmware and that it was installed correctly,
            // especially important when using OTA update, because the OTA update sends the last firmware state as UPDATING, meaning the device is restarting
            // if the device restarted correctly and has the new given firmware title and version it should then send thoose to the cloud with the state UPDATED,
            // to inform any end user that the device has successfully restarted and does actually contain the version it was flashed too
            currentFWSent = tb.Firmware_Send_Info(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION) && tb.Firmware_Send_State(FW_STATE_UPDATED);
        }

        if (!updateRequestSent) {
            const OTA_Update_Callback callback(&progressCallback, &updatedCallback, CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION, &updater, FIRMWARE_FAILURE_RETRIES, FIRMWARE_PACKET_SIZE);
            // See https://thingsboard.io/docs/user-guide/ota-updates/
            // to understand how to create a new OTA pacakge and assign it to a device so it can download it.
            updateRequestSent = tb.Start_Firmware_Update(callback);
        }

        tb.loop();
        
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
