/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include "Led.h"

// (PC4) LED green
// (PC5) LED red

class cLedsList final
{
public:
    cLedsList();
    ~cLedsList();

    void clear();

    void update();

    void switchOff();
    void showIdle();
    void showRed();
    void showGreen();

private:
    uint32_t m_hadlerId = 0;
    cLed m_green{ cLed::Port::C, 4, 300, true };
    cLed m_red{ cLed::Port::C, 5, 300, false };
};
