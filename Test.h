/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include "LedsList.h"
#include "PinsConfig.h"

class cTest
{
public:
    virtual ~cTest() = default;

    struct Result
    {
        bool hasError;
        uint32_t row;
        uint32_t col;
        uint32_t val;

        operator bool() const
        {
            return hasError;
        }
    };

    virtual Result doTest(uint32_t value, const cLedsList& leds) const = 0;

protected:
    Result returnOK() const
    {
        return { false, 0, 0, 0 };
    }

    Result returnError(uint32_t row, uint32_t col, uint32_t val) const
    {
        return { true, row, col, val };
    }

    void printElapsedTime(const String& prefix, uint32_t elapsed) const
    {
        Serial.println(prefix + String(elapsed) + "ms");
        Serial.flush();
    }

    void setAddress(uint32_t row, uint32_t col) const
    {
        setRAS(row);

        digitalWrite(WRITE, LOW);

        setCAS(col);

        digitalWrite(WRITE, HIGH);
        digitalWrite(CAS, HIGH);
        digitalWrite(RAS, HIGH);
    }

    uint32_t getAddress(uint32_t row, uint32_t col) const
    {
        setRAS(row);
        setCAS(col);

        const uint32_t val = digitalRead(DOUT);

        digitalWrite(CAS, HIGH);
        digitalWrite(RAS, HIGH);

        return val;
    }

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

protected:
    cTest(bool verbose, uint32_t addressBits)
        : m_verbose(verbose)
        , m_addressBits(addressBits)
    {
    }

protected:
    const bool m_verbose;
    const uint32_t m_addressBits;
};
