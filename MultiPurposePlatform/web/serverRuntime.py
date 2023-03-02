
#!/usr/bin/env python3

from flask import Flask, render_template, make_response, redirect, Response, request
import numpy
# import numpy.core.multiarray
# import cv2 #Potential future method for image recog, e.g :https://realpython.com/face-detection-in-python-using-a-webcam/
# For now, we use rpi_cam feed in the index.html for basic vid feed
import socket
import io
from werkzeug.http import dump_cookie
from FunctionHandler import FunctionHandler
import sys
import serial
import time
import threading
import subprocess
import os  # imports OS library for Shutdown control
from flask.sessions import SecureCookieSessionInterface
import json
from datetime import datetime
from mapEntity import MapEntity
import pyaudio
import wave

if sys.version_info[0] < 3:
    raise Exception("Python 3 or a more recent version is required.")

fhnd = FunctionHandler()
app = Flask(__name__)
# vc = cv2.VideoCapture(0)

doGPSOLEDPrint = False
doUltraSonicOLEDPrint = False
doAccMagOLEDPrint = False

currentPan = 0
currentTilt = 0

lastRecording = 0

# TODO  test this, make saveto file optional, and all memory playback or send via ajax request to web host


def doRecord(duration=10.0, sampleRate=44100):
    form_1 = pyaudio.paInt16  # 16-bit resolution
    chans = 1  # 1 channel
    samp_rate = 44100  # 44.1kHz sampling rate
    chunk = 4096  # 2^12 samples for buffer
    record_secs = 3  # seconds to record
    dev_index = 2  # device index found by p.get_device_info_by_index(ii)
    wav_output_filename = 'test1.wav'  # name of .wav file

    audio = pyaudio.PyAudio()  # create pyaudio instantiation

    # create pyaudio stream
    stream = audio.open(format=form_1, rate=samp_rate, channels=chans,
                        input_device_index=dev_index, input=True,
                        frames_per_buffer=chunk)
    print("recording")
    frames = []

    # loop through stream and append audio chunks to frame array
    for ii in range(0, int((samp_rate/chunk)*record_secs)):
        data = stream.read(chunk)
        frames.append(data)

    print("finished recording")

    # stop the stream, close it, and terminate the pyaudio instantiation
    stream.stop_stream()
    stream.close()
    audio.terminate()

    # save the audio frames as .wav file
    wavefile = wave.open(wav_output_filename, 'wb')
    wavefile.setnchannels(chans)
    wavefile.setsampwidth(audio.get_sample_size(form_1))
    wavefile.setframerate(samp_rate)
    wavefile.writeframes(b''.join(frames))
    wavefile.close()


def shellESpeak(text):
    os.popen('espeak "' + text + '" --stdout | aplay 2> /dev/null').read()


@app.route("/")
def index():
    # return render_template("index.html")
    resp = make_response(render_template("index.html"))
    resp.set_cookie('username', 'flask', secure=True,
                    httponly=True, samesite='Lax')
    return resp

# Disable caching


@app.after_request
def add_header(r):
    r.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    r.headers["Pragma"] = "no-cache"
    r.headers["Expires"] = "0"
    r.headers['Cache-Control'] = 'public, max-age=0'
    return r


@app.route("/toggleGPSOLEDDisplay", methods=['POST'])
def ToggledoGPSOLEDPrint():
    global doGPSOLEDPrint  # Use global scope
    doGPSOLEDPrint = not doGPSOLEDPrint
    print("doGPSOLEDPrint is" + str(doGPSOLEDPrint))
    infoDict = {}
    info_list = []
    infoDict['nowState'] = doGPSOLEDPrint
    info_list.append(infoDict)
    return json.dumps(info_list)


@app.route("/getGPSOLEDDisplay", methods=['GET'])
def GetGPSOLEDDisplay():
    global doGPSOLEDPrint  # Use global scope
    infoDict = {}
    info_list = []
    infoDict['nowState'] = doGPSOLEDPrint
    info_list.append(infoDict)
    return json.dumps(info_list)

# See mapApp.js -> DoMapUpdate


@app.route("/mapinjectapi", methods=['POST'])
def MapApi():
    infoDict = {}
    info_list = []

    global doGPSOLEDPrint  # Use global scope
    print('mapinjectapi->doGPSOLEDPrint:' + str(doGPSOLEDPrint))
    addCmd = ['OLEDPRNT'] if doGPSOLEDPrint == True else []
    fhnd.DoFunctionNow("<Out>GPS", [], addCmd, 'ARD')

    fhnd.GPSTime
    fhnd.GPSDate
    fhnd.GPSLatNorth
    fhnd.GPSLonEast
    fhnd.GPSSpeed
    fhnd.GPSAltAndSats
    if("<GPSDATETIME.End>" not in fhnd.GPSDate):

        mapEntity = MapEntity(
            ('My location' + ' ' + fhnd.GPSAltAndSats + ' ' + fhnd.GPSSpeed),
            fhnd.GPSLonEast,
            fhnd.GPSLatNorth,
            (fhnd.GPSDate + ' ' + fhnd.GPSTime))

        infoDict['Name'] = mapEntity.Name
        infoDict['Longitude'] = mapEntity.Longitude
        infoDict['Latitude'] = mapEntity.Latitude
        infoDict['DataTime'] = mapEntity.DataTime
        # Extra info
        infoDict['GPSHasFixLiveData'] = fhnd.GPSHasFixLiveData

        info_list.append(infoDict)
        data = json.dumps(info_list)
        print("MapApi data retreived:..")
        return data
    return json.dumps(info_list)


# PanTilt
@app.route('/doPanTilt', methods=['POST'])
def doPanTilCamera():
    global currentPan, currentTilt
    if request.method == 'POST':
        print("request.form:" + str(request.form))
        currentPan = 0.0
        currentTilt = 0.0
        if(len(request.form['pan']) != 0):
            currentPan = float(request.form['pan'])
        if(len(request.form['tilt']) != 0):
            currentTilt = float(request.form['tilt'])
        if currentPan != None and currentTilt != None:
            fhnd.DoFunctionNow('panTilt', [], [currentPan, currentTilt], 'RPI')

    infoDict = {}
    info_list = []
    infoDict['CurrentPan'] = currentPan
    infoDict['CurrentTilt'] = currentTilt
    info_list.append(infoDict)
    return json.dumps(info_list)


# doPanTilt
@app.route('/getCurrentPan', methods=['GET'])
def getCurrentPan():
    global currentPan
    infoDict = {}
    info_list = []
    infoDict['CurrentPan'] = currentPan
    info_list.append(infoDict)
    return json.dumps(info_list)


@app.route('/getCurrentTilt', methods=['GET'])
def getCurrentTilt():
    global currentTilt
    infoDict = {}
    info_list = []
    infoDict['currentTilt'] = currentTilt
    info_list.append(infoDict)
    return json.dumps(info_list)


# textToSpeech
@app.route('/textToSpeech', methods=['POST'])
def textToSpeech():
    if request.method == 'POST':
        # version 1:
        opt1 = request.form.to_dict()
        for key in opt1:
            if key == "string":
                string = opt1[key]
                print(string)
                shellESpeak(string)
    return ''

# oLEDDisplayTxt


@app.route('/writeOLEDText', methods=['POST'])
def writeOLEDText():
    if request.method == 'POST':

        opt = request.form.to_dict()
        print("opt:" + str(opt))
        string = ''
        x = ''
        y = ''
        s = ''
        for key in opt:
            if key == "string":
                string = opt[key].strip()
            if key == "x":
                x = 'X:'+opt[key]
            if key == "y":
                y = 'Y:'+opt[key]
            if key == "s":
                s = 'S:'+opt[key]

        print('x:' + x)
        print('y:' + x)
        print('z:' + x)

        fhnd.DoFunctionNow("<In>OLEDTXT", [string], [x, y, s], 'ARD')
        shellESpeak("OLED Text recieved")
    return ''


# ultraSonicReading
@app.route("/ultraSonicRequest", methods=['POST'])
def DoultraSonicRequest():
    sampleCount = '1'
    fhnd.DoFunctionNow("<Out>UltSonc", [sampleCount], ([
                       'OLEDPRNT'] if doUltraSonicOLEDPrint == True else []), 'ARD')
    infoDict = {}
    info_list = []
    infoDict['UltraSonicDistance'] = fhnd.UltraSonicDistance
    infoDict['UltraSonicTemp'] = fhnd.UltraSonicTemp
    info_list.append(infoDict)
    return json.dumps(info_list)


@app.route("/toggleOLEDUltraSonicDisplay", methods=['POST'])
def ToggledoUltraSonicOLEDPrint():
    global doUltraSonicOLEDPrint  # Use global scope
    doUltraSonicOLEDPrint = not doUltraSonicOLEDPrint
    print("doUltraSonicOLEDPrint is" + str(doUltraSonicOLEDPrint))
    infoDict = {}
    info_list = []
    infoDict['nowState'] = doUltraSonicOLEDPrint
    info_list.append(infoDict)
    return json.dumps(info_list)

# AccMagReading


@app.route("/toggleOLEDAccMagDisplay", methods=['POST'])
def ToggleOLEDaccMagDisplay():
    global doAccMagOLEDPrint  # Use global scope
    doAccMagOLEDPrint = not doAccMagOLEDPrint
    print("doAccMagOLEDPrint is" + str(doAccMagOLEDPrint))
    infoDict = {}
    info_list = []
    infoDict['nowState'] = doAccMagOLEDPrint
    info_list.append(infoDict)
    return json.dumps(info_list)


@app.route("/accMagRequest", methods=['POST'])
def DoAccMagRequest():
    sampleCount = '1'
    fhnd.DoFunctionNow("<Out>AccMag", [sampleCount], ([
                       'OLEDPRNT'] if doAccMagOLEDPrint == True else []), 'ARD')
    infoDict = {}
    info_list = []
    infoDict['AccMagAcc'] = fhnd.AccMagAccRaw
    infoDict['AccMagMag'] = fhnd.AccMagMagRaw
    infoDict['AccMagHeading'] = fhnd.AccMagHeading
    info_list.append(infoDict)
    return json.dumps(info_list)


# 4WD control
@app.route("/FourWheeledDriveRequest", methods=['POST'])
def DoWheeledDriveRequest():
    if request.method == 'POST':
        data = request.get_json()
        print("data:" + str(data))

        speed = data['speed']
        duration = data['duration']
        motors = data['motors']
        print("4WD.speed:" + str(speed))
        print("4WD.duration:" + str(duration))
        print("4WD.motors:" + str(motors))

        if len(motors) > 0:
            constructedCmd = '<In>4WD'
            for mc in motors:
                constructedCmd += mc

            constructedCmd += '['+speed+']'+'{'+duration+'}Y'

            print('constructedCmd: ' + constructedCmd)
            fhnd.DoFunctionNow(constructedCmd)
            return constructedCmd
        else:
            return 'No motors selected'

    return ''

# DOF Control


@app.route("/do6DOFARMCmd", methods=['POST'])
def do6DOFARMCmd():
    if request.method == 'POST':
        data = request.get_json()
        # print("data:" + str(data))

        base = (data['base'])
        baseTilt = (data['baseTilt'])
        elbow = (data['elbow'])
        wristElavate = (data['wristElavate'])
        wristRotate = (data['wristRotate'])
        claw = (data['claw'])

        print("base keys" + str(base.keys()))
        print("base values" + str(base.values()))

        if (base['enabled'] or baseTilt['enabled']  or elbow['enabled']  or wristElavate['enabled']  or wristRotate['enabled']  or claw['enabled'] ):
            cnstrctdCmd = ''
            if(base['enabled']):
                cnstrctdCmd += '<In>6Axis[B.'+str(base['angle'])+'].'
            if(baseTilt['enabled']):
                cnstrctdCmd += '<In>6Axis[BT.'+str(baseTilt['angle'])+'].'
            if(elbow['enabled']):
                cnstrctdCmd += '<In>6Axis[E.'+str(elbow['angle'])+'].'
            if(wristElavate['enabled']):
                cnstrctdCmd += '<In>6Axis[WE.'+str(wristElavate['angle'])+'].'
            if(wristRotate['enabled']):
                cnstrctdCmd += '<In>6Axis[WR.'+str(wristRotate['angle'])+'].'
            if(claw['enabled']):
                cnstrctdCmd += '<In>6Axis[C.'+str(claw['angle'])+'].'

            print('constructedCmd: ' + cnstrctdCmd)
            fhnd.DoFunctionNow(cnstrctdCmd)
            return cnstrctdCmd
        else:
            return 'No motors selected'

    return ''

# Runtime


def execv(command, path):
    if(len(path) > 0):
        command = '%s%s' % (path, command)
    result = subprocess.run(['/bin/bash', '-c', command],
                            stdout=subprocess.PIPE, encoding='UTF-8')
    print(result.stdout)


app.config.update(PERMANENT_SESSION_LIFETIME=600)

if __name__ == '__main__':
    # shellESpeak("Web server starting")
    execv('start.sh', '/home/pi/Desktop/RPi_Cam_Web_Interface/')
    execv('killWebServer.sh 2223',
          '/home/pi/Desktop/RoboClubStore/MultiPurposePlatform/web/')
    app.config.update(
        SESSION_COOKIE_SECURE=True,
        SESSION_COOKIE_HTTPONLY=True,
        SESSION_COOKIE_SAMESITE='Lax',
    )
    app.run(debug=True, port=2223, host='0.0.0.0')
