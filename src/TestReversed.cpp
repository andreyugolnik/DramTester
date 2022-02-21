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

cTestReversed::Result cTestReversed::writeCycle(uint32_t val, const cLedsList& leds) const
{
    Serial.print("Reversing bits starting with: ");
    Serial.println(val);
    Serial.println("--------------------------------------------------------");
    Serial.flush();

    for (uint32_t col = 0; col < (1 << m_addressBits); col++)
    {
        const bool enabled = m_verbose && col % m_addressBits == 0;

        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            digitalWrite(DIN, val);
            val = !val;
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

cTestReversed::Result cTestReversed::readCycle(uint32_t val, const cLedsList& leds) const
{
    for (uint32_t col = 0; col < (1 << m_addressBits); col++)
    {
        const bool enabled = m_verbose && col % m_addressBits == 0;

        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            if (getAddress(row, col) != val)
            {
                return returnError(row, col, val);
            }

            val = !val;

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
