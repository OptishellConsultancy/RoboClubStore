
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
bool UltraSonicSetupRequired = true;
//
bool GPSEnabled;
bool LSM303D_CompassAccelMagnetoEnabled;
bool ServoControllerEnabled;
bool OLEDDisplayEnabled;
bool FourWDHatEnabled;
bool UltrasonicEnabled;
bool ServoTestEnabled;
bool PWMInitialised;

//Ultrasonic vars:
unsigned char addr0 = 0x11; //The ic2 address of the Ultrasonic
unsigned char txbuf[10] = {0};
unsigned char rxbuf[10] = {0};

typedef enum {

  SLAVEADDR_INDEX = 0,
  PID_INDEX,
  VERSION_INDEX ,

  DIST_H_INDEX,
  DIST_L_INDEX,

  TEMP_H_INDEX,
  TEMP_L_INDEX,

  CFG_INDEX,
  CMD_INDEX,
  REG_NUM

} regindexTypedef;

#define    MEASURE_MODE_PASSIVE    (0x00)
#define    MEASURE_RANG_500        (0x20)
#define    CMD_DISTANCE_MEASURE    (0x01)