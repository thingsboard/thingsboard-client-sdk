#!/bin/bash

set -e

EXAMPLES_ARDUINO_UNO=(
    "examples/0000-arduino_send_telemetry"
    "examples/0001-arduino_send_batch"
    "examples/0002-arduino_rpc"
    "examples/0004-arduino-sim900_send_telemetry"
    "examples/0005-arduino-sim900_send_telemetry_http"
)

EXAMPLES_ESP8266=(
    "examples/0003-esp8266_send_data"
    "examples/0006-esp8266_process_shared_attribute_update"
    "examples/0007-esp8266_claim_device"
    "examples/0008-esp8266_provision_device"
)

EXAMPLES=( "${EXAMPLES_ESP8266[@]}" "${EXAMPLES_ARDUINO_UNO[@]}")

# Test if arduino command line interface is downloaded locally
if [ -f "$(pwd)/arduino-cli" ]
then
    ARDUINO_CLI="$(pwd)/arduino-cli"
    echo "Found arduino CLI in $ARDUINO_CLI"
else
    ARDUINO_CLI="arduino-cli"
fi

do_test() {
    for path in "${EXAMPLES_ARDUINO_UNO[@]}"
    do
        "${ARDUINO_CLI}" compile --warnings all -v -b arduino:avr:uno "${path}"
    done

    for path in "${EXAMPLES_ESP8266[@]}"
    do
        echo "Processing ESP8266 example with path: ${path}"
        "${ARDUINO_CLI}" compile -b esp8266:esp8266:generic "${path}"
    done
}

# Create symlinks to library under test
do_link() {
    for path in "${EXAMPLES[@]}"
    do
        ln -sf "$(pwd)/src/ThingsBoard.h" "${path}/ThingsBoard.h"
        ln -sf "$(pwd)/src/ThingsBoard.cpp" "${path}/ThingsBoard.cpp"
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
