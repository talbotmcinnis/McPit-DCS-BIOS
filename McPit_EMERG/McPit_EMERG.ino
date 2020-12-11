#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

/// Reminder: Run ConfigureDevice.cmd elevated first to set USB device IDs

typedef DcsBios::PotentiometerEWMA<100, 3000,1> McPitPot;

const int NC_PIN = 36;
/**** Emergency ****/
DcsBios::Switch3Pos efcpAileronEmerDisengage("EFCP_AILERON_EMER_DISENGAGE", 7, 8);
DcsBios::Switch3Pos efcpElevatorEmerDisengage("EFCP_ELEVATOR_EMER_DISENGAGE", 10, 9);

const byte efcpEmerTrimPins[5] = {DcsBios::PIN_NC, 2, 1, 3, 0};
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

McPitPot intAimVol("INT_AIM_VOL", 44);
McPitPot intIffVol("INT_IFF_VOL", 44);
McPitPot intIlsVol("INT_ILS_VOL", 44);
McPitPot intTcnVol("INT_TCN_VOL", 44);

DcsBios::Switch2Pos intHm("INT_HM", 16);

DcsBios::Switch2Pos intIntUnmute("INT_INT_UNMUTE", 26);
DcsBios::Switch2Pos intFmUnmute("INT_FM_UNMUTE", 26);
DcsBios::Switch2Pos intVhfUnmute("INT_VHF_UNMUTE", 26);
DcsBios::Switch2Pos intUhfUnmute("INT_UHF_UNMUTE", 26);

McPitPot intIntVol("INT_INT_VOL", 45);
McPitPot intFmVol("INT_FM_VOL", 45);
McPitPot intVhfVol("INT_VHF_VOL", 45);
McPitPot intUhfVol("INT_UHF_VOL", 45);

McPitPot intVol("INT_VOL", 45);

const byte intModePins[5] = {12, 13, 14, 15, NC_PIN};
DcsBios::SwitchMultiPos intMode("INT_MODE", intModePins, 5);

McPitPot stallPeakVol("STALL_PEAK_VOL", 43);
McPitPot stallVol("STALL_VOL", 42);

DcsBios::Switch2Pos gndSafeOverrideToggle("GND_SAFE_OVERRIDE", 17);

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

unsigned long uLastModelTimeS = -1; // Start big, to ensure the first step triggers a resync

void onModTimeChange(char* newValue) {
  unsigned long currentModelTimeS = atol(newValue);

  if( currentModelTimeS < uLastModelTimeS )
  {
    if( currentModelTimeS > 10 )// Intentional delay to give time for DCS to finish loading and become stable
    {
      DcsBios::resetAllStates();
      uLastModelTimeS = currentModelTimeS;
    }
  }
  else
  {
    uLastModelTimeS = currentModelTimeS;
  }
}
DcsBios::StringBuffer<5> modTimeBuffer(0x043e, onModTimeChange);
