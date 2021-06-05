#!/bin/bash
alias acompile="arduino-cli compile --fqbn arduino:avr:uno"
alias aupload="arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno"
acompile FullRuntime4WDPlatform/ && aupload FullRuntime4WDPlatform/