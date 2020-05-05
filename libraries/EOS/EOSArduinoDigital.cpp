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
#include "EOSArduinoDigital.h"

EOSArduinoDigital::EOSArduinoDigital(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4,
					  uint8_t pin5, uint8_t pin6, uint8_t pin7, uint8_t pin8)
{
	_pins[0].setPin(pin1);
	_pins[1].setPin(pin2);
	_pins[2].setPin(pin3);
	_pins[3].setPin(pin4);
	_pins[4].setPin(pin5);
	_pins[5].setPin(pin6);
	_pins[6].setPin(pin7);
	_pins[7].setPin(pin8);

	_pinCount = 8;
	for(uint8_t i = 1; i < 8; i++) 
	{
		if (_pins[i].getPin() == 0xff)
		{
			_pinCount = i;
			break;
		}
	}
}

void EOSArduinoDigital::begin()
{
	for(int i=0; i<_pinCount; i++)
	{
		pinMode(_pins[i].getPin(), INPUT_PULLUP);
	}
	_state.setState(readPins());
}

boolean EOSArduinoDigital::process()
{
	boolean changed = false;
	long current = millis();
	if (current - _nextPoll > 0)
	{
		changed |= _state.poll(readPins());
		_nextPoll = current + DEBOUNCE_POLL_INTERVAL;
	}
	return changed;
}

uint8_t EOSArduinoDigital::readPins()
{
	uint8_t port = 0x00;
	for(int i=0; i<_pinCount; i++)
	{
		if (digitalRead(_pins[i].getPin()) == LOW)
		{
			port |= 1 << i;			
		}
	}
	return port;
}

uint8_t EOSArduinoDigital::getState()
{
	return _state.getState();
}