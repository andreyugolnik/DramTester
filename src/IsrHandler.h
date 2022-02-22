/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include <Arduino.h>

class cIsrHandler final
{
public:
    static cIsrHandler* getInstance();

    typedef void (*Handler)(void* user);

    uint32_t addHandler(Handler handler, void* user);
    void removeHandler(uint32_t id);

public:
    void update();

private:
    cIsrHandler();

    uint32_t getNextId() const;

private:
    static constexpr uint32_t MaxHandlers = 3;
    static constexpr uint32_t InvalidHandlerId = 0;

private:
    uint32_t m_count = 0;

    struct HandlerImpl
    {
        uint32_t id;
        Handler handler;
        void* user;
    };

    HandlerImpl m_handlers[MaxHandlers];
};
