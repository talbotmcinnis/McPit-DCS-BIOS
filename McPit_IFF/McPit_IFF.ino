#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

/// Reminder: Run ConfigureDevice.cmd elevated first to set USB device IDs

/**** IFF ****/

// To save pins, I didn't implement a few, but need something to pass to BIOS stock interfaces
const int PIN_X = 36;

//IFF Code: ZERO - B - A - (HOLD)
// This is what it SHOULD be, BUT DCS does it with INC/DEC, so I have a hack below
// Effective JUL-2022, Warlord came up with a fix within DCS-BIOS, so I can use the desired behaviour now!
const byte iffCodePins[4] = {27,0,1,2};
DcsBios::SwitchMultiPos iffCode("IFF_CODE", iffCodePins, 4);
//DcsBios::RotaryEncoder iffCode("IFF_CODE", "DEC", "INC", 27, 2);
//DcsBios::ActionButton iffDec("IFF_CODE", "INC", 1);
//DcsBios::ActionButton iffInc("IFF_CODE", "DEC", 27);

// IFF Master: OFF - STBY - LOW - NORM - EMER
const byte iffMasterPins[5] = {8, 7, PIN_X, 34, 5};
DcsBios::SwitchMultiPos iffMaster("IFF_MASTER", iffMasterPins, 5);

// IFF On/Out
DcsBios::Switch2Pos iffOnOut("IFF_ON_OUT", 26);

// IFF Out: LIGHT - OFF - AUDIO
DcsBios::Switch3Pos iffOutAudioLight("IFF_OUT_AUDIO_LIGHT", 29, 32);


// TODO: DCS isn't toggling these values via export, i.e. DCS bios is showing constant 1:0
// TODO: Test helios in case it works and proves that DCS is right?
// IFF REPLY Light
DcsBios::LED iffReply(0x12d2, 0x0002, 35);

// IFF TEST Light
DcsBios::LED iffTest(0x12d2, 0x0004, 31);

// Ident/Mic
DcsBios::Switch3Pos iffMicIdent("IFF_MIC_IDENT", 30, 33);

// Mode-1 Wheel 1
DcsBios::BcdWheel iffMode1Wheel1("IFF_MODE1_WHEEL1", 25, 24, 21);

// Mode-1 Wheel 2
DcsBios::BcdWheel iffMode1Wheel2("IFF_MODE1_WHEEL2", 23, 22);

// Mode-3A Wheel 1
DcsBios::BcdWheel iffMode3aWheel1("IFF_MODE3A_WHEEL1", 45, 38, 28);

// Mode-3A Wheel 2
DcsBios::BcdWheel iffMode3aWheel2("IFF_MODE3A_WHEEL2", 20, 19, 18);

// Mode-3A Wheel 3
DcsBios::BcdWheel iffMode3aWheel3("IFF_MODE3A_WHEEL3", 39, 40, 41);

// Mode-3A Wheel 4
DcsBios::BcdWheel iffMode3aWheel4("IFF_MODE3A_WHEEL4", 42, 43, 44);

// RAD Test/Mon
DcsBios::Switch3Pos iffRadtest("IFF_RADTEST", 17, PIN_X);

// REPLY Push to Test
DcsBios::Switch2Pos iffReplyTest("IFF_REPLY_TEST", 3);

// TEST Push to Test
DcsBios::Switch2Pos iffTestTest("IFF_TEST_TEST", 4);

// Test M-1
DcsBios::Switch3Pos iffTestM1("IFF_TEST_M1", 9, 10);

// Test M-2
DcsBios::Switch3Pos iffTestM2("IFF_TEST_M2", 11, 12);

// Test M-3
DcsBios::Switch3Pos iffTestM3("IFF_TEST_M3", 13, 14);

// Test M-4
DcsBios::Switch3Pos iffTestM4("IFF_TEST_M4", 15, 16);


// Teensy++ 2.0: 46 IO

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

void setup() {
  pinMode(PIN_X, INPUT_PULLUP);
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
