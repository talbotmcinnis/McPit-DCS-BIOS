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
#include "EOSDigitalBank.h"

EOSDigitalBank::EOSDigitalBank()
{
	for(int i=0; i<DEBOUNCE_MAX_CHECKS; i++)
	{
		_states[i] = 0;
	}
	_stateIndex = 0;
	_debouncedState = 0;
}

bool EOSDigitalBank::poll(uint8_t newState)
{
	_states[_stateIndex++] = newState;
	if (_stateIndex >= DEBOUNCE_MAX_CHECKS)
	{
		_stateIndex = 0;
	}
	uint8_t newDebouce = 0xFF;
	for(int i=0; i<DEBOUNCE_MAX_CHECKS; i++)
	{
		newDebouce &= _states[i];
	}
	boolean changed = newDebouce != _debouncedState;
	_debouncedState = newDebouce;
	return changed;
}

uint8_t EOSDigitalBank::getState()
{
	return _debouncedState;
}

void EOSDigitalBank::setState(uint8_t newState)
{
	_debouncedState = newState;
}