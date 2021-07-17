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

void PrintfOneVar(int len, char *completeStr, int var)
{
  char buffer[len];
  int n = sprintf(buffer, completeStr, var);
  Serial.print(buffer);
  Serial.print("\r\n");
  return;
}

void PrintFeatureTestPlistInfo()
{
  Serial.print("\r\n");
  Serial.print("Please provide a port number, e.g FeatureTest0.\r\n");
  Serial.print("FeatureTest0. : GPS.\r\n");
  Serial.print("FeatureTest1. : Compass + Accel + Magneto.\r\n");
  Serial.print("FeatureTest2. : ServoControllerNumber.\r\n");
  Serial.print("FeatureTest3. : OLEDSCreen .\r\n");
  Serial.print("FeatureTest4. : 4WD Platform control.\r\n");
  Serial.print("FeatureTest5. : IC2 Ultrasonic enabled.\r\n");
  Serial.print("FeatureTest6. : Disable all tests.\r\n");
  Serial.print("\r\n");
}

void ReadCommand()
{
  while (Serial.available())
  {
    if (Serial.available() > 0)
    {
      char c = Serial.read();
      ReadString += c;
    }
    if (ReadString.length() > 0)
    {
      commandEntered = true;
    }
    else
    {
      PrintFeatureTestPlistInfo();
    }
  }
}

//-----------------------------------
bool PassCommandChk(String str, int &cmdToActivate)
{

  int idxOpenPort = str.indexOf(featureTestStr);
  int idxDot = str.indexOf(".");
  int strLen = str.length();
  if (idxOpenPort >= 0 && idxDot >= 0)
  {
    String numberChars = str.substring(featureTestStr.length(), idxDot);
    cmdToActivate = numberChars.toInt();
    PrintfOneVar(100, "Enabling FeatureTest: %d.", cmdToActivate);
    return true;
  }
  else
  {
    if (idxDot >= 0)
    {
      Serial.print("Emergency stop detected..\r\n");
      StopAllTestsRequested = true;
      return true;
    }
    else
    {
      PrintFeatureTestPlistInfo();
    }
    return false;
  }
  commandEntered = false;
}

void EnableFeatureTestMap(int FeatureTestToEnable)
{
  //Inline def to save program memory:
  int GPSEnabledFeatureTestNumber = 0;
  int LSM303D_CompassAccelMagnetoEnabledNumber = 1;
  int ServoControllerNumber = 2;
  int OLEDDisplayNumber = 3;
  int FourWDriveControlNumber = 4;
  int UltrasonicNumber = 5;
  int StopAllTestsNumber = 6;
  //
  GPSEnabled = (FeatureTestToEnable == GPSEnabledFeatureTestNumber);
  LSM303D_CompassAccelMagnetoEnabled = (FeatureTestToEnable == LSM303D_CompassAccelMagnetoEnabledNumber);
  ServoArmControllerEnabled = (FeatureTestToEnable == ServoControllerNumber);
  OLEDDisplayEnabled = (FeatureTestToEnable == OLEDDisplayNumber);
  FourWDHatEnabled = (FeatureTestToEnable == FourWDriveControlNumber);
  UltrasonicEnabled = (FeatureTestToEnable == UltrasonicNumber);
  //
  if (GPSEnabled)
  {
    Serial.print("GPSEnabled\r\n");
  }
  if (LSM303D_CompassAccelMagnetoEnabled)
  {
    Serial.print("LSM303D_CompassAccelMagnetoEnabled Enabled\r\n");
  }
  if (ServoArmControllerEnabled)
  {
    Serial.print("ServoArmControllerEnabled  Enabled\r\n");
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
  if (StopAllTestsRequested)
  {
    Serial.print("Disabling All tests..\r\n");
    GPSEnabled = false;
    LSM303D_CompassAccelMagnetoEnabled = false;
    ServoArmControllerEnabled = false;
    OLEDDisplayEnabled = false;
    UltrasonicEnabled = false;
    StopAllTestsRequested = false;
    PrintFeatureTestPlistInfo();
  }
}
//----------------------------------------
void ReadSerialCommForCommmandAndExecute()
{
  ReadCommand();
  if (commandEntered)
  {
    Serial.print("ReadSerialCommForCommmandAndExecute() -> Command entered\r\n");
    commandEntered = false;
    int portToOpen = -1;
    if (PassCommandChk(ReadString, portToOpen))
    {
      ReadString = "";
      EnableFeatureTestMap(portToOpen);
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
