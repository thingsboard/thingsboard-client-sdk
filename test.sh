#!/bin/sh

set -e

# Dependencies

arduino --install-boards arduino:avr

# Create symlinks to library under test

ln -sf "$(pwd)/src/ThingsBoard.h" "examples/process_rpc_esp8266/ThingsBoard.h"
ln -sf "$(pwd)/src/ThingsBoard.h" "examples/send_batch_esp8266/ThingsBoard.h"
ln -sf "$(pwd)/src/ThingsBoard.h" "examples/send_esp8266/ThingsBoard.h"

ln -sf "$(pwd)/src/ThingsBoard.cpp" "examples/process_rpc_esp8266/ThingsBoard.cpp"
ln -sf "$(pwd)/src/ThingsBoard.cpp" "examples/send_batch_esp8266/ThingsBoard.cpp"
ln -sf "$(pwd)/src/ThingsBoard.cpp" "examples/send_esp8266/ThingsBoard.cpp"

arduino --verify examples/process_rpc_esp8266/process_rpc_esp8266.ino --board arduino:avr:uno
arduino --verify examples/send_batch_esp8266/send_batch_esp8266.ino --board arduino:avr:uno
arduino --verify examples/send_esp8266/send_esp8266.ino --board arduino:avr:uno
