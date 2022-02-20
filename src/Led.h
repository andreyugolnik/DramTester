/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

class cLed final
{
public:
    cLed(uint32_t led, uint32_t timeout, bool initialState)
        : m_led(led)
        , m_timeout(timeout)
        , m_state(initialState)
    {
    }

    void setup(uint32_t timeout, bool initialState)
    {
        pinMode(m_led, OUTPUT);
        m_lastTime = millis();
        m_timeout = timeout;
        m_state = initialState;
        m_flushEnabled = true;
    }

    void setState(bool on)
    {
        digitalWrite(m_led, on ? HIGH : LOW);
        m_state = on;
        m_flushEnabled = false;
    }

    void setTimeout(uint32_t timeout)
    {
        m_timeout = timeout;
    }

    void update()
    {
        if (m_flushEnabled)
        {
            const uint32_t currentTime = millis();

            if (currentTime - m_lastTime >= m_timeout)
            {
                m_lastTime = currentTime;

                m_state = !m_state;
                digitalWrite(m_led, m_state ? HIGH : LOW);
            }
        }
    }

private:
    const uint32_t m_led;
    uint32_t m_timeout;
    bool m_state;

private:
    uint32_t m_lastTime = 0;
    bool m_flushEnabled = true;
};
