Multiservo
==========

Библиотека для Arduino, которая позволяет управлять 18 сервоприводами при помощи [Multiservo Shield](http://amperka.ru/product/arduino-multiservo-shield)
от Амперки.

Установка
=========

Скопируйте директорию ''Multiservo'' в ''$sketchbook/libraries'', где ''$sketchbook'' — ваша директория со скетчами.

После этого перезагрузите Arduino IDE.

Пример использования
====================

```cpp

#include <Wire.h>
#include <Multiservo.h>

#define MULTISERVO_PIN 17
 
Multiservo myservo;
 
int pos = 0;  
 
void setup(void)
{
  Wire.begin();
  myservo.attach(MULTISERVO_PIN);
}
 
void loop(void)
{
  for (pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees
  { // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) // goes from 180 degrees to 0 degrees
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
```
