#include "VarsInit.h"
#include <stdio.h>
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

void PrintfOneStr(int len, char *completeStr, String var)
{
  char buffer[len];
  int n = sprintf(buffer, completeStr, var);
  Serial.print(buffer);
  Serial.print("\n");
  return;
}

void PrintfOneVar(int len, char *completeStr, int var, bool noNewLine = false)
{
  char buffer[len];
  int n = sprintf(buffer, completeStr, var);
  Serial.print(buffer);
  if (!noNewLine)
  {
    Serial.print("\n");
  }
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
  int idxF = str.indexOf("F");
  int strLen = str.length();

  if (idxOpenPort == 0 && idxDot >= 0 && idxF >= 0 && strLen >= 3)
  {
    String numberChars = str.substring(idxDot - 1, idxDot);
    cmdToActivate = numberChars.toInt();
    PrintfOneVar(100, "Enabling Function: %d.", cmdToActivate);
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

bool PassCmd4WD(String str)
{
  Do4WD_FLA = false;
  Do4WD_FRA = false;
  Do4WD_BLA = false;
  Do4WD_BRA = false;
  Do4WD_FLB = false;
  Do4WD_FRB = false;
  Do4WD_BLB = false;
  Do4WD_BRB = false;
  Do4WD_SAD = false;
  //--
  Serial.print("Do4WD RECV!..\r\n");
  Do4WD_FLA = (str.indexOf("FLA") >= 0); //Front Left Advance
  Do4WD_FRA = (str.indexOf("FRA") >= 0); //Front Right Advance
  Do4WD_BLA = (str.indexOf("BLA") >= 0); //Back Left Advance
  Do4WD_BRA = (str.indexOf("BRA") >= 0); //Back Right Advance
  Do4WD_FLB = (str.indexOf("FLB") >= 0); //Front Left Back
  Do4WD_FRB = (str.indexOf("FRB") >= 0); //Front Right Back
  Do4WD_BLB = (str.indexOf("BLB") >= 0); //Back Left Back
  Do4WD_BRB = (str.indexOf("BRB") >= 0); //Back Right Back
  Do4WD_SpeedS = str.indexOf("[");       //Duration Parse start
  Do4WD_SpeedE = str.indexOf("]");       //Duration End End
  Do4WD_DurS = str.indexOf("{");         //Duration Parse start
  Do4WD_DurE = str.indexOf("}");         //Duration Parse End
  Do4WD_SAD = (str.indexOf("Y") >= 0);   //Duration End End
  Speed4WD = str.substring(Do4WD_SpeedS + 1, Do4WD_SpeedE).toInt();
  Dur4WD = str.substring(Do4WD_DurS + 1, Do4WD_DurE).toInt();
  CmdRcv4WD = (Speed4WD >= 0);
  //E.g. F7. 100 SPEED, FOR 100MS Front left Forward
  PrintfOneVar(100, "Speed4WD:", Speed4WD);
  PrintfOneVar(100, "Dur4WD:", Dur4WD);
  return true;
}

bool PassCmd6aAxis(String str)
{
  Serial.print("Do6Axis RECV!..\r\n ");
  //Strip '<In>6Axis' cmd
  int str_len = str.length();
  str = str.substring(9, str_len + 1);
  str_len = str.length();
  //Convert to charArray
  char strChar[str_len];
  str.toCharArray(strChar, str_len);

  char *pch;
  pch = strtok(strChar, ";");
  String pchStr(pch);
  String angleSS, jointName;
  int angle;
  while (pch != NULL)
  {
    int endBracket = pchStr.indexOf("]");
    angleSS = pchStr.substring(pchStr.indexOf(".") + 1, endBracket);
    angle = angleSS.toInt();
    jointName = pchStr.substring(pchStr.indexOf("[") + 1, pchStr.indexOf("."));

    pch = strtok(NULL, ";"); //If delim found, set to next
    pchStr = String(pch);

    angle = (angle < 0) ? -angle : angle;        //Invert if negative
    angle = (angle > 180) ? angle % 180 : angle; //Constrain to max 180

    char cstr[16];
    itoa(angle, cstr, 10);

    //PrintfOneStr(100,"jointName: %s",jointName);
    PrintfOneVar(100, "angle: %d", angle);

    Ang6Axis_Base = (jointName == "B") ? angle : -1;
    Ang6Axis_BaseTilt = (jointName == "BT") ? angle : -1;
    Ang6Axis_Elbow = (jointName == "E") ? angle : -1;
    Ang6Axis_WristElevate = (jointName == "WE") ? angle : -1;
    Ang6Axis_WristRotate = (jointName == "WR") ? angle : -1;
    Ang6Axis_Claw = (jointName == "C") ? angle : -1;
    Ang6Axis_HoldPos = !(pchStr.substring(endBracket + 1, endBracket + 2) == '-');
  }
}

void PrintGPS()
{
  if (GPSSetupRequired)
  {
    GPS = Adafruit_GPS(&Wire);
    GPS.begin(0x10); // The I2C address to use is 0x10
    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // uncomment this line to turn on only the "minimum recommended" data
    //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
    // the parser doesn't care about other sentences at this time
    // Set the update rate
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    // For the parsing code to work nicely and have time to sort thru the data, and
    // print it out we don't suggest using anything higher than 1 Hz
    //
    // Request updates on antenna status, comment out to keep quiet
    GPS.sendCommand(PGCMD_ANTENNA);

    delay(1000);

    // Ask for firmware version
    GPS.println(PMTK_Q_RELEASE);
    GPSSetupRequired = false;
  }

  Serial.print("<GPSTIME");
  GPS.hour < 10 ? PrintfOneVar(200, "0%d", GPS.hour, true) : PrintfOneVar(200, "%d", GPS.hour, true);
  Serial.print(':');
  GPS.minute < 10 ? PrintfOneVar(200, "0%d", GPS.minute, true) : PrintfOneVar(200, "%d", GPS.minute, true);
  Serial.print(':');
  GPS.seconds < 10 ? PrintfOneVar(200, "0%d", GPS.seconds, true) : PrintfOneVar(200, "%d", GPS.seconds, true);
  Serial.print('.');
  if (GPS.milliseconds < 10)
  {
    Serial.print("00");
  }
  else if (GPS.milliseconds > 9 && GPS.milliseconds < 100)
  {
    Serial.print("0");
  }
  Serial.println(GPS.milliseconds);
  Serial.print("GPSTIME>");
  Serial.print("<GPSDate");
  PrintfOneVar(100, "D: %d/", GPS.day);
  PrintfOneVar(100, "M: %d/", GPS.month);
  PrintfOneVar(100, "Y: %d/", GPS.year);
  PrintfOneVar(100, "Fix: %d/", (int)GPS.fix);
  PrintfOneVar(100, "Quality: %d/", (int)GPS.fixquality);
  Serial.print("GPSDate>");
  //
  Serial.print("<GPSLoc");
  if (GPS.fix)
  {
    Serial.print("Location: ");
    Serial.print(GPS.latitude, 4);
    Serial.print(GPS.lat);
    Serial.print(", ");
    Serial.print(GPS.longitude, 4);
    Serial.println(GPS.lon);
    PrintfOneVar(100, "SpeedKnots: %d,", GPS.speed);
    PrintfOneVar(100, "Angle: %d,", GPS.angle);
    PrintfOneVar(100, "Altitude: %d,", GPS.altitude);
    PrintfOneVar(100, "SatCount: %d,", GPS.satellites);
  }
  else
  {
    Serial.print("FIXNOTAQUIRED");
  }
  Serial.print("GPSLoc>");
}

bool ParseAndExecuteAPICommand(String str)
{
  //
  CmdIn = (str.indexOf("<In>") >= 0);
  CmdOut = (str.indexOf("<Out>") >= 0);
  RetToMainMenu = (str.indexOf("RM") >= 0);
  if (RetToMainMenu)
  {
    NonHumanReadableAPIIOEnabled = false;
    RetToMainMenu = false;
    PrintFPlistInfo();
  }
  if (CmdIn)
  {
    Do4WD = (str.indexOf("4WD") >= 0);     //<In>4W //4WheelDrive Data
    Do6Axis = (str.indexOf("6Axis") >= 0); //<In>6A / Axis data
    DoScrn = (str.indexOf("SCRN") >= 0);   //<In>SCRN //Screen data
    if (Do4WD)
    {
      return PassCmd4WD(str);
    }
    if (Do6Axis)
    {
      return PassCmd6aAxis(str);
    }
    if (DoScrn)
    {
      return true;
    }
    return true;
  }
  if (CmdOut)
  {
    DoGPS = (str.indexOf("GPS") >= 0); //<In>SCRN //Screen data
    if (DoGPS)
    {
      PrintGPS();
    }
    if (DoAccMag)
    {
    }
    if (DoUltraSnc)
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
