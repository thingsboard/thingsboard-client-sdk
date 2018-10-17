#!/bin/sh

set -e

# Test if arduino command line interface is downloaded locally
if [ -f "$(pwd)"/arduino-cli-linux64 ]
then
    ARDUINO_CLI="$(pwd)/arduino-cli-linux64"
    echo "Found arduino CLI in $ARDUINO_CLI"
else
    ARDUINO_CLI="arduino-cli"
fi

do_test() {
    $ARDUINO_CLI compile -b arduino:avr:uno examples/process_rpc_esp8266/
    $ARDUINO_CLI compile -b arduino:avr:uno examples/send_batch_esp8266/
    $ARDUINO_CLI compile -b arduino:avr:uno examples/send_esp8266/
}

# Create symlinks to library under test

ln -sf "$(pwd)/src/ThingsBoard.h" "examples/process_rpc_esp8266/ThingsBoard.h"
ln -sf "$(pwd)/src/ThingsBoard.h" "examples/send_batch_esp8266/ThingsBoard.h"
ln -sf "$(pwd)/src/ThingsBoard.h" "examples/send_esp8266/ThingsBoard.h"

ln -sf "$(pwd)/src/ThingsBoard.cpp" "examples/process_rpc_esp8266/ThingsBoard.cpp"
ln -sf "$(pwd)/src/ThingsBoard.cpp" "examples/send_batch_esp8266/ThingsBoard.cpp"
ln -sf "$(pwd)/src/ThingsBoard.cpp" "examples/send_esp8266/ThingsBoard.cpp"

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
