from flask import Flask, render_template, make_response, redirect, Response, request
import numpy
#import numpy.core.multiarray
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

if sys.version_info[0] < 3:
    raise Exception("Python 3 or a more recent version is required.")

functionHandler = FunctionHandler()
app = Flask(__name__)
#vc = cv2.VideoCapture(0)

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


@app.route("/ExecuteFunctions/", methods=['POST'])
def ExecuteFunctions():
    shellESpeak("Executing functions")
    return redirect("/")

@app.route("/AddToFunctionList/", methods=['POST'])
def AddToFunctionList():
    functionName = request.args.get('functionName')  
    shellESpeak("Added function: "+ functionName)
    functionHandler.AddToFunctionList(functionName, ["Some function data"]) 
    return redirect("/") 

def execv(command, path):
    if(len(path) > 0):
        command = '%s%s' % (path, command)
    result = subprocess.run(['/bin/bash', '-c', command],  stdout=subprocess.PIPE, encoding='UTF-8')
    print(result.stdout)


app.config.update(
    PERMANENT_SESSION_LIFETIME=600
)

if __name__ == '__main__':
    #shellESpeak("Web server starting")
    execv('start.sh', '/home/pi/Desktop/RPi_Cam_Web_Interface/')
    execv('killWebServer.sh 2223', '/home/pi/Desktop/RoboClubStore/web/')
    app.config.update(
        SESSION_COOKIE_SECURE=True,
        SESSION_COOKIE_HTTPONLY=True,
        SESSION_COOKIE_SAMESITE='Lax',
    )
    app.run(debug=True, port=2223, host='0.0.0.0')
