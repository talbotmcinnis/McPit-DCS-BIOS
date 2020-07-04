//#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_DEFAULT_SERIAL

// Because we need each device to be unique, we must change the product_ID in C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\usb_serial_hid\usb_private.h
// before each compilation
/*
#define STR_PRODUCT             L"McPit - UHF"
#define PRODUCT_ID    0x0492  // McPit - UHF
*/

#include <LedControl.h>

#include <DcsBios.h> // Originally build for DCS Bios 0.1.x  Update For 0.2.11

/**** UHF Radio 25D 1A ****/
DcsBios::RotaryEncoder uhf100mhzSel("UHF_100MHZ_SEL", "DEC", "INC", 4, 3);
DcsBios::RotaryEncoder uhf10mhzSel("UHF_10MHZ_SEL", "DEC", "INC", 7, 5);
DcsBios::RotaryEncoder uhf1mhzSel("UHF_1MHZ_SEL", "DEC", "INC", 9, 8);
DcsBios::RotaryEncoder uhfPoint1mhzSel("UHF_POINT1MHZ_SEL", "DEC", "INC", 11, 10);
DcsBios::RotaryEncoder uhfPoint25Sel("UHF_POINT25_SEL", "DEC", "INC", 13, 12);
DcsBios::RotaryEncoder uhfPresetSel("UHF_PRESET_SEL", "DEC", "INC", 15, 14);

const byte uhfFunctionPins[4] = {20, 19, 18, 27};
DcsBios::SwitchMultiPos uhfFunction("UHF_FUNCTION", uhfFunctionPins, 4);

DcsBios::Switch2Pos uhfLoad("UHF_LOAD", 16); // Fixed 2020-05-15

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

bool uhf_LastFunction;
bool vhfAm_LastMode;
bool vhfFm_LastMode;

// Teensy++ 2.0: 46 IO

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

void initLCD(int bank)
{
  mydisplay.shutdown(bank, false);  // turns on display
  mydisplay.setIntensity(bank, 8); // 15 = brightest
  mydisplay.clearDisplay(bank);

  mydisplay.setChar(bank, 0, ' ', false);
  mydisplay.setChar(bank, 1, ' ', false);
  mydisplay.setChar(bank, 2, ' ', true);
  mydisplay.setChar(bank, 3, ' ', false);
  mydisplay.setChar(bank, 4, ' ', false);
  mydisplay.setChar(bank, 5, ' ', false);
  mydisplay.setChar(bank, 6, ' ', false);
  mydisplay.setChar(bank, 7, ' ', false);
}

void setup() {
  uhf_LastFunction = 0;
  vhfAm_LastMode = 0;
  vhfFm_LastMode = 0;
  
  initLCD(0);
  initLCD(1);
  initLCD(2);

  DcsBios::setup();
}

void ConsiderRestartDisplay()
{
  if( uhf_LastFunction == 0 && vhfAm_LastMode == 0 && vhfFm_LastMode == 0 )
  {
    // All radios are off - Re-init the whole display controller
    mydisplay = LedControl(26, 25, 24, 3);

    initLCD(0);
    initLCD(1);
    initLCD(2);
  }
}

/*
Your main loop needs to pass data from the DCS-BIOS export
stream to the parser object you instantiated above.

It also needs to call DcsBios::PollingInput::pollInputs()
to detect changes in the state of connected controls and
pass them on to DCS.
*/
void loop() {
  DcsBios::loop();
}

///////////////////////////////////////////////////////////////////
// UHF (Mid)
void onUhfFunctionChange(unsigned int newValue) {
    if(newValue == 0)
    {
      // Radio is OFF
      mydisplay.shutdown(0, true);
      uhf_LastFunction = newValue;
      ConsiderRestartDisplay();
    }
    else if( uhf_LastFunction == 0 )
    {
      // Radio is ON, but wasn't before
      mydisplay.shutdown(0, false);
      uhf_LastFunction = newValue;
    }
}
DcsBios::IntegerBuffer uhfFunctionBuffer(0x117c, 0x000c, 2, onUhfFunctionChange);

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




///////////////////////////////////////////////////////////////////
// VHF AM (Front)

void onVhfamModeChange(unsigned int newValue) {
    if(newValue == 0)
    {
      // Radio is OFF
      mydisplay.shutdown(1, true);
      vhfAm_LastMode = newValue;
      ConsiderRestartDisplay();
    }
    else if( vhfAm_LastMode == 0 )
    {
      // Radio is ON, but wasn't before
      mydisplay.shutdown(1, false);
      vhfAm_LastMode = newValue;
    }
}
DcsBios::IntegerBuffer vhfamModeBuffer(0x1186, 0x0300, 8, onVhfamModeChange);

void onVhfamPresetChange(unsigned int newValue) {
  // newValue is zero based but presets are 1 based
  unsigned int preset = newValue + 1;
  mydisplay.setDigit(1, 7, preset %10, false);
  mydisplay.setDigit(1, 6, preset /10, false);
}
DcsBios::IntegerBuffer vhfamPresetBuffer(0x117c, 0xf800, 11, onVhfamPresetChange);

void onVhfAmFrequencySChange(char* newValue) {
  if( newValue[2] == '.' )
  {
    // ##.###
    mydisplay.setChar(1, 0, ' ', false);
    mydisplay.setChar(1, 1, newValue[0], false);
    mydisplay.setChar(1, 2, newValue[1], true);
    
    mydisplay.setChar(1, 3, newValue[3], false);
    mydisplay.setChar(1, 4, newValue[4], false);
    mydisplay.setChar(1, 5, newValue[5], false);
  }
  else
  {
    // ###.###
    mydisplay.setChar(1, 0, newValue[0], false);
    mydisplay.setChar(1, 1, newValue[1], false);
    mydisplay.setChar(1, 2, newValue[2], true);
    
    mydisplay.setChar(1, 3, newValue[4], false);
    mydisplay.setChar(1, 4, newValue[5], false);
    mydisplay.setChar(1, 5, newValue[6], false);
  }
}
DcsBios::StringBuffer<7> vhfAmFrequencySBuffer(0x12de, onVhfAmFrequencySChange);

/*void onVhfamFreq1RotChange(unsigned int newValue) {
    // AM Freq1:Tens
    unsigned int fTens = newValue + 3;
    mydisplay.setDigit(1, 1, fTens %10, false);
    mydisplay.setDigit(1, 0, fTens /10, false);
}
DcsBios::IntegerBuffer vhfamFreq1RotBuffer(0x11bc, 0x07f8, 3, onVhfamFreq1RotChange);

void onVhfamFreq2RotChange(unsigned int newValue) {
    mydisplay.setDigit(1, 2, newValue, true);
}
DcsBios::IntegerBuffer vhfamFreq2RotBuffer(0x12b8, 0x00ff, 0, onVhfamFreq2RotChange);

void onVhfamFreq3RotChange(unsigned int newValue) {
    mydisplay.setDigit(1, 3, newValue, false);    
}
DcsBios::IntegerBuffer vhfamFreq3RotBuffer(0x12b8, 0xff00, 8, onVhfamFreq3RotChange);

void onVhfamFreq4RotChange(unsigned int newValue) {
  // AM:Freq4:hundies
    unsigned int fHundies = newValue;
    mydisplay.setDigit(1, 5, fHundies%10, false);
    mydisplay.setDigit(1, 4, fHundies/10, false);
}
DcsBios::IntegerBuffer vhfamFreq4RotBuffer(0x12ba, 0x00ff, 0, onVhfamFreq4RotChange);
*/



///////////////////////////////////////////////////////////////////
// VHF FM (Rear)

void onVhffmModeChange(unsigned int newValue) {
    if(newValue == 0)
    {
      // Radio is OFF
      mydisplay.shutdown(2, true);
      vhfFm_LastMode = newValue;
      ConsiderRestartDisplay();
    }
    else if( vhfFm_LastMode == 0 )
    {
      // Radio is ON, but wasn't before
      mydisplay.shutdown(2, false);
      vhfFm_LastMode = newValue;
    }
}
DcsBios::IntegerBuffer vhffmModeBuffer(0x1194, 0x0060, 5, onVhffmModeChange);

void onVhffmPresetChange(unsigned int newValue) {
  // newValue is zero based but presets are 1 based
    unsigned int preset = newValue + 1;
    mydisplay.setDigit(2, 7, preset%10, false);
    mydisplay.setDigit(2, 6, preset/10, false);
}
DcsBios::IntegerBuffer vhffmPresetBuffer(0x1194, 0x001f, 0, onVhffmPresetChange);

void onVhfFmFrequencySChange(char* newValue) {
  if( newValue[2] == '.' )
  {
    // ##.###
    mydisplay.setChar(2, 0, ' ', false);
    mydisplay.setChar(2, 1, newValue[0], false);
    mydisplay.setChar(2, 2, newValue[1], true);
    
    mydisplay.setChar(2, 3, newValue[3], false);
    mydisplay.setChar(2, 4, newValue[4], false);
    mydisplay.setChar(2, 5, newValue[5], false);
  }
  else
  {
    // ###.###
    mydisplay.setChar(2, 0, newValue[0], false);
    mydisplay.setChar(2, 1, newValue[1], false);
    mydisplay.setChar(2, 2, newValue[2], true);
    
    mydisplay.setChar(2, 3, newValue[4], false);
    mydisplay.setChar(2, 4, newValue[5], false);
    mydisplay.setChar(2, 5, newValue[6], false);
  }
}
DcsBios::StringBuffer<7> vhfFmFrequencySBuffer(0x12e6, onVhfFmFrequencySChange);

/*
void onVhffmFreq1RotChange(unsigned int newValue) {
    unsigned int fTens = newValue - 3;
    mydisplay.setDigit(2, 1, fTens %10, false);
    mydisplay.setDigit(2, 0, fTens/10, false);
}
DcsBios::IntegerBuffer vhffmFreq1RotBuffer(0x12ba, 0xff00, 8, onVhffmFreq1RotChange);

void onVhffmFreq2RotChange(unsigned int newValue) {
  mydisplay.setDigit(2, 2, newValue, true);
}
DcsBios::IntegerBuffer vhffmFreq2RotBuffer(0x12bc, 0x00ff, 0, onVhffmFreq2RotChange);

void onVhffmFreq3RotChange(unsigned int newValue) {
    mydisplay.setDigit(2, 3, newValue, false);
}
DcsBios::IntegerBuffer vhffmFreq3RotBuffer(0x12bc, 0xff00, 8, onVhffmFreq3RotChange);

void onVhffmFreq4RotChange(unsigned int newValue) {
    unsigned int fHundies = newValue;
    mydisplay.setDigit(2, 5, fHundies%10, false);
    mydisplay.setDigit(2, 4, fHundies/10, false);
}
DcsBios::IntegerBuffer vhffmFreq4RotBuffer(0x12be, 0x00ff, 0, onVhffmFreq4RotChange);
*/
