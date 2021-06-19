#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

#include <LedControlMS.h>
//https://github.com/shaai/Arduino_LED_matrix_sketch/archive/master.zip


#define PIN_DATAIN 4
#define PIN_CLK 3
#define PIN_LOAD 2
#define NBR_MTX 16 //number of matrices attached, per group

LedControl lc_grp1=LedControl(PIN_DATAIN, PIN_CLK, PIN_LOAD, NBR_MTX);

void setup()
{
  for (int i=0; i< NBR_MTX; i++)
  {
    lc.shutdown(i,false);

    lc.setIntensity(i,4);
    lc.clearDisplay(i);
    delay(10);
  }
}

// Connector on left, writing "up"
// L to R, Matrix 0 to 3
// (row,col) 0,0=bottom,right
// row counts up, column counts left

void loop()
{
  lc.setLed(mat,row,col, true);
  delay(50); 
}

void onAcftNameChange(char* newValue) {
  // Change of Aircraft
  DcsBios::resetAllStates();
}
DcsBios::StringBuffer<24> AcftNameBuffer(0x0000, onAcftNameChange);

void onClA1Change(unsigned int newValue) {
    // ENG START CYCLE
}
DcsBios::IntegerBuffer clA1Buffer(0x10d4, 0x0001, 0, onClA1Change);

void onClA2Change(unsigned int newValue) {
    // L-HYD PRESS
}
DcsBios::IntegerBuffer clA2Buffer(0x10d4, 0x0002, 1, onClA2Change);

void onClA3Change(unsigned int newValue) {
    // R-HYD PRESS
}
DcsBios::IntegerBuffer clA3Buffer(0x10d4, 0x0004, 2, onClA3Change);

void onClA4Change(unsigned int newValue) {
    // GUN UNSAFE
}
DcsBios::IntegerBuffer clA4Buffer(0x10d4, 0x0008, 3, onClA4Change);

void onClB1Change(unsigned int newValue) {
    // ANTI-SKID
}
DcsBios::IntegerBuffer clB1Buffer(0x10d4, 0x0010, 4, onClB1Change);

void onClB2Change(unsigned int newValue) {
    // L-HYD RES
}
DcsBios::IntegerBuffer clB2Buffer(0x10d4, 0x0020, 5, onClB2Change);

void onClB3Change(unsigned int newValue) {
    R-HYD RES
}
DcsBios::IntegerBuffer clB3Buffer(0x10d4, 0x0040, 6, onClB3Change);

void onClB4Change(unsigned int newValue) {
    // OXY LOW
}
DcsBios::IntegerBuffer clB4Buffer(0x10d4, 0x0080, 7, onClB4Change);

void onClC1Change(unsigned int newValue) {
    // ELEV DISENG
}
DcsBios::IntegerBuffer clC1Buffer(0x10d4, 0x0100, 8, onClC1Change);

void onClC2Change(unsigned int newValue) {
    // VOID1
}
DcsBios::IntegerBuffer clC2Buffer(0x10d4, 0x0200, 9, onClC2Change);

void onClC3Change(unsigned int newValue) {
    // SEAT NOT ARMED
}
DcsBios::IntegerBuffer clC3Buffer(0x10d4, 0x0400, 10, onClC3Change);

void onClC4Change(unsigned int newValue) {
    // BLEED AIR LEAK
}
DcsBios::IntegerBuffer clC4Buffer(0x10d4, 0x0800, 11, onClC4Change);

void onClD1Change(unsigned int newValue) {
    // AIL DISENG
}
DcsBios::IntegerBuffer clD1Buffer(0x10d4, 0x1000, 12, onClD1Change);

void onClD2Change(unsigned int newValue) {
    // L-AIL TAB
}
DcsBios::IntegerBuffer clD2Buffer(0x10d4, 0x2000, 13, onClD2Change);

void onClD3Change(unsigned int newValue) {
    // R-AIL TAB
}
DcsBios::IntegerBuffer clD3Buffer(0x10d4, 0x4000, 14, onClD3Change);

void onClD4Change(unsigned int newValue) {
    // SERVICE AIR HOT
}
DcsBios::IntegerBuffer clD4Buffer(0x10d4, 0x8000, 15, onClD4Change);

void onClE1Change(unsigned int newValue) {
    // PITCH SAS
}
DcsBios::IntegerBuffer clE1Buffer(0x10d6, 0x0001, 0, onClE1Change);

void onClE2Change(unsigned int newValue) {
    // L-ENG HOT
}
DcsBios::IntegerBuffer clE2Buffer(0x10d6, 0x0002, 1, onClE2Change);

void onClE3Change(unsigned int newValue) {
    // R-ENG HOT
}
DcsBios::IntegerBuffer clE3Buffer(0x10d6, 0x0004, 2, onClE3Change);

void onClE4Change(unsigned int newValue) {
    // WINDSHIELD HOT
}
DcsBios::IntegerBuffer clE4Buffer(0x10d6, 0x0008, 3, onClE4Change);

void onClF1Change(unsigned int newValue) {
    // YAW SAS
}
DcsBios::IntegerBuffer clF1Buffer(0x10d6, 0x0010, 4, onClF1Change);

void onClF2Change(unsigned int newValue) {
    // L-ENG OIL PRESS
}
DcsBios::IntegerBuffer clF2Buffer(0x10d6, 0x0020, 5, onClF2Change);

void onClF3Change(unsigned int newValue) {
    // R-ENG OIL PRESS
}
DcsBios::IntegerBuffer clF3Buffer(0x10d6, 0x0040, 6, onClF3Change);

void onClF4Change(unsigned int newValue) {
    // CICU
}
DcsBios::IntegerBuffer clF4Buffer(0x10d6, 0x0080, 7, onClF4Change);

void onClG1Change(unsigned int newValue) {
    // GCAS
}
DcsBios::IntegerBuffer clG1Buffer(0x10d6, 0x0100, 8, onClG1Change);

void onClG2Change(unsigned int newValue) {
    // L-MAIN PUMP
}
DcsBios::IntegerBuffer clG2Buffer(0x10d6, 0x0200, 9, onClG2Change);

void onClG3Change(unsigned int newValue) {
    // R-MAIN PUMP
}
DcsBios::IntegerBuffer clG3Buffer(0x10d6, 0x0400, 10, onClG3Change);

void onClG4Change(unsigned int newValue) {
    // VOID2
}
DcsBios::IntegerBuffer clG4Buffer(0x10d6, 0x0800, 11, onClG4Change);

void onClH1Change(unsigned int newValue) {
    // LASTE
}
DcsBios::IntegerBuffer clH1Buffer(0x10d6, 0x1000, 12, onClH1Change);

void onClH2Change(unsigned int newValue) {
    // L-WING PUMP
}
DcsBios::IntegerBuffer clH2Buffer(0x10d6, 0x2000, 13, onClH2Change);

void onClH3Change(unsigned int newValue) {
    // R-WING PUMP
}
DcsBios::IntegerBuffer clH3Buffer(0x10d6, 0x4000, 14, onClH3Change);

void onClH4Change(unsigned int newValue) {
    // HARS
}
DcsBios::IntegerBuffer clH4Buffer(0x10d6, 0x8000, 15, onClH4Change);

void onClI1Change(unsigned int newValue) {
    // IFF MODE-4
}
DcsBios::IntegerBuffer clI1Buffer(0x10d8, 0x0001, 0, onClI1Change);

void onClI2Change(unsigned int newValue) {
    // L-MAIN FUEL FLOW
}
DcsBios::IntegerBuffer clI2Buffer(0x10d8, 0x0002, 1, onClI2Change);

void onClI3Change(unsigned int newValue) {
    // R-MAIN FUEL LOW
}
DcsBios::IntegerBuffer clI3Buffer(0x10d8, 0x0004, 2, onClI3Change);

void onClI4Change(unsigned int newValue) {
    // L-R TKS UNEQUAL
}
DcsBios::IntegerBuffer clI4Buffer(0x10d8, 0x0008, 3, onClI4Change);

void onClJ1Change(unsigned int newValue) {
    // EAC
}
DcsBios::IntegerBuffer clJ1Buffer(0x10d8, 0x0010, 4, onClJ1Change);

void onClJ2Change(unsigned int newValue) {
    // L-FUEL PRESS
}
DcsBios::IntegerBuffer clJ2Buffer(0x10d8, 0x0020, 5, onClJ2Change);

void onClJ3Change(unsigned int newValue) {
    // R-FUEL PRESS
}
DcsBios::IntegerBuffer clJ3Buffer(0x10d8, 0x0040, 6, onClJ3Change);

void onClJ4Change(unsigned int newValue) {
    // NAV
}
DcsBios::IntegerBuffer clJ4Buffer(0x10d8, 0x0080, 7, onClJ4Change);

void onClK1Change(unsigned int newValue) {
    // STALL SYS
}
DcsBios::IntegerBuffer clK1Buffer(0x10d8, 0x0100, 8, onClK1Change);

void onClK2Change(unsigned int newValue) {
    // L-CONV
}
DcsBios::IntegerBuffer clK2Buffer(0x10d8, 0x0200, 9, onClK2Change);

void onClK3Change(unsigned int newValue) {
    // R-CONV
}
DcsBios::IntegerBuffer clK3Buffer(0x10d8, 0x0400, 10, onClK3Change);

void onClK4Change(unsigned int newValue) {
    // CADC
}
DcsBios::IntegerBuffer clK4Buffer(0x10d8, 0x0800, 11, onClK4Change);

void onClL1Change(unsigned int newValue) {
    // APU GEN
}
DcsBios::IntegerBuffer clL1Buffer(0x10d8, 0x1000, 12, onClL1Change);

void onClL2Change(unsigned int newValue) {
    // L-GEN
}
DcsBios::IntegerBuffer clL2Buffer(0x10d8, 0x2000, 13, onClL2Change);

void onClL3Change(unsigned int newValue) {
    // R-GEN
}
DcsBios::IntegerBuffer clL3Buffer(0x10d8, 0x4000, 14, onClL3Change);

void onClL4Change(unsigned int newValue) {
    // INST INV
}
DcsBios::IntegerBuffer clL4Buffer(0x10d8, 0x8000, 15, onClL4Change);

void onCmsp1Change(char* newValue) {
    /* your code here */
}
DcsBios::StringBuffer<19> cmsp1Buffer(0x1000, onCmsp1Change);

void onCmsp2Change(char* newValue) {
    /* your code here */
}
DcsBios::StringBuffer<19> cmsp2Buffer(0x1014, onCmsp2Change);

DcsBios::Switch2Pos cmspArw1("CMSP_ARW1", PIN);
DcsBios::Switch2Pos cmspArw2("CMSP_ARW2", PIN);
DcsBios::Switch2Pos cmspArw3("CMSP_ARW3", PIN);
DcsBios::Switch2Pos cmspArw4("CMSP_ARW4", PIN);

DcsBios::Potentiometer cmspBrt("CMSP_BRT", PIN);
//Countermeasure Dispenser OFF - ON - (MENU)
DcsBios::Switch3Pos cmspDisp("CMSP_DISP", PIN_A, PIN_B);
//Jammer OFF - ON - (MENU)
DcsBios::Switch3Pos cmspJmr("CMSP_JMR", PIN_A, PIN_B);
DcsBios::Switch2Pos cmspJtsn("CMSP_JTSN", PIN);

const byte cmspModePins[5] = {PIN_0, PIN_1, PIN_2, PIN_3, PIN_4};
DcsBios::SwitchMultiPos cmspMode("CMSP_MODE", cmspModePins, 5);

DcsBios::Switch3Pos cmspMws("CMSP_MWS", PIN_A, PIN_B);

DcsBios::Switch2Pos cmspRtn("CMSP_RTN", PIN);
//Radar Warning Receiver OFF - ON - (MENU)
DcsBios::Switch3Pos cmspRwr("CMSP_RWR", PIN_A, PIN_B);

DcsBios::Switch3Pos cmspUpdn("CMSP_UPDN", PIN_A, PIN_B);
