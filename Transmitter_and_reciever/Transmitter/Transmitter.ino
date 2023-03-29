/*
Following code is a transmitter setup for a Master-Slave relationship between
a transmitter and receiver

Make sure VirtualWire library is installed

Src: https://ardustore.dk/produkt/wireless-transmitter-and-receiver-433mhz-wireless-module
*/
#include <VirtualWire.h>

char *controller;
void setup() {
  //Pin to read data from
  pinMode(13,OUTPUT);
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(12);

  // speed of data transfer Kbps
  vw_setup(4000);
}

void loop(){
 controller = "1";
 vw_send((uint8_t *)controller, strlen(controller));
 vw_wait_tx(); // Wait until the whole message is gone
 
 digitalWrite(13,1);
 delay(1000);

 controller = "0" ;
 vw_send((uint8_t *)controller, strlen(controller));
 vw_wait_tx(); // Wait until the whole message is gone
 digitalWrite(13,0);
 delay(1000);
}