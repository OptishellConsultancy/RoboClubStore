
#include <stdio.h>
#include <Arduino.h>
#include "TCA9548A.h"
#include <Adafruit_GPS.h>
#include <LSM303.h>
//----
String ReadString;
TCA9548A I2CMux;  
LSM303 compass;
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false
Adafruit_GPS GPS;
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//REMEMBER TO HAVE BATTERIES CHARGED!
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm;

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50
// our servo # counter
uint8_t servonum = 0;

bool DebugEnabled = true;
//Internal controls
bool commandEntered = false;
int idxOpenPort = 0;
int idxDot = 0;
int portToOpen;



char LSM30D3ReportBuffer[80];

//PortMappings:

bool GPSEnabled;
bool GPSSetupRequired = true;
bool LSM303D_CompassAccelMagnetoEnabled;
bool LSM303D_CompassAccelMagnetoInitRequired = true;
bool ServoControllerEnabled;
int GPSEnabledFeatureNumber = 0;
int LSM303D_CompassAccelMagnetoEnabledNumber = 1;
int ServoControllerNumber = 2;


//Ultrasonic vars
// defines pins numbers
bool UltrasonicEnabled = false;
const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;
//4WD commands/Port maps
bool FourWDHatEnabled = false;
const int E1 = 3; ///<Motor1 Speed
const int E2 = 11;///<Motor2 Speed
const int E3 = 5; ///<Motor3 Speed
const int E4 = 6; ///<Motor4 Speed

const int M1 = 4; ///<Motor1 Direction
const int M2 = 12;///<Motor2 Direction
const int M3 = 8; ///<Motor3 Direction
const int M4 = 7; ///<Motor4 Direction

bool ServoTestEnabled = true;
bool PWMInitialised = false;
