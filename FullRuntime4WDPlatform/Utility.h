#include "VarsInit.h"
//Util-----------------------------------
void DiscoverHubPortDevices()
{
  uint8_t error, i2cAddress, devCount, unCount;

  Serial.println("DiscoverHubPortDevices() -> Scanning...");

  devCount = 0;
  unCount = 0;
  for (i2cAddress = 1; i2cAddress < 127; i2cAddress++)
  {
    Wire.beginTransmission(i2cAddress);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at 0x");
      if (i2cAddress < 16)
        Serial.print("0");
      Serial.println(i2cAddress, HEX);
      devCount++;
    }
    else if (error == 4)
    {
      Serial.print("Unknow error at 0x");
      if (i2cAddress < 16)
        Serial.print("0");
      Serial.println(i2cAddress, HEX);
      unCount++;
    }
  }

  if (devCount + unCount == 0)
    Serial.println("No I2C devices found\n");
  else
  {
    Serial.print(devCount);
    Serial.print(" device(s) found");
    if (unCount > 0)
    {
      Serial.print(", and unknown error in ");
      Serial.print(unCount);
      Serial.print(" address");
    }
    Serial.println();
  }
  Serial.println();
}
void ReadCommand()
{
  while (Serial.available())
  {
    delay(3);
    if (Serial.available() > 0)
    {
      char c = Serial.read();
      ReadString += c;
    }
    if (ReadString.length() > 0)
    {
      commandEntered = true;
    }
  }
}

void PrintfOneVar(int len, char *completeStr, int var)
{
  char buffer[len];
  int n = sprintf(buffer, completeStr, var);
  Serial.print(buffer);
  Serial.print("\r\n");
  return;
}

void PrintFeaturePlistInfo()
{
  Serial.print("Please provide a port number, e.g Feature0.\r\n");
  Serial.print("Feature0. : GPS.\r\n");
  Serial.print("Feature1. : Compass + Accel + Magneto.\r\n");
  Serial.print("Feature2. : ServoControllerNumber.\r\n");
  Serial.print("Feature3. : OLEDSCreen .\r\n");
  Serial.print("Feature4. : 4WD Platform control.\r\n");
  Serial.print("Feature5. : IC2 Ultrasonic enabled.\r\n");
}

//-----------------------------------
bool PassCommandChk(String str, int &portToOpen)
{

  int idxOpenPort = str.indexOf("Feature");
  int idxDot = str.indexOf(".");
  int strLen = str.length();
  if (idxOpenPort >= 0 && idxDot >= 0)
  {
    String numberChars = str.substring(7, idxDot);
    portToOpen = numberChars.toInt();
    PrintfOneVar(100, "Enabling Feature: %d.", portToOpen);
    return true;
  }
  else
  {
    PrintFeaturePlistInfo();
    return false;
  }
  commandEntered = false;
}

void EnableFeatureMap(int featureToEnable)
{
  //Inline def to save program memory:
  int GPSEnabledFeatureNumber = 0;
  int LSM303D_CompassAccelMagnetoEnabledNumber = 1;
  int ServoControllerNumber = 2;
  int OLEDDisplayNumber = 3;
  int FourWDriveControlNumber = 4;
  int UltrasonicNumber = 5;
  //
  GPSEnabled = (featureToEnable == GPSEnabledFeatureNumber);
  LSM303D_CompassAccelMagnetoEnabled = (featureToEnable == LSM303D_CompassAccelMagnetoEnabledNumber);
  ServoControllerEnabled = (featureToEnable == ServoControllerNumber);
  OLEDDisplayEnabled = (featureToEnable == OLEDDisplayNumber);
  FourWDHatEnabled = (featureToEnable == FourWDriveControlNumber);
  UltrasonicEnabled = (featureToEnable == UltrasonicNumber);
  //
  if (GPSEnabled)
  {
    Serial.print("GPSEnabled\r\n");
  }
  if (LSM303D_CompassAccelMagnetoEnabled)
  {
    Serial.print("LSM303D_CompassAccelMagnetoEnabled Enabled\r\n");
  }
  if (ServoControllerEnabled)
  {
    Serial.print("ServoControllerEnabled  Enabled\r\n");
  }
  if (OLEDDisplayEnabled)
  {
    Serial.print("OLEDDisplayEnabled  Enabled\r\n");
  }

  if (FourWDHatEnabled)
  {
    Serial.print("FourWDHatEnabled  Enabled\r\n");
  }

  if (UltrasonicEnabled)
  {
    Serial.print("UltrasonicEnabled  Enabled\r\n");
  }
}
//----------------------------------------
void ReadSerialCommForCommmandAndExecute()
{
  ReadCommand();
  if (commandEntered)
  {
    Serial.print("ReadSerialCommForCommmandAndExecute() -> Command entered");
    commandEntered = false;
    int portToOpen = -1;
    if (PassCommandChk(ReadString, portToOpen))
    {
      ReadString = "";
      EnableFeatureMap(portToOpen);
      DiscoverHubPortDevices();
    }
  }
}

//Used by for example the ic2 ultrasonic 
void i2cWriteBytes(unsigned char addr_t, unsigned char Reg, unsigned char *pdata, unsigned char datalen)
{
  Wire.beginTransmission(addr_t); // transmit to device #8
  Wire.write(Reg);                // sends one byte

  for (uint8_t i = 0; i < datalen; i++)
  {
    Wire.write(*pdata);
    pdata++;
  }

  Wire.endTransmission(); // stop transmitting
}

void i2cReadBytes(unsigned char addr_t, unsigned char Reg, unsigned char Num)
{
  unsigned char i = 0;
  Wire.beginTransmission(addr_t); // transmit to device #8
  Wire.write(Reg);                // sends one byte
  Wire.endTransmission();         // stop transmitting
  Wire.requestFrom(addr_t, Num);
  while (Wire.available()) // slave may send less than requested
  {
    rxbuf[i] = Wire.read();
    i++;
  }
}
