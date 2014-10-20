Multiservo
==========

Библиотека для Arduino, которая позволяет управлять 18 сервоприводами при
помощи [Multiservo Shield](http://amperka.ru/product/arduino-multiservo-shield)
от Амперки.

Установка
=========

Скачайте последний [релиз библиотеки](https://github.com/amperka/Multiservo/releases/download/1.0.1/multiservo-1.0.1.zip):

<a style="padding-top: 7px; padding-bottom: 7px; padding-left: 12px; padding-right: 12px; background: #569e3d; background: linear-gradient(#79d858, #569e3d); border: 1px solid #4a993e; border-radius: 5px;font-size:13px;font:13px/1.4 Helvetica,arial,sans-serif;font-weight:bold;color:#fff;text-shadow:0 -1px 0 rgba(0,0,0,0.25);text-decoration:none;" href="">Haha</a>

<a href="https://github.com/amperka/Multiservo/releases/download/1.0.1/multiservo-1.0.1.zip"><img src="https://raw.githubusercontent.com/amperka/Multiservo/master/download.png" alt="Кнопка загрузки"/></a>

В Arduino IDE выберите пункт меню «Скетч» → «Импортировать библиотеку» →
«Добавить библиотеку…». В появившемся окне выберите скачаный архив с
библиотекой. Установка завершена.

Пример использования
====================

```cpp

#include <Wire.h>
#include <Multiservo.h>

#define MULTISERVO_PIN 17
 
Multiservo myservo;

int pos = 0;

void setup(void) {
  Wire.begin();
  myservo.attach(MULTISERVO_PIN);
}
 
void loop(void) {
  for (pos = 0; pos <= 180; pos += 1) {// goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  
  for (pos = 180; pos >= 0; pos -= 1) {// goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
```
