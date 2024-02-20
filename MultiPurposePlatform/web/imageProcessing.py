

# https://www.raspberrypi.com/documentation/computers/camera_software.html#getting-started
# https://github.com/raspberrypi/picamera2/blob/main/examples/mjpeg_server.py
# https://github.com/RaspberryPi/picamera2

import cv2
import time
import picamera2 #camera module for RPi camera
from picamera2 import Picamera2, MappedArray
from picamera2.encoders import JpegEncoder, H264Encoder
from picamera2.outputs import FileOutput, FfmpegOutput
import os
import io
from threading import Condition
import libcamera
import base64
import urllib.request 
import numpy as np
from PIL import Image, ImageDraw, ImageFont


VIDEO_HEIGHT = 1080
VIDEO_WIDTH = 1920

FONT_DEFAULT = cv2.FONT_HERSHEY_SIMPLEX

def addTimeStamp(img, color, thickness):
    timestamp = time.strftime("%Y-%m-%d %X")
    cv2.putText(img, timestamp, (0, 30), FONT_DEFAULT, 1, color=color, thickness=thickness)

def drawBox(img,x,y,w,h,color,thickness):
    cv2.line(img, (x, y), (x+w, y), thickness=thickness,color=color) #bottom
    cv2.line(img, (x+w, y), (x+w, y+h), thickness=thickness,color=color) #right
    cv2.line(img, (x, y+h), (x+w, y+h), thickness=thickness,color=color) #top
    cv2.line(img, (x, y), (x, y+h),  thickness=thickness,color=color) #left


#https://core-electronics.com.au/guides/object-identify-raspberry-pi/
#https://stackoverflow.com/questions/68466862/how-to-customize-object-detection-using-cv2-dnn-detectionmodel-method
#https://github.com/ultralytics/yolov5/issues/4558
#https://github.com/zafarRehan/object_detection_COCO
dir_path = os.path.dirname(os.path.realpath(__file__))
classNames = []
classFile = os.path.join(dir_path, "../ObjectDetectionModels/coco.names")
with open(classFile,"rt") as f:
    classNames = f.read().rstrip("\n").split("\n")
    configPath = os.path.join(dir_path, "../ObjectDetectionModels/ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt")
    weightsPath = os.path.join(dir_path, "../ObjectDetectionModels/frozen_inference_graph.pb")
    
    net = cv2.dnn_DetectionModel(weightsPath,configPath)
    net.setInputSize(320,320)
    net.setInputScale(1.0/ 127.5)
    net.setInputMean((127.5, 127.5, 127.5))
    net.setInputSwapRB(True)

def execDNN(img, thres, nms, objects=[]):
    classIds, confs, bbox = net.detect(img, confThreshold=thres,nmsThreshold=nms)
    if len(objects) == 0: objects = classNames
    objectInfo =[]
    if len(classIds) != 0:
        for classId, confidence,box in zip(classIds.flatten(),confs.flatten(),bbox):
            className = classNames[classId - 1]
            if className in objects: 
                objectInfo.append([box,className])
                
                cv2.rectangle(img,box,color=(0,255,0),thickness=2)
                cv2.rectangle(img,box,color=(0,255,0),thickness=2)
                cv2.putText(img, classNames[classId-1].upper(), (box[0]+10,box[1]+30), FONT_DEFAULT,1,color=(0,255,0),thickness=2)
                cv2.putText(img, str(round(confidence*100,2)), (box[0]+200,box[1]+30), FONT_DEFAULT,1,color=(0,255,0),thickness=2)

                
            # return img, objectInfo
    else:
        cv2.putText(img, "No classifications found", (0, 40), FONT_DEFAULT,1,color=(0,255,0),thickness=2)

global doOCVDNNAnyls
doOCVDNNAnyls = False

def updateImgPrcing_DNNEnbld(enabled):
    global doOCVDNNAnyls
    doOCVDNNAnyls = enabled
    print("updateImgPrcing_DNNEnbld")

 
def opencvProcesses(request):
    with MappedArray(request, "main") as m:
        img = (m.array)
        
        addTimeStamp(img,(0, 255, 0),2)

        # drawBox(img,50,50,100,100,(0, 255, 0),2)

        global doOCVDNNAnyls 
        
        strInfo = "DNN Analysis disabled" 
        if(doOCVDNNAnyls is True):
            strInfo = "DNN Analysis enabled"
            #the 0.45 number is the threshold number, the 0.2 number is the nms number)
            execDNN(img, 0.4,0.2)
        
        cv2.putText(img, strInfo, (500, 30), FONT_DEFAULT, 1, color=(0,255,0), thickness=2)
     

class StreamingOutput(io.BufferedIOBase):
    def __init__(self):
        self.frame = None
        self.condition = Condition()

    def write(self, buf):
        with self.condition:
            self.frame = buf
            self.condition.notify_all()



def genFrames():
    try:   
        picam2 = picamera2.Picamera2()
        output = StreamingOutput()
        config = picam2.create_video_configuration(main={"size": (VIDEO_WIDTH, VIDEO_HEIGHT), 'format': 'BGR888'})
        config["transform"] = libcamera.Transform(hflip=1, vflip=1)
        picam2.configure(config)
        global camOutput       
        camOutput = output

        picam2.pre_callback = opencvProcesses
        picam2.start_recording(JpegEncoder(), FileOutput(camOutput))
       
    except:   
        try:   
            picam2.stop()
        except Exception as err:
            print(err)



def getFrames():
    global camOutput
    try:
        while True:
            with camOutput.condition:
                camOutput.condition.wait()
                frame = camOutput.frame          
            yield (b'--frame\r\n' b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
    except Exception as err:  
        print("getFrames exception!: {e} ".format(e = err))
        genFrames()