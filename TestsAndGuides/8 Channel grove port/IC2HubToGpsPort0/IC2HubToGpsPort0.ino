#include <stdio.h>
#include <Arduino.h>
#include "TCA9548A.h"
#include <Adafruit_GPS.h>
//----
String readString;
TCA9548A I2CMux;  
//
bool commandEntered = false;
int idxOpenPort = 0;
int idxDot = 0;
int portToOpen;

//PortMappings:
int GPSPort = 0;
int LSM303D_CompassAccelMagnetoPort = 1;
int ServoControllerPort = 2;
bool GPSPortOpened;
bool LSM303D_CompassAccelMagnetoPortOpened;
bool ServoControllerPortOpened;
//

//-----------------------------------
void setup() {
   Serial.begin(9600);   
   I2CMux.begin(Wire); 
   I2CMux.closeAll();  

   Serial.print("Please provide a port number, e.g OpenPort0.\r\n");
}
//Util-----------------------------------
void DiscoverHubPortDevices() {
  uint8_t error, i2cAddress, devCount, unCount;
 
  Serial.println("Scanning...");
 
  devCount = 0;
  unCount = 0;
  for(i2cAddress = 1; i2cAddress < 127; i2cAddress++ )
  {
    Wire.beginTransmission(i2cAddress);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at 0x");
      if (i2cAddress<16) Serial.print("0");
      Serial.println(i2cAddress,HEX);
      devCount++;
    }
    else if (error==4)
    {
      Serial.print("Unknow error at 0x");
      if (i2cAddress<16) Serial.print("0");
      Serial.println(i2cAddress,HEX);
      unCount++;
    }    
  }
 
  if (devCount + unCount == 0)
    Serial.println("No I2C devices found\n");
  else {
    Serial.print(devCount);
    Serial.print(" device(s) found");
    if (unCount > 0) {
      Serial.print(", and unknown error in ");
      Serial.print(unCount);
      Serial.print(" address");
    }
    Serial.println();
  }
  Serial.println();
  delay(3000);   
}
void ReadCommand(){
  while (Serial.available())   {
    delay(3);  
    if (Serial.available() >0) {
      char c = Serial.read();  
      readString += c; 
    }
    if (readString.length() >0) {      
      commandEntered = true;   
    }
  }
}

void PrintfOneVar(int len, char* completeStr, int var) {
  char buffer [len];
  int n = sprintf (buffer, completeStr, var);
  Serial.print(buffer );
  return;
}
//-----------------------------------
bool PassCommandChk_OpenChannel(String str){

  idxOpenPort = str.indexOf("OpenPort");
  idxDot = str.indexOf(".");
  int strLen = str.length();
  if(idxOpenPort >= 0 && idxDot >=0){  
    String numberChars = str.substring(8,idxDot);
    portToOpen = numberChars.toInt();//(str[8]-'0');
    PrintfOneVar(100,"Opening IC2 Hub port: %d.\r\n",portToOpen);  
    return true;
  }
  else {
    Serial.print("Please provide a port number, e.g OpenPort0.\r\n");
    return false;
  } 
   commandEntered = false;
}

//This is redunant as all port are now always open
void OpenPortMap(int portToOpen){
  switch(portToOpen)
  {
    case(0):
    {
      I2CMux.openChannel(TCA_CHANNEL_0);
      break;
    }
    case(1):
    {
      I2CMux.openChannel(TCA_CHANNEL_1);
      break;
    }
    case(2):
    {
      I2CMux.openChannel(TCA_CHANNEL_2);
      break;
    }
    case(3):
    {
      I2CMux.openChannel(TCA_CHANNEL_3);
      break;
    }
    case(4):
    {
      I2CMux.openChannel(TCA_CHANNEL_4);
      break;
    }
    case(5):
    {
      I2CMux.openChannel(TCA_CHANNEL_5);
      break;
    }
    case(6):
    {
      I2CMux.openChannel(TCA_CHANNEL_6);
      break;
    }
    case(7):
    {
      I2CMux.openChannel(TCA_CHANNEL_7);
      break;
    }
  }
}

void EnablePortSensorFMap(int portToOpen)
{
  GPSPortOpened = (portToOpen == GPSPort);
  LSM303D_CompassAccelMagnetoPortOpened = (portToOpen == LSM303D_CompassAccelMagnetoPort);
  ServoControllerPortOpened = (portToOpen == ServoControllerPort);
  //
  if(GPSPortOpened){
    Serial.print("GPS Port Opened\r\n");
  }
  if(LSM303D_CompassAccelMagnetoPortOpened){
    Serial.print("LSM303D_CompassAccelMagneto Port Opened\r\n");
  }
  if(ServoControllerPortOpened){
    Serial.print("ServoControllerPortOpened Port Opened\r\n");
  }
}

void loop() {
  ReadCommand();
  if(commandEntered)  { 
    commandEntered = false;
    if(PassCommandChk_OpenChannel(readString))
    {        
      readString = "";
      OpenPortMap(portToOpen);
      EnablePortSensorFMap(portToOpen);
      Serial.print("Channel opened..");
      DiscoverHubPortDevices();
    }
  }
}
