#include <RCSwitch.h>

int MY_ID = 3;

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);

  // Optional set pulse length.
  mySwitch.setPulseLength(200);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(15);
  
}

void loop() {
  mySwitch.send(MY_ID, 12);
  Serial.println("Sent");
  delay(100);  
}
