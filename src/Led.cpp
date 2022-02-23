/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "Led.h"

cLed::cLed(Port port, uint8_t bit, uint32_t timeout, bool initialState)
    : m_port(port)
    , m_led(1 << bit)
    , m_timeout(timeout)
    , m_state(initialState)
{
    setup(timeout, initialState);
}

void cLed::setup(uint32_t timeout, bool initialState)
{
    switch (m_port)
    {
    case Port::B:
        DDRB |= ~m_led;
        break;
    case Port::C:
        DDRC |= ~m_led;
        break;
    case Port::D:
        DDRD |= ~m_led;
        break;
    }

    m_lastTime = millis();
    m_timeout = timeout;
    m_state = initialState;
    m_flushEnabled = true;
}

void cLed::setState(bool on)
{
    led(on);
    m_state = on;
    m_flushEnabled = false;
}

void cLed::setTimeout(uint32_t timeout)
{
    m_timeout = timeout;
}

void cLed::update()
{
    if (m_flushEnabled)
    {
        const uint32_t currentTime = millis();

        if (currentTime - m_lastTime >= m_timeout)
        {
            m_lastTime = currentTime;

            m_state = !m_state;
            led(m_state);
        }
    }
}

void cLed::led(bool on) const
{
    switch (m_port)
    {
    case Port::B:
        on
            ? PORTB |= m_led
            : PORTB &= ~m_led;
        break;

    case Port::C:
        on
            ? PORTC |= m_led
            : PORTC &= ~m_led;
        break;

    case Port::D:
        on
            ? PORTD |= m_led
            : PORTD &= ~m_led;
        break;
    }
}
