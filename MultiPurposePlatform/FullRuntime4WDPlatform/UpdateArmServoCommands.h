#include "Utility.h"

int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  Serial.println(analog_value);
  return analog_value;
}

int angleToPulse(int ang)
{
  return map(ang, 0, 180, SERVOMIN, SERVOMAX); // map angle of 0 to 180 to Servo min and Servo max;
}

void DoServoTest(int servoIndex)
{
  pwm.setPWM(servoIndex, 0, angleToPulse(0));
  Serial.println(String(servoIndex) + " 0");
  delay(500);
  pwm.setPWM(servoIndex, 0, angleToPulse(45));
  Serial.println(String(servoIndex) + " 45");
  delay(500);
  pwm.setPWM(servoIndex, 0, angleToPulse(90));
  Serial.println(String(servoIndex) + " 90");
  delay(500);
  pwm.setPWM(servoIndex, 0, angleToPulse(135));
  Serial.println(String(servoIndex) + " 135");
  delay(500);
  pwm.setPWM(servoIndex, 0, angleToPulse(180));
  Serial.println(String(servoIndex) + " 180");
  delay(500);
  pwm.setPWM(servoIndex, 0, angleToPulse(135));
  Serial.println(String(servoIndex) + " 135");
  delay(500);
  pwm.setPWM(servoIndex, 0, angleToPulse(90));
  Serial.println(String(servoIndex) + " 90");
  delay(500);
  pwm.setPWM(servoIndex, 0, angleToPulse(45));
  Serial.println(String(servoIndex) + " 45");
  delay(500);
}

void DoServosParallelTest(int servoIndexes[], int servoIndexesCount, int delayms, int numberOfLoops)
{
  Serial.println("DoServosParallelTest..");
  CurrentNumServoArmOfTests = 0;
  int commands[] = {0, 45, 90, 135, 180, 135, 90, 45};
  for (int c = 0; c < 8; c++)
  {
    for (int i = 0; i < servoIndexesCount; i++)
    {
      pwm.setPWM(servoIndexes[i], 0, angleToPulse(commands[c]));
    }
    delay(delayms);
  }
  CurrentNumServoArmOfTests++;
  if (numberOfLoops == CurrentNumServoArmOfTests)
  {
    ServoArmControllerEnabled = false;
  }
}

void ArmServosTest()
{
  DoServoTest(15);
  // int servoIndexes[] = {0, 1};
  // int servoIndexesCount = 2;
  // int numberOfLoops = 100000;
  // DoServosParallelTest(servoIndexes, servoIndexesCount, 200, numberOfLoops);
}


void SetZeroPWMValues()
{
  for (size_t i = 0; i < 6; i++)
  {
    pwm.setPWM(motorIndex[i], 0, 0);
  }
}

// https://www.jsumo.com/mg996r-servo-motor-digital
void EnableArmServos()
{
  if (!PWMInitialised)
  {
    Serial.print("Servo PWM Enabled");
    pwm = Adafruit_PWMServoDriver();
    
    // SetZeroPWMValues();
    PWMInitialised = true;
  }
}

void doPulseLerp(MotorName motorName, int servoIndex, int finalAng)
{

  PrintfOneVar(100, "doPulseLerp->Ang: %d.", finalAng);
  PrintfOneVar(100, "doPulseLerp->servoIndex: %d.", servoIndex);
  pwm.setPWM(servoIndex, 0, map(finalAng, 0, 180, SERVOMIN, SERVOMAX));

  int finalPulse = map(finalAng, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servoIndex, 0, finalPulse);
  // Stop signal

  // pwm.setPWM(servoIndex, 0, 0);
}

void Do6AxisAPICommand()
{
  Serial.print("Do6AxisAPICommand");
  if (Ang6Axis_Base > -1)
  {
    PrintfOneVar(100, "Base motor index:: %d.", motorIndex[Base]);
    doPulseLerp(Base, motorIndex[Base], Ang6Axis_Base);
  }
  if (Ang6Axis_BaseTilt > -1)
  {
    PrintfOneVar(100, "BaseTilt motor index:: %d.", motorIndex[BaseTilt]);
    doPulseLerp(BaseTilt, motorIndex[BaseTilt], Ang6Axis_BaseTilt);
  }
  if (Ang6Axis_Elbow > -1)
  {
    PrintfOneVar(100, "Elbow motor index:: %d.", motorIndex[Elbow]);
    doPulseLerp(Elbow, motorIndex[Elbow], Ang6Axis_Elbow);
  }
  if (Ang6Axis_WristElevate > -1)
  {
    PrintfOneVar(100, "WristElevate motor index:: %d.", motorIndex[WristElevate]);
    doPulseLerp(WristElevate, motorIndex[WristElevate], Ang6Axis_WristElevate);
  }
  if (Ang6Axis_WristRotate > -1)
  {
    PrintfOneVar(100, "WristRotate motor index:: %d.", motorIndex[WristRotate]);
    doPulseLerp(WristRotate, motorIndex[WristRotate], Ang6Axis_WristRotate);
  }
  if (Ang6Axis_Claw > -1)
  {
    PrintfOneVar(100, "Claw motor index:: %d.", motorIndex[Claw]);
    doPulseLerp(Claw, motorIndex[Claw], Ang6Axis_Claw);
  }
}
