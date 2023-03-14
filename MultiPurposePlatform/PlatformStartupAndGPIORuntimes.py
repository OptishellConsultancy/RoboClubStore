#!/usr/bin/python
# coding: utf-8

# To run on start:
# sudo raspi-config -> system options -> Boot / Auto Login ->  Console Autologin”
# sudo nano /etc/profile
# add: sudo python /home/pi/Desktop/RoboClubStore/PlatformStartupAndGPIORuntimes.py &
# CTRL+X

from gpiozero import Button #import button from the Pi GPIO library
import time # import time functions
import os #imports OS library for Shutdown control
from requests import get


def check_privileges():
    if not os.environ.get("SUDO_UID") and os.geteuid() != 0:
        raise PermissionError("You need to run this script with sudo or as root.")

check_privileges()

# http://espeak.sourceforge.net/commands.html
def shellESpeak(text, devicepcm = 'plughw:CARD=Audio,DEV=0'):
    os.popen('espeak "' + text + '" --stdout | aplay -D'+ devicepcm).read()

def StartupConvo():
    shellESpeak("Hello, I'm SAMUEL")
    shellESpeak("I am now completely operational, and all my circuits are functioning perfectly")



StartupConvo()


def held():
    print("shutting down")
    shellESpeak("Shutting Down.")
    os.system("shutdown -h now")
    quit()



btn = Button(26, hold_time=2) # defines the button as an object and chooses GPIO 26
btn.when_held = held

while True: #infinite loop
        time.sleep(1)
