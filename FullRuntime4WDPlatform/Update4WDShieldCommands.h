#include "UpdateArmServoCommands.h"

void M1_advance(char Speed) ///<Motor1 Advance
{
  int E1 = 3; ///<Motor1 Speed
  int M1 = 4; ///<Motor1 Direction
  digitalWrite(M1, LOW);
  analogWrite(E1, Speed);
}
void M2_advance(char Speed) ///<Motor2 Advance
{
  int E2 = 11; ///<Motor2 Speed
  int M2 = 12; ///<Motor2 Direction
  digitalWrite(M2, HIGH);
  analogWrite(E2, Speed);
}
void M3_advance(char Speed) ///<Motor3 Advance
{
  int E3 = 5; ///<Motor3 Speed
  int M3 = 8; ///<Motor3 Direction
  digitalWrite(M3, LOW);
  analogWrite(E3, Speed);
}
void M4_advance(char Speed) ///<Motor4 Advance
{
  int E4 = 6; ///<Motor4 Speed
  int M4 = 7; ///<Motor4 Direction
  digitalWrite(M4, HIGH);
  analogWrite(E4, Speed);
}

void M1_back(char Speed) ///<Motor1 Back off
{
  int E1 = 3; ///<Motor1 Speed
  int M1 = 4; ///<Motor1 Direction
  digitalWrite(M1, HIGH);
  analogWrite(E1, Speed);
}
void M2_back(char Speed) ///<Motor2 Back off
{
  int E2 = 11; ///<Motor2 Speed
  int M2 = 12; ///<Motor2 Direction
  digitalWrite(M2, LOW);
  analogWrite(E2, Speed);
}
void M3_back(char Speed) ///<Motor3 Back off
{
  int E3 = 5; ///<Motor3 Speed
  int M3 = 8; ///<Motor3 Direction
  digitalWrite(M3, HIGH);
  analogWrite(E3, Speed);
}
void M4_back(char Speed) ///<Motor4 Back off
{
  int E4 = 6; ///<Motor4 Speed
  int M4 = 7; ///<Motor4 Direction
  digitalWrite(M4, LOW);
  analogWrite(E4, Speed);
}

void Update4WDShieldCommands()
{
  M1_advance(100);
  M2_advance(100);
  M3_advance(100);
  M4_advance(100);
  delay(2000); ///<Delay 2S
  M1_back(100);
  M2_back(100);
  M3_back(100);
  M4_back(100);
  delay(2000); ///<Delay 2S
}
