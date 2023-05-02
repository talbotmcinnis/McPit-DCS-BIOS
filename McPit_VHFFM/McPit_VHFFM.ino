#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

// Reminder: Run ConfigureDevice.cmd elevated first to set USB device IDs

typedef DcsBios::PotentiometerEWMA<100,3000,1> McPitPot;
typedef DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> McPitRotary;

/**** VHF FM Radio 19D 1A ****/
McPitRotary vhffmFreq1("VHFFM_FREQ1", "DEC", "INC", 3, 2);
McPitRotary vhffmFreq2("VHFFM_FREQ2", "DEC", "INC", 5, 4);
McPitRotary vhffmFreq3("VHFFM_FREQ3", "DEC", "INC", 7, 8);
McPitRotary vhffmFreq4("VHFFM_FREQ4", "DEC", "INC", 9, 10);
const byte vhffmFreqemerPins[4] = {42, 43, 0, 1};
DcsBios::SwitchMultiPos vhffmFreqemer("VHFFM_FREQEMER", vhffmFreqemerPins, 4); // OK1
DcsBios::Switch2Pos vhffmLoad("VHFFM_LOAD", 11);  // OK1
const byte vhffmModePins[3] = {39, 38, 27};
DcsBios::SwitchMultiPos vhffmMode("VHFFM_MODE", vhffmModePins, 3);  // OK1
McPitRotary vhffmPreset("VHFFM_PRESET", "DEC", "INC", 12, 13);
DcsBios::Switch3Pos vhffmSquelch("VHFFM_SQUELCH", 41, 40);  // OK1
McPitPot vhffmVol("VHFFM_VOL", 45); // NO CHANGE

/**** VHF AM Radio 13D 1A ****/
// Retasked as poor man's ARC-210 for now
McPitRotary vhfamFreq1("ARC210_100MHZ_SEL", "DEC", "INC", 20, 19);
McPitRotary vhfamFreq2("ARC210_10MHZ_SEL", "DEC", "INC", 22, 21);
McPitRotary vhfamFreq3("ARC210_1MHZ_SEL", "DEC", "INC", 24, 23);
McPitRotary vhfamFreq4("ARC210_100KHZ_SEL", "DEC", "INC", 26, 25);
//DcsBios::Switch2Pos vhfamLoad("VHFAM_LOAD", 18);      // OK1
McPitRotary vhfamPreset("ARC210_CHN_KNB", "DEC", "INC", 16, 17);
DcsBios::Switch3Pos vhfamSquelch("ARC210_SQUELCH", 14, 15);  // OK1
//McPitPot vhfamVol("VHFAM_VOL", 44);  // OK1

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

void setup() {
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
