/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

class cTimer final
{
public:
    cTimer()
        : m_startTime(millis())
    {
    }

    void restart()
    {
        m_startTime = millis();
    }

    uint32_t getDuration() const
    {
        return millis() - m_startTime;
    }

private:
    uint32_t m_startTime = 0;
};
