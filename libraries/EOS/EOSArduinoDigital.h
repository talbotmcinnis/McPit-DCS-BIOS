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
#ifndef _EOS_EOSARDUINODIGITAL_H_
#define _EOS_EOSARDUINODIGITAL_H_

#include "EOSDigitalPin.h"
#include "EOSDigitalBank.h"

class EOSArduinoDigital
{
private:
	long _nextPoll;
	uint8_t _pinCount;
	EOSDigitalPin _pins[8];
	EOSDigitalBank _state;
	uint8_t readPins();
	
public:
	EOSArduinoDigital(uint8_t pin1, uint8_t pin2 = 0xff, uint8_t pin3 = 0xff, uint8_t pin4 = 0xff,
					  uint8_t pin5 = 0xff, uint8_t pin6 = 0xff, uint8_t pin7 = 0xff, uint8_t pin8 = 0xff);

	void begin();
	boolean process();
	uint8_t getState();	
};

#endif