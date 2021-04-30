// Библиотека для работы с Multiservo Shield
// https://github.com/amperka/Multiservo
#include <Multiservo.h>

// Задаём количество сервоприводов
constexpr uint8_t MULTI_SERVO_COUNT = 18;

// Создаём массив объектов для работы с сервомоторами
Multiservo multiservo[MULTI_SERVO_COUNT];

// Переменная для хранения текущей позиции сервомотора
int pos = 0;

void setup() {
    // Перебираем значения моторов от 0 до 17
    for (int count = 0; count < MULTI_SERVO_COUNT; count++) {
        // Подключаем сервомотор
        multiservo[count].attach(count);
    }
}

void loop() {
    // Перебираем значения моторов от 0 до 17
    for (int count = 0; count < MULTI_SERVO_COUNT; count++) {
        // Перебираем значения счётчика от 0 до 180
        for (pos = 0; pos <= 180; pos++) {
            // Отправляем текущий угол на серво
            multiservo[count].write(pos);
            // Ждём 15 мс
            delay(15);
        }
        // Перебираем значения счётчика от 180 до 0
        for (pos = 180; pos >= 0; pos--) {
            // Отправляем текущий угол на серво
            multiservo[count].write(pos);
            // Ждём 15 мс
            delay(15);
        }
    }
}