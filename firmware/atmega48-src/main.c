
#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "twi-slave.h"

// TODO: Check for collisions with Seeed
#define TWI_ADDRESS     0x47

// ==========================================================================
#define SERVO_COUNT     18
#define MAX_PULSE       2500
#define US_DIVIDER      4UL

/* ==========================================================================
 * Servo signal port mappings
 */

#define SS_MASK_B   (uint8_t)0b00111111
#define SS_MASK_C   (uint8_t)0b00001111
#define SS_MASK_D   (uint8_t)0b11111111

#define SS_PB(i)     (i)
#define SS_PC(i)     (8 + i)
#define SS_PD(i)     (16 + i)

uint8_t servoAddresses[] = {
    SS_PD(3),
    SS_PD(4),
    SS_PD(5),
    SS_PD(6),
    SS_PD(7),
    SS_PB(0),

    SS_PB(1),
    SS_PB(2),
    SS_PB(3),
    SS_PB(4),
    SS_PB(5),
    SS_PC(0),

    SS_PC(1),
    SS_PC(2),
    SS_PC(3),
    SS_PD(0),
    SS_PD(1),
    SS_PD(2)
};

struct Node
{
    uint16_t pulseWidth;
    uint8_t  servoCount;

    uint8_t  umaskB;
    uint8_t  umaskC;
    uint8_t  umaskD;
};

struct Node nodes[SERVO_COUNT];
uint8_t servo2node[SERVO_COUNT];
uint8_t nodeCount;


void initNodes(void)
{
    nodeCount = 1;
    struct Node* node = &nodes[0];

    node->pulseWidth = 0;
    node->servoCount = SERVO_COUNT;
    node->umaskB = ~SS_MASK_B;
    node->umaskC = ~SS_MASK_C;
    node->umaskD = (uint8_t)~SS_MASK_D;

    for (int i = 0; i < SERVO_COUNT; ++i)
        servo2node[i] = 0;
}

void initOutputs(void)
{
    // all servo signal pins are outputs
    DDRB = SS_MASK_B;
    DDRC = SS_MASK_C;
    DDRD = SS_MASK_D;
}

void initTimer(void)
{
    TCCR1B |= _BV(CS11) | _BV(CS10); // Start timer at F_CPU/64 = 250 KHz
    TCCR1B |= _BV(WGM12); // Configure timer 1 for CTC mode
    OCR1A   = 20000UL / US_DIVIDER - 1; // Set CTC compare value to 50 Hz (standard for servos)
}

void setServo(uint8_t servoIndex, uint16_t pulseWidth)
{
    uint8_t servoAddress = servoAddresses[servoIndex];
    uint8_t inode = servo2node[servoIndex];
    struct Node* node = &nodes[inode];

    if (node->pulseWidth == pulseWidth) {
        // nothing has actually changed
        return;
    }

    /*uint8_t maskB = 1 << servoAddress;
    uint8_t maskC = 1 << (servoAddress - 8);
    uint8_t maskD = 1 << (servoAddress - 16);*/

    uint8_t maskB = 0;
    uint8_t maskC = 0;
    uint8_t maskD = 0;

    if (servoAddress < 8) {
        maskB = 1 << servoAddress;
    } else if (servoAddress < 16) {
        maskC = 1 << (servoAddress - 8);
    } else {
        maskD = 1 << (servoAddress - 16);
    }

    /*
     *    Remove servo from its current node
     */

    if (--node->servoCount) {
        // The servo we're removing from its current node
        // shares the node with others, so just remove it
        // from mask, leaving node in place
        node->umaskB |= maskB;
        node->umaskC |= maskC;
        node->umaskD |= maskD;
    } else {
        // Remove the node, i.e. pull the tail
        --nodeCount;
        for (int i = inode; i < nodeCount; ++i)
            nodes[i] = nodes[i + 1];

        for (int i = 0; i < SERVO_COUNT; ++i)
            if (servo2node[i] >= inode)
                --servo2node[i];
    }

    /*
     *    Add servo to a new node
     */

    // Find a proper place for new pulse width
    inode = 0;
    while (nodes[inode].pulseWidth < pulseWidth && inode < nodeCount)
        ++inode;

    node = &nodes[inode];

    if (node->pulseWidth == pulseWidth && node->servoCount) {
        // We can share existing node with others
        ++node->servoCount;
        node->umaskB &= ~maskB;
        node->umaskC &= ~maskC;
        node->umaskD &= ~maskD;
    } else {
        // We have to insert a new node at `inode` position
        // So push the tail to make a room
        for (int i = nodeCount; i > inode; --i)
            nodes[i] = nodes[i - 1];
        ++nodeCount;

        for (int i = 0; i < SERVO_COUNT; ++i)
            if (servo2node[i] >= inode)
                ++servo2node[i];

        // Prepare new node
        node->pulseWidth = pulseWidth;
        node->servoCount = 1;
        node->umaskB = ~maskB;
        node->umaskC = ~maskC;
        node->umaskD = ~maskD;
    }

    servo2node[servoIndex] = inode;
}


void makePulses(void)
{
    uint8_t maskB = SS_MASK_B;
    uint8_t maskC = SS_MASK_C;
    uint8_t maskD = SS_MASK_D;
    uint8_t nc = nodeCount;
    struct Node* node = nodes;

    // Check if very first node has zero-length pulse
    // If so, some servos are disconnected and we shouldn't
    // ever make high pulse for them
    if (node->pulseWidth == 0) {
        maskB &= node->umaskB;
        maskC &= node->umaskC;
        maskD &= node->umaskD;
        --nc; ++node;
    }

    cli();

    PORTB |= maskB;
    PORTC |= maskC;
    PORTD |= maskD;

    // Remember the time when we set high pulses
    // and start local counting
    uint16_t tcntBias = TCNT1;
    TCNT1 = 0;

    // One by one take a subsequent node, wait
    // for the moment we should drop the pulse according
    // to pulse width, and do it
    for ( ; nc--; ++node) {
        int16_t counterVal = node->pulseWidth / US_DIVIDER;

        // wait for the moment to drop the signal to low
        while (TCNT1 < counterVal)
            ;

        // drop signal
        PORTB &= node->umaskB;
        PORTC &= node->umaskC;
        PORTD &= node->umaskD;
    }

    // Restore counter value
    TCNT1 += tcntBias;

    sei();
}


void processMessage(uint8_t* message)
{
    uint8_t servoIndex = message[0];
    uint16_t pulseWidth = (message[1] << 8) | message[2];

    if (servoIndex >= SERVO_COUNT)
        return;

    /*nodes->pulseWidth = pulseWidth;
    return;*/
    setServo(servoIndex, pulseWidth);
}


int main(void)
{
    /*wdt_enable(WDTO_8S);*/

    initNodes();
    initOutputs();
    initTimer();

    // init TWI
    TWI_Slave_Initialise(TWI_ADDRESS << TWI_ADR_BITS);
    TWI_Start_Transceiver();

    // enable interrupts
    sei();

    uint8_t rxMessage[TWI_BUFFER_SIZE];

    for(;;) {
        /*wdt_reset();*/
        if (!TWI_Transceiver_Busy() && TWI_statusReg.RxDataInBuf) {
            TWI_Get_Data_From_Transceiver(rxMessage, TWI_BUFFER_SIZE);
            processMessage(rxMessage);
            TWI_Start_Transceiver();
        }

        if (TIFR1 & _BV(OCF1A)) { 
            makePulses();

            // clear the CTC flag (writing a logic one to the set flag clears it) 
            TIFR1 = _BV(OCF1A); 
        } 
    }
}
