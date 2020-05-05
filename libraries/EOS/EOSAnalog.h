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
#ifndef _EOS_EOSANALOG_H_
#define _EOS_EOSANALOG_H_

#include <Arduino.h>

class EOSAnalog
{
private:
	long _interval;
	long _timeout;
	int _pin;
	int _value;
	
public:
	EOSAnalog(int pin, long interval);
	
	boolean process();
	int getValue();
};

inline boolean EOSAnalog::process()
{
	long current = millis();
	if (current - _timeout > 0)
	{
		int newValue = analogRead(_pin);
		int delta = newValue - _value;
		_timeout = current + _interval;
		if (delta < -2 || delta > 2)
		{
			_value = newValue;
			return true;
		}
	}
	return false;
}

inline int EOSAnalog::getValue()
{
	return _value;
}

#endif