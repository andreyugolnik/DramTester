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
    cTestReversed(bool verbose, uint32_t addressBits);

private:
    Result doTestImpl(uint32_t val, const cLedsList& leds, Error& error) const override;
};
