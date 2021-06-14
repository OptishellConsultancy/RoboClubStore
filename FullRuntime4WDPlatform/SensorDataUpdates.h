#include "Update4WDShieldCommands.h"

void InitLSM303D()
{
  Serial.println("InitLSM303D()..");
  compass.init();
  compass.enableDefault();
}

void InitUltraSonic()
{
  txbuf[0] = (MEASURE_MODE_PASSIVE | MEASURE_RANG_500); //the measurement mode is set to passive mode, measurement range is set to 500CM.
  i2cWriteBytes(addr0, CFG_INDEX, &txbuf[0], 1);        //
}

///
void UpdateGPSData()
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
  if (true)
  {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c)
        Serial.print(c);
    // if a sentence is received, we can check the checksum, parse it...
    if (GPS.newNMEAreceived())
    {
      // a tricky thing here is if we print the NMEA sentence, or data
      // we end up not listening and catching other sentences!
      // so be very wary if using OUTPUT_ALLDATA and trying to print out data
      Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
      if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
        return;                       // we can fail to parse a sentence in which case we should just wait for another
    }

    // approximately every 2 seconds or so, print out the current stats
    {
      Serial.print("\nTime: ");
      if (GPS.hour < 10)
      {
        Serial.print('0');
      }
      Serial.print(GPS.hour, DEC);
      Serial.print(':');
      if (GPS.minute < 10)
      {
        Serial.print('0');
      }
      Serial.print(GPS.minute, DEC);
      Serial.print(':');
      if (GPS.seconds < 10)
      {
        Serial.print('0');
      }
      Serial.print(GPS.seconds, DEC);
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
      Serial.print("Date: ");
      Serial.print(GPS.day, DEC);
      Serial.print('/');
      Serial.print(GPS.month, DEC);
      Serial.print("/20");
      Serial.println(GPS.year, DEC);
      Serial.print("Fix: ");
      Serial.print((int)GPS.fix);
      Serial.print(" quality: ");
      Serial.println((int)GPS.fixquality);
      if (GPS.fix)
      {
        Serial.print("Location: ");
        Serial.print(GPS.latitude, 4);
        Serial.print(GPS.lat);
        Serial.print(", ");
        Serial.print(GPS.longitude, 4);
        Serial.println(GPS.lon);
        Serial.print("Speed (knots): ");
        Serial.println(GPS.speed);
        Serial.print("Angle: ");
        Serial.println(GPS.angle);
        Serial.print("Altitude: ");
        Serial.println(GPS.altitude);
        Serial.print("Satellites: ");
        Serial.println((int)GPS.satellites);
      }
    }
  }
}

void LSM303D_UpdateCompassAccelMagnetoData()
{
  if (LSM303D_CompassAccelMagnetoInitRequired)
  {
    InitLSM303D();
    LSM303D_CompassAccelMagnetoInitRequired = false;
  }
  Serial.println("LSM303D report: \\\\\\\\\\");
  compass.read();
  char LSM30D3ReportBuffer[80];
  snprintf(LSM30D3ReportBuffer, sizeof(LSM30D3ReportBuffer), "A: %6d %6d %6d    M: %6d %6d %6d", compass.a.x, compass.a.y, compass.a.z, compass.m.x, compass.m.y, compass.m.z);
  Serial.println(LSM30D3ReportBuffer);
  Serial.println("\\\\\\\\\\\\\\\\\\\\\\\\\\");
}

void UpdateUltrasonicIC2Data()
{
  if (UltraSonicSetupRequired)
  {
    InitUltraSonic();
    UltraSonicSetupRequired = false;
  }
  int16_t dist, temp;
  txbuf[0] = CMD_DISTANCE_MEASURE;

  i2cWriteBytes(addr0, CMD_INDEX, &txbuf[0], 1); //write register, send ranging command
  delay(100);

  i2cReadBytes(addr0, DIST_H_INDEX, 2); //read distance register
  dist = ((uint16_t)rxbuf[0] << 8) + rxbuf[1];

  i2cReadBytes(addr0, TEMP_H_INDEX, 2); //read temperature register
  temp = ((uint16_t)rxbuf[0] << 8) + rxbuf[1];

  Serial.print(dist, DEC);
  Serial.print("cm");
  Serial.print("------");

  Serial.print((float)temp / 10, 1);
  Serial.println("℃");
}
