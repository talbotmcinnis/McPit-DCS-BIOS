#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

/// Reminder: Run ConfigureDevice.cmd elevated first to set USB device IDs

DcsBios::Switch2Pos ky58Pwr("KY58_PWR", 0, false, 100);
DcsBios::Switch2Pos ky58Zeroize("KY58_ZEROIZE", 2, false, 100);
const byte ky58PlainPins[5] = {9, 10, 11};
DcsBios::SwitchMultiPos ky58Plain("KY58_PLAIN", ky58PlainPins, 5);
DcsBios::Switch2Pos ky58Delay("KY58_DELAY", 1, false, 100);
const byte ky581to5Pins[11] = {3, 4, 5, 26, 7, 8};
DcsBios::SwitchMultiPos ky581to5("KY58_1TO5", ky581to5Pins, 11);
const byte ky58ModePins[5] = {12, 13, 14};
DcsBios::SwitchMultiPos ky58Mode("KY58_MODE", ky58ModePins, 5);

// Teensy++ 2.0: 46 IO

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
