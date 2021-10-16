#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// SCL D0
// SDA D1

// Set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C cmsp_lcd(0x27, 20, 2);

#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

#include <LedControlMS.h>
//https://github.com/shaai/Arduino_LED_matrix_sketch/archive/master.zip

#define NBR_MTX_PER_CONTROLLER 8 //number of matrices attached (Reminder lib supports max 8)
// PIN_DATAIN, PIN_CLK, PIN_LOAD, NBR_MTX
LedControl lc1=LedControl(4, 3, 2, NBR_MTX_PER_CONTROLLER);
LedControl lc2=LedControl(26,25,24,NBR_MTX_PER_CONTROLLER);

void setup()
{
  cmsp_lcd.init();
  // TODO: Consider binding backlight to... something
  cmsp_lcd.clear();
  cmsp_lcd.noBacklight();

  cmsp_lcd.setCursor(0,0);
  cmsp_lcd.print("--McPit--");
  cmsp_lcd.backlight();

  
  for (int i=0; i< NBR_MTX_PER_CONTROLLER; i++)
  {
    lc1.shutdown(i,false);  lc2.shutdown(i,false);

    lc1.setIntensity(i,8);  lc2.setIntensity(i,8);
    lc1.clearDisplay(i);    lc2.clearDisplay(i);
  }

  onClC2Change(1);
  onClG4Change(1);

  DcsBios::setup();
}

void loop()
{
  //lc1.setLed(mat,row,col, true);
  DcsBios::loop();
}

void onAcftNameChange(char* newValue) {
  // Change of Aircraft
  DcsBios::resetAllStates();
}
DcsBios::StringBuffer<24> AcftNameBuffer(0x0000, onAcftNameChange);

void onClA1Change(unsigned int newValue) {
    // ENG START CYCLE
    //lc1.setLed(mat,row,col, newValue);
    lc2.setLed(11-8,6,0, newValue);
    lc2.setLed(11-8,7,0, newValue);
    lc2.setLed(11-8,7,1, newValue);

    lc2.setLed(12-8,5,6, newValue);
    lc2.setLed(12-8,6,6, newValue);
    lc2.setLed(12-8,7,6, newValue);

    lc2.setLed(12-8,4,7, newValue);
    lc2.setLed(12-8,5,7, newValue);
    lc2.setLed(12-8,6,7, newValue);
    lc2.setLed(12-8,7,7, newValue);
}
DcsBios::IntegerBuffer clA1Buffer(0x10d4, 0x0001, 0, onClA1Change);

void onClA2Change(unsigned int newValue) {
    // L-HYD PRESS
    lc2.setLed(11-8,1,0, newValue);
    lc2.setLed(11-8,2,0, newValue);
    lc2.setLed(11-8,3,0, newValue);
    lc2.setLed(11-8,4,0, newValue);
    lc2.setLed(11-8,5,0, newValue);

    lc2.setLed(11-8,1,1, newValue);
    lc2.setLed(11-8,2,1, newValue);
    lc2.setLed(11-8,3,1, newValue);
    lc2.setLed(11-8,4,1, newValue);
    
}
DcsBios::IntegerBuffer clA2Buffer(0x10d4, 0x0002, 1, onClA2Change);

void onClA3Change(unsigned int newValue) {
    // R-HYD PRESS
    lc1.setLed(4,0,6, newValue);
    lc1.setLed(4,1,6, newValue);
    lc1.setLed(4,2,6, newValue);
    lc1.setLed(4,3,6, newValue);
    lc1.setLed(4,4,6, newValue);
    lc1.setLed(4,5,6, newValue);

    lc1.setLed(4,0,7, newValue);
    lc1.setLed(4,1,7, newValue);
    lc1.setLed(4,2,7, newValue);
    lc1.setLed(4,3,7, newValue);
    lc1.setLed(4,4,7, newValue);
    lc1.setLed(4,5,7, newValue);
}
DcsBios::IntegerBuffer clA3Buffer(0x10d4, 0x0004, 2, onClA3Change);

void onClA4Change(unsigned int newValue) {
    // GUN UNSAFE
    lc1.setLed(3,5,0, newValue);
    lc1.setLed(3,6,0, newValue);
    lc1.setLed(3,7,0, newValue);
    lc1.setLed(3,4,1, newValue);
    lc1.setLed(3,5,1, newValue);
    lc1.setLed(3,6,1, newValue);
    lc1.setLed(3,7,1, newValue);
    lc1.setLed(3,5,2, newValue);
    lc1.setLed(3,6,2, newValue);
    lc1.setLed(3,7,2, newValue);

    lc1.setLed(4,6,6, newValue);
    lc1.setLed(4,7,6, newValue);
    lc1.setLed(4,7,7, newValue);
}
DcsBios::IntegerBuffer clA4Buffer(0x10d4, 0x0008, 3, onClA4Change);

void onClB1Change(unsigned int newValue) {
    // ANTI-SKID
    lc2.setLed(11-8,7,3, newValue);
    lc2.setLed(11-8,7,4, newValue);

    lc2.setLed(12-8,6,3, newValue);
    lc2.setLed(12-8,7,3, newValue);

    lc2.setLed(12-8,5,4, newValue);
    lc2.setLed(12-8,6,4, newValue);
    lc2.setLed(12-8,7,4, newValue);

    lc2.setLed(12-8,6,5, newValue);
    lc2.setLed(12-8,7,5, newValue);
}
DcsBios::IntegerBuffer clB1Buffer(0x10d4, 0x0010, 4, onClB1Change);

void onClB2Change(unsigned int newValue) {
    // L-HYD RES
    lc2.setLed(11-8,2,2, newValue);
    lc2.setLed(11-8,3,2, newValue);
    lc2.setLed(11-8,4,2, newValue);

    lc2.setLed(11-8,1,3, newValue);
    lc2.setLed(11-8,2,3, newValue);
    lc2.setLed(11-8,3,3, newValue);
    lc2.setLed(11-8,4,3, newValue);
    lc2.setLed(11-8,5,3, newValue);

    lc2.setLed(11-8,1,4, newValue);
    lc2.setLed(11-8,2,4, newValue);
    lc2.setLed(11-8,3,4, newValue);
    lc2.setLed(11-8,4,4, newValue);
}
DcsBios::IntegerBuffer clB2Buffer(0x10d4, 0x0020, 5, onClB2Change);

void onClB3Change(unsigned int newValue) {
    // R-HYD RES
    lc1.setLed(4,1,3, newValue);
    lc1.setLed(4,2,3, newValue);
    lc1.setLed(4,3,3, newValue);
    lc1.setLed(4,4,3, newValue);

    lc1.setLed(4,0,4, newValue);
    lc1.setLed(4,1,4, newValue);
    lc1.setLed(4,2,4, newValue);
    lc1.setLed(4,3,4, newValue);
    lc1.setLed(4,4,4, newValue);
    lc1.setLed(4,5,4, newValue);

    lc1.setLed(4,1,5, newValue);
    lc1.setLed(4,2,5, newValue);
    lc1.setLed(4,3,5, newValue);
    lc1.setLed(4,4,5, newValue);
    
}
DcsBios::IntegerBuffer clB3Buffer(0x10d4, 0x0040, 6, onClB3Change);

void onClB4Change(unsigned int newValue) {
    // OXY LOW
    lc1.setLed(3,5,3, newValue);
    lc1.setLed(3,6,3, newValue);
    lc1.setLed(3,7,3, newValue);
    lc1.setLed(3,5,4, newValue);
    lc1.setLed(3,6,4, newValue);
    lc1.setLed(3,7,4, newValue);

    lc1.setLed(4,7,3, newValue);
    lc1.setLed(4,7,4, newValue);
}
DcsBios::IntegerBuffer clB4Buffer(0x10d4, 0x0080, 7, onClB4Change);

void onClC1Change(unsigned int newValue) {
    // ELEV DISENG
    lc2.setLed(11-8,7,5, newValue);
    lc2.setLed(11-8,7,6, newValue);
    lc2.setLed(11-8,7,7, newValue);

    lc2.setLed(12-8,5,0, newValue);
    lc2.setLed(12-8,6,0, newValue);
    lc2.setLed(12-8,7,0, newValue);

    lc2.setLed(12-8,5,1, newValue);
    lc2.setLed(12-8,6,1, newValue);
    lc2.setLed(12-8,7,1, newValue);

    lc2.setLed(12-8,5,2, newValue);
    lc2.setLed(12-8,6,2, newValue);
    lc2.setLed(12-8,7,2, newValue);
}
DcsBios::IntegerBuffer clC1Buffer(0x10d4, 0x0100, 8, onClC1Change);

void onClC2Change(unsigned int newValue) {
    // VOID1
    lc2.setLed(11-8,1,5, newValue);
    lc2.setLed(11-8,2,5, newValue);
    lc2.setLed(11-8,3,5, newValue);
    lc2.setLed(11-8,4,5, newValue);
    
    lc2.setLed(11-8,1,6, newValue);
    lc2.setLed(11-8,2,6, newValue);
    lc2.setLed(11-8,3,6, newValue);
    lc2.setLed(11-8,4,6, newValue);
}
DcsBios::IntegerBuffer clC2Buffer(0x10d4, 0x0200, 9, onClC2Change);

void onClC3Change(unsigned int newValue) {
    // SEAT NOT ARMED
    lc1.setLed(4,1,0, newValue);
    lc1.setLed(4,2,0, newValue);
    lc1.setLed(4,3,0, newValue);
    lc1.setLed(4,4,0, newValue);
    lc1.setLed(4,5,0, newValue);

    lc1.setLed(4,0,1, newValue);
    lc1.setLed(4,1,1, newValue);
    lc1.setLed(4,2,1, newValue);
    lc1.setLed(4,3,1, newValue);
    lc1.setLed(4,4,1, newValue);
    lc1.setLed(4,5,1, newValue);

    lc1.setLed(4,0,2, newValue);
    lc1.setLed(4,1,2, newValue);
    lc1.setLed(4,2,2, newValue);
    lc1.setLed(4,3,2, newValue);
    lc1.setLed(4,4,2, newValue);
    lc1.setLed(4,5,2, newValue);
}
DcsBios::IntegerBuffer clC3Buffer(0x10d4, 0x0400, 10, onClC3Change);

void onClC4Change(unsigned int newValue) {
    // BLEED AIR LEAK
    lc1.setLed(3,4,5, newValue);
    lc1.setLed(3,6,5, newValue);
    lc1.setLed(3,7,5, newValue);
    lc1.setLed(3,4,6, newValue);
    lc1.setLed(3,6,6, newValue);
    lc1.setLed(3,7,6, newValue);
    lc1.setLed(3,4,7, newValue);
    lc1.setLed(3,6,7, newValue);
    lc1.setLed(3,7,7, newValue);

    lc1.setLed(4,6,0, newValue);
    lc1.setLed(4,7,0, newValue);
    lc1.setLed(4,6,1, newValue);
    lc1.setLed(4,7,1, newValue);
    lc1.setLed(4,6,2, newValue);
    lc1.setLed(4,7,2, newValue);
}
DcsBios::IntegerBuffer clC4Buffer(0x10d4, 0x0800, 11, onClC4Change);

void onClD1Change(unsigned int newValue) {
    // AIL DISENG
    lc2.setLed(10-8,7,0, newValue);
    lc2.setLed(10-8,7,1, newValue);
    lc2.setLed(10-8,7,2, newValue);

    lc2.setLed(13-8,5,6, newValue);
    lc2.setLed(13-8,6,6, newValue);
    lc2.setLed(13-8,7,6, newValue);

    lc2.setLed(13-8,5,7, newValue);
    lc2.setLed(13-8,6,7, newValue);
    lc2.setLed(13-8,7,7, newValue);
}
DcsBios::IntegerBuffer clD1Buffer(0x10d4, 0x1000, 12, onClD1Change);

void onClD2Change(unsigned int newValue) {
    // L-AIL TAB
    lc2.setLed(10-8,1,0, newValue);
    lc2.setLed(10-8,2,0, newValue);
    lc2.setLed(10-8,3,0, newValue);
    lc2.setLed(10-8,4,0, newValue);

    lc2.setLed(10-8,1,1, newValue);
    lc2.setLed(10-8,2,1, newValue);
    lc2.setLed(10-8,3,1, newValue);
    lc2.setLed(10-8,4,1, newValue);
}
DcsBios::IntegerBuffer clD2Buffer(0x10d4, 0x2000, 13, onClD2Change);

void onClD3Change(unsigned int newValue) {
    // R-AIL TAB
    lc1.setLed(5,1,6, newValue);
    lc1.setLed(5,2,6, newValue);
    lc1.setLed(5,3,6, newValue);
    lc1.setLed(5,4,6, newValue);

    lc1.setLed(5,1,7, newValue);
    lc1.setLed(5,2,7, newValue);
    lc1.setLed(5,3,7, newValue);
    lc1.setLed(5,4,7, newValue);
}
DcsBios::IntegerBuffer clD3Buffer(0x10d4, 0x4000, 14, onClD3Change);

void onClD4Change(unsigned int newValue) {
    // SERVICE AIR HOT
    lc1.setLed(2,4,0, newValue);
    lc1.setLed(2,5,0, newValue);
    lc1.setLed(2,6,0, newValue);
    lc1.setLed(2,7,0, newValue);
    lc1.setLed(2,4,1, newValue);
    lc1.setLed(2,5,1, newValue);
    lc1.setLed(2,6,1, newValue);
    lc1.setLed(2,7,1, newValue);
    lc1.setLed(2,5,2, newValue);
    lc1.setLed(2,6,2, newValue);
    lc1.setLed(2,7,2, newValue);

    lc1.setLed(5,6,6, newValue);
    lc1.setLed(5,7,6, newValue);
    lc1.setLed(5,6,7, newValue);
    lc1.setLed(5,7,7, newValue);
}
DcsBios::IntegerBuffer clD4Buffer(0x10d4, 0x8000, 15, onClD4Change);

void onClE1Change(unsigned int newValue) {
    // PITCH SAS
    lc2.setLed(10-8,7,3, newValue);
    lc2.setLed(10-8,7,4, newValue);

    lc2.setLed(13-8,5,3, newValue);
    lc2.setLed(13-8,6,3, newValue);
    lc2.setLed(13-8,7,3, newValue);

    lc2.setLed(13-8,5,4, newValue);
    lc2.setLed(13-8,6,4, newValue);
    lc2.setLed(13-8,7,4, newValue);
}
DcsBios::IntegerBuffer clE1Buffer(0x10d6, 0x0001, 0, onClE1Change);

void onClE2Change(unsigned int newValue) {
    // L-ENG HOT
    lc2.setLed(10-8,1,3, newValue);
    lc2.setLed(10-8,2,3, newValue);
    lc2.setLed(10-8,3,3, newValue);
    lc2.setLed(10-8,4,3, newValue);
    lc2.setLed(10-8,5,3, newValue);

    lc2.setLed(10-8,1,4, newValue);
    lc2.setLed(10-8,2,4, newValue);
    lc2.setLed(10-8,3,4, newValue);
    lc2.setLed(10-8,4,4, newValue);
    
}
DcsBios::IntegerBuffer clE2Buffer(0x10d6, 0x0002, 1, onClE2Change);

void onClE3Change(unsigned int newValue) {
    // R-ENG HOT
    lc1.setLed(5,1,3, newValue);
    lc1.setLed(5,2,3, newValue);
    lc1.setLed(5,3,3, newValue);
    lc1.setLed(5,4,3, newValue);
    lc1.setLed(5,5,3, newValue);

    lc1.setLed(5,1,4, newValue);
    lc1.setLed(5,2,4, newValue);
    lc1.setLed(5,3,4, newValue);
    lc1.setLed(5,4,4, newValue);
    lc1.setLed(5,5,4, newValue);
}
DcsBios::IntegerBuffer clE3Buffer(0x10d6, 0x0004, 2, onClE3Change);

void onClE4Change(unsigned int newValue) {
    // WINDSHIELD HOT
    lc1.setLed(2,4,3, newValue);
    lc1.setLed(2,5,3, newValue);
    lc1.setLed(2,6,3, newValue);
    lc1.setLed(2,7,3, newValue);
    lc1.setLed(2,4,4, newValue);
    lc1.setLed(2,5,4, newValue);
    lc1.setLed(2,6,4, newValue);
    lc1.setLed(2,7,4, newValue);

    lc1.setLed(2,4,5, newValue);
    lc1.setLed(2,5,5, newValue);
    lc1.setLed(2,6,5, newValue);
    lc1.setLed(2,7,5, newValue);

    lc1.setLed(5,6,3, newValue);
    lc1.setLed(5,7,3, newValue);
    lc1.setLed(5,6,4, newValue);
    lc1.setLed(5,7,4, newValue);
}
DcsBios::IntegerBuffer clE4Buffer(0x10d6, 0x0008, 3, onClE4Change);

void onClF1Change(unsigned int newValue) {
    // YAW SAS
    lc2.setLed(10-8,7,6, newValue);
    lc2.setLed(10-8,7,7, newValue);

    lc2.setLed(13-8,5,0, newValue);
    lc2.setLed(13-8,6,0, newValue);
    lc2.setLed(13-8,7,0, newValue);

    lc2.setLed(13-8,5,1, newValue);
    lc2.setLed(13-8,6,1, newValue);
    lc2.setLed(13-8,7,1, newValue);

    lc2.setLed(13-8,6,2, newValue);
    lc2.setLed(13-8,7,2, newValue);
}
DcsBios::IntegerBuffer clF1Buffer(0x10d6, 0x0010, 4, onClF1Change);

void onClF2Change(unsigned int newValue) {
    // L-ENG OIL PRESS
    lc2.setLed(10-8,1,6, newValue);
    lc2.setLed(10-8,2,6, newValue);
    lc2.setLed(10-8,3,6, newValue);
    lc2.setLed(10-8,4,6, newValue);
    lc2.setLed(10-8,5,6, newValue);

    lc2.setLed(10-8,0,7, newValue);
    lc2.setLed(10-8,1,7, newValue);
    lc2.setLed(10-8,2,7, newValue);
    lc2.setLed(10-8,3,7, newValue);
    lc2.setLed(10-8,4,7, newValue);
    lc2.setLed(10-8,5,7, newValue);
}
DcsBios::IntegerBuffer clF2Buffer(0x10d6, 0x0020, 5, onClF2Change);

void onClF3Change(unsigned int newValue) {
    // R-ENG OIL PRESS
    lc1.setLed(5,0,0, newValue);
    lc1.setLed(5,1,0, newValue);
    lc1.setLed(5,2,0, newValue);
    lc1.setLed(5,3,0, newValue);
    lc1.setLed(5,4,0, newValue);
    lc1.setLed(5,5,0, newValue);

    lc1.setLed(5,0,1, newValue);
    lc1.setLed(5,1,1, newValue);
    lc1.setLed(5,2,1, newValue);
    lc1.setLed(5,3,1, newValue);
    lc1.setLed(5,4,1, newValue);
    lc1.setLed(5,5,1, newValue);

    lc1.setLed(5,2,2, newValue);
    lc1.setLed(5,3,2, newValue);
    lc1.setLed(5,4,2, newValue);
}
DcsBios::IntegerBuffer clF3Buffer(0x10d6, 0x0040, 6, onClF3Change);

void onClF4Change(unsigned int newValue) {
    // CICU
    lc1.setLed(2,5,6, newValue);
    lc1.setLed(2,6,6, newValue);
    lc1.setLed(2,7,6, newValue);
    lc1.setLed(2,5,7, newValue);
    lc1.setLed(2,6,7, newValue);
    lc1.setLed(2,7,7, newValue);

    lc1.setLed(5,7,0, newValue);
    lc1.setLed(5,7,1, newValue);
}
DcsBios::IntegerBuffer clF4Buffer(0x10d6, 0x0080, 7, onClF4Change);

void onClG1Change(unsigned int newValue) {
    // GCAS
    lc2.setLed(9-8,7,1, newValue);

    lc2.setLed(14-8,6,5, newValue);
    lc2.setLed(14-8,7,5, newValue);

    lc2.setLed(14-8,5,6, newValue);
    lc2.setLed(14-8,6,6, newValue);
    lc2.setLed(14-8,7,6, newValue);

    lc2.setLed(14-8,6,7, newValue);
    lc2.setLed(14-8,7,7, newValue);
}
DcsBios::IntegerBuffer clG1Buffer(0x10d6, 0x0100, 8, onClG1Change);

void onClG2Change(unsigned int newValue) {
    // L-MAIN PUMP
    lc2.setLed(9-8,2,0, newValue);
    lc2.setLed(9-8,3,0, newValue);
    lc2.setLed(9-8,4,0, newValue);

    lc2.setLed(9-8,1,1, newValue);
    lc2.setLed(9-8,2,1, newValue);
    lc2.setLed(9-8,3,1, newValue);
    lc2.setLed(9-8,4,1, newValue);

    lc2.setLed(9-8,1,2, newValue);
    lc2.setLed(9-8,2,2, newValue);
    lc2.setLed(9-8,3,2, newValue);
    lc2.setLed(9-8,4,2, newValue);
}
DcsBios::IntegerBuffer clG2Buffer(0x10d6, 0x0200, 9, onClG2Change);

void onClG3Change(unsigned int newValue) {
    // R-MAIN PUMP
    lc1.setLed(6,1,5, newValue);
    lc1.setLed(6,2,5, newValue);
    lc1.setLed(6,3,5, newValue);
    lc1.setLed(6,4,5, newValue);

    lc1.setLed(6,1,6, newValue);
    lc1.setLed(6,2,6, newValue);
    lc1.setLed(6,3,6, newValue);
    lc1.setLed(6,4,6, newValue);

    lc1.setLed(6,1,7, newValue);
    lc1.setLed(6,2,7, newValue);
    lc1.setLed(6,3,7, newValue);
    lc1.setLed(6,4,7, newValue);
    lc1.setLed(6,5,7, newValue);
}
DcsBios::IntegerBuffer clG3Buffer(0x10d6, 0x0400, 10, onClG3Change);

void onClG4Change(unsigned int newValue) {
    // VOID2
    lc1.setLed(1,4,0, newValue);
    lc1.setLed(1,5,0, newValue);
    lc1.setLed(1,6,0, newValue);
    lc1.setLed(1,7,0, newValue);
    lc1.setLed(1,4,1, newValue);
    lc1.setLed(1,5,1, newValue);
    lc1.setLed(1,6,1, newValue);
    lc1.setLed(1,7,1, newValue);
    lc1.setLed(1,4,2, newValue);
    lc1.setLed(1,5,2, newValue);
    lc1.setLed(1,6,2, newValue);
    lc1.setLed(1,7,2, newValue);

    lc1.setLed(6,7,5, newValue);
    lc1.setLed(6,6,6, newValue);
    lc1.setLed(6,7,6, newValue);
    lc1.setLed(6,7,7, newValue);
}
DcsBios::IntegerBuffer clG4Buffer(0x10d6, 0x0800, 11, onClG4Change);

void onClH1Change(unsigned int newValue) {
    // LASTE
    lc2.setLed(9-8,7,3, newValue);
    lc2.setLed(9-8,7,4, newValue);

    lc2.setLed(14-8,5,3, newValue);
    lc2.setLed(14-8,6,3, newValue);
    lc2.setLed(14-8,7,3, newValue);

    lc2.setLed(14-8,5,4, newValue);
    lc2.setLed(14-8,6,4, newValue);
    lc2.setLed(14-8,7,4, newValue);
}
DcsBios::IntegerBuffer clH1Buffer(0x10d6, 0x1000, 12, onClH1Change);

void onClH2Change(unsigned int newValue) {
    // L-WING PUMP
    lc2.setLed(9-8,1,3, newValue);
    lc2.setLed(9-8,2,3, newValue);
    lc2.setLed(9-8,3,3, newValue);
    lc2.setLed(9-8,4,3, newValue);
    lc2.setLed(9-8,5,3, newValue);

    lc2.setLed(9-8,1,4, newValue);
    lc2.setLed(9-8,2,4, newValue);
    lc2.setLed(9-8,3,4, newValue);
    lc2.setLed(9-8,4,4, newValue);
    lc2.setLed(9-8,5,4, newValue);

    lc2.setLed(9-8,2,5, newValue);
    lc2.setLed(9-8,3,5, newValue);
    lc2.setLed(9-8,4,5, newValue);
}
DcsBios::IntegerBuffer clH2Buffer(0x10d6, 0x2000, 13, onClH2Change);

void onClH3Change(unsigned int newValue) {
    // R-WING PUMP
    lc1.setLed(6,1,2, newValue);
    lc1.setLed(6,2,2, newValue);
    lc1.setLed(6,3,2, newValue);
    lc1.setLed(6,4,2, newValue);

    lc1.setLed(6,1,3, newValue);
    lc1.setLed(6,2,3, newValue);
    lc1.setLed(6,3,3, newValue);
    lc1.setLed(6,4,3, newValue);

    lc1.setLed(6,1,4, newValue);
    lc1.setLed(6,2,4, newValue);
    lc1.setLed(6,3,4, newValue);
    lc1.setLed(6,4,4, newValue);
}
DcsBios::IntegerBuffer clH3Buffer(0x10d6, 0x4000, 14, onClH3Change);

void onClH4Change(unsigned int newValue) {
    // HARS
    //lc1.setLed(1,6,3, newValue);
    //lc1.setLed(1,7,3, newValue);
    lc1.setLed(1,4,4, newValue);
    lc1.setLed(1,5,4, newValue);
    lc1.setLed(1,6,4, newValue);
    lc1.setLed(1,7,4, newValue);
    lc1.setLed(1,5,5, newValue);
    lc1.setLed(1,6,5, newValue);
    lc1.setLed(1,7,5, newValue);

    lc1.setLed(6,7,3, newValue);
}
DcsBios::IntegerBuffer clH4Buffer(0x10d6, 0x8000, 15, onClH4Change);

void onClI1Change(unsigned int newValue) {
    // IFF MODE-4
    lc2.setLed(9-8,7,6, newValue);
    lc2.setLed(9-8,6,7, newValue);
    lc2.setLed(9-8,7,7, newValue);

    lc2.setLed(14-8,5,0, newValue);
    lc2.setLed(14-8,6,0, newValue);
    lc2.setLed(14-8,7,0, newValue);

    lc2.setLed(14-8,6,1, newValue);
    lc2.setLed(14-8,7,1, newValue);
}
DcsBios::IntegerBuffer clI1Buffer(0x10d8, 0x0001, 0, onClI1Change);

void onClI2Change(unsigned int newValue) {
    // L-MAIN FUEL LOW
    lc2.setLed(9-8,0,6, newValue);
    lc2.setLed(9-8,1,6, newValue);
    lc2.setLed(9-8,2,6, newValue);
    lc2.setLed(9-8,3,6, newValue);
    lc2.setLed(9-8,4,6, newValue);

    lc2.setLed(9-8,0,7, newValue);
    lc2.setLed(9-8,1,7, newValue);
    lc2.setLed(9-8,2,7, newValue);
    lc2.setLed(9-8,3,7, newValue);
    lc2.setLed(9-8,4,7, newValue);
    lc2.setLed(9-8,5,7, newValue);
    
}
DcsBios::IntegerBuffer clI2Buffer(0x10d8, 0x0002, 1, onClI2Change);

void onClI3Change(unsigned int newValue) {
    // R-MAIN FUEL LOW
    lc1.setLed(6,0,0, newValue);
    lc1.setLed(6,1,0, newValue);
    lc1.setLed(6,2,0, newValue);
    lc1.setLed(6,3,0, newValue);
    lc1.setLed(6,4,0, newValue);
    lc1.setLed(6,5,0, newValue);

    lc1.setLed(6,1,1, newValue);
    lc1.setLed(6,2,1, newValue);
    lc1.setLed(6,3,1, newValue);
    lc1.setLed(6,4,1, newValue);
    lc1.setLed(6,5,1, newValue);
}
DcsBios::IntegerBuffer clI3Buffer(0x10d8, 0x0004, 2, onClI3Change);

void onClI4Change(unsigned int newValue) {
    // L-R TKS UNEQUAL
    lc1.setLed(0,4,0, newValue);
    lc1.setLed(0,5,0, newValue);
    lc1.setLed(0,6,0, newValue);
    lc1.setLed(0,7,0, newValue);
    lc1.setLed(1,4,6, newValue);
    lc1.setLed(1,5,6, newValue);
    lc1.setLed(1,6,6, newValue);
    lc1.setLed(1,7,6, newValue);
    lc1.setLed(1,4,7, newValue);
    lc1.setLed(1,5,7, newValue);
    lc1.setLed(1,6,7, newValue);
    lc1.setLed(1,7,7, newValue);

    lc1.setLed(6,7,0, newValue);
    lc1.setLed(6,7,1, newValue);
}
DcsBios::IntegerBuffer clI4Buffer(0x10d8, 0x0008, 3, onClI4Change);

void onClJ1Change(unsigned int newValue) {
    // EAC
    lc2.setLed(8-8,7,1, newValue);
    lc2.setLed(8-8,7,2, newValue);

    lc2.setLed(15-8,5,5, newValue);
    lc2.setLed(15-8,6,5, newValue);
    lc2.setLed(15-8,7,5, newValue);

    lc2.setLed(15-8,6,6, newValue);
    lc2.setLed(15-8,7,6, newValue);
}
DcsBios::IntegerBuffer clJ1Buffer(0x10d8, 0x0010, 4, onClJ1Change);

void onClJ2Change(unsigned int newValue) {
    // L-FUEL PRESS

    lc2.setLed(8-8,1,0, newValue);
    lc2.setLed(8-8,2,0, newValue);
    lc2.setLed(8-8,3,0, newValue);
    lc2.setLed(8-8,4,0, newValue);

    lc2.setLed(8-8,0,1, newValue);
    lc2.setLed(8-8,1,1, newValue);
    lc2.setLed(8-8,2,1, newValue);
    lc2.setLed(8-8,3,1, newValue);
    lc2.setLed(8-8,4,1, newValue);
    lc2.setLed(8-8,5,1, newValue);

    lc2.setLed(8-8,1,2, newValue);
    lc2.setLed(8-8,2,2, newValue);
    lc2.setLed(8-8,3,2, newValue);
    lc2.setLed(8-8,4,2, newValue);
    lc2.setLed(8-8,5,2, newValue);
    
}
DcsBios::IntegerBuffer clJ2Buffer(0x10d8, 0x0020, 5, onClJ2Change);

void onClJ3Change(unsigned int newValue) {
    // R-FUEL PRESS
    lc1.setLed(7,1,5, newValue);
    lc1.setLed(7,2,5, newValue);
    lc1.setLed(7,3,5, newValue);
    lc1.setLed(7,4,5, newValue);
    lc1.setLed(7,5,5, newValue);

    lc1.setLed(7,1,6, newValue);
    lc1.setLed(7,2,6, newValue);
    lc1.setLed(7,3,6, newValue);
    lc1.setLed(7,4,6, newValue);
    lc1.setLed(7,5,6, newValue);
}
DcsBios::IntegerBuffer clJ3Buffer(0x10d8, 0x0040, 6, onClJ3Change);

void onClJ4Change(unsigned int newValue) {
    // NAV
    lc1.setLed(0,5,1, newValue);
    lc1.setLed(0,6,1, newValue);
    lc1.setLed(0,7,1, newValue);
    lc1.setLed(0,5,2, newValue);
    lc1.setLed(0,6,2, newValue);
    lc1.setLed(0,7,2, newValue);

    lc1.setLed(7,7,5, newValue);
    lc1.setLed(7,7,6, newValue);
    
}
DcsBios::IntegerBuffer clJ4Buffer(0x10d8, 0x0080, 7, onClJ4Change);

void onClK1Change(unsigned int newValue) {
    // STALL SYS
    lc2.setLed(8-8,7,3, newValue);
    lc2.setLed(8-8,7,4, newValue);
    lc2.setLed(8-8,7,5, newValue);

    lc2.setLed(15-8,6,2, newValue);
    lc2.setLed(15-8,7,2, newValue);

    lc2.setLed(15-8,5,3, newValue);
    lc2.setLed(15-8,6,3, newValue);
    lc2.setLed(15-8,7,3, newValue);

    lc2.setLed(15-8,5,4, newValue);
    lc2.setLed(15-8,6,4, newValue);
    lc2.setLed(15-8,7,4, newValue);
}
DcsBios::IntegerBuffer clK1Buffer(0x10d8, 0x0100, 8, onClK1Change);

void onClK2Change(unsigned int newValue) {
    // L-CONV
    lc2.setLed(8-8,1,3, newValue);
    lc2.setLed(8-8,2,3, newValue);
    lc2.setLed(8-8,3,3, newValue);
    lc2.setLed(8-8,4,3, newValue);
    lc2.setLed(8-8,5,3, newValue);

    lc2.setLed(8-8,1,4, newValue);
    lc2.setLed(8-8,2,4, newValue);
    lc2.setLed(8-8,3,4, newValue);
    lc2.setLed(8-8,4,4, newValue);
    lc2.setLed(8-8,5,4, newValue);

    lc2.setLed(8-8,1,5, newValue);
    lc2.setLed(8-8,2,5, newValue);
    lc2.setLed(8-8,3,5, newValue);
    lc2.setLed(8-8,4,5, newValue);
    lc2.setLed(8-8,5,5, newValue);
}
DcsBios::IntegerBuffer clK2Buffer(0x10d8, 0x0200, 9, onClK2Change);

void onClK3Change(unsigned int newValue) {
    // R-CONV
    lc1.setLed(7,1,2, newValue);
    lc1.setLed(7,2,2, newValue);
    lc1.setLed(7,3,2, newValue);
    lc1.setLed(7,4,2, newValue);
    lc1.setLed(7,5,2, newValue);

    lc1.setLed(7,1,3, newValue);
    lc1.setLed(7,2,3, newValue);
    lc1.setLed(7,3,3, newValue);
    lc1.setLed(7,4,3, newValue);
    lc1.setLed(7,5,3, newValue);

    lc1.setLed(7,3,4, newValue);
    lc1.setLed(7,4,4, newValue);
    
}
DcsBios::IntegerBuffer clK3Buffer(0x10d8, 0x0400, 10, onClK3Change);

void onClK4Change(unsigned int newValue) {
    // CADC
    lc1.setLed(0,5,4, newValue);
    lc1.setLed(0,6,4, newValue);
    lc1.setLed(0,7,4, newValue);
    lc1.setLed(0,5,5, newValue);
    lc1.setLed(0,6,5, newValue);
    lc1.setLed(0,7,5, newValue);
}
DcsBios::IntegerBuffer clK4Buffer(0x10d8, 0x0800, 11, onClK4Change);

void onClL1Change(unsigned int newValue) {
    // APU GEN
    lc2.setLed(8-8,7,6, newValue);
    lc2.setLed(8-8,7,7, newValue);

    lc2.setLed(15-8,5,0, newValue);
    lc2.setLed(15-8,6,0, newValue);
    lc2.setLed(15-8,7,0, newValue);

    lc2.setLed(15-8,6,1, newValue);
    lc2.setLed(15-8,7,1, newValue);
}
DcsBios::IntegerBuffer clL1Buffer(0x10d8, 0x1000, 12, onClL1Change);

void onClL2Change(unsigned int newValue) {
    // L-GEN
    lc2.setLed(8-8,0,6, newValue);
    lc2.setLed(8-8,1,6, newValue);
    lc2.setLed(8-8,2,6, newValue);
    lc2.setLed(8-8,3,6, newValue);
    lc2.setLed(8-8,4,6, newValue);

    lc2.setLed(8-8,0,7, newValue);
    lc2.setLed(8-8,1,7, newValue);
    lc2.setLed(8-8,2,7, newValue);
    lc2.setLed(8-8,3,7, newValue);
    lc2.setLed(8-8,4,7, newValue);
}
DcsBios::IntegerBuffer clL2Buffer(0x10d8, 0x2000, 13, onClL2Change);

void onClL3Change(unsigned int newValue) {
    // R-GEN
    lc1.setLed(7,1,0, newValue);
    lc1.setLed(7,2,0, newValue);
    lc1.setLed(7,3,0, newValue);
    lc1.setLed(7,4,0, newValue);
    lc1.setLed(7,5,0, newValue);

    lc1.setLed(7,2,1, newValue);
    lc1.setLed(7,3,1, newValue);
    lc1.setLed(7,4,1, newValue);
}
DcsBios::IntegerBuffer clL3Buffer(0x10d8, 0x4000, 14, onClL3Change);

void onClL4Change(unsigned int newValue) {
    // INST INV
    lc1.setLed(0,6,6, newValue);
    lc1.setLed(0,5,7, newValue);
    lc1.setLed(0,6,7, newValue);
    lc1.setLed(0,7,7, newValue);
    
    lc1.setLed(7,7,0, newValue);
}
DcsBios::IntegerBuffer clL4Buffer(0x10d8, 0x8000, 15, onClL4Change);

void onCmsp1Change(char* newValue) {
  cmsp_lcd.setCursor(0,0);
  cmsp_lcd.print(newValue);
}
DcsBios::StringBuffer<19> cmsp1Buffer(0x1000, onCmsp1Change);

void onCmsp2Change(char* newValue) {
  cmsp_lcd.setCursor(0,1);
  cmsp_lcd.print(newValue);
}
DcsBios::StringBuffer<19> cmsp2Buffer(0x1014, onCmsp2Change);

void onCmspModeChange(unsigned int newValue) {
    if( newValue == 0 )
      cmsp_lcd.noBacklight();
    else
      cmsp_lcd.backlight();
}
DcsBios::IntegerBuffer cmspModeBuffer(0x10e4, 0x3800, 11, onCmspModeChange);

// CMSP relevant switches count: 4+12+5=21
// LCD 2, Cautions 6
// 36 available, so this fits
/*
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
*/
