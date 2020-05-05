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
#include "EOSMCP23018.h"
#include "../Wire/Wire.h"

EOSMCP23018::EOSMCP23018(uint8_t address)
{
	_portAState = 0xFF;
	_portBState = 0xFF;
	_address = address;
}

void EOSMCP23018::begin()
{
	Wire.beginTransmission(_address);
	Wire.write(IODIRA);
	Wire.write((uint8_t)0x00);
	Wire.write((uint8_t)0x00);
	Wire.write((uint8_t)0xFF);
	Wire.write((uint8_t)0xFF);
	Wire.endTransmission();

	Wire.beginTransmission(_address);
	Wire.write(GPIOA);
	Wire.write((uint8_t)0xFF);
	Wire.write((uint8_t)0xFF);
	Wire.endTransmission();	
}

void EOSMCP23018::setPower(uint8_t led, bool power)
{
	if (led < 8)
	{
		if (power)
		{
			_portAState &= ~(1 << led);
		}
		else
		{
			_portAState |= 1 << led;
		}			
	}
	else if (led < 16)
	{
		if (power)
		{
			_portBState &= ~(1 << (led-8));
		}
		else
		{
			_portBState |= 1 << (led-8);
		}			
	}
	else 
	{
		// This exandper only has 16 outputs.
		return;
	}

	Wire.beginTransmission(_address);
	Wire.write(GPIOA);
	Wire.write((uint8_t)_portAState);
	Wire.write((uint8_t)_portBState);
	Wire.endTransmission();
}