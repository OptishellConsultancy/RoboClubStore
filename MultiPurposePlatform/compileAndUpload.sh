#!/bin/bash
#alias acompile="arduino-cli compile --fqbn arduino:avr:uno"
#alias aupload="arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno"
echo "list of known fqbn boards:"
arduino-cli board list
echo "This script assumes te board is an atmega2560, fqbn = arduino:avr:mega: Edit this script according if you have a difference device"
arduino-cli -v compile --fqbn arduino:avr:mega MultiPurposePlatform/FullRuntime4WDPlatform/
echo "Uploading..:"
arduino-cli -v upload -p /dev/ttyACM0 --fqbn arduino:avr:mega MultiPurposePlatform/FullRuntime4WDPlatform/