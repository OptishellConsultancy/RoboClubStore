#!/bin/bash
echo "Beginning recording for 10 seconds.."
rm ARecording.wav
arecord -D plughw:1,0 -d 10 ARecording.wav
echo "Done, now playing back.."
omxplayer -o local ARecording.wav
echo "Done"