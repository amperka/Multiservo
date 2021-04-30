// Библиотека для работы с Multiservo Shield
// https://github.com/amperka/Multiservo
#include <Multiservo.h>

// Создаём объект для работы с сервомоторами
Multiservo multiservo;

// Задаём имя пина к которому подключён сервопривод
constexpr uint8_t MULTI_SERVO_PIN = 7;

// Переменная для хранения текущей позиции сервомотора
int pos = 0;

void setup() {
    // Подключаем сервомотор
    multiservo.attach(MULTI_SERVO_PIN);
}

void loop() {
    // Перебираем значения счётчика от 0 до 180
    for (pos = 0; pos <= 180; pos++) {
        // Отправляем текущий угол на серво
        multiservo.write(pos);
        // Ждём 15 мс
        delay(15);
    }
    // Перебираем значения счётчика от 180 до 0
    for (pos = 180; pos >= 0; pos--) {
        // Отправляем текущий угол на серво
        multiservo.write(pos);
        // Ждём 15 мс
        delay(15);
    }
}