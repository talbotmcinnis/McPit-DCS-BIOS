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
#ifndef _EOS_EOSDIGITALLED_H_
#define _EOS_EOSDIGITALLED_H_

#include <Arduino.h>
#include "EOSDigitalPin.h"

class EOSDigitalLed
{
private:
	EOSDigitalPin _pin;
	bool _power;
	
public:
	EOSDigitalLed(int pin);
	void begin();
	
	void setPower(bool on);
	bool getPower();
};

inline void EOSDigitalLed::setPower(bool on)
{
	_power = on;
	if (on)
	{
		_pin.set();
	}
	else
	{
		_pin.clear();
	}
}

inline bool EOSDigitalLed::getPower()
{
	return _power;
}

#endif