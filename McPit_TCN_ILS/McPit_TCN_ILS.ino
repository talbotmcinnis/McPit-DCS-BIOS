#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>

typedef DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> McRotary;

//http://werner.rothschopf.net/201909_arduino_ht16k33.htm
#include <Wire.h>
// SCL D0
// SDA D1
#include <NoiascaHt16k33.h>
Noiasca_ht16k33_hw_14_4 tcn_led = Noiasca_ht16k33_hw_14_4();

#include <TM1637.h>
//https://github.com/AKJ7/TM1637
// Instantiation and pins configurations
// (CLK, DIO)
TM1637 ils_led(21, 20);


void setup()
{
  ils_led.begin();
  ils_led.setBrightness(1);
  ils_led.display("123.456");

  Wire.begin(); //Join I2C bus
  tcn_led.begin(0x70,1);
  tcn_led.setDigits(4);
  tcn_led.setBrightness(0);  // 0 to 15
  tcn_led.print("--- ");
  
  DcsBios::setup();
}

void loop()
{
  DcsBios::loop();
}

// TODO: Hysterisis on my volumes

void onAcftNameChange(char* newValue) {
  // Change of Aircraft
  DcsBios::resetAllStates();
}
DcsBios::StringBuffer<24> AcftNameBuffer(0x0000, onAcftNameChange);

McRotary tacan1("TACAN_1", "DEC", "INC", 16, 17);

McRotary tacan10("TACAN_10", "DEC", "INC", 2, 3);

//DcsBios::Switch2Pos tacanXy("TACAN_XY", 4);
DcsBios::ActionButton tacanXyToggle("TACAN_XY", "TOGGLE", 4);

void onTacanChannelChange(char* newValue) {
    tcn_led.print(newValue);
}
DcsBios::StringBuffer<4> tacanChannelBuffer(0x1162, onTacanChannelChange);



const byte tacanModePins[5] = {5, 6, 7, 8, 9};
DcsBios::SwitchMultiPos tacanMode("TACAN_MODE", tacanModePins, 5);

//DcsBios::Potentiometer tacanVol("TACAN_VOL", A0);

DcsBios::Switch2Pos ilsPwr("ILS_PWR", 26);

//DcsBios::Potentiometer ilsVol("ILS_VOL", A1);

McRotary ilsKhz("ILS_KHZ", "DEC", "INC", 25,24);

McRotary ilsMhz("ILS_MHZ", "DEC", "INC", 23, 22);

char ilsBuffer[8];
void onIlsFrequencySChange(char* newValue) {
    // LEDs on my board are reversed, so remap here
    ilsBuffer[3] = newValue[0];
    ilsBuffer[2] = newValue[1];
    ilsBuffer[0] = newValue[2];
    ilsBuffer[1] = newValue[3];
    ilsBuffer[6] = newValue[4];
    ilsBuffer[5] = newValue[5];
    ilsBuffer[4] = ' ';
    ilsBuffer[7] = 0;
    ils_led.display((const char*)ilsBuffer);
}
DcsBios::StringBuffer<6> ilsFrequencySBuffer(0x12d8, onIlsFrequencySChange);
