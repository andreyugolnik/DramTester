/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

class cButton final
{
public:
    cButton(uint32_t btn)
        : m_btn(btn)
    {
    }

    void setup()
    {
        pinMode(m_btn, INPUT_PULLUP);
    }

    bool update()
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

private:
    const uint32_t m_btn;

private:
    uint32_t m_lastTime = 0;
};
