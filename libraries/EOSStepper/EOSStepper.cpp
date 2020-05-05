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
#include "EOSStepper.h"

static bool timerActive = false;
static EOSStepper *interruptStepper;
static unsigned long isrTicks = 0;
static unsigned long isrInterval = 0;

// static volatile unsigned long ticks = 0;
// static volatile uint16_t nextInterval = 100;

/// Setup timer signals based on timer defined
#if defined(_useTimer1)
SIGNAL (TIMER1_COMPA_vect) 
#elif defined(_useTimer3)
SIGNAL (TIMER3_COMPA_vect) 
#elif defined(_useTimer4)
SIGNAL (TIMER4_COMPA_vect) 
#elif defined(_useTimer5)
SIGNAL (TIMER5_COMPA_vect) 
#endif
{
	isrInterval = interruptStepper->runStepper();
	if (isrInterval == 0) { isrInterval = 500; }

#if defined(_useTimer1)
	OCR1A = (isrInterval<<1)-1;
#elif defined(_useTimer3)	
	OCR3A = (isrInterval<<1)-1;
#elif defined(_useTimer4)
	OCR4A = (isrInterval<<1)-1;
#elif defined(_useTimer5)
	OCR5A = (isrInterval<<1)-1;
#endif	
}

/// Initialize interupt timer
static void initISR(EOSStepper *stepper)
{  
	interruptStepper = stepper;
#if defined (_useTimer1)
    TCCR1A = 0;             // normal counting mode 
    TCCR1B = _BV(CS11);		// 8 prescaler
    TCCR1B |= _BV(WGM12);   // Turn on CTC mode
    OCR1A = 899;			// Call every 100us
#if defined(__AVR_ATmega8__)|| defined(__AVR_ATmega128__)
	TIFR |= _BV(OCF1A);      // clear any pending interrupts; 
	TIMSK |=  _BV(OCIE1A) ;  // enable the output compare interrupt  
#else
	// here if not ATmega8 or ATmega128
	TIFR1 |= _BV(OCF1A);     // clear any pending interrupts; 
	TIMSK1 |=  _BV(OCIE1A) ; // enable the output compare interrupt 
#endif    
#elif defined (_useTimer3)
	TCCR3A = 0;             // normal counting mode 
	TCCR3B = _BV(CS31);     // no prescaler
	TCCR3B |= _BV(WGM32);
    OCR3A = 899;			// Call every 100us
#if defined(__AVR_ATmega128__)
    TIFR |= _BV(OCF3A);     // clear any pending interrupts;   
	ETIMSK |= _BV(OCIE3A);  // enable the output compare interrupt     
#else  
	TIFR3 = _BV(OCF3A);     // clear any pending interrupts; 
	TIMSK3 =  _BV(OCIE3A) ; // enable the output compare interrupt      
#endif
#elif defined (_useTimer4)
	TCCR4A = 0;             // normal counting mode 
	TCCR4B = _BV(CS41);     // 8 prescaler 
	TCCR4B |= _BV(WGM42);
    OCR4A = 899;			// Call every 100us
	TIFR4 = _BV(OCF4A);     // clear any pending interrupts; 
	TIMSK4 =  _BV(OCIE4A) ; // enable the output compare interrupt
#elif defined (_useTimer5)
    TCCR5A = 0;             // normal counting mode 
    TCCR5B = _BV(CS51);     // 8 prescaler
	TCCR5B |= _BV(WGM52);
    OCR5A = 899;			// Call every 100us
    TIFR5 = _BV(OCF5A);     // clear any pending interrupts; 
    TIMSK5 =  _BV(OCIE5A) ; // enable the output compare interrupt      
#endif    
    timerActive = true;
}

EOSStepper::EOSStepper(uint8_t directionPin, uint8_t stepPin, uint8_t enablePin)
{
	_directionPin.setPin(directionPin);
	_stepPin.setPin(stepPin);
	_enablePin.setPin(enablePin);  	
	_currentPosition = 0;
	_targetPosition = 0;
	_speed = 0;
	_sleepTime = 0;
	_direction = DIRECTION_NONE;
	_enablePin.set();
	_sleep = true;
  	setIdleSleep(1000000);
  	setMaxSpeed(400);	
}

void EOSStepper::begin(bool timer)
{
	_directionPin.setMode(OUTPUT);
	_stepPin.setMode(OUTPUT);
	_enablePin.setMode(OUTPUT);
	_directionPin.clear();
	_stepPin.clear();

	if (timer && !timerActive)
	{
		_isTimer = true;
		initISR(this);
	}
}

// Returns the current idle sleep period
unsigned long EOSStepper::getIdleSleep()
{
	return _idleSleep;
}
// Sets the ammount of idle time in microseconds before disabling the driver.  0 to disable sleep.
void EOSStepper::setIdleSleep(unsigned long idleSleep)
{
	_idleSleep = idleSleep;
}

// Returns the acceleration in steps per second per second
uint16_t EOSStepper::getMaxSpeed()
{
	return _maxSpeed;
}

// Sets the acceleration in steps per second per second
void EOSStepper::setMaxSpeed(uint16_t maxSpeed)
{
	if (maxSpeed == 0) { return; }
	if (_maxSpeed != maxSpeed)
	{
		_maxSpeed = maxSpeed;
		unsigned long minInterval = 1000000L / _maxSpeed;
		unsigned long initialInterval = minInterval / (sqrt(STEPPER_RAMP_SIZE) - sqrt(STEPPER_RAMP_SIZE-1));
		for(int i=0; i<STEPPER_RAMP_SIZE; i++)
		{			
			_intervalRamp[i] = initialInterval * (sqrt(i+1)-sqrt(i));
		}  		
	}
}

// Returns the current step position of the motor
long EOSStepper::getCurrentPosition()
{
	return _currentPosition;
}

// Sets the current step position of the motor
void EOSStepper::setCurrentPosition(long position)
{
    _targetPosition = _currentPosition = position;
}

// Returns the current target step position
long EOSStepper::getTargetPosition() 
{
	return _targetPosition;
}

// Sets a new target step position
void EOSStepper::setTargetPosition(long newPosition)
{
    uint8_t oldSREG = SREG;
    cli();
	_targetPosition = newPosition;
	if (_direction == DIRECTION_NONE)
	{
		long distance = _targetPosition - _currentPosition;
		if (distance > 0)
		{
			_direction = DIRECTION_CW;
			_directionPin.clear();
			_enablePin.clear();
			_nextStepTime = micros();
			_sleep = false;
			_sleepTime = 0;
		}
		else if (distance < 0)
		{
			_direction = DIRECTION_CCW;
			_directionPin.set();
			_enablePin.clear();
			_nextStepTime = micros();
			_sleep = false;
			_sleepTime = 0;
		}		
	}
	SREG = oldSREG; 
}

// Increments the current target step position by a number of steps
void EOSStepper::incrementTargetPosition(long increment)
{
	setTargetPosition(_targetPosition+increment);
}

volatile uint8_t EOSStepper::getSpeed()
{
	return _speed;
}

uint16_t EOSStepper::runStepper()
{
	uint16_t interval = 0;
	if (_direction != DIRECTION_NONE)
	{
		long distance = _targetPosition - _currentPosition;
		// Step
		if (_direction == DIRECTION_CW)
		{
		    _currentPosition += 1;
		    distance--;
		}
		else
		{
		    _currentPosition -= 1;
		    distance++;
		}			
		_stepPin.set();
		interval = _intervalRamp[_speed];

		// Check for complete and apply acceleration
		if (distance == 0)
		{
			_speed = 0;
			_direction = DIRECTION_NONE;
			interval = 0;
		}
		else if (distance > 0)
		{
			if (_speed >= distance || _direction == DIRECTION_CCW)
			{
				if (_speed > 0) { _speed -= 1; }
				if (_speed == 0 && _direction == DIRECTION_CCW)
				{
					_direction = DIRECTION_CW;
					_directionPin.clear();
				}					
			}
			else if (_speed < distance && _speed < MAX_SPEED && _direction == DIRECTION_CW)
			{
				_speed = _speed += 1;
			}
		}
		else if (distance < 0)
		{
			if (_speed >= -distance || _direction == DIRECTION_CW)
			{
				if (_speed > 0) { _speed -= 1; }
				if (_speed == 0 && _direction == DIRECTION_CW)
				{
					_direction = DIRECTION_CCW;
					_directionPin.set();
				}						
			}
			else if (_speed < -distance && _speed < MAX_SPEED && _direction == DIRECTION_CCW)
			{
				_speed += 1;
			}
		}

		_stepPin.clear();
	}

	return interval;
}

void EOSStepper::process()
{
	unsigned long current = micros();
	if (_direction == DIRECTION_NONE && _idleSleep > 0)
	{		
		if (_sleepTime == 0)
		{
			_sleepTime = current + _idleSleep;
		}
	 	else if ((long)(current - _sleepTime) > 0)
		{
			_enablePin.set();
		}
	}

	if (!timerActive && !_isTimer)
	{
		if ((long)(current - _nextStepTime) > 0)
		{
			_nextStepTime = current + runStepper();
		}
	}
}

// Flag indicating wheter we are still moving or not.
bool EOSStepper::isMoving()
{
	return _currentPosition != _targetPosition;
}