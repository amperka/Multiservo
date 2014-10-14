#include <Wire.h>
#include <Multiservo.h>

#define SERVO_COUNT 18

Multiservo servo[SERVO_COUNT];

int pos = 0;

void setup(void)
{
  Wire.begin();

  for (int i = 0; i < SERVO_COUNT; ++i)
  {
    servo[i].attach(i);
    servo[i].write(0);
  }
}

void loop(void)
{
  for (int i = 0; i < SERVO_COUNT; ++i)
  {
    for (pos = 0; pos <= 180; pos += 10) 
    { 
      servo[i].write(pos);              
      delay(20);                       
    }
  }
  for (int i = 0; i < SERVO_COUNT; ++i)
  {
    for (pos = 180; pos >= 0; pos -= 10) 
    {
      servo[i].write(pos);                
      delay(20);                       
    }
  }
}
