
#include <stdio.h>
#include <Arduino.h>
#include "TCA9548A.h"
#include <Adafruit_GPS.h>
#include <LSM303.h>
//----
String ReadString;
TCA9548A I2CMux;
LSM303 lsm303D;
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false
Adafruit_GPS GPS;
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// REMEMBER TO HAVE BATTERIES CHARGED!
//  called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm;

#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50

#define SERVOMIN 125 //  minimum pulse length count (out of 4096)
#define SERVOMAX 600 // maximum pulse length count (out of 4096)
// our servo # counter
uint8_t servonum = 0;

// Internal controls
bool commandEntered = false;
bool APICommandEntered = false;

// PortMappings:

bool GPSSetupRequired = true;
bool LSM303D_CompassAccelMagnetoInitRequired = true;
bool OLEDDisplaySetupRequired = true;
bool UltraSonicSetupRequired = true;
//
bool GPSEnabled;
bool LSM303D_CompassAccelMagnetoEnabled;
bool OLEDDisplayEnabled;
bool FourWDHatEnabled;
bool UltrasonicEnabled;
bool ServoArmControllerEnabled;
bool PWMInitialised;
bool OLEDInitialised;
bool StopAllTestsRequested;
bool NonHumanReadableAPIIOEnabled;

// Ultrasonic vars:
unsigned char addr0 = 0x11; // The ic2 address of the Ultrasonic
unsigned char txbuf[10] = {0};
unsigned char rxbuf[10] = {0};

// ArmServoTestVars
int CurrentNumServoArmOfTests = 0;

// API parse ints:
bool CmdIn;
bool CmdOut;
bool Do4WD;
bool Do6Axis;
bool OLEDImg;
bool OLEDTxt;
bool DoGPS;
bool DoAccMag;
bool DoUltraSnc;
bool RetToMainMenu = false;
// 4WD
bool Do4WD_FLA;
bool Do4WD_FRA;
bool Do4WD_BLA;
bool Do4WD_BRA;
bool Do4WD_FLB;
bool Do4WD_FRB;
bool Do4WD_BLB;
bool Do4WD_BRB;
int Do4WD_SpeedS;
int Do4WD_SpeedE;
int Do4WD_DurS;
int Do4WD_DurE;
bool Do4WD_SAD;
int Speed4WD;
int Dur4WD;
bool CmdRcv4WD = false;
// 6Axis

enum MotorName
{
  Base,
  BaseTilt,
  Elbow,
  WristElevate,
  WristRotate,
  Claw
};

int Ang6Axis_Base;
int Ang6Axis_BaseTilt;
int Ang6Axis_Elbow;
int Ang6Axis_WristElevate;
int Ang6Axis_WristRotate;
int Ang6Axis_Claw;

int motorIndex[6] = {1, 2, 11, 12, 14, 15};

#define stepsPerPWM 10
#define delayPerPWN 50

// OLEDIMG
String OLEDIMG_BITMAPDATA;
String OLEDIMG_BITMAPCONFIG;

const int OLEDIMG_X_DEF = 42;
const int OLEDIMG_Y_DEF = 32;
int OLEDIMG_X, OLEDIMG_Y, OLEDIMG_S;
bool OLEDIMG_VALIDCONFIG = false;

// OLEDTXT
String OLEDTXT_TEXTDATA;
String OLEDTXT_TXTCONFIG;
const int OLEDTXT_X_DEF = 42;
const int OLEDTXT_Y_DEF = 32;
int OLEDTXT_X, OLEDTXT_Y, OLEDTXT_S;
bool OLEDTXT_VALIDCONFIG = false;

//
// GPS meta data
int GPSSampleCount = 1;
bool GPSPrintOLED = false;
String GPSData_DateTime;
String GPSData_LocLat;
String GPSData_LocLon;
String GPSData_Misc;
String GPSData_HasFixLiveData;
bool GPSHasData = false;

// AccMag meta data
int AccMagSampleCount = 1;
bool AccMagPrintOLED = false;
//
// Ultrasonic meta data
int UltraSoncSampleCount = 1;
bool UltraSoncPrintOLED = false;
//------
typedef enum
{

  SLAVEADDR_INDEX = 0,
  PID_INDEX,
  VERSION_INDEX,

  DIST_H_INDEX,
  DIST_L_INDEX,

  TEMP_H_INDEX,
  TEMP_L_INDEX,

  CFG_INDEX,
  CMD_INDEX,
  REG_NUM

} regindexTypedef;

#define MEASURE_MODE_PASSIVE (0x00)
#define MEASURE_RANG_500 (0x20)
#define CMD_DISTANCE_MEASURE (0x01)