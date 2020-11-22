"""This example prints to the serial console when the Circuit Playground is shaken."""
from adafruit_circuitplayground import cp

while True:
    if cp.shake():
        print("Shake detected!")
