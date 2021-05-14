#include "Utility.h"
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  Serial.println(analog_value);
  return analog_value;
}

void DoServoTest(int servoIndex)
{
  pwm.setPWM(servoIndex, 0, pulseWidth(0));
  Serial.println("0");
  delay(500);
  pwm.setPWM(servoIndex, 0, pulseWidth(45));
  Serial.println("45");
  delay(500);
  pwm.setPWM(servoIndex, 0, pulseWidth(90));
  Serial.println("90");
  delay(500);
  pwm.setPWM(servoIndex, 0, pulseWidth(135));
  Serial.println("135");
  delay(500);
  pwm.setPWM(servoIndex, 0, pulseWidth(180));
  Serial.println("180");
  delay(500);
  pwm.setPWM(servoIndex, 0, pulseWidth(135));
  Serial.println("135");
  delay(500);
  pwm.setPWM(servoIndex, 0, pulseWidth(90));
  Serial.println("90");
  delay(500);
  pwm.setPWM(servoIndex, 0, pulseWidth(45));
  Serial.println("45");
  delay(500);
}

void DoServosParallelTest(int servoIndexes[], int servoIndexesCount, int delayms)
{
  int commands[] = {0, 45, 90, 135, 180, 135, 90, 45};
  for(int c = 0; c < 8; c++ )
  {
    for(int i = 0; i < servoIndexesCount; i++ )
    {
      pwm.setPWM(servoIndexes[i], 0, pulseWidth(commands[c]));
      //pwm.setPWM(0, 0, commands(45));
      //DoServoTest(servoIndexes[i]);     
    }
    delay(delayms);
  }
}

void ServoTest(int servoIndexes[]) {
  DoServosParallelTest(servoIndexes,2,200);
}

void UpdateServoCommands(bool debug){
  if(PWMInitialised == false){
    if(debug){
      Serial.print("Servo PWM Enabled");
    }
    pwm = Adafruit_PWMServoDriver();
    PWMInitialised = true;
    }
 }
