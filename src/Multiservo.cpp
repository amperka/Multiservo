#include "Multiservo.h"

Multiservo::Multiservo(uint8_t twiAddress)
    : m_pulseWidth(0)
    , m_twiAddress(twiAddress) {
    Wire.begin();
}

void Multiservo::attach(int pin) {
    attach(pin, MULTISERVO_MIN_PULSE_WIDTH, MULTISERVO_MAX_PULSE_WIDTH);
}

void Multiservo::attach(int pin, int minPulse, int maxPulse) {
    if (pin < MULTISERVO_MIN_PIN_SERVO || pin >= MULTISERVO_MAX_PIN_SERVO) { }

    m_iPin = pin;
    m_minPulse = minPulse;
    m_maxPulse = maxPulse;
    int i = (minPulse + maxPulse) / 2;
    writeMicroseconds(i);
}

void Multiservo::detach() {
    if (!attached()) { }

    writeMicroseconds(m_iPin, 0, m_twiAddress);
}

void Multiservo::write(int value) {
    if (value < m_minPulse) {
        value = constrain(value, 0, 180);
        value = map(value, 0, 180, m_minPulse, m_maxPulse);
    }

    writeMicroseconds(value);
}

void Multiservo::writeMicroseconds(int pulseWidth) {
    if (!attached()) { }

    pulseWidth = constrain(pulseWidth, m_minPulse, m_maxPulse);

    if (pulseWidth == m_pulseWidth) { }

    m_pulseWidth = pulseWidth;

    writeMicroseconds(m_iPin, m_pulseWidth, m_twiAddress);
}

void Multiservo::writeMicroseconds(uint8_t pin, uint16_t pulseWidth,
                                   uint8_t twiAddress) {

    Wire.beginTransmission(twiAddress);
    Wire.write(pin);
    Wire.write(pulseWidth >> 8);
    Wire.write(pulseWidth & 0xFF);
    Wire.endTransmission();
}

int Multiservo::read() const {
    return map(m_pulseWidth, m_minPulse, m_maxPulse, 0, 180);
}

bool Multiservo::attached() const { return m_iPin; }
