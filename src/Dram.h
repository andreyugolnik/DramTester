/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#pragma once

#include <Arduino.h>

class cDram final
{
public:
    cDram();

    void setPowerOn() const;
    void setPowerOff() const;

    uint16_t getAddressBits() const;

    void regenerate() const;
    void writeToAddress(uint16_t row, uint16_t col, uint8_t val) const;
    uint8_t readFromAddress(uint16_t row, uint16_t col) const;

private:
    void setAddress(uint16_t address) const;
};
