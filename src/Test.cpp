/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "Test.h"
#include "LedsList.h"
#include "Dram.h"
#include "Timer.h"

cTest::cTest(bool verbose)
    : m_verbose(verbose)
{
}

cTest::Result cTest::doTest(uint32_t value, const cDram& dram, const cLedsList& leds) const
{
    Serial.println("o------------------------------------------------------o");

    dram.setPowerOn();

    cTimer timer;

    Error error;
    const Result result = doTestImpl(value, dram, leds, error);

    if (result == Result::OK)
    {
        Serial.println("| Test is OK: " + String(timer.getDuration()) + "ms");
    }
    else
    {
        Serial.println("| FAILED at row " + String(error.row)
                       + " col " + String(error.col)
                       + ", was expecting " + String(error.val)
                       + " got " + String(!error.val)
                       + ".");
    }

    dram.setPowerOff();

    Serial.println("o------------------------------------------------------o");
    Serial.flush();

    return result;
}

void cTest::setError(uint32_t row, uint32_t col, uint32_t val, Error& error) const
{
    error = { row, col, val };
}
