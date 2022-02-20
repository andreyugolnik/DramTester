/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include "Test.h"

class cTestReversed final : public cTest
{
public:
    cTestReversed(bool verbose, uint32_t addressBits)
        : cTest(verbose, addressBits)
    {
    }

    Result doTest(uint32_t val, const cLedsList& leds) const override
    {
        Serial.println("--------------------------------------------------------");

        {
            Serial.print("Reversing bits starting with: ");
            Serial.println(val);
            Serial.println("--------------------------------------------------------");
            Serial.flush();

            const uint32_t start = millis();
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
            printElapsedTime("Write: ", millis() - start);
        }

        {
            const uint32_t start = millis();
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
            printElapsedTime("Read OK: ", millis() - start);
        }

        Serial.println("--------------------------------------------------------");
        Serial.flush();

        return returnOK();
    }
};
