#!/bin/sh

set -e

# Test if arduino command line interface is downloaded locally
if [ -f "$(pwd)"/arduino-cli-0.3.2-alpha.preview-linux64 ]
then
    ARDUINO_CLI="$(pwd)/arduino-cli-0.3.2-alpha.preview-linux64"
    echo "Found arduino CLI in $ARDUINO_CLI"
else
    ARDUINO_CLI="arduino-cli"
fi

do_test() {
    $ARDUINO_CLI compile -b arduino:avr:uno examples/0000-arduino_send_telemetry/
    $ARDUINO_CLI compile -b arduino:avr:uno examples/0001-arduino_send_batch/
    $ARDUINO_CLI compile -b arduino:avr:uno examples/0002-arduino_rpc/
}

# Create symlinks to library under test

ln -sf "$(pwd)/src/ThingsBoard.h" "examples/0000-arduino_send_telemetry/ThingsBoard.h"
ln -sf "$(pwd)/src/ThingsBoard.h" "examples/0001-arduino_send_batch/ThingsBoard.h"
ln -sf "$(pwd)/src/ThingsBoard.h" "examples/0002-arduino_rpc/ThingsBoard.h"

ln -sf "$(pwd)/src/ThingsBoard.cpp" "examples/0000-arduino_send_telemetry/ThingsBoard.cpp"
ln -sf "$(pwd)/src/ThingsBoard.cpp" "examples/0001-arduino_send_batch/ThingsBoard.cpp"
ln -sf "$(pwd)/src/ThingsBoard.cpp" "examples/0002-arduino_rpc/ThingsBoard.cpp"

# Dependencies

# Ignore if already installed
$ARDUINO_CLI lib install PubSubClient || true
# Ignore if already installed
$ARDUINO_CLI lib install WiFiEsp || true
# Ignore if already installed
$ARDUINO_CLI lib install ArduinoJson || true

do_test

# Install old library version. Ignore if already installed
$ARDUINO_CLI lib install ArduinoJson@5.13.3 || true

do_test
