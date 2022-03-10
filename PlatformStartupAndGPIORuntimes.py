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

shellESpeak("Welcome to the mutlipurpose platform.")
shellESpeak("I currently have the following functionality: ")
shellESpeak("GPS querying")
shellESpeak("Accelerometer, Magnetometer")
shellESpeak("3 meter sonic range finder")
shellESpeak("HD Camera with 2 axis pan tilt and webserver streaming option")
shellESpeak("Four wheeled drive")
shellESpeak("Six Axis High torque arm")
shellESpeak("OLED Screen")
shellESpeak("Microphone")
shellESpeak("Speaker")
shellESpeak("Spotify integration")

time.sleep(0.5)
shellESpeak("Please consult 'APIExamples.txt' for API documentation on these features")

time.sleep(1)
shellESpeak("GPIO Runtime initialised.")
time.sleep(0.5)
shellESpeak("Press button for shutdown.")
shellESpeak("Double press Button for restart.")
time.sleep(0.5)
shellESpeak("Please consult 'APIExamples.txt' for API documentation")

shellESpeak("I am now completely operational Dave, and all my circuits are functionin perfectly")
time.sleep(0.2)
shellESpeak("Just Kidding")

btn = Button(26) # defines the button as an object and chooses GPIO 26
shutdownButtonHoldTime = 5
retartButtonHoldTime = 2
while True: #infinite loop
    if btn.is_pressed: #Check to see if button is pressed
        time.sleep(1) # wait for the hold time we want. 
        if btn.is_pressed: #check if the user let go of the button
            print("pressed and held, shutting down")
            shellESpeak("shuting down.")
            os.system("shutdown now -h") #shut down the Pi -h is or -r will reset
        time.sleep(1) # wait to loop again so we don’t use the processor too much.
    else:
        btn.wait_for_press()
        btn.wait_for_release()
        if btn.wait_for_press(timeout=0.6):
            print("pressed twice, restarting")
            shellESpeak("Restarting.")
            os.system("shutdown -r now")