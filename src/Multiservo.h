#ifndef __MULTISERVO_H__
#define __MULTISERVO_H__

#include <Arduino.h>
#include <Wire.h>

// I²C-address device
constexpr uint8_t MULTISERVO_I2C_ADDRESS_DEFAULT = 0x47;
// Minimum and maximum pin of servos
constexpr uint8_t MULTISERVO_MIN_PIN_SERVO = 0;
constexpr uint8_t MULTISERVO_MAX_PIN_SERVO = 18;
// Shortest and longest pulse sent to a servo
constexpr uint16_t MULTISERVO_MIN_PULSE_WIDTH = 544;
constexpr uint16_t MULTISERVO_MAX_PULSE_WIDTH = 2400;

class Multiservo {
public:
    Multiservo(uint8_t i2cAddress = MULTISERVO_I2C_ADDRESS_DEFAULT);
    void attach(int pin, int minPulse = MULTISERVO_MIN_PULSE_WIDTH, int maxPulse = MULTISERVO_MAX_PULSE_WIDTH);
    void detach();
    void write(int angle);
    void writeMicroseconds(int pulse);
    void writeMicroseconds(uint8_t pin, uint16_t pulse,
                                  uint8_t i2cAddress);
    int read() const;
    bool attached() const;

private:
    uint8_t _i2cAddress;
    uint8_t _pin;
    uint16_t _pulse;
    uint16_t _minPulse;
    uint16_t _maxPulse;

};

#endif // __MULTISERVO_H__