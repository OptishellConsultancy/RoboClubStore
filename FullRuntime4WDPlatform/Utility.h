#include "VarsInit.h"
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
}
void ReadCommand(){
  while (Serial.available())   {
    delay(3);  
    if (Serial.available() >0) {
      char c = Serial.read();  
      ReadString += c; 
    }
    if (ReadString.length() >0) {      
      commandEntered = true;   
    }
  }
}

void PrintfOneVar(int len, char* completeStr, int var) {
  char buffer [len];
  int n = sprintf (buffer, completeStr, var);
  Serial.print(buffer);
  Serial.print("\r\n");
  return;
}
//-----------------------------------
bool PassCommandChk(String str){

  idxOpenPort = str.indexOf("Feature");
  idxDot = str.indexOf(".");
  int strLen = str.length();
  if(idxOpenPort >= 0 && idxDot >=0){  
    String numberChars = str.substring(7,idxDot);
    portToOpen = numberChars.toInt();
    PrintfOneVar(100,"Enabling Feature: %d.",portToOpen);  
    return true;
  }
  else {
    Serial.print("Please provide a port number, e.g Feature0.\r\n");
    return false;
  } 
   commandEntered = false;
}


void EnableFeatureMap(int featureToEnable)
{
  GPSEnabled = (featureToEnable == GPSEnabledFeatureNumber);
  LSM303D_CompassAccelMagnetoEnabled = (featureToEnable == LSM303D_CompassAccelMagnetoEnabledNumber);
  ServoControllerEnabled = (featureToEnable == ServoControllerNumber);
  //
  if(GPSEnabled){
    Serial.print("GPSEnabled\r\n");
  }
  if(LSM303D_CompassAccelMagnetoEnabled){
    Serial.print("LSM303D_CompassAccelMagnetoEnabled Enabled\r\n");
  }
  if(ServoControllerEnabled){
    Serial.print("ServoControllerEnabled  Enabled\r\n");
  }
}
//----------------------------------------
void ReadSerialCommForCommmandAndExecute()
{
  ReadCommand();
  if(commandEntered)  { 
    commandEntered = false;
    if(PassCommandChk(ReadString))
    {        
      ReadString = "";
      EnableFeatureMap(portToOpen);
      DiscoverHubPortDevices();
    }
  }
}
