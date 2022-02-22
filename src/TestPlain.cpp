/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "TestPlain.h"
#include "LedsList.h"
#include "PinsConfig.h"

cTestPlain::cTestPlain(bool verbose, uint32_t addressBits)
    : cTest(verbose, addressBits)
{
}

cTest::Result cTestPlain::doTestImpl(uint32_t val, const cLedsList& leds, Error& error) const
{
    Serial.println("| All bits set to: " + String(val));
    Serial.flush();

    val %= 2; // just clamp value to 0..1

    for (uint32_t col = 0; col < (1 << m_addressBits); col++)
    {
        const bool enabled = m_verbose && col % m_addressBits == 0;

        digitalWrite(DIN, val);
        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            writeToAddress(row, col);

            // leds.update();
        }

        digitalWrite(DIN, !val);
        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            if (readFromAddress(row, col) != val)
            {
                setError(row, col, val, error);
                return Result::Error;
            }

            // leds.Lupdate();
        }

        if (enabled)
        {
            Serial.print(".");
            Serial.flush();
        }
    }

    return Result::OK;
}
