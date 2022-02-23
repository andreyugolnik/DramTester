/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "TestReversed.h"
#include "Dram.h"
#include "LedsList.h"
#include "PinsConfig.h"

cTestReversed::cTestReversed(bool verbose)
    : cTest(verbose)
{
}

cTest::Result cTestReversed::doTestImpl(uint32_t value, const cDram& dram, const cLedsList& leds, Error& error) const
{
    Serial.println("| Reversing bits starting with: " + String(value));
    Serial.flush();

    const uint16_t addressBits = dram.getAddressBits();
    const uint16_t size = 1 << addressBits;

    for (uint32_t col = 0; col < size; col++)
    {
        const bool enabled = m_verbose && col % addressBits == 0;

        for (uint16_t row = 0; row < size; row++)
        {
            const uint8_t val = (value + row) % 2;

            dram.writeToAddress(row, col, val);
        }

        for (uint16_t row = 0; row < size; row++)
        {
            const uint8_t val = (value + row) % 2;

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
