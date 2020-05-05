/* UART Example, any character received on either the real
   serial port, or USB serial (or emulated serial to the
   Arduino Serial Monitor when using non-serial USB types)
   is printed as a message to both ports.

   This example code is in the public domain.
*/

const int ledPin = 11;

void setup() {
	Serial.begin(250000);
      pinMode(ledPin, OUTPUT);
}

void loop() {
        int incomingByte;
        
	if (Serial.available() > 0) {
		incomingByte = Serial.read();
		  digitalWrite(ledPin, HIGH);   // set the LED on
                  delay(250);
                  		  digitalWrite(ledPin, LOW);   // set the LED on
	}
}

