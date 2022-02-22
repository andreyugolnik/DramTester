/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "IsrHandler.h"
#include <assert.h>

ISR(TIMER0_COMPA_vect)
{
    cIsrHandler::getInstance()->update();
}

cIsrHandler::cIsrHandler()
{
    // TCCR0A |= (1 << WGM01);              // reset on match
    // OCR0A = 0xF9;                        // start overlow at 0xf9
    TIMSK0 |= (1 << OCIE0A); // enable interrupt on a-register match

    // CS02 | CS01 | CS00
    // 0   |  0   |  0     stopped
    // 0   |  0   |  1     clk
    // 0   |  1   |  0     clk/8
    // 0   |  1   |  1     clk/64
    // 1   |  0   |  0     clk/256
    // 1   |  0   |  1     clk/1024
    // TCCR0B |= (1 << CS01) | (1 << CS00); // clk/64
    // sei();
}

cIsrHandler* cIsrHandler::getInstance()
{
    static cIsrHandler IsrHandler;
    return &IsrHandler;
}

uint32_t cIsrHandler::addHandler(Handler handler, void* user)
{
    assert(handler != nullptr);
    assert(m_count < MaxHandlers);

    HandlerImpl& h = m_handlers[m_count++];
    h.id = getNextId();
    h.handler = handler;
    h.user = user;

    return h.id;
}

void cIsrHandler::removeHandler(uint32_t id)
{
    for (uint32_t i = 0; i < m_count; i++)
    {
        HandlerImpl& h = m_handlers[i];
        if (h.id == id)
        {
            assert(h.handler != nullptr);
            m_handlers[i] = m_handlers[--m_count];
            return;
        }
    }
}

void cIsrHandler::update()
{
    for (uint32_t i = 0; i < m_count; i++)
    {
        HandlerImpl& h = m_handlers[i];
        h.handler(h.user);
    }
}

uint32_t cIsrHandler::getNextId() const
{
    static uint32_t HandleId = 1;

    const uint32_t id = HandleId != InvalidHandlerId
        ? HandleId
        : (HandleId + 1);

    HandleId = id + 1;

    return id;
}
