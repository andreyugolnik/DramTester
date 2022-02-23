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
    cTestRandom(bool verbose);

private:
    Result doTestImpl(uint32_t val, const cDram& dram, const cLedsList& leds, Error& error) const override;
};
