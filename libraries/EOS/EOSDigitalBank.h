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
#ifndef _EOS_EOSDIGITALBANK_H_
#define _EOS_EOSDIGITALBANK_H_

#include <Arduino.h>

// Amount of time in milliseconds between polls of our digital inputs
#define DEBOUNCE_POLL_INTERVAL 5
// How many polled reads do we use to debounce our intputs
#define DEBOUNCE_MAX_CHECKS 10

class EOSDigitalBank
{
private:
	uint8_t _states[DEBOUNCE_MAX_CHECKS];
	uint8_t _stateIndex;
	uint8_t _debouncedState;

public:
	EOSDigitalBank();
	bool poll(uint8_t state);
	uint8_t getState();
	void setState(uint8_t newState);
};

#endif