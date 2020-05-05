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
#ifndef _EOS_SERVO_H_
#define _EOS_SERVO_H_

#include <Arduino.h>
#include "../Servo/Servo.h"

class EOSServo {
private:
	uint8_t _pin;
	Servo _servo;
	int _minValue;
	int _maxValue;
	int _defaultValue;
	int _signalValue;

public:
	EOSServo(uint8_t pin);

	// Attaches the servo and begins controling it.
	void begin();

	// Sets the minium signal in microseconds.
	void setMin(int minMs);

	// Gets the minium signal in microseconds.
	int getMin();

	// Sets the maximum signal in microseconds.
	void setMax(int minMs);

	// Gets the maximum signal in microseconds.
	int getMax();

	// Sets the default value which will used at startup.
	void setDefault(int defaultMs);

	// Returns the default value which will be used at startup.
	int getDefault();

	// Sets the signal value in microseconds.
	void setValue(int ms);

	// Writes the config for this to the eprom
	// using a given offset into the eprom address
	// space.
	// Returns the number bytes written.
	uint8_t writeEprom(uint8_t offset);

	// Reads the config for this from the eprom
	// using a given offset into the eprom address
	// space.
	// Returns the number bytes read.
	uint8_t readEprom(uint8_t offset);
};

#endif