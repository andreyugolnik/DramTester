/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "TestRandom.h"
#include "LedsList.h"
#include "PinsConfig.h"

cTestRandom::cTestRandom(bool verbose, uint32_t addressBits)
    : cTest(verbose, addressBits)
{
}

cTest::Result cTestRandom::doTestImpl(uint32_t seed, const cLedsList& leds, Error& error) const
{
    Serial.println("| Random bit values with seed: 0x" + String(seed, HEX));
    Serial.flush();

    for (uint32_t col = 0; col < (1 << m_addressBits); col++)
    {
        const bool enabled = m_verbose && col % m_addressBits == 0;

        randomSeed(seed + col);
        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            const uint32_t val = random(0, 2);
            digitalWrite(DIN, val);
            writeToAddress(row, col);

            // leds.update();
        }

        randomSeed(seed + col);
        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            const uint32_t val = random(0, 2);
            if (readFromAddress(row, col) != val)
            {
                setError(row, col, val, error);
                return Result::Error;
            }

            // leds.update();
        }

        if (enabled)
        {
            Serial.print(".");
            Serial.flush();
        }
    }

    return Result::OK;
}
