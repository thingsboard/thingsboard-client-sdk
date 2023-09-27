# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
cmake_minimum_required(VERSION 3.5)
project(ESP32_SEND_DATA)

# Specify all source files to keep the project compatible with espidf build system
idf_component_register(
    SRCS
    "0014-espressif_esp32_send_data.cpp"
    INCLUDE_DIRS "."
)
