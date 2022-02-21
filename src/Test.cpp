/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "Test.h"
#include "LedsList.h"
#include "Timer.h"

cTest::cTest(bool verbose, uint32_t addressBits)
    : m_verbose(verbose)
    , m_addressBits(addressBits)
{
}

cTest::Result cTest::doTest(uint32_t value, const cLedsList& leds) const
{
    Serial.println("o------------------------------------------------------o");

    cTimer timer;

    Error error;
    const Result result = doTestImpl(value, leds, error);

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

    Serial.println("o------------------------------------------------------o");
    Serial.flush();

    return result;
}

void cTest::setError(uint32_t row, uint32_t col, uint32_t val, Error& error) const
{
    error = { row, col, val };
}

void cTest::writeToAddress(uint32_t row, uint32_t col) const
{
    setRAS(row);
    digitalWrite(WRITE, LOW);
    setCAS(col);

    digitalWrite(CAS, HIGH);
    digitalWrite(RAS, HIGH);
}

uint32_t cTest::readFromAddress(uint32_t row, uint32_t col) const
{
    setRAS(row);
    digitalWrite(WRITE, HIGH);
    setCAS(col);

    const uint32_t val = digitalRead(DOUT);

    digitalWrite(CAS, HIGH);
    digitalWrite(RAS, HIGH);

    return val;
}
