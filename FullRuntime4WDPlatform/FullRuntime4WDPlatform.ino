#include "SensorDataUpdates.h"
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
  //
  DiscoverHubPortDevices();
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

void loop()
{
  delay(200);
  ReadSerialCommForCommmandAndExecute();
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
  }
  if (OLEDDisplayEnabled)
  {
    OLEDTest();
  }

  if (UltrasonicEnabled)
  {
    UpdateUltrasonicIC2Data();
  }

  if (FourWDHatEnabled)
  {
    Serial.print("Update4WDShieldCommands()");
    Update4WDShieldCommands();
  }

  if (UltrasonicEnabled)
  {
    UpdateUltrasonicIC2Data();
  }
}