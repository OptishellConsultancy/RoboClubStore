import sys
import serial
import time
import threading

class InputThread(threading.Thread):

    def __init__(self, input_cbk = None, name='InputThread'):
        self.input_cbk = input_cbk
        super(InputThread, self).__init__(name=name)
        self.start()
	
    def run(self):
        while True:            
            self.input_cbk(input()) 

class HandleFunctionRequest():
   arduino = 0
   inputThread = 0

   def __init__(self, function, functionData):
      print("HandlePySAIWebRequest Invoke..")


   def write_read(self,x):
      arduino.write(bytes(x, 'utf-8'))

   def Read_PrintIfValue(self):
      value = arduino.readline().decode("windows-1252")
      if(len(value) > 0):
         print(value)

   def InvokeSerialComm(self):
      self.arduino = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=.5)
      self.inputThread = InputThread(write_read)   
      while True:
         Read_PrintIfValue()


