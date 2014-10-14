
#include "Multiservo.h"

#include <Arduino.h>
#include <Wire.h>

#define NO_PIN  0xFF


XSERVO_ERROR xservoWriteMicroseconds(
        uint8_t pin, uint16_t pulseWidth, 
        uint8_t twiAddress, uint8_t retryAttempts)
{
    XSERVO_ERROR errorCode;

    do {
        Wire.beginTransmission(twiAddress);
        Wire.write(pin);
        Wire.write(pulseWidth >> 8);
        Wire.write(pulseWidth & 0xFF);
        errorCode = (XSERVO_ERROR)Wire.endTransmission();
    } while (errorCode && --retryAttempts);

    return errorCode;
}

Multiservo::Multiservo(uint8_t twiAddress)
{
    _twiAddress = twiAddress;
    _pulseWidth = 0;
    _pin = NO_PIN;
}

XSERVO_ERROR Multiservo::attach(int pin, int minPulse, int maxPulse)
{
    if (pin < 0 || pin >= XSERVO_MAX_PIN) {
        detach();
        return XS_BAD_PIN;
    }

    if (minPulse < 0 || minPulse >= XSERVO_PULSE_LIMIT) {
        detach();
        return XS_BAD_PULSE;
    }

    if (maxPulse < 0 || maxPulse >= XSERVO_PULSE_LIMIT) {
        detach();
        return XS_BAD_PULSE;
    }

    _pin = pin;
    _minPulse = minPulse;
    _maxPulse = maxPulse;

    return XS_OK;
}

XSERVO_ERROR Multiservo::detach()
{
    if (!attached())
        return XS_OK;

    XSERVO_ERROR err = xservoWriteMicroseconds(_pin, 0, _twiAddress);
    if (!err)
        _pin = NO_PIN;

    return err;
}

XSERVO_ERROR Multiservo::write(int value)
{
    if (value < _minPulse) {
        value = constrain(value, 0, 180);
        value = map(value, 0, 180, _minPulse, _maxPulse);
    }

    return writeMicroseconds(value);
}

XSERVO_ERROR Multiservo::writeMicroseconds(int pulseWidth)
{
    if (!attached())
        return XS_BAD_PIN;

    pulseWidth = constrain(pulseWidth, _minPulse, _maxPulse);
    if (pulseWidth == _pulseWidth)
        return XS_OK;

    _pulseWidth = pulseWidth;

    return xservoWriteMicroseconds(_pin, _pulseWidth, _twiAddress);
}

int Multiservo::read() const
{
    return map(_pulseWidth, _minPulse, _maxPulse, 0, 180);
}

bool Multiservo::attached() const
{
    return _pin != NO_PIN;
}

// vim: sw=2 sts=2 ts=8:

