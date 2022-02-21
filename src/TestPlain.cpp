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

cTestPlain::Result cTestPlain::writeCycle(uint32_t val, const cLedsList& leds) const
{
    Serial.print("Setting all bits set to: ");
    Serial.println(val);
    Serial.println("--------------------------------------------------------");
    Serial.flush();

    digitalWrite(DIN, val);

    for (uint32_t col = 0; col < (1 << m_addressBits); col++)
    {
        const bool enabled = m_verbose && col % m_addressBits == 0;

        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            setAddress(row, col);

            leds.update();

            if (enabled && row % m_addressBits == 0)
            {
                Serial.print(".");
                Serial.flush();
            }
        }

        if (enabled)
        {
            Serial.println("");
        }
    }

    return returnOK();
}

cTestPlain::Result cTestPlain::readCycle(uint32_t val, const cLedsList& leds) const
{
    // Reverse DIN in case DOUT is floating
    digitalWrite(DIN, !val);

    for (uint32_t col = 0; col < (1 << m_addressBits); col++)
    {
        const bool enabled = m_verbose && col % m_addressBits == 0;

        for (uint32_t row = 0; row < (1 << m_addressBits); row++)
        {
            if (getAddress(row, col) != val)
            {
                return returnError(row, col, val);
            }

            leds.update();

            if (enabled && row % m_addressBits == 0)
            {
                Serial.print(".");
                Serial.flush();
            }
        }

        if (enabled)
        {
            Serial.println("");
        }
    }

    return returnOK();
}
