#!/bin/bash

set -e

EXAMPLES_ARDUINO=(
    "examples/0000-arduino_send_telemetry"
    "examples/0001-arduino_send_batch"
    "examples/0002-arduino_rpc"
    "examples/0004-arduino-sim900_send_telemetry"
    "examples/0005-arduino-sim900_send_telemetry_http"
)

EXAMPLES_ESP8266_ESP32=(
    "examples/0003-esp8266_esp32_send_data"
    "examples/0006-esp8266_esp32_process_shared_attribute_update"
    "examples/0007-esp8266_esp32_claim_device"
    "examples/0008-esp8266_esp32_provision_device"
    "examples/0009-esp8266_esp32_process_OTA_MQTT"
    "examples/0010-esp8266_esp32_rpc"
)

ARDUINOS=( "nano" "uno" "mega" )

EXAMPLES=( "${EXAMPLES_ESP8266_ESP32[@]}" "${EXAMPLES_ARDUINO[@]}")

# Test if arduino command line interface is downloaded locally
if [ -f "$(pwd)/arduino-cli" ]
then
    ARDUINO_CLI="$(pwd)/arduino-cli"
    echo "Found arduino CLI in $ARDUINO_CLI"
else
    ARDUINO_CLI="arduino-cli"
fi

do_test() {
    for path in "${EXAMPLES_ARDUINO[@]}"
    do
        for arduino_board in "${ARDUINOS[@]}"
        do
            echo "Building for ${arduino_board}"
            "${ARDUINO_CLI}" compile --warnings all -b arduino:avr:${arduino_board} "${path}"
        done
    done

    for path in "${EXAMPLES_ESP8266_ESP32[@]}"
    do
        echo "Processing ESP8266 example with path: ${path}"
        "${ARDUINO_CLI}" compile -b esp8266:esp8266:generic "${path}"
        echo "Processing ESP32 example with path: ${path}"
        "${ARDUINO_CLI}" compile -b esp32:esp32:generic "${path}"
    done
}

# Create symlinks to library under test
do_link() {
    for path in "${EXAMPLES[@]}"
    do
        ln -sf "$(pwd)/src/Constants.h" "${path}/Constants.h"
        ln -sf "$(pwd)/src/HashGenerator.h" "${path}/HashGenerator.h"
        ln -sf "$(pwd)/src/HashGenerator.cpp" "${path}/HashGenerator.cpp"
        ln -sf "$(pwd)/src/Telemetry.h" "${path}/Telemetry.h"
        ln -sf "$(pwd)/src/ThingsBoardDefaultLogger.h" "${path}/ThingsBoardDefaultLogger.h"
        ln -sf "$(pwd)/src/ThingsBoardDefaultLogger.cpp" "${path}/ThingsBoardDefaultLogger.cpp"
        ln -sf "$(pwd)/src/ThingsBoardHttp.h" "${path}/ThingsBoardHttp.h"
        ln -sf "$(pwd)/src/ThingsBoard.h" "${path}/ThingsBoard.h"
    done
}

do_link

# Dependencies

# `|| true`` added to ignore an error if a lib is already installed

"${ARDUINO_CLI}" lib install PubSubClient || true
"${ARDUINO_CLI}" lib install WiFiEsp || true
"${ARDUINO_CLI}" lib install ArduinoJson || true
"${ARDUINO_CLI}" lib install TinyGSM || true
"${ARDUINO_CLI}" lib install ArduinoHttpClient || true

do_test
