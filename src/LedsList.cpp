/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "LedsList.h"

void cLedsList::setup()
{
    m_green.setup(300, true);
    m_red.setup(300, false);
}

void cLedsList::update()
{
    m_green.update();
    m_red.update();
}

void cLedsList::switchOff()
{
    m_green.setState(false);
    m_red.setState(false);
}

void cLedsList::showIdle()
{
    setup();
}

void cLedsList::showRed()
{
    m_green.setState(false);
    m_red.setTimeout(150);
}

void cLedsList::showGreen()
{
    m_red.setState(false);
    m_green.setTimeout(150);
}
