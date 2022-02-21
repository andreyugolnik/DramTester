/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include <Arduino.h>

class cLed final
{
public:
    cLed(uint32_t led, uint32_t timeout, bool initialState);

    void setup(uint32_t timeout, bool initialState);

    void setState(bool on);

    void setTimeout(uint32_t timeout);

    void update();

private:
    const uint32_t m_led;
    uint32_t m_timeout;
    bool m_state;

private:
    uint32_t m_lastTime = 0;
    bool m_flushEnabled = true;
};
