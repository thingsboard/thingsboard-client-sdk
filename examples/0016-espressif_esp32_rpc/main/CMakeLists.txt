# Be aware this file should not be takes as inspiration on how to set up compilation with the CMake build system when using ESP-IDF, because it directly includes the implementation files.
# This has to be done because the examples are build to test if they are still working and to automatically inform the library if a pull request would break examples.
# To actually include the library in your ESP-IDF project read the documentation especially the Installation section
set(srcs
    0016-espressif_esp32_rpc.cpp
    ../../../src/Arduino_HTTP_Client.cpp
    ../../../src/Arduino_MQTT_Client.cpp
    ../../../src/Arduino_ESP32_Updater.cpp
    ../../../src/Arduino_ESP8266_Updater.cpp
    ../../../src/HashGenerator.cpp
    ../../../src/Helper.cpp
    ../../../src/OTA_Update_Callback.cpp
    ../../../src/Provision_Callback.cpp
    ../../../src/RPC_Request_Callback.cpp
    ../../../src/Telemetry.cpp
)

idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS "../../../src"
)
