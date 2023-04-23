#define DCSBIOS_DEFAULT_SERIAL
#include <LedControl.h>
#include <DcsBios.h> // Originally built for DCS Bios 0.1.x  Update For 0.2.11

/// Reminder: Run ConfigureDevice.cmd elevated first to set USB device IDs

typedef DcsBios::PotentiometerEWMA<100,3000,1> McPitPot;
typedef DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> McRotary;

/**** UHF Radio 25D 1A ****/
McRotary uhf100mhzSel("UHF_100MHZ_SEL", "DEC", "INC", 4, 3);
McRotary uhf10mhzSel("UHF_10MHZ_SEL", "DEC", "INC", 7, 5);
McRotary uhf1mhzSel("UHF_1MHZ_SEL", "DEC", "INC", 9, 8);
McRotary uhfPoint1mhzSel("UHF_POINT1MHZ_SEL", "DEC", "INC", 11, 10);
McRotary uhfPoint25Sel("UHF_POINT25_SEL", "DEC", "INC", 13, 12);
McRotary uhfPresetSel("UHF_PRESET_SEL", "DEC", "INC", 15, 14);

const byte uhfFunctionPins[4] = {20, 19, 18, 27};
DcsBios::SwitchMultiPos uhfFunction("UHF_FUNCTION", uhfFunctionPins, 4);

DcsBios::Switch2Pos uhfLoad("UHF_LOAD", 16); // Fixed 2020-05-15

const byte uhfModePins[3] = {0, 1, 2};
DcsBios::SwitchMultiPos uhfMode("UHF_MODE", uhfModePins, 3);
DcsBios::Switch2Pos uhfSquelch("UHF_SQUELCH", 21);
DcsBios::Switch2Pos uhfStatus("UHF_STATUS", 22);
DcsBios::Switch2Pos uhfTest("UHF_TEST", 23);
//DcsBios::Switch3Pos uhfTTone("UHF_T_TONE", PIN_A, PIN_B);  // No Function
McPitPot uhfVol("UHF_VOL", 39);

//const byte vhfamFreqemerPins[4] = {42, 41, 40, 38};
//DcsBios::SwitchMultiPos vhfamFreqemer("VHFAM_FREQEMER", vhfamFreqemerPins, 4);
//const byte vhfamModePins[3] = {43, 44, 45};
//DcsBios::SwitchMultiPos vhfamMode("VHFAM_MODE", vhfamModePins, 3);

// inputs: DIN pin, CLK pin, LOAD pin. number of chips
LedControl mydisplay = LedControl(26, 25, 24, 3);

// Teensy++ 2.0: 46 IO

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

void initLCD(int bank)
{
  delay(2000);
  mydisplay.shutdown(bank, false);  // turns on display
  mydisplay.setIntensity(bank, 8); // 15 = brightest

  delay(1000);
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

bool uhf_LastFunction;

void setup() {
  uhf_LastFunction = -1;
  initLCD(0);
  initLCD(1);
  initLCD(2);

  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}

void onAcftNameChange(char* newValue) {
  // Change of Aircraft
  DcsBios::resetAllStates();
}
DcsBios::StringBuffer<24> AcftNameBuffer(0x0000, onAcftNameChange);

///////////////////////////////////////////////////////////////////
// UHF (Mid)

void onUhfFunctionChange(unsigned int newValue) {
    if( newValue != uhf_LastFunction )
    {
      mydisplay.shutdown(0, newValue == 0);
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

  // Handle asterisks
  if( newValue[0] == 42 )
  {
    mydisplay.setChar(0, 0, '-', false);
    mydisplay.setChar(0, 1, '-', false);
    mydisplay.setChar(0, 2, '-', true);
    mydisplay.setChar(0, 3, '-', false);
    mydisplay.setChar(0, 4, '-', false);
    mydisplay.setChar(0, 5, '-', false);
    return;
  }
  mydisplay.setDigit(0, 1, newValue[1]-48, false);
  mydisplay.setDigit(0, 2, newValue[2]-48, true);
  mydisplay.setDigit(0, 3, newValue[4]-48, false);
  mydisplay.setDigit(0, 4, newValue[5]-48, false);
  mydisplay.setDigit(0, 5, newValue[6]-48, false);
}
DcsBios::StringBuffer<7> uhfFrequencyBuffer(0x1180, onUhfFrequencyChange);

void onUhfPresetChange(char* newValue) {
  // Handle asterisks
  if( newValue[0] == 42 )
  {
      mydisplay.setChar(0, 6, '-', false);
      mydisplay.setChar(0, 7, '-', false);
      return;
  }
  
  mydisplay.setDigit(0, 6, newValue[0]-48, false);
  mydisplay.setDigit(0, 7, newValue[1]-48, false);
}
DcsBios::StringBuffer<2> uhfPresetBuffer(0x1188, onUhfPresetChange);

///////////////////////////////////////////////////////////////////
// VHF FM (Rear)
void onVhffmPresetChange(unsigned int newValue) {
  // newValue is zero based but presets are 1 based
    unsigned int preset = newValue + 1;
    mydisplay.setDigit(2, 7, preset%10, false);
    mydisplay.setDigit(2, 6, preset/10, false);
}
DcsBios::IntegerBuffer vhffmPresetBuffer(0x117c, 0xf800, 11, onVhffmPresetChange);

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
DcsBios::StringBuffer<7> vhfFmFrequencySBuffer(0x12ce, onVhfFmFrequencySChange);
