# coding: utf-8

# sudo nano /etc/rc.local
# sudo python /home/pi/Desktop/RoboClubStore/GPIOToggleOnOff.py &

from gpiozero import Button #import button from the Pi GPIO library
import time # import time functions
import os #imports OS library for Shutdown control



speak = "Hello world"
os.popen('espeak "' + speak + '" --stdout | aplay 2> /dev/null').read()

print('Done speech')

btn = Button(26) # defines the button as an object and chooses GPIO 26
shutdownButtonHoldTime = 5
retartButtonHoldTime = 2
while True: #infinite loop
    if btn.is_pressed: #Check to see if button is pressed
        time.sleep(1) # wait for the hold time we want. 
        if btn.is_pressed: #check if the user let go of the button
            print("pressed and held, shutting down")
            os.system("shutdown now -h") #shut down the Pi -h is or -r will reset
        time.sleep(1) # wait to loop again so we don’t use the processor too much.
    else:
        btn.wait_for_press()
        btn.wait_for_release()
        if btn.wait_for_press(timeout=0.6):
            print("pressed twice, restarting")
            os.system("shutdown -r now")