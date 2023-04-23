#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

// IFF On/Out
DcsBios::Switch2Pos iffOnOut("IFF_ON_OUT", 26);

unsigned long uLastModelTimeS = 0xFFFFFFFF; // Start big, to ensure the first step triggers a resync

int ledState = true;

void onModTimeChange(char* newValue) {
  unsigned long currentModelTimeS = atol(newValue);

  if( currentModelTimeS < uLastModelTimeS )
  {
    if( currentModelTimeS > 20 )// Delay to give time for DCS to finish loading and become stable and responsive
    {
      DcsBios::resetAllStates();
      uLastModelTimeS = currentModelTimeS;
      ledState = !ledState;
      digitalWrite(6, ledState);
    }
  }
  else
  {
    uLastModelTimeS = currentModelTimeS;
  }

  
}
DcsBios::StringBuffer<5> modTimeBuffer(0x043e, onModTimeChange);

void setup() {
  pinMode(6, OUTPUT);
  digitalWrite(6, true);
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
