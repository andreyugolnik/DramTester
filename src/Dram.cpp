/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "Dram.h"
#include "PinsConfig.h"

// All these pins are valid for Arduino Nano only

//             -------------------
// (PB0)  5 --| A0  |       |     |
// (PB1)  7 --| A1  | 4164/ |     |
// (PB2)  6 --| A2  | 41256 |     |
// (PB3) 12 --| A3  |       |     |
// (PB4) 11 --| A4  |       |  DO |-- 14 (PD3)
// (PB5) 10 --| A5  |       |     |
// (PC0) 13 --| A6  |       |     |
// (PC1)  9 --| A7  |       |     |
// (PC2)  1 --| A8* |       |     |
//            |-----|       |-----|
// (PD2)  2 --| DI  |       |     |
//            |-----|       |     |
// (PD5)  4 --o RAS |       | +5V |-- 8 (PC3)
// (PD4) 15 --o CAS |       |     |
// (PD6)  3 --o WE  |       | GND |-- 16
//             -------------------

// PB0..PB5 + PC0..PC2 is used to connect to the address line A0..A8 of the RAM
// Up to 9 bits for address line
// 8 bits for 4164
// 9 bits for 41256
#define ADDRESS_BITS 8

#define TOBIT(bit) (1 << bit)

#define DI_HIGH() PORTD |= TOBIT(2)
#define DI_LOW() PORTD &= ~TOBIT(2)

#define RAS_HIGH() PORTD |= TOBIT(5)
#define RAS_LOW() PORTD &= ~TOBIT(5)

#define CAS_HIGH() PORTD |= TOBIT(4)
#define CAS_LOW() PORTD &= ~TOBIT(4)

#define WE_HIGH() PORTD |= TOBIT(6)
#define WE_LOW() PORTD &= ~TOBIT(6)

#define POWER_ON() PORTC |= TOBIT(3)
#define POWER_OFF() PORTC &= ~TOBIT(3)

cDram::cDram()
{
    // reserve 8 (9) bits for address line
    // according to ADDRESS_BITS
    DDRB = 0x3f; // PB0..PB5 0b00111111
#if ADDRESS_BITS == 8
    DDRC = 0x03; // PC0..PC2 0b00000011
#else
    DDRC = 0x07; // PC0..PC2 0b00000111
#endif

    DDRD &= ~TOBIT(3); // DO as input

    DDRD |= TOBIT(2); // DI as output
    DDRD |= TOBIT(5); // RAS as output
    DDRD |= TOBIT(4); // CAS as output
    DDRD |= TOBIT(6); // WE as output

    CAS_HIGH();
    RAS_HIGH();
    WE_HIGH();
}

void cDram::setPowerOn() const
{
    POWER_ON();
}

void cDram::setPowerOff() const
{
    POWER_OFF();
}

uint16_t cDram::getAddressBits() const
{
    return ADDRESS_BITS;
}

void cDram::writeToAddress(uint16_t row, uint16_t col, uint8_t val) const
{
    RAS_HIGH();
    CAS_HIGH();

    setAddress(row);
    RAS_LOW();

    WE_LOW();
    val
        ? DI_HIGH()
        : DI_LOW();

    setAddress(col);
    CAS_LOW();

    _delay_us(1);

    RAS_HIGH();
    CAS_HIGH();
}

uint8_t cDram::readFromAddress(uint16_t row, uint16_t col) const
{
    RAS_HIGH();
    CAS_HIGH();
    WE_HIGH();

    setAddress(row);
    RAS_LOW();

    setAddress(col);
    CAS_LOW();

    _delay_us(1);

    const uint8_t val = (PIND & TOBIT(3)) != 0;

    RAS_HIGH();
    CAS_HIGH();

    return val;
}

void cDram::setAddress(uint16_t address) const
{
    PORTB = address & 0x3f;
#if ADDRESS_BITS == 8
    // 0b11111100
    PORTC = (PORTC & 0xf8) | ((address >> 6) & 0x07);
#else
    // 0b11111100
    PORTC = (PORTC & 0xfc) | ((address >> 6) & 0x03);
#endif
}

void cDram::regenerate() const
{
    RAS_HIGH();
    CAS_HIGH();

    for (uint16_t row = 0; row < (1 << ADDRESS_BITS); row++)
    {
        setAddress(row);
        RAS_LOW();
        RAS_HIGH();
    }
}
