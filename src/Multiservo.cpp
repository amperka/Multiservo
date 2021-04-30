#include "Multiservo.h"

Multiservo::Multiservo(TwoWire& wire, uint8_t i2cAddress) {
    _i2cAddress = i2cAddress;
    _wire = &wire;
    _attached = false;
}

void Multiservo::attach(int pin) {
    attach(pin, MULTISERVO_MIN_PULSE_WIDTH, MULTISERVO_MAX_PULSE_WIDTH);
}

void Multiservo::attach(int pin, int minPulse, int maxPulse) {
    if (pin < MULTISERVO_MIN_PIN_SERVO || pin >= MULTISERVO_MAX_PIN_SERVO) {
        return;
    }
    _wire->begin();
    _pin = pin;
    _attached = true;
    _minPulse = minPulse;
    _maxPulse = maxPulse;
    _pulse = (_minPulse + _maxPulse) / 2;
    _writeByte16(_pin, _pulse);
}

void Multiservo::detach() {
    if (!attached()) {
        return;
    }
    _attached = false;
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

int Multiservo::readMicroseconds() const { return _pulse; }

bool Multiservo::attached() const { return _attached; }

bool Multiservo::readVoltageCurrent() {
    uint8_t data[4];
    // Multiservo receive 4 bytes: 2 bytes voltage and 2 bytes current
    // The details see in schematic for Multiservo
    if (_readBytes(0, data, 4)) {
        return false;
    }
    _voltage = (data[1] << 8) | data[0];
    _voltage = (_voltage * 59) / 5;
    _current = (data[3] << 8) | data[2];
    _current = (_current * 39) / 2;
    return true;
}

uint16_t Multiservo::getVoltage() const { return _voltage; }

uint16_t Multiservo::getCurrent() const { return _current; }

void Multiservo::_writeByte(uint8_t regAddress, uint8_t data) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(regAddress);
    _wire->write(data);
    _wire->endTransmission();
}

void Multiservo::_writeByte16(uint8_t regAddress, uint16_t data) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(regAddress);
    _wire->write((data >> 8) & 0xFF);
    _wire->write(data & 0xFF);
    _wire->endTransmission();
}

void Multiservo::_writeBytes(uint8_t regAddress, uint8_t* data,
                             uint8_t length) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(regAddress);
    _wire->write(data, length);
    _wire->endTransmission();
}

uint8_t Multiservo::_readByte(uint8_t regAddress) {
    uint8_t data;
    _wire->beginTransmission(_i2cAddress);
    _wire->write(regAddress);
    _wire->endTransmission();
    _wire->requestFrom(_i2cAddress, 1u);
    data = _wire->read();
    return data;
}

uint16_t Multiservo::_readByte16(uint8_t regAddress) {
    uint16_t data;
    _wire->beginTransmission(_i2cAddress);
    _wire->write(regAddress);
    _wire->endTransmission();
    _wire->requestFrom(_i2cAddress, 2u);
    data = _wire->read();
    data = (data << 8) | _wire->read();
    return data;
}

bool Multiservo::_readBytes(uint8_t regAddress, uint8_t* data, uint8_t length) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(regAddress);
    _wire->endTransmission();
    _wire->requestFrom(_i2cAddress, length);
    if (Wire.available() < length) {
        return false;
    }
    for (size_t i = 0; i < length; i++) {
        *data++ = _wire->read();
    }
    return true;
}