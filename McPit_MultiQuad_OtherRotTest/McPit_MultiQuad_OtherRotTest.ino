/* McPit multi-quad encoder
*/
#include <Bounce.h>
#define DEBUG 1

class RotaryEncoder{
   private:
		char _pinA;
		char _pinB;
		char _lastState;
		signed char _delta;
		
		char _joyBtnInc;
		char _joyBtnDec;
 	
		char readState()
		{
			return (digitalRead(_pinA) << 1) | digitalRead(_pinB);
		}
   
 public:
   RotaryEncoder(char pinA, char pinB, char joyBtnInc, char joyBtnDec)
   {
     _pinA = pinA;
     _pinB = pinB;
	 _joyBtnInc = joyBtnInc;
	 _joyBtnDec = joyBtnDec;
	 
     pinMode(_pinA, INPUT_PULLUP);
     pinMode(_pinB, INPUT_PULLUP);
     _lastState = readState();
	 
	 #ifdef DEBUG
    Serial.print("Created encoder on pins ");
    Serial.print(_pinA);
    Serial.print(",");
    Serial.print(_pinB);
    Serial.print(". Inc Joy Btn: ");
    Serial.print(_joyBtnInc);
	Serial.print(". Dec Joy Btn: ");
    Serial.print(_joyBtnDec);
	#endif
   }
   
   void pollInput() {
		char state = readState();
		switch(_lastState) {
			case 0:
				if (state == 2) _delta--;
				if (state == 1) _delta++;
				break;
			case 1:
				if (state == 0) _delta--;
				if (state == 3) _delta++;
				break;
			case 2:
				if (state == 3) _delta--;
				if (state == 0) _delta++;
				break;
			case 3:
				if (state == 1) _delta--;
				if (state == 2) _delta++;
				break;
		}
		_lastState = state;
		
		if (_delta == 4) 
		{
			#ifdef DEBUG
			Serial.println("Increment");
			#endif

			Joystick.button(_joyBtnInc, 1);    // Press
			Joystick.button(_joyBtnInc, 0);    // Release
			_delta = 0;
		}
		else if (_delta == -4) 
		{
			#ifdef DEBUG
			Serial.println("Decrement");
			#endif
			
			Joystick.button(_joyBtnInc, 1);    // Press
			Joystick.button(_joyBtnDec, 0);    // Release
			_delta = 0;
		}
	}
};

RotaryEncoder reRot1(1,0,1,2);
RotaryEncoder reRot2(18,19,3,4);
RotaryEncoder reRot3(36,37,5,6);

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.println("McPit Encoder Test:");
  #endif
}

void loop()
{
        reRot1.pollInput();
	reRot2.pollInput();
	reRot3.pollInput();
}

