/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "LedsList.h"
#include "IsrHandler.h"

cLedsList::cLedsList()
{
    m_hadlerId = cIsrHandler::getInstance()->addHandler([](void* user) {
        static_cast<cLedsList*>(user)->update();
    }, this);
}

cLedsList::~cLedsList()
{
    clear();
}

void cLedsList::clear()
{
    if (m_hadlerId != 0)
    {
        cIsrHandler::getInstance()->removeHandler(m_hadlerId);
        m_hadlerId = 0;
    }
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
    m_green.setup(300, true);
    m_red.setup(300, false);
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
