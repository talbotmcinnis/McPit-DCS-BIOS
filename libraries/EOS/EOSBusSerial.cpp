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
#include "EosBusSerial.h"

#if !defined(TXC0)
#if defined(TXC)
#define TXC0 TXC
#elif defined(TXC1)
// Some devices have uart1 but no uart0
#define TXC0 TXC1
#else
#error TXC0 not definable in EosBusSerial.cpp
#endif
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

enum bus_state {
	BUS_S_IDLE,			// Bus has nothing to do and is listening for packets
	BUS_S_RECEIVING,	// Bus is activley receiving data
	BUS_S_TRANSMITTING,	// Bus is transmitting data
	BUS_S_WAITING,		// Bus is waiting on a response
};

enum packet_state {
	PACKET_S_START,
	PACKET_S_LEADIN,
	PACKET_S_ADDRESS,
	PACKET_S_SRC,
	PACKET_S_COMMAND,
	PACKET_S_DATALEN,
	PACKET_S_DATA,
	PACKET_S_CHKSUM,
	PACKET_S_READY
};

EosBusSerial::EosBusSerial(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
      volatile uint8_t *ucsrc, volatile uint8_t *udr,
      uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udrie, uint8_t u2x)
{
	_ubrrh = ubrrh;
	_ubrrl = ubrrl;
	_ucsra = ucsra;
	_ucsrb = ucsrb;
	_ucsrc = ucsrc;
	_udr = udr;
	_rxen = rxen;
	_txen = txen;
	_rxcie = rxcie;
	_udrie = udrie;
	_u2x = u2x;

	_txBufferInfo.head = 0;
	_txBufferInfo.tail = 0;
	_txResponseSource = 0;
	_txChecksum = 0;

	_rxBufferInfo.head = 0;
	_rxBufferInfo.tail = 0;
	_rxPacketOffset = 0;
	_rxChecksum = 0;

	_nodeAddress = 31;
	_groupAddress = 120;
	_promMode = false;
	_packetState = PACKET_S_START;
	_busState = BUS_S_IDLE;
}

void EosBusSerial::begin(uint8_t txPin, unsigned long baud)
{
	// Serial.println("UDR0 is");
	// Serial.println((int)UDR0);
	// Serial.println(".");

	_txPin.setPin(txPin);
	_txPin.setMode(OUTPUT);
	_txPin.clear();

	uint16_t baud_setting;
	uint8_t config = 0x06;
	bool use_u2x = true;

#if F_CPU == 16000000UL
	// hardcoded exception for compatibility with the bootloader shipped
	// with the Duemilanove and previous boards and the firmware on the 8U2
	// on the Uno and Mega 2560.
	if (baud == 57600) {
	use_u2x = false;
	}
#endif

try_again:
  
	if (use_u2x) {
	*_ucsra = 1 << _u2x;
	baud_setting = (F_CPU / 4 / baud - 1) / 2;
	} else {
	*_ucsra = 0;
	baud_setting = (F_CPU / 8 / baud - 1) / 2;
	}

	if ((baud_setting > 4095) && use_u2x)
	{
	use_u2x = false;
	goto try_again;
	}

	// assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
	*_ubrrh = baud_setting >> 8;
	*_ubrrl = baud_setting;

  	//set the data bits, parity, and stop bits
	#if defined(__AVR_ATmega8__)
		config |= 0x80; // select UCSRC register (shared with UBRRH)
	#endif
	*_ucsrc = config;

	sbi(*_ucsrb, _rxen);
	sbi(*_ucsrb, _txen);
	sbi(*_ucsrb, _rxcie);
	cbi(*_ucsrb, _udrie);	
}

void EosBusSerial::receiveByte(unsigned char c) 
{
	if (_busState == BUS_S_IDLE) {
		_busState = BUS_S_RECEIVING;	
	}

	if (_busState == BUS_S_WAITING ||
		_busState == BUS_S_RECEIVING)
	{
		_timeout = millis() + PACKET_RECEIVE_TIMEOUT;

		int i = (unsigned int)(_rxBufferInfo.head + 1) % RX_BUFFER_SIZE;
		if (i == _rxBufferInfo.tail) 
		{
			_overuns++;
		}
		else
		{
			_rxBuffer[_rxBufferInfo.head] = c;
			_rxBufferInfo.head = i;
		}
	}
	else 
	{	
		_collisions++;			
	}
}

void EosBusSerial::beginCommand(uint8_t destination, uint8_t command)
{
	_txBufferInfo.head = 0;
	_txBufferInfo.tail = 0;
	_txChecksum = destination + command + _nodeAddress;

	write(PACKET_START_BYTE);
	write(PACKET_LEADIN_BYTE);
	write(destination);
	write(_nodeAddress);
	write(command);
	write(0);

	_txResponseSource = (command > 127) ? destination : 0;
}

void EosBusSerial::beginResponse()
{
	return beginCommand(getSource(), (getCommand() & 63) | 64);
}

void EosBusSerial::write(uint8_t data)
{
	int i = (unsigned int)(_txBufferInfo.head + 1) % TX_BUFFER_SIZE;
	if (i == _txBufferInfo.tail) 
	{
		_overuns++;
	}
	else
	{
		_txBuffer[_txBufferInfo.head] = data;
		_txBufferInfo.head = i;
	}
}

void EosBusSerial::addData(uint8_t data)
{
	write(data);
	int i = (unsigned int)(_txBufferInfo.tail + 5) % TX_BUFFER_SIZE;
	_txChecksum += data;
	_txBuffer[i]++;
}

void EosBusSerial::addData(bool data)
{
	addData((uint8_t)(data ? 1 : 0));
}

void EosBusSerial::addData(int data)
{
	addData((uint8_t)((data >> 8) & 0xff));
	addData((uint8_t)((data) & 0xff));
}

void EosBusSerial::addData(long data) 
{
	addData((uint8_t)((data >> 24) & 0xff));
	addData((uint8_t)((data >> 16) & 0xff));
	addData((uint8_t)((data >> 8) & 0xff));
	addData((uint8_t)((data) & 0xff));
}

void EosBusSerial::cancel() 
{
	resetRx();
	_busState = available() ? BUS_S_RECEIVING : BUS_S_IDLE;
}

bool EosBusSerial::send()
{
	if (_busState == BUS_S_IDLE || _busState == BUS_S_RECEIVING) {
		_busState = BUS_S_TRANSMITTING;

		int i = (unsigned int)(_txBufferInfo.tail + 5) % TX_BUFFER_SIZE;
		_txChecksum += _txBuffer[i];

		write(_txChecksum);
		write(PACKET_TRAIL_BYTE);
		write(PACKET_TRAIL_BYTE);

		_txPin.set();

		sbi(*_ucsrb, _udrie);
		sbi(*_ucsra, TXC0);

		return true;		
	}
	return false;
}

bool EosBusSerial::sendAvailable()
{
	return _txBufferInfo.head != _txBufferInfo.tail;
}

unsigned char EosBusSerial::getSendByte()
{
    unsigned char c = _txBuffer[_txBufferInfo.tail];
    _txBufferInfo.tail = (_txBufferInfo.tail + 1) % TX_BUFFER_SIZE;

    return c;    
}

void EosBusSerial::sendComplete()
{
	// Check for response required.
	if (_busState == BUS_S_TRANSMITTING) 
	{		
		_txPin.clear();

		_timeout = millis() + PACKET_RESPONSE_TIMEOUT;
		_busState = (_txResponseSource > 0) ? BUS_S_WAITING : BUS_S_IDLE;
	}	
}

bus_status EosBusSerial::process()
{
	switch (_busState) 
	{
		case BUS_S_IDLE:
			return BUS_IDLE;
			break;

		case BUS_S_WAITING:
			if (available()) 
			{
				uint8_t status = processByte(peek());
				if (status == -1)
				{
					resetRx();					
					return BUS_ERROR;
				}
				else if (status == 1)
				{
					_rxReadIndex = 0;

					uint8_t dest = getDestination();
					uint8_t src = getSource();


					if (dest == _nodeAddress &&
						src == _txResponseSource)
					{
						return BUS_PACKETREADY;
					}
					else 
					{
						resetRx();
						return BUS_ERROR;
					}
				}				
			}

			else if	(checkTimeout())
			{
				_bufferTimeouts++;
				resetRx();
				return BUS_TIMEOUT;	
			}

			return BUS_WAITINGRESPONSE;			
			break;

		case BUS_S_RECEIVING:
			if (available()) 
			{
				uint8_t status = processByte(peek());
				if (status == -1)
				{
					resetRx();
					return BUS_ERROR;					
				}
				else if (status == 1)
				{
					_rxReadIndex = 0;

					uint8_t dest = getDestination();

					if (dest == _nodeAddress ||
						dest == _groupAddress ||
						dest == 255 || _promMode)
					{
						return BUS_PACKETREADY;
					}
					else
					{
						resetRx();
					}
				}				
			}

			else if	(checkTimeout())
			{
				resetRx();
			}

			return BUS_RECEIVING;
			break;

		case BUS_S_TRANSMITTING:
			return BUS_TRANSMITTING;
			break;
	}
}

uint8_t EosBusSerial::processByte(uint8_t in) 
{
	uint8_t len;
	switch(_packetState)
	{
		case PACKET_S_READY:
			resetRx();

		case PACKET_S_START:
			_rxPacketOffset++;
			if (in == PACKET_START_BYTE)
			{
				_packetState = PACKET_S_LEADIN;
			}
			else 
			{
				resetRx();
			}
			break;

		case PACKET_S_LEADIN:
			if (in == PACKET_LEADIN_BYTE)
			{
				_rxPacketOffset++;
				_packetState = PACKET_S_ADDRESS;
				_rxChecksum = 0;
			}	
			else
			{
				resetRx();
			}
			break;

		case PACKET_S_ADDRESS:
			_rxPacketOffset++;
			_packetState = PACKET_S_SRC;
			_rxChecksum += in;
			break;

		case PACKET_S_SRC:
			_rxPacketOffset++;
			_packetState = PACKET_S_COMMAND;
			_rxChecksum += in;
			break;

		case PACKET_S_COMMAND:
			_rxPacketOffset++;
			_packetState = PACKET_S_DATALEN;
			_rxChecksum += in;
			break;

		case PACKET_S_DATALEN:
			_rxPacketOffset++;
			_packetState = (in==0) ? PACKET_S_CHKSUM : PACKET_S_DATA;
			_rxChecksum += in;
			break;

		case PACKET_S_DATA:
			len = getDataLength();
			_rxChecksum += in;
			_rxPacketOffset++;

			if (_rxPacketOffset - 6 == len)
			{
				_packetState = PACKET_S_CHKSUM;
			}
			break;			

		case PACKET_S_CHKSUM:
			_rxPacketOffset++;		
			if (in == _rxChecksum)
			{
				_packetState = PACKET_S_READY;
				return 1;
			}
			else 
			{
				_packetErrors++;
				resetRx();
				return -1;
			}
	}
	return 0;
}

void EosBusSerial::resetRx() {
	uint8_t i = (_rxBufferInfo.tail + _rxPacketOffset) % RX_BUFFER_SIZE;
	_rxBufferInfo.tail = i;
	_rxPacketOffset = 0;
	_packetState = PACKET_S_START;
	_busState = available() ? BUS_S_RECEIVING : BUS_S_IDLE;
}

bool EosBusSerial::available() 
{
	uint8_t i = (_rxBufferInfo.tail + _rxPacketOffset) % RX_BUFFER_SIZE;
	return (i != _rxBufferInfo.head);
}

uint8_t EosBusSerial::peek() 
{
	uint8_t i = (_rxBufferInfo.tail + _rxPacketOffset) % RX_BUFFER_SIZE;
	return _rxBuffer[i];
}

bool EosBusSerial::checkTimeout() 
{
	return (long)(millis() - _timeout) >= 0;
}

uint8_t EosBusSerial::getDestination()
{
	uint8_t i = (_rxBufferInfo.tail + 2) % RX_BUFFER_SIZE;
	return _rxBuffer[i];
}

uint8_t EosBusSerial::getSource()
{
	uint8_t i = (_rxBufferInfo.tail + 3) % RX_BUFFER_SIZE;
	return _rxBuffer[i];
}

uint8_t EosBusSerial::getCommand()
{
	uint8_t i = (_rxBufferInfo.tail + 4) % RX_BUFFER_SIZE;
	return _rxBuffer[i];
}

bool EosBusSerial::isResponse()
{
	uint8_t command = getCommand();
	return (command & 63) && (command & 64);
}

uint8_t EosBusSerial::getDataLength()
{
	uint8_t i = (_rxBufferInfo.tail + 5) % RX_BUFFER_SIZE;
	return _rxBuffer[i];
}

uint8_t EosBusSerial::readByte()
{
	if (!isEndOfPacketData()) {
		uint8_t i = (_rxBufferInfo.tail + 6 + _rxReadIndex++) % RX_BUFFER_SIZE;
		return _rxBuffer[i];		
	}
	return 0;
}

bool EosBusSerial::readBoolean()
{
	uint8_t data = readByte();
	return data != 0;
}

int EosBusSerial::readInt()
{
	int val = 0;
	val = readByte() << 8;
	val |= readByte();
	return val;	
}

long EosBusSerial::readLong()
{
	long val = 0;
	val = (long)readByte() << 24;		
	val |= (long)readByte() << 16;
	val |= (long)readByte() << 8;
	val |= (long)readByte();
	return val;	
}

bool EosBusSerial::isEndOfPacketData() 
{
	return (_rxReadIndex >= getDataLength());
}

uint8_t EosBusSerial::getNodeAddress()
{
	return _nodeAddress;
}

void EosBusSerial::setNodeAddress(uint8_t address)
{
	_nodeAddress = address;
}

uint8_t EosBusSerial::getGroup()
{
	return (_groupAddress - 240);
}

void EosBusSerial::setGroup(uint8_t groupId)
{
	_groupAddress = 240 + groupId;
}

void EosBusSerial::setPromiscuous(bool mode)
{
	_promMode = mode;
}

bool EosBusSerial::getPromiscuous()
{
	return _promMode;
}

void EosBusSerial::resetStats()
{
	_overuns = 0;
	_packetErrors = 0;
	_collisions = 0;
	_bufferTimeouts = 0;
}

unsigned long EosBusSerial::getTimeouts()
{
	return _bufferTimeouts;
}

unsigned long EosBusSerial::getErrors()
{
	return _packetErrors;
}

unsigned long EosBusSerial::getCollisions()
{
	return _collisions;
}

unsigned long EosBusSerial::getOverruns()
{
	return _overuns;
}


// Create bus on UART1 if present leaving UART0 for Host communications.
#if defined(UBRR1H)
  EosBusSerial EosBus(&UBRR1H, &UBRR1L, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1, RXEN1, TXEN1, RXCIE1, UDRIE1, U2X1);
	// Teensy++ runs this
#elif defined(UBRRH) && defined(UBRRL)
  EosBusSerial EosBus(&UBRRH, &UBRRL, &UCSRA, &UCSRB, &UCSRC, &UDR, RXEN, TXEN, RXCIE, UDRIE, U2X);
#elif defined(UBRR0H) && defined(UBRR0L)
  EosBusSerial EosBus(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, RXEN0, TXEN0, RXCIE0, UDRIE0, U2X0);
#endif

#if defined(USART1_RX_vect)
	// Teensy++ is here
	ISR(USART1_RX_vect)	
#elif defined(USART_RX_vect)
	ISR(USART_RX_vect)
#elif defined(SIG_USART0_RECV)
	SIGNAL(SIG_USART0_RECV)
#elif defined(SIG_UART0_RECV)
	SIGNAL(SIG_UART0_RECV)
#elif defined(USART0_RX_vect)
	SIGNAL(USART0_RX_vect)
#elif defined(SIG_UART_RECV)
	SIGNAL(SIG_UART_RECV)
#else
#error No UART defined
#endif
{
#if defined(UDR1)
	// Teensy++ gets here
	Serial.println("Firing USART RX");
	if (bit_is_clear(UCSR1A, UPE1)) {
	  unsigned char c = UDR1;
	  EosBus.receiveByte(c);
	} else {
	  unsigned char c = UDR1;
	};    
#elif defined(UDR0)
	if (bit_is_clear(UCSR0A, UPE0)) {
	  unsigned char c = UDR0;
	  EosBus.receiveByte(c);
	} else {
	  unsigned char c = UDR0;
	};
#elif defined(UDR)
	if (bit_is_clear(UCSRA, PE)) {
	  unsigned char c = UDR;
	  EosBus.receiveByte(c);
	} else {
	  unsigned char c = UDR;
	};
#else
#error UDR not defined
#endif
}

#ifdef USART1_UDRE_vect
	ISR(USART1_UDRE_vect)
#elif defined(UART0_UDRE_vect)
	ISR(UART0_UDRE_vect)
#elif defined(UART_UDRE_vect)
	ISR(UART_UDRE_vect)
#elif defined(USART0_UDRE_vect)
	ISR(USART0_UDRE_vect)
#elif defined(USART_UDRE_vect)
	ISR(USART_UDRE_vect)
#endif
{
	if (!EosBus.sendAvailable())
	{
	// Buffer empty, so disable interrupts
#if defined(UCSR1B)
	    cbi(UCSR1B, UDRIE1);
#elif defined(UCSR0B)
    	cbi(UCSR0B, UDRIE0);
#else
    	cbi(UCSRB, UDRIE);
#endif
    	EosBus.sendComplete();
	}
	else
	{
		// There is more data in the output buffer. Send the next byte
		unsigned char c = EosBus.getSendByte();
	
#if defined(UDR1)
	    UDR1 = c;
#elif defined(UDR0)
		UDR0 = c;
#elif defined(UDR)
		UDR = c;
#else
	#error UDR not defined
#endif
  	}
}