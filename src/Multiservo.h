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
    Multiservo(TwoWire& wire = Wire, uint8_t i2cAddress = MULTISERVO_I2C_ADDRESS_DEFAULT);
    void attach(int pin);
    void attach(int pin, int minPulse, int maxPulse);
    void detach();
    void write(int angle);
    void writeMicroseconds(int pulse);
    int read() const;
    int readMicroseconds() const;
    bool attached() const;
    bool readVoltageCurrent();
    uint16_t getVoltage() const;
    uint16_t getCurrent() const;

private:
    TwoWire* _wire;
    void _writeByte(uint8_t regAddress, uint8_t data);
    void _writeByte16(uint8_t regAddress, uint16_t data);
    void _writeBytes(uint8_t regAddress, uint8_t* data, uint8_t length);
    uint8_t _readByte(uint8_t regAddress);
    uint16_t _readByte16(uint8_t regAddress);
    bool _readBytes(uint8_t regAddress, uint8_t* data, uint8_t length);
    uint8_t _i2cAddress;
    uint8_t _pin;
    uint16_t _pulse;
    uint16_t _minPulse;
    uint16_t _maxPulse;
    uint16_t _voltage;
    uint16_t _current;
    bool _attached;
};

#endif // __MULTISERVO_H__