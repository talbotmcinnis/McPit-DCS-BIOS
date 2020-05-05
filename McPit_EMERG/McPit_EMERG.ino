#include <DcsBios.h>
#include <Servo.h>
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
DcsBios::Potentiometer intAimVol("INT_AIM_VOL", 44);
DcsBios::Switch2Pos intHm("INT_HM", 16);
DcsBios::Switch2Pos intIntUnmute("INT_INT_UNMUTE", 26);
DcsBios::Potentiometer intIntVol("INT_INT_VOL", 45);
const byte intModePins[5] = {12, 13, 14, 15, 36};  // I didn't wire the BLANK spot.   // Note 36 is NC, but I can't wire CENTER pos, so hopefully this works!!!!
DcsBios::SwitchMultiPos intMode("INT_MODE", intModePins, 5);

DcsBios::Potentiometer stallPeakVol("STALL_PEAK_VOL", 43);
DcsBios::Potentiometer stallVol("STALL_VOL", 42);

DcsBios::Switch2Pos gndSafeOverrideToggle("GND_SAFE_OVERRIDE", 17);

// Teensy++ 2.0: 46 IO

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

void setup() {
  Serial.begin(500000);
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
}

void onDcsBiosFrameSync()
{
}
