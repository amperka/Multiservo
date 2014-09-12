#ifndef MULTISERVO_H
#define MULTISERVO_H

#include <inttypes.h>

class Multiservo
{
public:
  enum Error
  {
    OK = 0,
    DATA_TOO_LONG,
    NACK_ON_ADDRESS,
    NACK_ON_DATA,
    TWI_ERROR,
    BAD_PIN,
    BAD_PULSE
  };

  Multiservo();
  Multiservo(uint8_t twiAddress);

  Error attach(int pin);
  Error attach(int pin, int minPulse, int maxPulse);

  Error detach();

  Error write(int value);
  Error writeMicroseconds(int pulseWidth);

  int read() const;
  bool attached() const;

protected:
  static const unsigned int  AddressDefault;
  static const unsigned int  PulseMinDefault;
  static const unsigned int  PulseMaxDefault;
  static const unsigned int  nAttemptsDefault;
  static const unsigned char PinInvalid;
  static const unsigned int  nPinMax;
  static const unsigned int  PulseMaxAbsolute;

  uint8_t  m_iPin;
  uint8_t  m_twiAddress;
  uint16_t m_minPulse;
  uint16_t m_maxPulse;
  uint16_t m_pulseWidth;

//private:
public:
  static Error writeMicroseconds(
                                        uint8_t pin, uint16_t pulseWidth, 
                                        uint8_t twiAddress, uint8_t retryAttempts
                                      );
};

#endif

// vim: sw=2 sts=2 ts=8:

