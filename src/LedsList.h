/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include "Led.h"
#include "PinsConfig.h"

class cLedsList final
{
public:
    ~cLedsList();

    void setup();
    void clear();

    void update();

    void switchOff();
    void showIdle();
    void showRed();
    void showGreen();

private:
    uint32_t m_hadlerId = 0;
    cLed m_green{ LED_G, 300, true };
    cLed m_red{ LED_R, 300, false };
};
