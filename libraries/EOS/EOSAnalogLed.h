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
#ifndef _EOS_EOSANALOGLED_H_
#define _EOS_EOSANALOGLED_H_

#include <Arduino.h>

class EOSAnalogLed
{
private:
	int _pin;
	uint8_t _level;
	bool _power;
	
public:
	EOSAnalogLed(int pin);
	void begin();
	
	void setLevel(uint8_t level);
	uint8_t getLevel();
	void setPower(bool on);
	bool getPower();
};

inline void EOSAnalogLed::setLevel(uint8_t level)
{
	_level = level;
	if (_power)
	{
		analogWrite(_pin, level);	
	}
}

inline uint8_t EOSAnalogLed::getLevel()
{
	return _level;
}

inline void EOSAnalogLed::setPower(bool on)
{
	_power = on;
	if (on)
	{
		analogWrite(_pin, _level);
	}
	else
	{
		analogWrite(_pin, 0);
	}
}

inline bool EOSAnalogLed::getPower()
{
	return _power;
}

#endif