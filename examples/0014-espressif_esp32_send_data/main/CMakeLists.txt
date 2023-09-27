# Specify all source files to keep the project compatible with espidf build system
idf_component_register(
    SRCS
    "../../../src"
    "0014-espressif_esp32_send_data.cpp"
    INCLUDE_DIRS "../../../src"
)
