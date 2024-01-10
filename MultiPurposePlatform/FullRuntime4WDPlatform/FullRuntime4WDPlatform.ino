#include "OLEDCommands.h"

//-----------------------------------
void setup()
{
  Serial.begin(9600);
  I2CMux.begin(Wire);
  I2CMux.closeAll();
  I2CMux.openAll();

  //
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY); // Analog servos run at ~60 Hz updates

  DiscoverHubPortDevices();
  SetupGPS();
  
  PrintFPlistInfo();
}

void loop()
{
  GPSReadAndResultCache(); //Do GPS read continuously, later on we print data, this prevents an error wwhere we dsync requests and get empty data - annoying
  delay(200);

  // TESTS
  if (!NonHumanReadableAPIIOEnabled)
  {
    ReadSerialFeatureCommForCommmandAndExecute();
    if (GPSEnabled)
    {
      UpdateGPSDataTest();
      GPSEnabled = false;
    }
    if (LSM303D_CompassAccelMagnetoEnabled)
    {
      LSM303D_CompassAccelMagnetoEnabled = false;
      LSM303D_UpdateCompassAccelMagnetoData();
    }
    if (ServoArmControllerEnabled)
    {
      ServoArmControllerEnabled = false;
      EnableArmServos();
      ArmServosTest();
    }
    if (OLEDDisplayEnabled)
    {
      OLEDDisplayEnabled = false;
      //OLEDTest_Emoji();
      EnabledOLED();
      OLEDTest_FullDefault();
    }

    if (FourWDHatEnabled)
    {
      SetupW4DPins();
      Serial.print("Update4WDShieldCommands()");
      Test4WDCommands();
      FourWDHatEnabled = false;
    }

    if (UltrasonicEnabled)
    {
      UpdateUltrasonicIC2Data();
      UltrasonicEnabled = false;
    }
  }
  else
  {
    ReadAPICommand();
    if (ParseAndExecuteAPICommand(ReadString))
    {
      ReadString = "";
    }
    // E.G. <In>4WD[100]{100}FLA
    //      <In>4WD[100]{100}FRA
    if (CmdRcv4WD)
    {
      CmdRcv4WD = false;
      SetupW4DPins();
      Do4WDAPICommand();
    }
    if (Do6Axis)
    {
      Do6Axis = false;
      EnableArmServos();
      // E.G <In>6Axis[C.5]; Claw to 60 degrees
      Do6AxisAPICommand();
    }
    if (OLEDImg)
    {
      OLEDImg = false;
      EnabledOLED();
      DoOLEDImgCommand();
    }
    if (OLEDTxt)
    {
      OLEDTxt = false;
      EnabledOLED();
      DoOLEDTxtCommand();
    }
    if (DoGPS)
    {
      ProcessGPSData();
      GPSSampleCount--;
      DoGPS = !(GPSSampleCount <= 0);
    }
    if (DoAccMag)
    {
      DoAccMag = false; //Test override
      AccMagSampleCount--;
      DoAccMag = !(AccMagSampleCount <= 0);
      PrintDoAccMag();
    }
    if (DoUltraSnc)
    {
      UltraSoncSampleCount--;
      DoUltraSnc = !(UltraSoncSampleCount <= 0);
      PrintDoUltSonc();
    }

  }
}