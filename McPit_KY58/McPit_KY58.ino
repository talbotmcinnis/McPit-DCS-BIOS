#define DCSBIOS_DEFAULT_SERIAL

#include <DcsBios.h>
//#include <Servo.h>

// For C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\usb_serial_hid\usb_private.h
/*
#define PRODUCT_ID		0x0494  // McPit-KY58
#define STR_PRODUCT             L"McPIT-KY58"
*/

DcsBios::Switch2Pos ky58Pwr("KY58_PWR", 0);
DcsBios::Switch2Pos ky58Zeroize("KY58_ZEROIZE", 2);
const byte ky58PlainPins[5] = {9, 10, 11};
DcsBios::SwitchMultiPos ky58Plain("KY58_PLAIN", ky58PlainPins, 5);
DcsBios::Switch2Pos ky58Delay("KY58_DELAY", 1);
const byte ky581to5Pins[11] = {3, 4, 5, 6, 7, 8};
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
