/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "TestReversed.h"
#include "LedsList.h"
#include "PinsConfig.h"

cTestReversed::cTestReversed(bool verbose, uint32_t addressBits)
    : cTest(verbose, addressBits)
{
}

cTest::Result cTestReversed::doTestImpl(uint32_t value, const cLedsList& leds, Error& error) const
{
    Serial.println("| Reversing bits starting with: " + String(value));
    Serial.flush();

    for (uint32_t col = 0; col < (1 << m_addressBits); col++)
    {
        const bool enabled = m_verbose && col % m_addressBits == 0;

        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            const uint32_t val = (value + row) % 2;

            digitalWrite(DIN, val);
            writeToAddress(row, col);

            leds.update();
        }

        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            const uint32_t val = (value + row) % 2;

            if (readFromAddress(row, col) != val)
            {
                setError(row, col, val, error);
                return Result::Error;
            }

            leds.update();
        }

        if (enabled)
        {
            Serial.print(".");
            Serial.flush();
        }
    }

    return Result::OK;
}
