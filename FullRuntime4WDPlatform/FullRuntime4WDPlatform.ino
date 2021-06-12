#include "SensorDataUpdates.h"
#include "OLEDDemoTest2.h"
//-----------------------------------
void setup()
{
  Serial.begin(9600);
  I2CMux.begin(Wire);
  I2CMux.closeAll();
  I2CMux.openAll();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
  Serial.begin(9600);       // Starts the serial communication
  //
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY); // Analog servos run at ~60 Hz updates
  //
  DiscoverHubPortDevices();
  Serial.print("Please provide a feature number, e.g Feature2.\r\n");
  Serial.print("Feature0. : GPS.\r\n");
  Serial.print("Feature1. : Compass + Accel + Magneto.\r\n");
  Serial.print("Feature2. : ServoControllerNumber .\r\n");
  Serial.print("Feature3. : OLEDSCreen .\r\n");
  Serial.print("Feature4. : 4WD Platform control .\r\n");
}

void loop()
{
  delay(1000);
  ReadSerialCommForCommmandAndExecute();
  if (GPSEnabled)
  {
    UpdateGPSData();
  }
  if (LSM303D_CompassAccelMagnetoEnabled)
  {
    LSM303D_UpdateCompassAccelMagnetoData();
  }
  if (ServoControllerEnabled)
  {
    UpdateServoCommands();
    if (ServoTestEnabled)
    {
      Serial.print("Executing serial test..");
      int servoIndexes[] = {0, 1};
      ServoTest(servoIndexes);
    }
  }
  if (OLEDDisplayEnabled)
  {     
     SetupOLEDTest();
  }

  if (UltrasonicEnabled)
  {
    UpdateUltrasonicIC2Data();
  }

  if (FourWDHatEnabled)
  {
    Update4WDShieldCommands();
  }

  if (UltrasonicEnabled)
  {
    UpdateUltrasonicIC2Data();
  }
}