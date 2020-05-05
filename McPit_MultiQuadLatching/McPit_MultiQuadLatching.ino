/* McPit multi-quad encoder
*/
#include <Encoder.h>
#include <Bounce.h>
//  #define DEBUG 1

struct quadInfo
{
  // Change these two numbers to the pins connected to your encoder.
  //   Best Performance: both pins have interrupt capability
  //   Good Performance: only the first pin has interrupt capability
  //   Low Performance:  neither pin has interrupt capability
  uint8_t quadPin1;
  uint8_t quadPin2;
  long lastPosition;
  int consecutiveDirection;
};

#define CONSECUTIVE_MAX_LATCH  5
#define NUM_ENCODERS 3
quadInfo aQuadInfo[] =
{
  {1,0,0,0},
//  {2,3,0,0},
  {18,19,0,0},
  {36,37,0,0}
};

Encoder *myEncoders[NUM_ENCODERS];

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
    aQuadInfo[e].lastPosition = myEncoders[e]->read();
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
    int delta = ((int)curPosition - (int)aQuadInfo[e].lastPosition);
    aQuadInfo[e].lastPosition = curPosition;
    
    if( delta > 0 )
    {
      // Potential Increment
      if( aQuadInfo[e].consecutiveDirection >= 0 )
      {
        // Consecutive Increment - All is good
        SignalButton(2*e+1, 1 /*delta*/);
        aQuadInfo[e].lastPosition = curPosition;

        #ifdef DEBUG
        quadTestValue += delta;
        #endif
        
        aQuadInfo[e].consecutiveDirection = min(aQuadInfo[e].consecutiveDirection+1,CONSECUTIVE_MAX_LATCH);
      }
      else
      {
        // Looks like an increment, but we think we are in decrementing mode, so ignore it but keep track
        aQuadInfo[e].consecutiveDirection++;
      }  
    }
    else if ( delta < 0 )
    {
      // Potential Decrement
      if( aQuadInfo[e].consecutiveDirection <= 0 )
      {
        // Consecutive Decrement - All is good
        SignalButton(2*e+2, 1 /*delta*/);
        aQuadInfo[e].lastPosition = curPosition;
        
        #ifdef DEBUG
        quadTestValue += delta;
        #endif
        
        aQuadInfo[e].consecutiveDirection = max(aQuadInfo[e].consecutiveDirection-1,-CONSECUTIVE_MAX_LATCH);
      }
      else
      {
        // Looks like an decrement, but we think we are in increment mode, so ignore it but keep track
        aQuadInfo[e].consecutiveDirection--;
      }       
    }
    
    #ifdef DEBUG
    if( delta != 0 )
    {
      Serial.print(e);
      Serial.print(":\tdlt=");
      Serial.print(delta);
      Serial.print("\tdir=");
      Serial.print(aQuadInfo[e].consecutiveDirection);
      
      Serial.print("\tValue=");
      Serial.println(quadTestValue);
    }
    #endif
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
