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



def check_privileges():
    if not os.environ.get("SUDO_UID") and os.geteuid() != 0:
        raise PermissionError("You need to run this script with sudo or as root.")

check_privileges()

# http://espeak.sourceforge.net/commands.html
def shellESpeak(text):

    os.popen('espeak "' + text + '" --stdout | aplay 5> /dev/null').read()

def StartupConvo():
    shellESpeak("Welcome to the mutlipurpose platform.")

    time.sleep(0.5)
    shellESpeak("Please consult 'APIExamples.txt' for API documentation on these features")

    time.sleep(0.5)
    shellESpeak("Press Button and release quickly for shutdown.")
    shellESpeak("Press Button and hold for restart.")

    shellESpeak("I am now completely operational Dave, and all my circuits are functionin perfectly")
    time.sleep(0.2)
    shellESpeak("Just Kidding")


StartupConvo()

def deactivated():
    print(btn.active_time)
    print("released after {} seconds".format(btn.active_time))
    if btn.active_time < btn.hold_time:
        time.sleep(3)
        print("pressed and held, shutting down")
        shellESpeak("shutting down.")
        os.system("shutdown now -h") #shut down the Pi -h is or -r will reset
        quit()


def held():
    print("Restarting")
    shellESpeak("Restarting.")
    os.system("shutdown -r now")
    quit()



btn = Button(26, hold_time=2) # defines the button as an object and chooses GPIO 26
btn.when_held = held
btn.when_deactivated = deactivated

while True: #infinite loop
        time.sleep(1)
