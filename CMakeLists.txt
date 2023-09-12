cmake_minimum_required(VERSION 3.12)

set(srcs
    src/Arduino_HTTP_Client.cpp
    src/Arduino_MQTT_Client.cpp
    src/Attribute_Request_Callback.cpp
    src/Callback_Watchdog.cpp
    src/Arduino_ESP32_Updater.cpp
    src/Arduino_ESP8266_Updater.cpp
    src/Espressif_Updater.cpp
    src/Espressif_MQTT_Client.cpp
    src/HashGenerator.cpp
    src/Helper.cpp
    src/OTA_Update_Callback.cpp
    src/Provision_Callback.cpp
    src/RPC_Callback.cpp
    src/RPC_Request_Callback.cpp
    src/RPC_Response.cpp
    src/Shared_Attribute_Callback.cpp
    src/Telemetry.cpp
    src/ThingsBoardDefaultLogger.cpp
)

set(dependencies
    mqtt
    mbedtls
)

set(private_dependencies
    esp_timer
)

if(ESP_PLATFORM)
	# Build ThingsBoard Arduino SDK as an ESP-IDF component
	idf_component_register(
		SRCS ${srcs}
		INCLUDE_DIRS "src"
        PRIV_INCLUDE_DIRS "src"
		REQUIRES ${dependencies}
		PRIV_REQUIRES ${private_dependencies}
	)
	return()
endif()

project(ThingsBoardArduinoSDK VERSION 0.12.0)
