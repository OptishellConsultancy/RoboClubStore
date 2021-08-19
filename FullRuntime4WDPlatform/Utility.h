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

void PrintFPlistInfo()
{
  Serial.print("\r\n");
  Serial.print("Please provide a port number, e.g F0.\r\n");
  Serial.print("F0. : GPS.\r\n");
  Serial.print("F1. : Compass + Accel + Magneto.\r\n");
  Serial.print("F2. : ServoArmControllerEnabled.\r\n");
  Serial.print("F3. : OLEDSCreen .\r\n");
  Serial.print("F4. : 4WD Platform control.\r\n");
  Serial.print("F5. : IC2 Ultrasonic enabled.\r\n");
  Serial.print("F6. : Disable all tests.\r\n");
  Serial.print("F7. : Enable NonHuman Readable APIIO.\r\n");
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
      PrintFPlistInfo();
    }
  }
}

void ReadAPICommand()
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
      APICommandEntered = true;
    }
    if (ReadString.indexOf("\r\n") >= 0 | ReadString.length() > 512)
    {
      ReadString = "";
    }
  }
}

//-----------------------------------
bool PassCommandChk(String str, int &cmdToActivate)
{
  int idxOpenPort = str.indexOf(FStr);
  int idxDot = str.indexOf(".");
  int strLen = str.length();
  if (idxOpenPort >= 0 && idxDot >= 0)
  {
    String numberChars = str.substring(FStr.length(), idxDot);
    cmdToActivate = numberChars.toInt();
    PrintfOneVar(100, "Enabling F: %d.", cmdToActivate);
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
      PrintFPlistInfo();
    }
    return false;
  }
  commandEntered = false;
}

bool ParseAndExecuteAPICommand(String str)
{
  //
  idxCmdIn = str.indexOf("<In>");
  idxCmdOut = str.indexOf("<Out>");
  if (idxCmdIn >= 0)
  {
    idx4WD = str.indexOf("4WD");   //<In>4W //4WheelDrive Data
    idx6A = str.indexOf("6A");     //<In>6A / Axis data
    idxScrn = str.indexOf("SCRN"); //<In>SCRN //Screen data
    if (idx4WD >= 0)
    {
      idx4WD_FLA = false;
      idx4WD_FRA = false;
      idx4WD_BLA = false;
      idx4WD_BRA = false;
      idx4WD_FLB = false;
      idx4WD_FRB = false;
      idx4WD_BLB = false;
      idx4WD_BRB = false;
      idx4WD_SAD = false;
      //--
      Serial.print("idx4WD RECV!..\r\n");
      idx4WD_FLA = (str.indexOf("FLA") >= 0); //Front Left Advance
      idx4WD_FRA = (str.indexOf("FRA") >= 0); //Front Right Advance
      idx4WD_BLA = (str.indexOf("BLA") >= 0); //Back Left Advance
      idx4WD_BRA = (str.indexOf("BRA") >= 0); //Back Right Advance
      idx4WD_FLB = (str.indexOf("FLB") >= 0); //Front Left Back
      idx4WD_FRB = (str.indexOf("FRB") >= 0); //Front Right Back
      idx4WD_BLB = (str.indexOf("BLB") >= 0); //Back Left Back
      idx4WD_BRB = (str.indexOf("BRB") >= 0); //Back Right Back
      idx4WD_SpeedS = str.indexOf("[");       //Duration Parse start
      idx4WD_SpeedE = str.indexOf("]");       //Duration End End
      idx4WD_DurS = str.indexOf("{");         //Duration Parse start
      idx4WD_DurE = str.indexOf("}");         //Duration Parse End
      idx4WD_SAD = (str.indexOf("Y") >= 0);   //Duration End End
      Speed4WD = str.substring(idx4WD_SpeedS + 1, idx4WD_SpeedE).toInt();
      Dur4WD = str.substring(idx4WD_DurS + 1, idx4WD_DurE).toInt();
      CmdRcv4WD = (Speed4WD >= 0);
      //E.g. F7. 100 SPEED, FOR 100MS Front left Forward
      Serial.print("Speed4WD: ");
      Serial.print(Speed4WD);
      Serial.print("\n");
      Serial.print("Dur4WD: ");
      Serial.print(Dur4WD);
      Serial.print("\n");
      if (idx4WD_SAD)
      {
        Serial.print("Stopping after cmd.\r\n");
      }
      return true;
    }
    if (idx6A > 0)
    {
    }
    if (idxScrn > 0)
    {
    }
  }
  return false;
}

void EnableFMap(int FToEnable)
{
  //Inline def to save program memory:
  int GPSEnabledFNumber = 0;
  int LSM303D_CompassAccelMagnetoEnabledNumber = 1;
  int ServoControllerNumber = 2;
  int OLEDDisplayNumber = 3;
  int FourWDriveControlNumber = 4;
  int UltrasonicNumber = 5;
  int StopAllTestsNumber = 6;
  int NonHumanReadableAPIIO = 7;
  //
  GPSEnabled = (FToEnable == GPSEnabledFNumber);
  LSM303D_CompassAccelMagnetoEnabled = (FToEnable == LSM303D_CompassAccelMagnetoEnabledNumber);
  ServoArmControllerEnabled = (FToEnable == ServoControllerNumber);
  OLEDDisplayEnabled = (FToEnable == OLEDDisplayNumber);
  FourWDHatEnabled = (FToEnable == FourWDriveControlNumber);
  UltrasonicEnabled = (FToEnable == UltrasonicNumber);
  //
  NonHumanReadableAPIIOEnabled = (FToEnable == NonHumanReadableAPIIO);
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
    NonHumanReadableAPIIOEnabled = false;
    PrintFPlistInfo();
  }
  if (NonHumanReadableAPIIOEnabled)
  {
    Serial.print("NonHumanReadableAPIIOEnabled  Enabled\r\n");
  }
}
//----------------------------------------
void ReadSerialFeatureCommForCommmandAndExecute()
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
      EnableFMap(portToOpen);
      DiscoverHubPortDevices();
    }
  }
}

void ReadAPICommAndExecute()
{
  ReadAPICommand();
  if (APICommandEntered)
  {
    ParseAndExecuteAPICommand(ReadString);
    APICommandEntered = false;
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
