#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

// Reminder: Run ConfigureDevice.cmd elevated first to set USB device IDs

#include "Arduino.h"

typedef DcsBios::RotaryAcceleratedEncoderT<POLL_EVERY_TIME, DcsBios::EIGHT_STEPS_PER_DETENT> McPitRotaryAcceleratedEncoder;
typedef DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::EIGHT_STEPS_PER_DETENT> McPitRotaryEncoder;

DcsBios::Switch2Pos nmspAbleStow("NMSP_ABLE_STOW", 11, true);

McPitRotaryAcceleratedEncoder hsiCrs("HSI_CRS_KNOB", "-700", "+700", "-5000", "+5000", 1, 0);
McPitRotaryAcceleratedEncoder hsiHdg("HSI_HDG_KNOB", "-700", "+700", "-5000", "+5000", 36, 37);

McPitRotaryAcceleratedEncoder altSetPressure("ALT_SET_PRESSURE", "-10000", "+10000", "-30000", "+30000", 18, 19);

DcsBios::Switch2Pos emerBrake("EMER_BRAKE", 25);
DcsBios::ActionButton saiCageToggle("SAI_CAGE", "TOGGLE", 24);
McPitRotaryEncoder saiPitchTrim("SAI_PITCH_TRIM", "+3200", "-3200", 22, 23);

DcsBios::Switch3Pos hmcsPw("A102_HMCS_PW", 15, 16);
//DcsBios::Switch2Pos boardingLadderExtend("LADDER_EXTEND", 14);
//DcsBios::Switch3Pos aapSteer("AAP_STEER", 12, 13, 100); // As of 2020-11-21 DCS has a bug where CDU:steer doubule-taps, so use UFC until thats fixed
DcsBios::Switch3Pos aapSteer("UFC_STEER", 12, 13, 100);

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

//G meter reset is not mapped, so use a joy button
const int GMETER_RESET_PIN=26;

void setup() {
  pinMode(GMETER_RESET_PIN, INPUT_PULLUP);
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
  Joystick.button(1, !digitalRead(GMETER_RESET_PIN));
}

void onAcftNameChange(char* newValue) {
  // Change of Aircraft
  DcsBios::resetAllStates();
}
DcsBios::StringBuffer<24> AcftNameBuffer(0x0000, onAcftNameChange);
