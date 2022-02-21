/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include "Test.h"

class cTestRandom final : public cTest
{
public:
    cTestRandom(bool verbose, uint32_t addressBits);

private:
    Result writeCycle(uint32_t val, const cLedsList& leds) const override;
    Result readCycle(uint32_t val, const cLedsList& leds) const override;
};
