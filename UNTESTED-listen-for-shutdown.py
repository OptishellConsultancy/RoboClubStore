#!/usr/bin/env python
import time
import RPi.GPIO as GPIO
import subprocess

shutdown_pin = 15

# Use "GPIO" pin numbering
GPIO.setmode(GPIO.BCM)

GPIO.setup(shutdown_pin, GPIO.IN)

def restart():
    print("restarting Pi")
    command = "/usr/bin/sudo /sbin/shutdown -r now"
    import subprocess
    process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
    output = process.communicate()[0]
    print(output)

def shut_down():
    print("shutting down")
    command = "/usr/bin/sudo /sbin/shutdown -h now"
    import subprocess
    process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
    output = process.communicate()[0]
    print(output)

while True:
    #short delay, otherwise this code will take up a lot of the Pi's processing power
    time.sleep(0.5)

    # For troubleshooting, uncomment this line to output button status on command line
    #print('GPIO state is = ", GPIO.input(reset_shutdown_pin))
    if GPIO.input(shutdown_pin) == False:
        counter = 0

        while GPIO.input(shutdown_pin) == False:
            #For troubleshooting, uncomment this line to view the counter. If it reaches a value above 4, we will restart.     
            #print(counter)
            counter += 1
            time.sleep(0.5)

            # long button press
            if counter > 4:
                shut_down()

        #if short button press, restart!
        restart()


#To execute on startup
#sudo nano/etc/rc.local. ->
# Add command: 'python ~/home/pi/Desktop/RoboClubStore/listen-for-shutdown.py '