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
#ifndef _EOS_EOSMCP23017_H_
#define _EOS_EOSMCP23017_H_

#include "EOSDigitalBank.h"

#define IODIRA    (byte)0x00
#define IODIRB    (byte)0x01
#define IPOLA    (byte)0x02
#define IPOLB    (byte)0x03
#define GPINTENA    (byte)0x04
#define GPINTENB    (byte)0x05
#define DEFVALA    (byte)0x06
#define DEFVALB   (byte)0x07
#define INTCONA    (byte)0x08
#define INTCONB   (byte)0x09
#define IOCONA      (byte)0x0A
#define IOCONB      (byte)0x0B
#define GPPUA      (byte)0x0C
#define GPPUB      (byte)0x0D
#define INTFA      (byte)0x0E
#define INTFB      (byte)0x0F
#define INTCAPA   (byte)0x10
#define INTCAPB   (byte)0x11
#define GPIOA      (byte)0x12
#define GPIOB      (byte)0x13
#define OLATA      (byte)0x14
#define OLATB      (byte)0x15

class EOSMCP23017
{
private:
	long _nextPoll;
	byte _address;
	void poll();
	EOSDigitalBank _bank1;
	EOSDigitalBank _bank2;
	
public:
	EOSMCP23017(byte address);

	void begin();
	boolean process();
	byte getPortAState();
	byte getPortBState();
};

#endif