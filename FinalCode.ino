#include <Arduino.h>
#include "MeMegaPi.h"
#include <SPI.h>
#include <PixyI2C.h>

MeMegaPiDCMotor motor1(PORT1B);
MeMegaPiDCMotor motor2(PORT2B);
MeMegaPiDCMotor motor3(PORT3B);
const int MOTOR_SPEED = 100;

PixyI2C pixy;

int signature = 0;
int x = 0;
static int i = 0;
int Xmin = 80;
int Xmax = 250;
int XminBG = 120;
int XmaxBG = 210;
int XminBl = 100;
int XmaxBl = 200;
int width = 0;
int height = 0;
int area;
double dis;

MeUltrasonicSensor ultrasonic_7(7);

void setup()
{
  pixy.init();
}

void loop()
{
  motor1.run(-255);
  dis = ultrasonic_7.distanceCm();

  while(dis > 4)
  {
    scan();

    if(signature == 1) //Red Ball Detected
    {
      while(x < Xmax && x > Xmin)
      {
        moveFor();
        scan();

        if(dis < 4)
        {
          signature = 0;
          break;
        }
      }

      while(x > Xmax)
      {
        moveRight();
        scan();
        dis = ultrasonic_7.distanceCm();

        if(dis < 4)
        {
          signature = 0;
          break;
        }
      }

      while(x < Xmin)
      {
        moveLeft();
        scan();
        dis = ultrasonic_7.distanceCm();

        if(dis < 4)
        {
          signature = 0;
          break;
        }
      }

      dis = ultrasonic_7.distanceCm();
    } //End if red ball

    if(signature == 2 || signature == 3)
    {
      scan();

      while(x < XmaxBG && x >  XminBG)
      {
        moveRev();
        delay(1000);
        moveLeft();
        delay(1000);
        scan();
      }

      while(x > XmaxBG)
      {
        moveRight();
        scan();
      }

      while(x < XminBG)
      {
        moveLeft();
        scan();
      }
    } //End if not red ball
    
  else
    {
      while(signature != 1 || signature != 2 || signature != 3)
        {
          moveRev();
          delay(1000);
          scan();
          moveRight();
          delay(2000);
          scan();
          moveFor();
          delay(1000);
          scan();
        }
      }
  }

  if(dis < 4)
  {
    scan();

    if(signature == 4)
    {
      area = width * height;

      while(x < XmaxBl && x > XminBl)
      {
        moveFor();
        scan();
        area = width * height;

        if(area > 2000)
        {
          moveFor();
          delay(1000);
          break;
        }
      }

      while(x > XmaxBl)
      {
        moveRight();
        scan();
        area = width * height;

        if(area > 2000)
        {
          moveFor();
          delay(1000);
          break;
        }
      }

      while(x < XminBl)
      {
        moveLeft();
        scan();
        area = width * height;

        if(area > 2000)
        {
          moveFor();
          delay(1000);
          break;
        }
      }

      while(signature != 4)
      {
        moveFor();
        delay(1000);
        moveLeft();
        delay(2000);
        scan();
      }

      if(area > 2000)
      {
        empty();
        delay(2000);
        Stop();
        delay(500);
        moveRev();
        delay(3000);
        victoryDance();
        Stop();
      }
    } //EndZone Detected
  }

  
}

//Function to move the robot forward
void moveFor()
{
  motor3.run(MOTOR_SPEED);
  motor2.run(-MOTOR_SPEED);
}

//Function to move the robot backwards
void moveRev()
{
  motor3.run(-MOTOR_SPEED);
  motor2.run(MOTOR_SPEED);
}

//Function to move the robot right
void moveRight()
{
  motor3.run(MOTOR_SPEED);
  motor2.run(MOTOR_SPEED);
}

//Function to move the robot left
void moveLeft()
{
  motor3.run(-MOTOR_SPEED);
  motor2.run(-MOTOR_SPEED);
}

//Function to stop every motor
void Stop()
{
  motor1.stop();
  motor2.stop();
  motor3.stop();
}

//Function to empty the conveyor
void empty()
{
  motor1.run(255);
  motor2.run(50);
  motor3.run(-50);
}

//Victory Dance function
void victoryDance()
{
  motor1.stop();
  motor2.run(255);
  motor3.run(-255);
  delay(5000);
  motor2.run(-255);
  motor3.run(255);
  delay(5000);
}

//Pixy Camera Scan Function
void scan()
{
  uint16_t blocks();
  blocks = pixy.getBlocks();
  signature = pixy.blocks[i].signature;
  x = pixy.blocks[i].x;
  width = pixy.blocks[i].width;
  height = pixy.blocks[i].height;
}
