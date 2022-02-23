/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include <Arduino.h>

class cDram;
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

    Result doTest(uint32_t value, const cDram& dram, const cLedsList& leds) const;

protected:
    cTest(bool verbose);

protected:
    struct Error
    {
        uint32_t row;
        uint32_t col;
        uint32_t val;
    };

    void setError(uint32_t row, uint32_t col, uint32_t val, Error& error) const;

private:
    virtual Result doTestImpl(uint32_t value, const cDram& dram, const cLedsList& leds, Error& error) const = 0;

protected:
    const bool m_verbose;
};
