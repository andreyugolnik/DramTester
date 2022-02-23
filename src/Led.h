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
    enum class Port
    {
        B,
        C,
        D
    };

    cLed(Port port, uint8_t bit, uint32_t timeout, bool initialState);

    void setup(uint32_t timeout, bool initialState);

    void setState(bool on);

    void setTimeout(uint32_t timeout);

    void update();

private:
    void led(bool on) const;
    void ledOff() const;

private:
    const Port m_port;
    const uint8_t m_led;
    uint32_t m_timeout;
    bool m_state;

private:
    uint32_t m_lastTime = 0;
    bool m_flushEnabled = true;
};
