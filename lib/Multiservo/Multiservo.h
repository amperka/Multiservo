
#ifndef MULTISERVO_H
#define MULTISERVO_H

#include <inttypes.h>

#define XSERVO_DEFAULT_ADDRESS      0x47
#define XSERVO_DEFAULT_MIN_PULSE    544
#define XSERVO_DEFAULT_MAX_PULSE    2400
#define XSERVO_DEFAULT_ATTEMPTS     4

#define XSERVO_MAX_PIN              18
#define XSERVO_PULSE_LIMIT          19000

enum XSERVO_ERROR
{
    XS_OK = 0,
    XS_DATA_TOO_LONG = 1,
    XS_NACK_ON_ADDRESS = 2,
    XS_NACK_ON_DATA = 3,
    XS_TWI_ERROR = 4,
    XS_BAD_PIN = 5,
    XS_BAD_PULSE = 6
};


XSERVO_ERROR xservoWriteMicroseconds(
        uint8_t pin, uint16_t pulseWidth, 
        uint8_t twiAddress = XSERVO_DEFAULT_ADDRESS, 
        uint8_t retryAttempts = XSERVO_DEFAULT_ATTEMPTS);


class XServo
{
    public:
        XServo(uint8_t twiAddress = XSERVO_DEFAULT_ADDRESS);

        XSERVO_ERROR attach(
                int pin, 
                int minPulse = XSERVO_DEFAULT_MIN_PULSE, 
                int maxPulse = XSERVO_DEFAULT_MAX_PULSE);

        XSERVO_ERROR detach();

        XSERVO_ERROR write(int value);
        XSERVO_ERROR writeMicroseconds(int pulseWidth);

        int read() const;
        bool attached() const;

    private:
        uint8_t _pin;
        uint8_t _twiAddress;
        uint16_t _minPulse;
        uint16_t _maxPulse;
        uint16_t _pulseWidth;
};

#endif
