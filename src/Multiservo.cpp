#include "Multiservo.h"

Multiservo::Multiservo(uint8_t i2cAddress)
    : _i2cAddress(i2cAddress) {
    Wire.begin();
}

void Multiservo::attach(int pin, int minPulse, int maxPulse) {
    if (pin < MULTISERVO_MIN_PIN_SERVO || pin >= MULTISERVO_MAX_PIN_SERVO) {
        return;
    }
    _pin = pin;
    _minPulse = minPulse;
    _maxPulse = maxPulse;
    _pulse = (_minPulse + _maxPulse) / 2;
    writeMicroseconds(_pulse);
}

void Multiservo::detach() {
    if (!attached()) {
        return;
    }
    _writeByte16(_pin, 0);
}

void Multiservo::write(int angle) {
    uint16_t pulse;
    angle = constrain(angle, 0, 180);
    pulse = map(angle, 0, 180, _minPulse, _maxPulse);
    writeMicroseconds(pulse);
}

void Multiservo::writeMicroseconds(int pulse) {
    if (!attached()) {
        return;
    }
    pulse = constrain(pulse, _minPulse, _maxPulse);
    if (pulse == _pulse) {
        return;
    }
    _pulse = pulse;

    _writeByte16(_pin, _pulse);
}

int Multiservo::read() const {
    return map(_pulse, _minPulse, _maxPulse, 0, 180);
}

bool Multiservo::attached() const { return _pin; }

void Multiservo::_writeByte(uint8_t regAddress, uint8_t data) {
    Wire.beginTransmission(_i2cAddress);
    Wire.write(regAddress);
    Wire.write(data);
    Wire.endTransmission();
}

void Multiservo::_writeByte16(uint8_t regAddress, uint16_t data) {
    Wire.beginTransmission(_i2cAddress);
    Wire.write(regAddress);
    Wire.write((data >> 8) & 0xFF);
    Wire.write(data & 0xFF);
    Wire.endTransmission();
}

uint8_t Multiservo::_readByte(uint8_t regAddress) {
    uint8_t data;
    Wire.beginTransmission(_i2cAddress);
    Wire.write(regAddress);
    Wire.endTransmission();
    Wire.requestFrom(_i2cAddress, 1u);
    data = Wire.read();
    return data;
}

uint16_t Multiservo::_readByte16(uint8_t regAddress) {
    uint16_t data;
    Wire.beginTransmission(_i2cAddress);
    Wire.write(regAddress);
    Wire.endTransmission();
    Wire.requestFrom(_i2cAddress, 2u);
    data = Wire.read();
    data = (data << 8) | Wire.read();
    return data;
}