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
#ifndef _EOS_STEPPER_H_
#define _EOS_STEPPER_H_

#include <Arduino.h>
#include "EOSDigitalPin.h"

#define MAX_STEPPERS 4
#define INVALID_STEPPER 255
#define STEPPER_RAMP_SIZE 40
#define MAX_SPEED 39

// Identify which timer to use based on platform.  We use the same timers that the servo
// library would use.  This means backlight and other pins can be the same on Servo and
// stepper boards, but you can not drive servos and steppers from the same board.
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define _useTimer5
#elif defined(__AVR_ATmega32U4__)  
#define _useTimer1 
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define _useTimer3
#elif defined(__AVR_ATmega128__) ||defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
#define _useTimer3
#else  // everything else
#define _useTimer1
#endif

enum Direction
{
	DIRECTION_NONE = 0,
	DIRECTION_CW = 1,  
    DIRECTION_CCW = 2
};

class EOSStepper {
private:
	EOSDigitalPin _enablePin;
	EOSDigitalPin _directionPin;
	EOSDigitalPin _stepPin;

	bool _isTimer;

	bool _sleep;

	// Index into the ramp table indicating current speed
	volatile uint8_t _speed;

	// How many microseconds do we wait to disable driver after
	// movement complete. (0 to disable)
	unsigned long _idleSleep;

	// Maximum speed in steps per second.
	uint16_t _maxSpeed;

	// Current direction of travel
	volatile uint8_t _direction;

	uint16_t _intervalRamp[STEPPER_RAMP_SIZE];

    // Next step timestamp for main loop drive 
    volatile unsigned long _nextStepTime;
    // Timestamp when motor should be put to slep
    unsigned long _sleepTime;

    // Current step positions
	volatile long _currentPosition;
	// Target step position we should be moving towards
	long _targetPosition;

public:
	// Constructor for a stepper.
	EOSStepper(uint8_t directionPin, uint8_t stepPin, uint8_t enablePin = 0xff);

	// Initializes all appropriate pins and settings.  Should be called in setup().
	// One stepper per board can be driven by interupt.  The first to be called 
	// with timer = true will register as an interrupt and be driven with smooth
	// pusles even during heavy bus activity.
	void begin(bool timer = false);
	
	// Returns the current idle sleep period
	unsigned long getIdleSleep();
	// Sets the ammount of idle time in microseconds before disabling the driver.  0 to disable sleep.
	void setIdleSleep(unsigned long idleSleep);

	// Returns the max speed for this stepper in steps per second
	uint16_t getMaxSpeed();
	// Sets the max speed in steps per second
	void setMaxSpeed(uint16_t maxSpeed);

	// Returns the current step position of the motor
	long getCurrentPosition();
	// Sets the current step position of the motor
	void setCurrentPosition(long currentPosition);
	
	// Returns the current target step position
	long getTargetPosition();
	// Sets a new target step position
	void setTargetPosition(long newPosition);
	// Increments the current target step position by a number of steps
	void incrementTargetPosition(long increment);

	volatile uint8_t getSpeed();

	// Returns the current speed
	volatile unsigned long getLastStep();

	// Returns the current speed
	unsigned long getInterval();

	volatile unsigned long getTicks();

	void process();
	uint16_t runStepper();

	// Flag indicating wheter we are still moving or not.
	bool isMoving();
};
#endif