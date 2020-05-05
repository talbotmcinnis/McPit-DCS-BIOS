#define DCSBIOS_DEFAULT_SERIAL

#include <DcsBios.h>
#include <Servo.h>

// For C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\usb_serial_hid\usb_private.h
/*
#define PRODUCT_ID		0x0493
#define STR_PRODUCT             L"McPIT-VHF"
*/

/**** VHF FM Radio 19D 1A ****/
DcsBios::RotaryEncoder vhffmFreq1("VHFFM_FREQ1", "DEC", "INC", 3, 2);
DcsBios::RotaryEncoder vhffmFreq2("VHFFM_FREQ2", "DEC", "INC", 5, 4);
DcsBios::RotaryEncoder vhffmFreq3("VHFFM_FREQ3", "DEC", "INC", 7, 8);
DcsBios::RotaryEncoder vhffmFreq4("VHFFM_FREQ4", "DEC", "INC", 9, 10);
const byte vhffmFreqemerPins[4] = {42, 43, 0, 1};
DcsBios::SwitchMultiPos vhffmFreqemer("VHFFM_FREQEMER", vhffmFreqemerPins, 4); // OK1
DcsBios::Switch2Pos vhffmLoad("VHFFM_LOAD", 11);  // OK1
const byte vhffmModePins[3] = {39, 38, 27};
DcsBios::SwitchMultiPos vhffmMode("VHFFM_MODE", vhffmModePins, 3);  // OK1
DcsBios::RotaryEncoder vhffmPreset("VHFFM_PRESET", "DEC", "INC", 12, 13);  // REV 1
DcsBios::Switch3Pos vhffmSquelch("VHFFM_SQUELCH", 41, 40);  // OK1
DcsBios::Potentiometer vhffmVol("VHFFM_VOL", 45); // NO CHANGE

/**** VHF AM Radio 13D 1A ****/
DcsBios::RotaryEncoder vhfamFreq1("VHFAM_FREQ1", "DEC", "INC", 20, 19);  // OK1
DcsBios::RotaryEncoder vhfamFreq2("VHFAM_FREQ2", "DEC", "INC", 22, 21);  // OK1
DcsBios::RotaryEncoder vhfamFreq3("VHFAM_FREQ3", "DEC", "INC", 24, 23);  // OK1
DcsBios::RotaryEncoder vhfamFreq4("VHFAM_FREQ4", "DEC", "INC", 26, 25);  // OK1
DcsBios::Switch2Pos vhfamLoad("VHFAM_LOAD", 18);      // OK1
DcsBios::RotaryEncoder vhfamPreset("VHFAM_PRESET", "DEC", "INC", 16, 17);  //REV1
DcsBios::Switch3Pos vhfamSquelch("VHFAM_SQUELCH", 14, 15);  // OK1
DcsBios::Potentiometer vhfamVol("VHFAM_VOL", 44);  // OK1

// Teensy++ 2.0: 46 IO

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}

