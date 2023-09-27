# Specify all source files to keep the project compatible with espidf build system
idf_component_register(
    SRCS
    "0015-espressif_esp32_process_OTA_MQTT.cpp"
    INCLUDE_DIRS "../../src"
)
