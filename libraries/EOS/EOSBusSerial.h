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
#ifndef _EOS_EOSBUS_H_
#define _EOS_EOSBUS_H_

#include <Arduino.h>
#include "EOSDigitalPin.h"

#define PACKET_RESPONSE_TIMEOUT 150
#define PACKET_RECEIVE_TIMEOUT 100
#define PACKET_START_BYTE 0xbb
#define PACKET_LEADIN_BYTE 0x88
#define PACKET_TRAIL_BYTE 0xFF
#define PACKET_MAX_DATA 55
#define PACKET_OVERHEAD 9
#define RX_BUFFER_SIZE 80
#define TX_BUFFER_SIZE 64

enum bus_status {
    BUS_IDLE,				// Bus is idle and waiting for recieive or send traffic
    BUS_RECEIVING, 			// Bus is receiving data
    BUS_TRANSMITTING, 		// Bus is sending data
    BUS_ERROR, 				// Bus has encountered an error
	BUS_PACKETREADY, 		// Bus has a packet ready for processing
	BUS_WAITINGRESPONSE,	// Bus is waiting for a response
	BUS_TIMEOUT 			// Bus has timed out waiting for a resoponse.
};

struct ring_buffer_info
{
	volatile uint8_t head;
	volatile uint8_t tail;
};

class EosBusSerial 
{
private:
	unsigned char _txBuffer[TX_BUFFER_SIZE];
	volatile ring_buffer_info _txBufferInfo;
	uint8_t _txResponseSource;
	uint8_t _txChecksum;

	unsigned char _rxBuffer[RX_BUFFER_SIZE];
	volatile ring_buffer_info _rxBufferInfo;
	uint8_t _rxPacketOffset;
	uint8_t _rxChecksum;
	uint8_t _rxReadIndex;
	bool _packetValid;

	// UART config data
    volatile uint8_t *_ubrrh;
    volatile uint8_t *_ubrrl;
    volatile uint8_t *_ucsra;
    volatile uint8_t *_ucsrb;
    volatile uint8_t *_ucsrc;
    volatile uint8_t *_udr;
    uint8_t _rxen;
    uint8_t _txen;
    uint8_t _rxcie;
    uint8_t _udrie;
    uint8_t _u2x;

	EOSDigitalPin _txPin;		// Pin to set high for TX
	uint8_t _writeDelay;		// Milliseconds to wait before transmitting after settting TX pin

	volatile uint8_t _busState;	// State flag of bus rx/tx state

	uint8_t _packetState; 		// Current state of the protocol parser
	uint8_t _receiveRemaining;	// Excpected data length left

	uint8_t _nodeAddress; 		// Address for this node
	uint8_t _groupAddress;		// Group address for this node
	bool _promMode; 			// Flag indicating we are in promiscuous mode and should return all packets

	volatile long _timeout;		// Time in milliseconds of the next timeout

	volatile unsigned long _bufferTimeouts;	// Number of timeouts the bus has expierienced
	volatile unsigned long _packetErrors;	// Number of timeouts the bus has received a corrupt packet
	volatile unsigned long _overuns;			// Number of overruns on the receive buffer that have occured
	volatile unsigned long _collisions;		// Number of times a collisions occurs between writing / receiving

	void write(uint8_t data);	// Writes data into the output buffer
	uint8_t processByte(uint8_t in);
	bool checkTimeout();
	void resetRx();

public:
	EosBusSerial(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
      volatile uint8_t *ucsrc, volatile uint8_t *udr,
      uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udrie, uint8_t u2x);
	void begin(uint8_t txPin, unsigned long baud);
	
	// Process any available data.  Returns true if a packet is ready to be processed
	bus_status process();

	// Receives a byte into the rx buffer.
	void receiveByte(uint8_t in);

	bool sendAvailable();
	unsigned char getSendByte();
	void sendComplete();	// Writes data into the output buffer
	
	// Send packet data
	void beginCommand(uint8_t destination, uint8_t command);
	void beginResponse();
	void addData(uint8_t data);
	void addData(bool data);
	void addData(int data);
	void addData(long data);
	void cancel();
	bool send();

	// Receive Packet Data
	uint8_t getDestination();
	uint8_t getSource();
	uint8_t getCommand();
	bool isResponse();
	uint8_t getDataLength();
	uint8_t readByte();
	bool readBoolean();
	int readInt();
	long readLong();
	bool isEndOfPacketData();
	void packetProcessed();

	// Bus Configuration
	uint8_t getNodeAddress();
	void setNodeAddress(uint8_t address);
	uint8_t getGroup();
	void setGroup(uint8_t groupId);
	void setPromiscuous(bool mode);
	bool getPromiscuous();

	// Debug Support For hosts
	bool available();
	uint8_t peek();

	// Stats
	void resetStats();
	unsigned long getTimeouts();
	unsigned long getErrors();
	unsigned long getCollisions();
	unsigned long getOverruns();
};

extern EosBusSerial EosBus;

#endif