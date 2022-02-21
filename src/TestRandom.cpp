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

cTestRandom::Result cTestRandom::writeCycle(uint32_t seed, const cLedsList& leds) const
{
    randomSeed(seed);

    Serial.print("Random bit values with seed: 0x");
    Serial.println(seed, HEX);
    Serial.println("--------------------------------------------------------");
    Serial.flush();

    for (uint32_t col = 0; col < (1 << m_addressBits); col++)
    {
        const bool enabled = m_verbose && col % m_addressBits == 0;

        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            const uint32_t val = random(0, 2);
            digitalWrite(DIN, val);
            setAddress(row, col);

            leds.update();

            if (enabled && row % m_addressBits == 0)
            {
                Serial.print(".");
            }
        }

        if (enabled)
        {
            Serial.println("");
        }
    }

    return returnOK();
}

cTestRandom::Result cTestRandom::readCycle(uint32_t seed, const cLedsList& leds) const
{
    randomSeed(seed);

    for (uint32_t col = 0; col < (1 << m_addressBits); col++)
    {
        const bool enabled = m_verbose && col % m_addressBits == 0;

        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            const uint32_t val = random(0, 2);
            if (getAddress(row, col) != val)
            {
                return returnError(row, col, val);
            }

            leds.update();

            if (enabled && row % m_addressBits == 0)
            {
                Serial.print(".");
            }
        }

        if (enabled)
        {
            Serial.println("");
        }
    }

    return returnOK();
}
