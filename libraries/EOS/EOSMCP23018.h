/*
	Copyright 2013 Craig Courtney

    This file is part of EOS-Firmware.

    EOS-Firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    EOS-Firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with EOS-Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _EOS_EOSMCP23018_H_
#define _EOS_EOSMCP23018_H_

#include <Arduino.h>

#define IODIRA    (uint8_t)0x00
#define IODIRB    (uint8_t)0x01
#define IPOLA    (uint8_t)0x02
#define IPOLB    (uint8_t)0x03
#define GPINTENA    (uint8_t)0x04
#define GPINTENB    (uint8_t)0x05
#define DEFVALA    (uint8_t)0x06
#define DEFVALB   (uint8_t)0x07
#define INTCONA    (uint8_t)0x08
#define INTCONB   (uint8_t)0x09
#define IOCONA      (uint8_t)0x0A
#define IOCONB      (uint8_t)0x0B
#define GPPUA      (uint8_t)0x0C
#define GPPUB      (uint8_t)0x0D
#define INTFA      (uint8_t)0x0E
#define INTFB      (uint8_t)0x0F
#define INTCAPA   (uint8_t)0x10
#define INTCAPB   (uint8_t)0x11
#define GPIOA      (uint8_t)0x12
#define GPIOB      (uint8_t)0x13
#define OLATA      (uint8_t)0x14
#define OLATB      (uint8_t)0x15

// Implments a bank of 16 LED outputs using an i2c conencted
// MCP23018 chip.  Power (on/off) state is supported but dimming
// (setLevel) is not supported.
class EOSMCP23018
{
private:
    uint8_t _address;
    uint8_t _portAState;
    uint8_t _portBState;
    
public:
    // Create an EOSMCP23018 object using a given i2c address.
    EOSMCP23018(uint8_t address);

    // Initializes the LED bank.  Should be called from setup().
    void begin();

    // Sets the power state for an LED on this bank.
    // led - led to set the power for (0-15)
    // power - state of the power (true = on, false = off)
    void setPower(uint8_t led, bool power);
};

#endif