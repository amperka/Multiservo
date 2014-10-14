#include "Multiservo.h"
#include <Arduino.h>
#include <Wire.h>


const unsigned int  Multiservo::AddressDefault   = 0x47;
const unsigned int  Multiservo::PulseMinDefault  = 490;
const unsigned int  Multiservo::PulseMaxDefault  = 2400;
const unsigned int  Multiservo::nAttemptsDefault = 4;
const unsigned char Multiservo::PinInvalid       = 0xFF;
const unsigned int  Multiservo::nPinMax          = 18;
const unsigned int  Multiservo::PulseMaxAbsolute = 19000;


Multiservo::Error Multiservo::writeMicroseconds
  (
    uint8_t  pin,
    uint16_t pulseWidth, 
    uint8_t  twiAddress,
    uint8_t  retryAttempts
  )
{
    Error errorCode;

    do {
        Wire.beginTransmission(twiAddress);
        Wire.write(pin);
        Wire.write(pulseWidth >> 8);
        Wire.write(pulseWidth & 0xFF);
        errorCode = (Error)Wire.endTransmission();
    } while (errorCode && --retryAttempts);

    return errorCode;
}

Multiservo::Multiservo() :
  m_pulseWidth(0),
  m_twiAddress(AddressDefault),
  m_iPin(PinInvalid)
{
  Wire.begin();
}

Multiservo::Multiservo(uint8_t twiAddress) :
  m_pulseWidth(0),
  m_twiAddress(twiAddress),
  m_iPin(PinInvalid)
{
  Wire.begin();
}

Multiservo::Error Multiservo::attach(int pin) {
  attach(pin, Multiservo::PulseMinDefault, Multiservo::PulseMaxDefault);
}

Multiservo::Error Multiservo::attach(int pin, int minPulse, int maxPulse)
{
    if (pin < 0 || pin >= nPinMax) {
        detach();

        return BAD_PIN;
    }

    if (minPulse < 0 || minPulse >= PulseMaxAbsolute) {
        detach();

        return BAD_PULSE;
    }

    if (maxPulse < 0 || maxPulse >= PulseMaxAbsolute) {
        detach();

        return BAD_PULSE;
    }

    m_iPin = pin;
    m_minPulse = minPulse;
    m_maxPulse = maxPulse;

    return OK;
}

Multiservo::Error Multiservo::detach()
{
    if (!attached()) return OK;

    Error err = writeMicroseconds(m_iPin, 0, m_twiAddress, nAttemptsDefault);

    if (!err) m_iPin = PinInvalid;

    return err;
}

Multiservo::Error Multiservo::write(int value)
{
    if (value < m_minPulse) {
        value = constrain(value, 0, 180);
        value = map(value, 0, 180, m_minPulse, m_maxPulse);
    }

    return writeMicroseconds(value);
}

Multiservo::Error Multiservo::writeMicroseconds(int pulseWidth)
{
    if (!attached()) return BAD_PIN;

    pulseWidth = constrain(pulseWidth, m_minPulse, m_maxPulse);

    if (pulseWidth == m_pulseWidth) return OK;

    m_pulseWidth = pulseWidth;

    return writeMicroseconds(m_iPin, m_pulseWidth, m_twiAddress, nAttemptsDefault);
}

int Multiservo::read() const
{
    return map(m_pulseWidth, m_minPulse, m_maxPulse, 0, 180);
}

bool Multiservo::attached() const
{
    return m_iPin != PinInvalid;
}

// vim: sw=2 sts=2 ts=8:

