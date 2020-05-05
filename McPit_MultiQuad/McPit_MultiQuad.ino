/* McPit multi-quad encoder
*/
#include <Encoder.h>
#include <Bounce.h>
#define DEBUG 1

struct quadInfo
{
  // Change these two numbers to the pins connected to your encoder.
  //   Best Performance: both pins have interrupt capability
  //   Good Performance: only the first pin has interrupt capability
  //   Low Performance:  neither pin has interrupt capability
  uint8_t quadPin1;
  uint8_t quadPin2;
  int rotaryResolution;
};

#define NUM_ENCODERS 3
quadInfo aQuadInfo[] =
{
  {0,1,1},
//  {2,3,1},
  {18,19,1},
  {36,37,1}
};

Encoder *myEncoders[NUM_ENCODERS];
long lastPositions[NUM_ENCODERS];
#ifdef DEBUG
int quadTestValue = 0;
#endif

struct buttonInfo
{
  uint8_t pin;
  uint8_t joyButton;
  uint8_t lastState;
};

#define NUM_BUTTONS 1
buttonInfo aButtonInfo[] =
{
  {11,11, LOW}
};
Bounce *myBouncers[NUM_BUTTONS];

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.println("McPit Encoder Test:");
  #endif
  
  for( int e = 0; e < NUM_ENCODERS ; e++ )
  {
    #ifdef DEBUG
    Serial.print("Creating encoder on pins ");
    Serial.print(aQuadInfo[e].quadPin1);
    Serial.print(",");
    Serial.println(aQuadInfo[e].quadPin2);
    #endif
    myEncoders[e] = new Encoder(aQuadInfo[e].quadPin1, aQuadInfo[e].quadPin2);
    lastPositions[e] = myEncoders[e]->read();
  }
  
  for( int b = 0 ; b < NUM_BUTTONS ; b++ )
  {
    #ifdef DEBUG
    Serial.print("Creating button on pin ");
    Serial.println(aButtonInfo[b].pin);
    #endif
    pinMode(aButtonInfo[b].pin, INPUT);
    myBouncers[b] = new Bounce(aButtonInfo[b].pin, 10);
    aButtonInfo[b].lastState = -1;
  }
}

void SignalButton(int btn, int numEvents)
{
  for( int event = 0 ; event < numEvents ; event++)
  {
       Joystick.button(btn, 1);    // Press
       // delay(75);
       Joystick.button(btn, 0);    // Release
  }
}

void loop() {
  for( int e = 0; e < NUM_ENCODERS ; e++ )
  {
    long curPosition = myEncoders[e]->read();
    if( curPosition > lastPositions[e] + aQuadInfo[e].rotaryResolution - 1 )
    {
      int delta = (curPosition - lastPositions[e]) / aQuadInfo[e].rotaryResolution;
      #ifdef DEBUG
      Serial.print("Encoder ");
      Serial.print(e);
      Serial.print("Increment ");
      Serial.println(delta);
      
      quadTestValue += delta;
      Serial.print("Value=");
      Serial.println(quadTestValue);
      #endif
      SignalButton(2*e+1, delta);
      lastPositions[e] = curPosition;
    }
    else if( curPosition < lastPositions[e] - aQuadInfo[e].rotaryResolution + 1)
    {
      int delta = (lastPositions[e] - curPosition) / aQuadInfo[e].rotaryResolution;
      #ifdef DEBUG
      Serial.print("Encoder ");
      Serial.print(e);
      Serial.print("Decrement ");
      Serial.println(delta);

      quadTestValue -= delta;
      Serial.print("Value=");
      Serial.println(quadTestValue);
      #endif
      SignalButton(2*e+2, delta);
      lastPositions[e] = curPosition;
    }
  }
  
  for( int b = 0 ; b < NUM_BUTTONS ; b++ )
  {
    myBouncers[b]->update();
    uint8_t current = myBouncers[b]->read();
    if( current != aButtonInfo[b].lastState )
    {
      #ifdef DEBUG
      Serial.print("Button ");
      Serial.print(b);
      Serial.println((current==HIGH)?" ON":" OFF");
      #endif
    
      Joystick.button(aButtonInfo[b].joyButton, (current==HIGH)?1:0);
      aButtonInfo[b].lastState = current;
    }
  }

}
