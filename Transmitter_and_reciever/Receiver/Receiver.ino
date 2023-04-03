/*
  Example for receiving
  
  https://github.com/sui77/rc-switch/
  
  If you want to visualize a telegram copy the raw data and 
  paste it into http://test.sui.li/oszi/
*/

#include <RCSwitch.h>
#include <stdio.h>

int MY_SPOT = 2;
int delay_time = 100;

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(74880);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  
}

int checkForBoat(){
  int reading = mySwitch.getReceivedValue();
  int boat_IDs [] = {1,2,3,4,5,69};
  for (int i = 0; i < sizeof(boat_IDs); i++){
    if(boat_IDs[i] == reading){
      return boat_IDs[i];
    }
  }
  return 0;  
}

void loop() {
  int boat_ID = checkForBoat();
  
  if (boat_ID != 0 ){
    //Add API-connection here
    Serial.println(boat_ID);
    delay_time = 2000;
  }
  else{
    Serial.println("No boat found");
    delay_time = 100;
  }

  delay(delay_time);
}
