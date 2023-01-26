import sys
import serial
import time
import threading
import re
import pantilthat


class FunctionHandler():

    GPSTime = ''
    GPSDate = ''
    GPSLatNorth = ''
    GPSLonEast = ''
    GPSSpeed = ''
    GPSAltAndSats = ''

    def __init__(self):
        print("HandlePySerialArdunioIF.pyWebRequest Invoke..")
        self.arduino = serial.Serial(
            port='/dev/ttyACM0', baudrate=9600, timeout=.5)

    def write(self, x):
        self.arduino.write(bytes(x, 'utf-8'))

    def Read_PrintIfValueUntilNoData(self, results=[]):
        try:
            value = self.arduino.readline().decode("windows-1252")
            if(len(value) > 0):
                print(value)
                results.append(value)
                if(len(results) > 0):
                    self.Read_PrintIfValueUntilNoData(results)
            return results
        except:
            return ''


    def InvokeSerialComm(self):
        self.arduino = serial.Serial(
            port='/dev/ttyACM0', baudrate=9600, timeout=.5)
        while True:
            self.Read_PrintIfValueUntilNoData()

    def HandleGPS(self, resultsConcat):
        idx = 0
        foundGPSCoords = False
        self.GPSTime = resultsConcat[1].replace("\n", "")
        self.GPSDate = resultsConcat[2].replace("\n", "")
        self.GPSLatNorth = resultsConcat[6].replace('N', '').replace("\n", "")
        self.GPSLonEast = resultsConcat[7].replace('E', '').replace("\n", "")
        self.GPSSpeed = resultsConcat[8].replace("\n", "")
        self.GPSAltAndSats = resultsConcat[9].replace("\n", "")

    def PanOrTilt(self, panAngle, tiltAngle):
        panAngle = float(panAngle)
        tiltAngle = float(tiltAngle)
        if panAngle < -90.0 or panAngle > 90.0 or tiltAngle < -90.0 or tiltAngle > 90.0:
            print("PanOrTilt-> Bad angle!")
            return

        incPan = panAngle/20.0
        incTilt = tiltAngle/20.0

        for i in range(10):
            pantilthat.pan(float(i*incPan))
            pantilthat.tilt(float(i*incTilt))
            time.sleep(0.005)

    def DoFunctionNow(self, functionName, commandData=[], configData=[], type='ARD'):

        if(type == 'RPI'):
            if(functionName == 'panTilt'):
                print(commandData)
                print(configData)
                self.PanOrTilt(configData[0], configData[1])

        if(type == 'ARD'):
            # Encase not in F7. API Mode..
            self.Read_PrintIfValueUntilNoData()
            self.write("F7.")

            # Construct command
            fullcmd = functionName

            cmdLen = len(commandData)
            if(cmdLen > 0):
                fullcmd += '['
                for i in range(len(commandData)):
                    fullcmd += commandData[i]
                fullcmd += "]"

            cnfLen = len(configData)
            if(cnfLen > 0):
                fullcmd += '{'
                for i in range(len(configData)):
                    fullcmd += configData[i]
                fullcmd += "}"
            #######
            print("fullcmd: " + fullcmd)

            self.write(fullcmd)
            results = self.Read_PrintIfValueUntilNoData()

            for i in range(len(results)):
                if "<GPSDATETIME.Start:" in results[i]:
                    self.HandleGPS(results[i:])
