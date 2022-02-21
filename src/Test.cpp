/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "Test.h"
#include "LedsList.h"
#include "PinsConfig.h"
#include "Timer.h"

cTest::cTest(bool verbose, uint32_t addressBits)
    : m_verbose(verbose)
    , m_addressBits(addressBits)
{
}

cTest::Result cTest::doTest(uint32_t value, const cLedsList& leds) const
{
    Serial.println("--------------------------------------------------------");

    Result result;

    cTimer timer;

    if ((result = writeCycle(value, leds)))
    {
        return result;
    }

    const uint32_t duration = timer.getDuration();

    printElapsedTime("Write OK: ", duration);

    // ---------------------------------------------

    const uint32_t timeout = 100;
    if (timeout > duration)
    {
        delay(timeout - duration);
    }

    // ---------------------------------------------

    timer.restart();

    if ((result = readCycle(value, leds)))
    {
        return result;
    }

    printElapsedTime("Read OK: ", timer.getDuration());

    Serial.println("--------------------------------------------------------");
    Serial.flush();

    return result;
}

cTest::Result cTest::returnOK() const
{
    return { false, 0, 0, 0 };
}

cTest::Result cTest::returnError(uint32_t row, uint32_t col, uint32_t val) const
{
    return { true, row, col, val };
}

void cTest::printElapsedTime(const String& prefix, uint32_t elapsed) const
{
    Serial.println(prefix + String(elapsed) + "ms");
    Serial.flush();
}

void cTest::setAddress(uint32_t row, uint32_t col) const
{
    setRAS(row);

    digitalWrite(WRITE, LOW);

    setCAS(col);

    digitalWrite(WRITE, HIGH);
    digitalWrite(CAS, HIGH);
    digitalWrite(RAS, HIGH);
}

uint32_t cTest::getAddress(uint32_t row, uint32_t col) const
{
    setRAS(row);
    setCAS(col);

    const uint32_t val = digitalRead(DOUT);

    digitalWrite(CAS, HIGH);
    digitalWrite(RAS, HIGH);

    return val;
}

void cTest::setRAS(uint32_t row) const
{
    PORTB = row & 0x3f;
    PORTC = (PORTC & 0xf0) | ((row >> 6) & 0x0f);
    digitalWrite(RAS, LOW);
}

void cTest::setCAS(uint32_t col) const
{
    PORTB = col & 0x3f;
    PORTC = (PORTC & 0xf0) | ((col >> 6) & 0x0f);
    digitalWrite(CAS, LOW);
}
