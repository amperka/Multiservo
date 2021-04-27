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
    writeMicroseconds(_pin, 0, _i2cAddress);
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
    writeMicroseconds(_pin, _pulse, _i2cAddress);
}

void Multiservo::writeMicroseconds(uint8_t pin, uint16_t pulse,
                                   uint8_t i2cAddress) {

    Wire.beginTransmission(i2cAddress);
    Wire.write(pin);
    Wire.write((pulse >> 8) & 0xFF);
    Wire.write(pulse & 0xFF);
    Wire.endTransmission();
}

int Multiservo::read() const {
    return map(_pulse, _minPulse, _maxPulse, 0, 180);
}

bool Multiservo::attached() const { return _pin; }
