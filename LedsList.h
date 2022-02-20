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
    void setup()
    {
        m_green.setup(300, true);
        m_red.setup(300, false);
    }

    void update()
    {
        m_green.update();
        m_red.update();
    }

    void switchOff()
    {
        m_green.setState(false);
        m_red.setState(false);
    }

    void showIdle()
    {
        setup();
    }

    void showRed()
    {
        m_green.setState(false);
        m_red.setTimeout(150);
    }

    void showGreen()
    {
        m_red.setState(false);
        m_green.setTimeout(150);
    }

private:
    cLed m_green{ LED_G, 300, true };
    cLed m_red{ LED_R, 300, false };
};
