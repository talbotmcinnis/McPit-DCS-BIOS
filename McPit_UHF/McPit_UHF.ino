#include <LedControl.h>

#include <DcsBios.h>
#include <Servo.h>

// For C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\usb_serial_hid\usb_private.h
/*
#define STR_PRODUCT             L"McPit - UHF"
#define PRODUCT_ID		0x0492
*/

/**** UHF Radio 25D 1A ****/
DcsBios::RotaryEncoder uhf100mhzSel("UHF_100MHZ_SEL", "DEC", "INC", 4, 3);
DcsBios::RotaryEncoder uhf10mhzSel("UHF_10MHZ_SEL", "DEC", "INC", 7, 5);
DcsBios::RotaryEncoder uhf1mhzSel("UHF_1MHZ_SEL", "DEC", "INC", 9, 8);
DcsBios::RotaryEncoder uhfPoint1mhzSel("UHF_POINT1MHZ_SEL", "DEC", "INC", 11, 10);
DcsBios::RotaryEncoder uhfPoint25Sel("UHF_POINT25_SEL", "DEC", "INC", 13, 12);
DcsBios::RotaryEncoder uhfPresetSel("UHF_PRESET_SEL", "DEC", "INC", 15, 14);

const byte uhfFunctionPins[4] = {20, 19, 18, 27};
DcsBios::SwitchMultiPos uhfFunction("UHF_FUNCTION", uhfFunctionPins, 4);

DcsBios::ActionButton uhfLoadToggle("UHF_LOAD", "TOGGLE", 16);
const byte uhfModePins[3] = {0, 1, 2};
DcsBios::SwitchMultiPos uhfMode("UHF_MODE", uhfModePins, 3);
DcsBios::Switch2Pos uhfSquelch("UHF_SQUELCH", 21);
DcsBios::Switch2Pos uhfStatus("UHF_STATUS", 22);
DcsBios::Switch2Pos uhfTest("UHF_TEST", 23);
//DcsBios::Switch3Pos uhfTTone("UHF_T_TONE", PIN_A, PIN_B);  // No Function
DcsBios::Potentiometer uhfVol("UHF_VOL", 39);

const byte vhfamFreqemerPins[4] = {42, 41, 40, 38};
DcsBios::SwitchMultiPos vhfamFreqemer("VHFAM_FREQEMER", vhfamFreqemerPins, 4);
const byte vhfamModePins[3] = {43, 44, 45};
DcsBios::SwitchMultiPos vhfamMode("VHFAM_MODE", vhfamModePins, 3);

// inputs: DIN pin, CLK pin, LOAD pin. number of chips
LedControl mydisplay = LedControl(26, 25, 24, 3);

// Teensy++ 2.0: 46 IO

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

void setup() {
  Serial.begin(500000);
  
  mydisplay.shutdown(0, false);  // turns on display
  mydisplay.setIntensity(0, 8); // 15 = brightest
  mydisplay.clearDisplay(0);
  mydisplay.setDigit(0, 0, 0, false);
  mydisplay.setDigit(0, 1, 0, false);
  mydisplay.setDigit(0, 2, 0, true);
  mydisplay.setDigit(0, 3, 0, false);
  mydisplay.setDigit(0, 4, 0, false);
  mydisplay.setDigit(0, 5, 0, false);
  mydisplay.setDigit(0, 6, 0, false);
  mydisplay.setDigit(0, 7, 0, false);

  mydisplay.shutdown(1, false);  // turns on display
  mydisplay.setIntensity(1, 8); // 15 = brightest
  mydisplay.clearDisplay(1);  
  mydisplay.setDigit(1, 0, 0, false);
  mydisplay.setDigit(1, 1, 0, false);
  mydisplay.setDigit(1, 2, 0, true);
  mydisplay.setDigit(1, 3, 0, false);
  mydisplay.setDigit(1, 4, 0, false);
  mydisplay.setDigit(1, 5, 0, false);
  mydisplay.setDigit(1, 6, 0, false);
  mydisplay.setDigit(1, 7, 0, false);
  
  mydisplay.shutdown(2, false);  // turns on display
  mydisplay.setIntensity(2, 8); // 15 = brightest
  mydisplay.clearDisplay(2);  
  mydisplay.setDigit(2, 0, 0, false);
  mydisplay.setDigit(2, 1, 0, false);
  mydisplay.setDigit(2, 2, 0, true);
  mydisplay.setDigit(2, 3, 0, false);
  mydisplay.setDigit(2, 4, 0, false);
  mydisplay.setDigit(2, 5, 0, false);
  mydisplay.setDigit(2, 6, 0, false);
  mydisplay.setDigit(2, 7, 0, false);
}

/*
Your main loop needs to pass data from the DCS-BIOS export
stream to the parser object you instantiated above.

It also needs to call DcsBios::PollingInput::pollInputs()
to detect changes in the state of connected controls and
pass them on to DCS.
*/
void loop() {
  // feed incoming data to the parser
  int data = Serial.read();
  while (data > -1) {
      parser.processChar(data);
      data = Serial.read();
  }
  
  // poll inputs
  DcsBios::PollingInput::pollInputs();
}

/*
You need to define
void sendDcsBiosMessage(const char* msg, const char* arg)
so that the string msg, followed by a space, the string arg
and a newline gets sent to the DCS-BIOS import stream.

In this example we send it to the serial port, so you need to
run socat to read the data from the serial port and send it
over UDP to DCS-BIOS.

If you are using an Ethernet Shield, you would probably want
to send a UDP packet from this subroutine.
*/
void sendDcsBiosMessage(const char* msg, const char* arg) {
  Serial.write(msg);
  Serial.write(' ');
  Serial.write(arg);
  Serial.write('\n');
}

/*
This subroutine gets called every time a message is received
from the export stream (you need to define it even if it
does nothing).

Use this to handle outputs which are not covered by the
DcsBios Arduino library (e.g. displays).
*/
void onDcsBiosWrite(unsigned int address, unsigned int value) {
  switch(address)
  {
    case 0x117c:
    {
      unsigned int uhfFunctionValue = (value & 0x000c) >> 2;
      if( uhfFunctionValue == 0 )
        mydisplay.clearDisplay(0);
        
      unsigned int vhfamPresetIntValue = ((value & 0xf800) >> 11)+1;
      mydisplay.setDigit(1, 7, vhfamPresetIntValue%10, false);
      mydisplay.setDigit(1, 6, vhfamPresetIntValue/10, false);

    }
    break;
    // 112.344
    case 0x118e:
    {
        unsigned int vhfamFreq1IntValue = ((value & 0x000f) >> 0)+3;
        mydisplay.setDigit(1, 1, vhfamFreq1IntValue%10, false);
        mydisplay.setDigit(1, 0, vhfamFreq1IntValue/10, false);

        unsigned int vhfamFreq2Value = (value & 0x00f0) >> 4;
        mydisplay.setDigit(1, 2, vhfamFreq2Value, true);
        
        unsigned int vhfamFreq3Value = (value & 0x0f00) >> 8;
        mydisplay.setDigit(1, 3, vhfamFreq3Value, false);
        
        unsigned int vhfamFreq4IntValue = ((value & 0x7000) >> 12)*25;
        mydisplay.setDigit(1, 5, vhfamFreq4IntValue%10, false);
        mydisplay.setDigit(1, 4, vhfamFreq4IntValue/10, false);
    }
    break;
    
    case 0x1194:
    {
        unsigned int vhffmFreq1IntValue = ((value & 0x7800) >> 11) + 3;
        mydisplay.setDigit(2, 1, vhffmFreq1IntValue%10, false);
        mydisplay.setDigit(2, 0, vhffmFreq1IntValue/10, false);

        unsigned int vhffmPresetIntValue = ((value & 0x001f) >> 0)+1;
        mydisplay.setDigit(2, 7, vhffmPresetIntValue%10, false);
        mydisplay.setDigit(2, 6, vhffmPresetIntValue/10, false);

    }
    break;
    
    case 0x119c:
    {
        unsigned int vhffmFreq2Value = (value & 0x000f) >> 0;
        mydisplay.setDigit(2, 2, vhffmFreq2Value, true);

        unsigned int vhffmFreq3Value = (value & 0x00f0) >> 4;
        mydisplay.setDigit(2, 3, vhffmFreq3Value, false);

        unsigned int vhffmFreq4IntValue = ((value & 0x0700) >> 8)*25;
        mydisplay.setDigit(2, 5, vhffmFreq4IntValue%10, false);
        mydisplay.setDigit(2, 4, vhffmFreq4IntValue/10, false);
    }
    break;
  }
}

void onUhfFrequencyChange(char* newValue)
{
  if( newValue[0] == 65 || newValue[0] == 97 )
    mydisplay.setDigit(0, 0, 0xA, false);
  else
    mydisplay.setDigit(0, 0, newValue[0]-48, false);
    
  mydisplay.setDigit(0, 1, newValue[1]-48, false);
  mydisplay.setDigit(0, 2, newValue[2]-48, true);
  mydisplay.setDigit(0, 3, newValue[4]-48, false);
  mydisplay.setDigit(0, 4, newValue[5]-48, false);
  mydisplay.setDigit(0, 5, newValue[6]-48, false);
}
DcsBios::StringBuffer<7> uhfFrequencyBuffer(0x1180, onUhfFrequencyChange);

void onUhfPresetChange(char* newValue) {
  mydisplay.setDigit(0, 6, newValue[0]-48, false);
  mydisplay.setDigit(0, 7, newValue[1]-48, false);
}
DcsBios::StringBuffer<2> uhfPresetBuffer(0x1188, onUhfPresetChange);

void onDcsBiosFrameSync()
{
}
