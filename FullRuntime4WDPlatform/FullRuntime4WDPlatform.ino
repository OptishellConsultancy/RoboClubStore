#include "SensorDataUpdates.h"


//-----------------------------------
void setup() {
   Serial.begin(9600);   
   I2CMux.begin(Wire); 
   I2CMux.closeAll();  
   I2CMux.openAll();
   pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
   pinMode(echoPin, INPUT); // Sets the echoPin as an Input
   Serial.begin(9600); // Starts the serial communication
   //
   pwm.begin();
   pwm.setPWMFreq(FREQUENCY);  // Analog servos run at ~60 Hz updates
   //
   Serial.print("Please provide a feature number, e.g Feature2.\r\n");
}



void loop() {
  delay(1000);
  ReadSerialCommForCommmandAndExecute();
  if(GPSEnabled) {
    UpdateGPSData(DebugEnabled);
  }
   if(LSM303D_CompassAccelMagnetoEnabled) {
    UpdateCompassAccelMagnetoData(DebugEnabled);
  }
  if(ServoControllerEnabled){
    UpdateServoCommands(DebugEnabled);
    if(ServoTestEnabled){
      Serial.print("Executing serial test..");
      int servoIndexes[] = {0,1};
      ServoTest(servoIndexes);
  }
  }

  if(UltrasonicEnabled)  {
    UpdateUltrasonicIC2Data(DebugEnabled);
  }

  if(FourWDHatEnabled){
    Update4WDShieldCommands(DebugEnabled);
  }


}
