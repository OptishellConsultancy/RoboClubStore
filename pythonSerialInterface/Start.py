import serial
import time
arduino = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=.1)
def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data.decode("utf-8") 

def Read_PrintIfValue():
    value = arduino.readline().decode("utf-8")
    if(len(value) > 0):
        print(value)
while True:
    Read_PrintIfValue()
    value = write_read(input())    
    print(value) # printing the value