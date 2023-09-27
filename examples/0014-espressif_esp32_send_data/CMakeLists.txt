# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
cmake_minimum_required(VERSION 3.5)

# Set project version
set(PROJECT_VER "0.1.1")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ESP32_SEND_DATA)
