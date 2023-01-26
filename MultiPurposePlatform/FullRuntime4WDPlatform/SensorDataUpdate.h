#include "Update4WDShieldCommands.h"

void InitLSM303D()
{
  Serial.println("InitLSM303D()..");
  lsm303D.init();
  lsm303D.enableDefault();
  lsm303D.getDeviceType();

  lsm303D.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  lsm303D.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
}

void InitUltraSonic()
{
  txbuf[0] = (MEASURE_MODE_PASSIVE | MEASURE_RANG_500); // the measurement mode is set to passive mode, measurement range is set to 500CM.
  i2cWriteBytes(addr0, CFG_INDEX, &txbuf[0], 1);        //
}

///
void UpdateGPSDataTest(bool doAPICondensedFormat = false)
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
  if (!doAPICondensedFormat)
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
  else
  {

    //----------
  }
}

void GPSReadAndResultCache()
{
  GPSHasData = false;
  if (GPS.available())
  {

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
    if (GPS.fix)
    {
      GPSData_DateTime = GPSDateTime();
      GPSData_LocLat = GPSLocLat();
      GPSData_LocLon = GPSLocLon();
      GPSData_Misc = GPSMisc();
      GPSHasData = true;
    }
  }
}

void ProcessGPSData()
{

  if (GPSHasData)
  {

    Serial.print("<GPSDATETIME.Start: \n");
    Serial.println(GPSData_DateTime);
    Serial.print("<GPSDATETIME.End> \n");
    //
    Serial.print("<GPSLoc.Start> \n");
    if (GPS.fix)
    {
      Serial.println(GPSData_LocLat);
      Serial.println(GPSData_LocLon);
      Serial.println(GPSData_Misc);

      if (GPSPrintOLED)
      {
        OLEDTxt = true;
        //
        String oledCommand = ("<In>OLEDTXT[" + GPSData_DateTime + "\n" + GPSData_LocLat + "\n" + GPSData_LocLon + "\n" + GPSData_Misc + "]{X:0,Y:0,S:1}");
        PassOLEDTxt(oledCommand);
      }
      DoGPS = false;
    }
    else
    {
      Serial.print(" ERROR: FIX NOT ACQUIRED \n");
      OLEDTxt = true;
      String oledCommand = "<In>OLEDTXT[" + GPSData_DateTime + "\n" + GPSData_LocLat + "\n" + GPSData_LocLon + "\n" + GPSData_Misc + "\nERR:FIX NOT ACQUIRED]{X:0,Y:0,S:1}";
      PassOLEDTxt(oledCommand);
    }
    Serial.print("<GPSLoc.End> \n");
  }
  else
  {
    Serial.print("ERROR: GPS NOT AVAILABLE \n");
    OLEDTxt = true;
    String gpsTimeStr = GPSDateTime();
    Serial.print(" GPS NOT AVAILABLE \n ");
    String oledCommand = "<In>OLEDTXT[ERR:GPS FIX AVAILABLE]{X:0,Y:0,S:1}";
    PassOLEDTxt(oledCommand);
    Serial.print(oledCommand);
  }
}

PrintDoAccMag()
{

  if (LSM303D_CompassAccelMagnetoInitRequired)
  {
    InitLSM303D();
    LSM303D_CompassAccelMagnetoInitRequired = false;
  }
  lsm303D.read();
  // Raw values:
  lsm303D.readAcc();
  char LSM30D3ReportBuffer_AccRaw[30];
  snprintf(LSM30D3ReportBuffer_AccRaw, sizeof(LSM30D3ReportBuffer_AccRaw), "Acc:%d,%d,%d", lsm303D.a.x, lsm303D.a.y, lsm303D.a.z);
  lsm303D.readMag();
  char LSM30D3ReportBuffer_MagRaw[30];
  snprintf(LSM30D3ReportBuffer_MagRaw, sizeof(LSM30D3ReportBuffer_MagRaw), "Mag:%d,%d,%d", (lsm303D.m.x / 32767.0) * 2.0, (lsm303D.m.y * 32767.0) * 2.0, (lsm303D.m.z * 32767.0) * 2.0);

  int headingInt = (int)lsm303D.heading();
  String heading = String(headingInt, DEC) + "Deg/" + String(Deg2Dir(headingInt));

  Serial.println(LSM30D3ReportBuffer_AccRaw);
  Serial.println(LSM30D3ReportBuffer_MagRaw);
  Serial.println(heading);

  if (AccMagPrintOLED)
  {
    OLEDTxt = true;
    String LSM30DStrAcc = String(LSM30D3ReportBuffer_AccRaw);
    String LSM30DStrMag = String(LSM30D3ReportBuffer_MagRaw);
    //
    String oledCommand = ("<In>OLEDTXT[" + LSM30DStrAcc + "\n" + LSM30DStrMag + "\n" + heading + "]{X:0,Y:0.S:1}");
  }
}

void LSM303D_UpdateCompassAccelMagnetoData()
{
  PrintDoAccMag();
}

void PrintDoUltSonc()
{
  if (UltraSonicSetupRequired)
  {
    InitUltraSonic();
    UltraSonicSetupRequired = false;
  }
  int16_t dist, temp;
  txbuf[0] = CMD_DISTANCE_MEASURE;

  i2cWriteBytes(addr0, CMD_INDEX, &txbuf[0], 1); // write register, send ranging command
  delay(100);

  i2cReadBytes(addr0, DIST_H_INDEX, 2); // read distance register
  dist = ((uint16_t)rxbuf[0] << 8) + rxbuf[1];

  i2cReadBytes(addr0, TEMP_H_INDEX, 2); // read temperature register
  temp = ((uint16_t)rxbuf[0] << 8) + rxbuf[1];

  String distStr = String(dist, DEC);
  String tempStr = String((float)temp / 10, 1) + "c";

  Serial.println(distStr);
  Serial.println(tempStr);

  if (UltraSoncPrintOLED)
  {
    OLEDTxt = true;
    String oledCommand = ("<In>OLEDTXT[" + distStr + "\n" + tempStr + "]{X:0,Y:26.S:3}");
  }
}

void UpdateUltrasonicIC2Data()
{
  PrintDoUltSonc();
}
