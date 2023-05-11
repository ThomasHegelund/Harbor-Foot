/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

int MY_ID = 3;

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
  
}

void loop() {
  // 24 bits is send
  mySwitch.send(MY_ID, 24);
  Serial.println("Ping")
  delay(1000);  

}
