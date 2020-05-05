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
#include "EOSServo.h"

#include "../EEPROM/EEPROM.h"


EOSServo::EOSServo(uint8_t pin)
{
    _pin = pin;
    _minValue = 544;
    _maxValue = 2400;
    _defaultValue = 1500;
    _signalValue = _defaultValue;
}

void EOSServo::begin()
{
    _servo.attach(_pin, _minValue, _maxValue);
    _servo.writeMicroseconds(_defaultValue);
}

void EOSServo::setMin(int minMs)
{
    _minValue = minMs;
    if (_defaultValue < _minValue) { _defaultValue = _minValue; }
    if (_signalValue < _minValue) { _signalValue = _minValue; }
    if (_servo.attached())
    {
        _servo.detach();
        _servo.attach(_pin, _minValue, _maxValue);
        _servo.writeMicroseconds(_signalValue);
    }
}

int EOSServo::getMin()
{
    return _minValue;
}

void EOSServo::setMax(int maxMs)
{
    _maxValue = maxMs;
    if (_defaultValue > _maxValue) { _defaultValue = _maxValue; }
    if (_signalValue > _maxValue) { _signalValue = _maxValue; }
    if (_servo.attached())
    {
        _servo.detach();
        _servo.attach(_pin, _minValue, _maxValue);
        _servo.writeMicroseconds(_signalValue);
    }
}

int EOSServo::getMax()
{
    return _maxValue;
}

void EOSServo::setDefault(int defaultMs)
{
    _defaultValue = defaultMs;
    if (_defaultValue < _minValue) { _defaultValue = _minValue; }
    if (_defaultValue > _maxValue) { _defaultValue = _maxValue; }
}

int EOSServo::getDefault()
{
    return _defaultValue;
}

void EOSServo::setValue(int ms)
{
    _signalValue = ms;
    if (_signalValue < _minValue) { _signalValue = _minValue; }
    if (_signalValue > _maxValue) { _signalValue = _maxValue; }
    _servo.writeMicroseconds(_signalValue);
}

uint8_t EOSServo::writeEprom(uint8_t offset)
{
    EEPROM.write(offset++, (uint8_t)((_minValue >> 8) & 0xff));
    EEPROM.write(offset++, (uint8_t)((_minValue) & 0xff));

    EEPROM.write(offset++, (uint8_t)((_maxValue >> 8) & 0xff));
    EEPROM.write(offset++, (uint8_t)((_maxValue) & 0xff));

    EEPROM.write(offset++, (uint8_t)((_defaultValue >> 8) & 0xff));
    EEPROM.write(offset++, (uint8_t)((_defaultValue) & 0xff));

    return 6;
}

uint8_t EOSServo::readEprom(uint8_t offset)
{
    _minValue = (EEPROM.read(offset++) << 8) | (EEPROM.read(offset++));
    _maxValue = (EEPROM.read(offset++) << 8) | (EEPROM.read(offset++));
    _defaultValue = (EEPROM.read(offset++) << 8) | (EEPROM.read(offset++));
   return 6;
}