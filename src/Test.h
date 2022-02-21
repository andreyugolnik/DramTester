/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include <Arduino.h>
#include "PinsConfig.h"

class cLedsList;

class cTest
{
public:
    virtual ~cTest() = default;

    enum class Result
    {
        OK,
        Error
    };

    Result doTest(uint32_t value, const cLedsList& leds) const;

protected:
    cTest(bool verbose, uint32_t addressBits);

protected:
    struct Error
    {
        uint32_t row;
        uint32_t col;
        uint32_t val;
    };

    void setError(uint32_t row, uint32_t col, uint32_t val, Error& error) const;

    void writeToAddress(uint32_t row, uint32_t col) const;
    uint32_t readFromAddress(uint32_t row, uint32_t col) const;

private:
    void setRAS(uint32_t row) const
    {
        PORTB = row & 0x3f;
        PORTC = (PORTC & 0xf0) | ((row >> 6) & 0x0f);
        digitalWrite(RAS, LOW);
    }

    void setCAS(uint32_t col) const
    {
        PORTB = col & 0x3f;
        PORTC = (PORTC & 0xf0) | ((col >> 6) & 0x0f);
        digitalWrite(CAS, LOW);
    }

private:
    virtual Result doTestImpl(uint32_t value, const cLedsList& leds, Error& error) const = 0;

protected:
    const bool m_verbose;
    const uint32_t m_addressBits;
};
