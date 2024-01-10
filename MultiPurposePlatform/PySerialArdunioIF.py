import sys
import serial
import time
import threading

if sys.version_info[0] < 3:
    raise Exception("Python 3 or a more recent version is required.")


class InputThread(threading.Thread):

    def __init__(self, input_cbk=None, name='InputThread'):
        self.input_cbk = input_cbk
        super(InputThread, self).__init__(name=name)
        self.start()

    def run(self):
        while True:
            self.input_cbk(input())


#########################
arduino = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=.5)


def write(x):
    arduino.write(bytes(x, 'utf-8'))


def Read_PrintIfValue():
    value = arduino.readline().decode("windows-1252")
    if(len(value) > 0):
        print(value)


inputThread = InputThread(write)
while True:
    Read_PrintIfValue()


