/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "TestRandom.h"
#include "Dram.h"
#include "LedsList.h"
#include "PinsConfig.h"
#include "Random.h"

cTestRandom::cTestRandom(bool verbose)
    : cTest(verbose)
{
}

cTest::Result cTestRandom::doTestImpl(uint32_t seed, const cDram& dram, const cLedsList& leds, Error& error) const
{
    Serial.println("| Random bit values with seed: 0x" + String(seed, HEX));
    Serial.flush();

    cRandom rnd;

    const uint16_t addressBits = dram.getAddressBits();
    const uint16_t size = 1 << addressBits;

    for (uint16_t col = 0; col < size; col++)
    {
        const bool enabled = m_verbose && col % addressBits == 0;

        rnd.setSeed(seed + col);
        for (uint16_t row = 0; row < size; row++)
        {
            const uint8_t val = rnd.getUInt(0, 1);
            dram.writeToAddress(row, col, val);
        }

        rnd.setSeed(seed + col);
        for (uint16_t row = 0; row < size; row++)
        {
            const uint8_t val = rnd.getUInt(0, 1);
            if (dram.readFromAddress(row, col) != val)
            {
                setError(row, col, val, error);
                return Result::Error;
            }
        }

        if (enabled)
        {
            Serial.print(".");
            Serial.flush();
        }
    }

    return Result::OK;
}
