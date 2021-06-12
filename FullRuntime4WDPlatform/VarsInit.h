
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

#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50
// our servo # counter
uint8_t servonum = 0;

//Internal controls
bool commandEntered = false;

//PortMappings:

bool GPSSetupRequired = true;
bool LSM303D_CompassAccelMagnetoInitRequired = true;
bool OLEDDisplaySetupRequired = true;
//
bool GPSEnabled;
bool LSM303D_CompassAccelMagnetoEnabled;
bool ServoControllerEnabled;
bool OLEDDisplayEnabled;
bool FourWDHatEnabled;
bool UltrasonicEnabled;
//

//

const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;
//4WD commands/Port maps


bool ServoTestEnabled = true;
bool PWMInitialised = false;
