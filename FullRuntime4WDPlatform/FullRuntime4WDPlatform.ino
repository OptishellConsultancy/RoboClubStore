#include "SensorDataUpdateTests.h"
#include "OLEDDemoTest2.h"

//-----------------------------------
void setup()
{
  Serial.begin(9600);
  I2CMux.begin(Wire);
  I2CMux.closeAll();
  I2CMux.openAll();

  Serial.begin(9600); // Starts the serial communication
  //
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY); // Analog servos run at ~60 Hz updates

  SetupW4DPins();
  EnableArmServos();

  //
  //DiscoverHubPortDevices();
  Serial.print("\r\n");
  Serial.print("Please provide a port number, e.g F0.\r\n");
  Serial.print("F0. : GPS.\r\n");
  Serial.print("F1. : Compass + Accel + Magneto.\r\n");
  Serial.print("F2. : 6AxisServoController.\r\n");
  Serial.print("F3. : OLEDSCreen .\r\n");
  Serial.print("F4. : 4WD Platform control.\r\n");
  Serial.print("F5. : IC2 Ultrasonic enabled.\r\n");
  Serial.print("F6. : Disable all tests.\r\n");
  Serial.print("F7. : Enable NonHuman Readable APIIO.\r\n");
  Serial.print("\r\n");
}

void loop()
{
  delay(200);
  if (!NonHumanReadableAPIIOEnabled)
  {
    ReadSerialFeatureCommForCommmandAndExecute();
    if (GPSEnabled)
    {
      UpdateGPSData();
    }
    if (LSM303D_CompassAccelMagnetoEnabled)
    {
      LSM303D_UpdateCompassAccelMagnetoData();
    }
    if (ServoArmControllerEnabled)
    {
      EnableArmServos();
      ArmServosTest();
      ServoArmControllerEnabled = false;
    }
    if (OLEDDisplayEnabled)
    {
      OLEDTest_Emoji();
      //OLEDTest_FullDefault();
      OLEDDisplayEnabled = false;
    }

    if (FourWDHatEnabled)
    {
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
    if(ParseAndExecuteAPICommand(ReadString))
    {
      ReadString = "";
      //E.G. <In>4WD[100]{100}FLA
      //     <In>4WD[100]{100}FRA
      if (CmdRcv4WD)
      {
        Do4WDAPICommand();
        CmdRcv4WD = false;
      }
      if(Do6Axis)
      {
        //E.G <In>6Axis[C.5]; Claw to 60 degrees
        Do6AxisAPICommand();
        Do6Axis = false;
      }
    }
  }
}