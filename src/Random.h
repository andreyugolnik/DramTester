/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include <Arduino.h>

class cRandom final
{
public:
    void setSeed(uint32_t seed)
    {
        m_state[0] = seed;
        m_state[1] = seed << 1;
        m_state[2] = seed << 2;
        m_state[3] = seed << 3;
    }

    uint32_t getGetSeed() const
    {
        return m_state[0];
    }

    // TODO: consider to implement one of http://www.cse.yorku.ca/~oz/marsaglia-rng.html
    // algorithm, KISS for example.

    uint32_t getUInt() const
    {
        return xorshift128();
    }

    uint32_t getUInt(uint32_t min, uint32_t max) const
    {
        const auto rnd = getUInt();
        return min + rnd % (max - min + 1);
    }

private:
    // Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs"
    // https://en.wikipedia.org/wiki/Xorshift
    uint32_t xorshift128() const
    {
        uint32_t t = m_state[3];
        t ^= t << 11;
        t ^= t >> 8;

        m_state[3] = m_state[2];
        m_state[2] = m_state[1];
        const uint32_t s = m_state[0];
        m_state[1] = s;

        t ^= s;
        t ^= s >> 19;
        m_state[0] = t;

        return t;
    }

public:
    static const uint32_t RandMax = 0xffffffff;

private:
    mutable uint32_t m_state[4] = { 123456789, 362436069, 521288629, 88675123 };
};
