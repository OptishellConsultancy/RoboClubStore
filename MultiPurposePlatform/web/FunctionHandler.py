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
    LastCachedResultsRaw = ''
    UltraSonicDistance = ''
    UltraSonicTemp = ''
    GPSHasFixLiveData = ''
    AccMagAccRaw = ''
    AccMagMagRaw = ''
    AccMagHeading = ''

    def __init__(self):
        print("HandlePySerialArdunioIF.pyWebRequest Invoke..")
        self.ardSerial = serial.Serial(
            port='/dev/ttyACM0', baudrate=9600, timeout=.5)

    def write(self, x):
        self.ardSerial.write(bytes(x, 'utf-8'))

    def Read_PrintIfValueUntilNoData(self, results=[], rlvlDebug = 0):
        if rlvlDebug == 0:
            results=[]
            self.ardSerial.flush()
            self.ardSerial.reset_output_buffer()
            self.ardSerial.read_all()
        try:
            value = self.ardSerial.readline().decode("windows-1252")
            if(len(value) > 0):
                results.append(value)
                if(len(results) > 0):
                    self.Read_PrintIfValueUntilNoData(results, rlvlDebug +1)                        
                return results
        except:
            return []


    def InvokeSerialComm(self):
        self.ardSerial = serial.Serial(
            port='/dev/ttyACM0', baudrate=9600, timeout=.5)
        while True:
            self.Read_PrintIfValueUntilNoData()

    def HandleGPS(self, resultsConcat):
        idx = 0
        foundGPSCoords = False
        validData = True
        for i in range(len(resultsConcat)):
            if(validData == True):
                if('FIX NOT ACQUIRED' in resultsConcat[i]):
                    validData = False

        # print("HandllingGPS from: " + "-----" + str(resultsConcat) + "-----")
        if(validData == True):
            try:
                self.GPSTime = resultsConcat[1].replace("\n", "")
                self.GPSDate = resultsConcat[2].replace("\n", "")
                self.GPSLatNorth = resultsConcat[6].replace('N', '').replace("\n", "")
                self.GPSLonEast = resultsConcat[7].replace('E', '').replace("\n", "")
                self.GPSSpeed = resultsConcat[8].replace("\n", "")
                self.GPSAltAndSats = resultsConcat[9].replace("\n", "")
                validData = 'Sats' in  self.GPSAltAndSats # Additional validation
                self.GPSHasFixLiveData = resultsConcat[10].replace("\n", "")
            except:
                print('error parsing gps data')

            # print("All GPSresultsConcat:" + str(resultsConcat)) DEBUG
            print("GPSTime:" + self.GPSTime)
            print("GPSDate:" + self.GPSDate)
            print("GPSLatNorth:" + self.GPSLatNorth)
            print("GPSLonEast:" + self.GPSLonEast)
            print("GPSSpeed:" + self.GPSSpeed)
            print("GPSAltAndSats:" + self.GPSAltAndSats)
            print("GPSHasFixLiveData:" + self.GPSHasFixLiveData)
        else:
            print("Invalid GPS data detected from: " + "-----" + str(resultsConcat) + "-----")
            self.GPSTime = ''
            self.GPSDate = ''
            self.GPSLatNorth = ''
            self.GPSLonEast = ''
            self.GPSSpeed = ''
            self.GPSAltAndSats = ''
            self.GPSHasFixLiveData = ''
            return

    def HandleUltraSonic(self, resultsConcat):
        self.LastCachedResultsRaw = resultsConcat
        print("HandleUltraSonic -> " +  str(resultsConcat))
        self.UltraSonicDistance = resultsConcat[1].replace("\n", "")
        self.UltraSonicTemp = resultsConcat[2].replace("\n", "")

    def HandleAccMag(self, resultsConcat):
        self.LastCachedResultsRaw = resultsConcat
        print("HandleAccMag -> " +  str(resultsConcat))
        self.AccMagAccRaw = resultsConcat[1].replace("\n", "")
        self.AccMagMagRaw = resultsConcat[2].replace("\n", "")
        self.AccMagHeading = resultsConcat[3].replace("\n", "")
        print(self.AccMagAccRaw)
        print(self.AccMagMagRaw)
        print(self.AccMagHeading)

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
            resp = ''
            resp = self.Read_PrintIfValueUntilNoData()

            for i in range(len(resp)):
                if "<UltraSonic.Start:" in resp[i]:
                    self.HandleUltraSonic(resp[i:])
                    return 

            for i in range(len(resp)):
                if "<AccMag.Start:" in resp[i]:
                    self.HandleAccMag(resp[i:])
                    return        

            for i in range(len(resp)):
                if "<GPSDATETIME.Start:" in resp[i]:
                    self.HandleGPS(resp[i:])
                    return 
