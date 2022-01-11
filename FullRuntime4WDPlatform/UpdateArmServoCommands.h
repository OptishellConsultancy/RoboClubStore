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
  //int servoIndexes[] = {0, 1};
  //int servoIndexesCount = 2;
  //int numberOfLoops = 100000;
  //DoServosParallelTest(servoIndexes, servoIndexesCount, 200, numberOfLoops);
}

void ResetPositions()
{
  for (size_t i = 0; i < 6; i++)
  {
    pwm.setPWM(motorIndex[i], 0, defaultPos);
  }
}

//https://www.jsumo.com/mg996r-servo-motor-digital
void EnableArmServos()
{
  if (!PWMInitialised)
  {
    Serial.print("Servo PWM Enabled");
    pwm = Adafruit_PWMServoDriver();
    ResetPositions();
    PWMInitialised = true;
  }
}

void doPulseLerp(MotorName motorName, int servoIndex, int finalAng)
{
  int lastAng = motorNameLastAng[(int)motorName];
  int angStep = (finalAng - lastAng) / stepsPerPWM;
  if (angStep > 0)
  {
    //
    int interAng;
    int interPulse;
    //
    for (size_t i = 0; i < stepsPerPWM; i++)
    {
      interAng = (lastAng + (angStep * i));
      interPulse = map(interAng, 0, 180, SERVOMIN, SERVOMAX);
      PrintfOneVar(100,"InterAng: %d.",interAng);  
    
      pwm.setPWM(servoIndex, 0, interPulse);
      delay(delayPerPWN);
    }

    //Set and do last
    PrintfOneVar(100,"FinalAng: %d.",interAng); 
    int finalPulse = map(finalAng, 0, 180, SERVOMIN, SERVOMAX);
    motorNameLastAng[(int)motorName] = finalAng;
    pwm.setPWM(servoIndex, 0, finalPulse);
  }
  else
  {
    PrintfOneVar(100,"FinalAng: %d.",finalAng);  
    motorNameLastAng[(int)motorName] = finalAng;
    pwm.setPWM(servoIndex, 0, map(finalAng, 0, 180, SERVOMIN, SERVOMAX));
  }
}

void Do6AxisAPICommand()
{
  Serial.print("Do6AxisAPICommand..\r\n ");
  EnableArmServos(); //Just incase
  if (Ang6Axis_Base > -1)
  {
    doPulseLerp(Base, motorIndex[Base], Ang6Axis_Base);
    //pwm.setPWM(motorIndex[Base], 0, angleToPulse(Ang6Axis_Base));
    Serial.print("Base done..\r\n ");
  }
  if (Ang6Axis_BaseTilt > -1)
  {
    doPulseLerp(BaseTilt, motorIndex[BaseTilt], Ang6Axis_BaseTilt);
    //pwm.setPWM(motorIndex[BaseTilt], 0, angleToPulse(Ang6Axis_BaseTilt));
    Serial.print("BaseTilt done..\r\n ");
  }
  if (Ang6Axis_Elbow > -1)
  {
    doPulseLerp(Elbow, motorIndex[Elbow], Ang6Axis_Elbow);
    //pwm.setPWM(motorIndex[Elbow], 0, angleToPulse(Ang6Axis_Elbow));
    Serial.print("Elbow done..\r\n ");
  }
  if (Ang6Axis_WristElevate > -1)
  {
    doPulseLerp(WristElevate, motorIndex[WristElevate], Ang6Axis_WristElevate);
    //pwm.setPWM(motorIndex[WristElevate], 0, angleToPulse(Ang6Axis_WristElevate));
    Serial.print("WristElevate done..\r\n ");
  }
  if (Ang6Axis_WristRotate > -1)
  {
    doPulseLerp(WristRotate, motorIndex[WristRotate], Ang6Axis_WristRotate);
    //pwm.setPWM(motorIndex[WristRotate], 0, angleToPulse(Ang6Axis_WristRotate));
    Serial.print("WristRotate done..\r\n ");
  }
  if (Ang6Axis_Claw > -1)
  {
    doPulseLerp(Claw, motorIndex[Claw], Ang6Axis_Claw);
    //pwm.setPWM(motorIndex[Claw], 0, angleToPulse(Ang6Axis_Claw));
    Serial.print("Claw done..\r\n ");
  }
}
