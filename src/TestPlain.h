/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include "Test.h"

class cTestPlain final : public cTest
{
public:
    cTestPlain(bool verbose, uint32_t addressBits);

private:
    Result writeCycle(uint32_t val, const cLedsList& leds) const override;
    Result readCycle(uint32_t val, const cLedsList& leds) const override;
};
