/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "TestPlain.h"
#include "Dram.h"
#include "LedsList.h"
#include "PinsConfig.h"

cTestPlain::cTestPlain(bool verbose)
    : cTest(verbose)
{
}

cTest::Result cTestPlain::doTestImpl(uint32_t val, const cDram& dram, const cLedsList& leds, Error& error) const
{
    Serial.println("| All bits set to: " + String(val));
    Serial.flush();

    val %= 2; // just clamp value to 0..1

    const uint16_t addressBits = dram.getAddressBits();
    const uint16_t size = 1 << addressBits;

    for (uint16_t col = 0; col < size; col++)
    {
        const bool enabled = m_verbose && col % addressBits == 0;

        for (uint16_t row = 0; row < size; row++)
        {
            dram.writeToAddress(row, col, val);
        }

        for (uint16_t row = 0; row < size; row++)
        {
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
