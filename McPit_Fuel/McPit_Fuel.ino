// For usb_desc.h:
/*
#define PRODUCT_ID		0x0488
#define PRODUCT_NAME		{'M','C','P','I','T','-','F','U','E','L'}
#define PRODUCT_NAME_LEN	10
*/

void setup() {
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  //pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  
  pinMode(42, INPUT_PULLUP);
  pinMode(43, INPUT_PULLUP);
  pinMode(44, INPUT_PULLUP);
  pinMode(45, INPUT_PULLUP);
}

void loop() {

  // read analog inputs and set X-Y position
  Joystick.X(analogRead(0));
  Joystick.Y(analogRead(1));
  Joystick.Z(analogRead(2));
  Joystick.Zrotate(analogRead(3));

  // read the digital inputs and set the buttons
  Joystick.button(1, !digitalRead(0));
  Joystick.button(2, !digitalRead(1));
  Joystick.button(3, !digitalRead(2));
  Joystick.button(4, !digitalRead(3));
  Joystick.button(5, !digitalRead(4));
  Joystick.button(6, !digitalRead(5));
//  Joystick.button(7, !digitalRead(6));
  Joystick.button(8, !digitalRead(7));
  Joystick.button(9, !digitalRead(8));
  Joystick.button(10, !digitalRead(9));
  Joystick.button(11, !digitalRead(10));
  Joystick.button(12, !digitalRead(11));
  Joystick.button(13, !digitalRead(12));
  Joystick.button(14, !digitalRead(13));
  Joystick.button(15, !digitalRead(14));
  Joystick.button(16, !digitalRead(15));
  Joystick.button(17, !digitalRead(16));
  Joystick.button(18, !digitalRead(17));
  
  Joystick.button(19, !digitalRead(18));
  Joystick.button(20, !digitalRead(19));
  Joystick.button(21, !digitalRead(20));
  Joystick.button(22, !digitalRead(21));
  Joystick.button(23, !digitalRead(22));
  Joystick.button(24, !digitalRead(23));
  Joystick.button(25, !digitalRead(24));
  Joystick.button(26, !digitalRead(25));
  Joystick.button(27, !digitalRead(26));
  Joystick.button(28, !digitalRead(27));

  Joystick.button(29, !digitalRead(42));
  Joystick.button(30, !digitalRead(43));
  Joystick.button(31, !digitalRead(44));
  Joystick.button(32, !digitalRead(45));
  
  delay(100);
}
