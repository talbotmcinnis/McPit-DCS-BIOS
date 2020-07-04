//#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_DEFAULT_SERIAL

#include <DcsBios.h>
// For usb_desc.h:
/*
#define PRODUCT_ID		0x0487
#define STR_PRODUCT             L"McPIT-EMERG"
*/

/**** Emergency ****/
DcsBios::Switch3Pos efcpAileronEmerDisengage("EFCP_AILERON_EMER_DISENGAGE", 7, 8);
DcsBios::Switch3Pos efcpElevatorEmerDisengage("EFCP_ELEVATOR_EMER_DISENGAGE", 10, 9);
const byte efcpEmerTrimPins[5] = {-1, 2, 1, 3, 0};
DcsBios::SwitchMultiPos efcpEmerTrim("EFCP_EMER_TRIM", efcpEmerTrimPins, 5);
DcsBios::Switch2Pos efcpFlapsEmerRetr("EFCP_FLAPS_EMER_RETR", 5);
DcsBios::Switch2Pos efcpMrfcs("EFCP_MRFCS", 4, true);
DcsBios::Switch2Pos efcpSpdbkEmerRetr("EFCP_SPDBK_EMER_RETR", 27);
DcsBios::Switch2Pos efcpTrimOverride("EFCP_TRIM_OVERRIDE", 11, true);

/**** Volumes ****/
DcsBios::Switch2Pos intAimUnmute("INT_AIM_UNMUTE", 25);
DcsBios::Switch2Pos intIffUnmute("INT_IFF_UNMUTE", 25);
DcsBios::Switch2Pos intIlsUnmute("INT_ILS_UNMUTE", 25);
DcsBios::Switch2Pos intTcnUnmute("INT_TCN_UNMUTE", 25);

DcsBios::Potentiometer intAimVol("INT_AIM_VOL", 44);
DcsBios::Potentiometer intIffVol("INT_IFF_VOL", 44);
DcsBios::Potentiometer intIlsVol("INT_ILS_VOL", 44);
DcsBios::Potentiometer intTcnVol("INT_TCN_VOL", 44);

DcsBios::Switch2Pos intHm("INT_HM", 16);

DcsBios::Switch2Pos intIntUnmute("INT_INT_UNMUTE", 26);
DcsBios::Switch2Pos intFmUnmute("INT_FM_UNMUTE", 26);
DcsBios::Switch2Pos intVhfUnmute("INT_VHF_UNMUTE", 26);
DcsBios::Switch2Pos intUhfUnmute("INT_UHF_UNMUTE", 26);

DcsBios::Potentiometer intIntVol("INT_INT_VOL", 45);
DcsBios::Potentiometer intFmVol("INT_FM_VOL", 45);
DcsBios::Potentiometer intVhfVol("INT_VHF_VOL", 45);
DcsBios::Potentiometer intUhfVol("INT_UHF_VOL", 45);

DcsBios::Potentiometer intVol("INT_VOL", 45);

const byte intModePins[5] = {12, 13, 14, 15, 36};  // I didn't wire the BLANK spot.   // Note 36 is NC, but I can't wire CENTER pos, so hopefully this works!!!!
DcsBios::SwitchMultiPos intMode("INT_MODE", intModePins, 5);

DcsBios::Potentiometer stallPeakVol("STALL_PEAK_VOL", 43);
DcsBios::Potentiometer stallVol("STALL_VOL", 42);

DcsBios::Switch2Pos gndSafeOverrideToggle("GND_SAFE_OVERRIDE", 17);

// Teensy++ 2.0: 46 IO

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

void setup() {
  DcsBios::setup();
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
