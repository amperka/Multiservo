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
    Multiservo(uint8_t twiAddress = MULTISERVO_I2C_ADDRESS_DEFAULT);
    void attach(int pin);
    void attach(int pin, int minPulse, int maxPulse);
    void detach();
    void write(int value);
    void writeMicroseconds(int pulseWidth);
    void writeMicroseconds(uint8_t pin, uint16_t pulseWidth,
                                  uint8_t twiAddress);
    int read() const;
    bool attached() const;

private:
    uint8_t m_iPin;
    uint8_t m_twiAddress;
    uint16_t m_minPulse;
    uint16_t m_maxPulse;
    uint16_t m_pulseWidth;
};

#endif // __MULTISERVO_H__