/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "Button.h"

cButton::cButton(uint32_t btn)
    : m_btn(btn)
{
    pinMode(m_btn, INPUT_PULLUP);
}

bool cButton::update()
{
    const uint32_t timeout = 300;
    const uint32_t currentTime = millis();
    if (m_lastTime + timeout <= currentTime)
    {
        if (digitalRead(m_btn) == LOW)
        {
            m_lastTime = currentTime;
            return true;
        }
    }

    return false;
}
