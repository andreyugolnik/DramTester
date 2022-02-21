/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include <Arduino.h>

class cLedsList;

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

    Result doTest(uint32_t value, const cLedsList& leds) const;

protected:
    Result returnOK() const;
    Result returnError(uint32_t row, uint32_t col, uint32_t val) const;

    void printElapsedTime(const String& prefix, uint32_t elapsed) const;

    void setAddress(uint32_t row, uint32_t col) const;
    uint32_t getAddress(uint32_t row, uint32_t col) const;

    void setRAS(uint32_t row) const;
    void setCAS(uint32_t col) const;

protected:
    cTest(bool verbose, uint32_t addressBits);

private:
    virtual Result writeCycle(uint32_t value, const cLedsList& leds) const = 0;
    virtual Result readCycle(uint32_t value, const cLedsList& leds) const = 0;

protected:
    const bool m_verbose;
    const uint32_t m_addressBits;
};
