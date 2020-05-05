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
#include "EOSMCP23017.h"
#include "../Wire/Wire.h"

EOSMCP23017::EOSMCP23017(byte address)
{
	_nextPoll = millis();
	_address = address;
}

void EOSMCP23017::begin()
{
	Wire.beginTransmission(_address);
	Wire.write(IPOLA);
	Wire.write((byte)0xFF);
	Wire.write((byte)0xFF);
	Wire.endTransmission();

	Wire.beginTransmission(_address);
	Wire.write(GPPUA);
	Wire.write((byte)0xFF);
	Wire.write((byte)0xFF);
	Wire.endTransmission();
	
	poll();
	_bank1.setState(Wire.read());
	_bank2.setState(Wire.read());
}

boolean EOSMCP23017::process()
{
	boolean changed = false;
	long current = millis();
	if (current - _nextPoll > 0)
	{
		poll();
		changed |= _bank1.poll(Wire.read());
		changed |= _bank2.poll(Wire.read());
		_nextPoll = current + DEBOUNCE_POLL_INTERVAL;
	}
	return changed;
}

void EOSMCP23017::poll()
{
	boolean changed = false;
	Wire.beginTransmission(_address);
	Wire.write((byte)GPIOA);
	Wire.endTransmission();
	
	Wire.requestFrom(_address,(byte)2);
	while (Wire.available() < 2) { }
}

byte EOSMCP23017::getPortAState()
{
	return _bank1.getState();
}

byte EOSMCP23017::getPortBState()
{
	return _bank2.getState();
}