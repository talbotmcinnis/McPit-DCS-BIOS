//#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_DEFAULT_SERIAL

// Because we need each device to be unique, we must change the product_ID in C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\usb_serial_hid\usb_private.h
// before each compilation
/*
#define STR_PRODUCT             L"MCPIT-NMSP"
#define PRODUCT_ID    0x0490
*/

#include <DcsBios.h> // Originally build for DCS Bios 0.1.x  Update For 0.2.11
#include <Servo.h>

// TODO: Extract this into my own library?
#include "Arduino.h"
#include <internal/PollingInput.h>


namespace DcsBios {
  /* Forked off Dcs Rotary Encoder, but trying to include filtering of the glitches my rotary present
   *  
   */

  class McPitRotaryEncoder : PollingInput {
    private:
      const char* msg_;
      const char* decArg_;
      const char* incArg_;
      const char* fastDecArg_;
      const char* fastIncArg_;
      char pinA_;
      char pinB_;
      char lastState_;
      signed char delta_;
      char stepsPerDetent_;

      unsigned long timeLastDetent_;
      
      char readState() {
        return (digitalRead(pinA_) << 1) | digitalRead(pinB_);
      }
      
      void pollInput() {
        char state = readState();
        switch(lastState_) {
          case 0:
            if (state == 2) delta_--;
            if (state == 1) delta_++;
            break;
          case 1:
            if (state == 0) delta_--;
            if (state == 3) delta_++;
            break;
          case 2:
            if (state == 3) delta_--;
            if (state == 0) delta_++;
            break;
          case 3:
            if (state == 1) delta_--;
            if (state == 2) delta_++;
            break;
        }
        lastState_ = state;
        
        if (delta_ >= stepsPerDetent_) {
          const char *arg;
          if( (millis() - timeLastDetent_) < 750 )
            arg = fastIncArg_;
          else
            arg = incArg_;
          if (tryToSendDcsBiosMessage(msg_, arg))
          {
            delta_ -= stepsPerDetent_;
            timeLastDetent_ = millis();
          }
        }
        if (delta_ <= -stepsPerDetent_) {
          const char *arg;
          if( (millis() - timeLastDetent_) < 750 )
            arg = fastDecArg_;
          else
            arg = decArg_;
          if (tryToSendDcsBiosMessage(msg_, arg))
          {
            delta_ += stepsPerDetent_;
            timeLastDetent_ = millis();
          }
        }
      }
    public:
      McPitRotaryEncoder(const char* msg, const char* decArg, const char* incArg, const char* fastDecArg, const char* fastIncArg, char pinA, char pinB, char stepsPerDetent) {
        msg_ = msg;
        decArg_ = decArg;
        incArg_ = incArg;
        fastDecArg_ = fastDecArg;
        fastIncArg_ = fastIncArg;
        pinA_ = pinA;
        pinB_ = pinB;
        stepsPerDetent_ = stepsPerDetent;
        pinMode(pinA_, INPUT_PULLUP);
        pinMode(pinB_, INPUT_PULLUP);
        delta_ = 0;
        lastState_ = readState();
        timeLastDetent_ = millis();
      }
  };
}

DcsBios::Switch2Pos nmspAbleStow("NMSP_ABLE_STOW", 11, true);

DcsBios::McPitRotaryEncoder hsiCrs("HSI_CRS", "-1000", "+1000", "-5000", "+5000", 1, 0, 6);
DcsBios::McPitRotaryEncoder hsiHdg("HSI_HDG", "-1000", "+1000", "-5000", "+5000", 36, 37, 6);

DcsBios::McPitRotaryEncoder altSetPressure("ALT_SET_PRESSURE", "-15000", "+15000", "-80000", "+80000", 18, 19, 6);

DcsBios::Switch2Pos emerBrake("EMER_BRAKE", 25);
DcsBios::ActionButton saiCageToggle("SAI_CAGE", "TOGGLE", 24);
DcsBios::RotaryEncoder saiPitchTrim("SAI_PITCH_TRIM", "-3200", "+3200", 22, 23);

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

void setup() {
  pinMode(26, INPUT_PULLUP);
  
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();

  // Not sure what I was trying here.  Maybe just an experiment?
  Joystick.button(1, !digitalRead(26));
}
