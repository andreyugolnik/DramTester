/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include <Arduino.h>

class cButton final
{
public:
    cButton(uint32_t btn);

    bool update();

private:
    const uint32_t m_btn;

private:
    uint32_t m_lastTime = 0;
};
