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
#ifndef _EOS_COMMANDS_H_
#define _EOS_COMMANDS_H_

// Broadcast Addressing
// Broadcast address are 240-249 coresponding with groups 0-9.  Broadcast
// address 250 is listened to by all boards regardless of broadcast group
// setting.  Broadcast addresses should not be used with commands which
// response packets.  If done this will cause data conflict on the bus.

#define PACKET_COMMAND_RESET 1
// Command tells board to rest itself to default state
//
// Data: 0 byte data length
//
// Response: none

#define PACKET_COMMAND_INFO 130
// Command sent to device to ask for it's capabilities
//
// Data: 0 data length
//
// Response: 19 byte data length
// Bytes 1-8 = Board Name
// Bytes 9-12 = Firmware Version
// Byte 13 = Number of digital intputs
// Byte 14 = Number of analog inputs
// Byte 15 = Number of rotary encoders
// Byte 16 = Number of led outputs
// Byte 17 = Number of steppers motor outputs
// Byte 18 = Number of servo motor outputs
// Byte 19 = Number of alpha/numeric displays (0-255)
// Byte 20 = Group address this board listens to.
#define PACKET_RESPONSE_INFO 66

#define PACKET_COMMAND_SET_ADDRESS 3
// Command which resets board address, should always follow up with a bus reset
//  so the new address is registered in the polling loop.
//
// Data: 1 Byte data length
// Byte 1 New Address - Valid addresses are 1-100
//
// Response: None

#define PACKET_COMMAND_SET_NAME 4
// Command which sets a board name.
//
// Data: 8 byte data length
// Byte 1-8: Name (zero terminated if shorter than 8)

#define PACKET_COMMAND_SET_GROUP 5
// Command which resets board broadcast group. 
//
// Data: 1 Byte data length
// Byte 1 New Address - Valid addresses are 0-9
// 
// Response: None

#define PACKET_COMMAND_GET_CONFIG 134
// Command to retrieve  config settings for an interface board.
//
// Data: data length and content are specified per board
// Reponse: data length and content specified per board
#define PACKET_RESOPNSE_GET_CONFIG 70

#define PACKET_COMMAND_SET_CONFIG 7
// Command to sets config setting for an interface board.
//
// Data: data length and content are specified per board
// Response: data length and content are specified per board

#define PACKET_COMMAND_BACKLIGHT_SET_LEVEL 8
// Command to set the backlight brightness.
//
// Data: 1 byte data length
// Byte 1 = Brightness (0 = Full Dark, 255 = Full Bright)
//
// Response: None

#define PACKET_COMMAND_BACKLIGHT_POWER 9
// Command to set backlight power.
//
// Data: 1 byte data length
// Byte 1 = Power (0 = Off, 1 = On)
// 
// Response: None

#define PACKET_COMMAND_LED_SET_LEVEL 10
// Command to set the led brightness.
//
// Data: 2 byte data length
// Byte 1 = LED to set brightness for
// Byte 2 = Brightness (0 = Full Dark, 255 = Full Bright)
//
// Response: None

#define PACKET_COMMAND_LED_POWER 11
// Command to set led power.
//
// Data: 2 byte data length
// Byte 1 = LED to set power for
// Byte 2 = Power (0 = Off, 1 = On)
// 
// Response: None

#define PACKET_COMMAND_DISPLAY_SETTEXT 12
// Packet which will set the display text for a given display.
//
// Data: Variable
// Byte 1 = Display ID
// Byte 2 = Text Length
// Bytes 3-x = Text to be displayed.

#define PACKET_COMMAND_STEPPER_ZERO_POSITION 13
// Packet which will set the current position of a stepper motor
// without moving the stepper.
//
// Data: 5 bytes data length
// Byte 1 = Stepper Number
// Bytes 2-5 = Long value of new current position
//
// Response: None

#define PACKET_COMMAND_STEPPER_TARGET_POSITION_SET 14
// Packet which will set the target position of a stepper motor.
//
// Data: 5 bytes data length
// Byte 1    = Stepper Number
// Bytes 2-5 = Long value of new current position
//
// Response: None

#define PACKET_COMMAND_SERVO_SET_SIGNAL 15
// Packet which will set the signal value for the servo.
//
// Data: 3 bytes data length
// Byte 1 = Servo Number
// Bytes 2-3 = Singal value in microseconds

#define PACKET_COMMAND_SERVO_SET_CONFIG 16
// Packet which will set the configuration parameters
// for a servo.
//
// Data: 7 bytes data length
// Byte 1 = Servo Number
// Bytes 2-3 = Minimum signal value in microseconds.
// Bytes 4-5 = Maximum signal value in microseconds.
// Bytes 6-7 = Default signal value in microseconds.

#define PACKET_COMMAND_SERVO_GET_CONFIG 145
// Packet which will get the configuration parameters
// for a servo.
//
// Data: 1 data length
// Byte 1 = Servo Number
//
// Response: 7 bytes data length
// Byte 1 = Servo Number
// Bytes 2-3 = Minimum signal value in microseconds.
// Bytes 4-5 = Maximum signal value in microseconds.
// Bytes 6-7 = Default signal value in microseconds.
#define PACKET_RESPONSE_SERVO_GET_CONFIG 81

#define PACKET_COMMAND_STEPPER_SET_CONFIG 18
// Packet which will set the configuration parameters
// for a stepper.
//
// Data: 1 data length
// Byte 1 = Stepper Number
// Bytes 2-3 = Maximum speed in steps per second.
// Bytes 4-7 = Idle sleep timeout.

#define PACKET_COMMAND_STEPPER_GET_CONFIG 147
// Packet which will get the configuration parameters
// for a stepper.
//
// Data: 1 data length
// Byte 1 = Stepper Number
//
// Response: 7 bytes data length
// Byte 1 = Stepper Number
// Bytes 2-3 = Maximum speed in steps per second.
// Bytes 4-7 = Idle sleep timeout.
#define PACKET_RESPONSE_STEPPER_GET_CONFIG 83

#define PACKET_COMMAND_POLL 191
// Command to poll input changes for an interface board
//
// Data: 0 data length
//
// Response: data length depending on inputs with a zero length pack data
//          meaning no change since last poll.
// 1 bit per digital input in 8bit banks
// 2 bytes per analong input
// 1 byte per rotary encoder (signed byte indicating number of steps per last poll)
#define PACKET_RESPONSE_POLL 127

// *************************
// Packet Structure
// Byte 0 = Destination
// Byte 1 = Source
// Byte 2 = Command
// Byte 3 = Data Length
// Byte 4 - X = Packet Data
// Byte X + 1 (Last Byte) = CheckSum

#endif