/*
Following code implements a receiver in a Master-slave relationship between
a transmitter and receiver

Make sure VirtualWire library is installed

Src: https://ardustore.dk/produkt/wireless-transmitter-and-receiver-433mhz-wireless-module
*/
#include <VirtualWire.h>

void setup(){
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(12);
  vw_setup(4000); // Bits per sec
  pinMode(13, OUTPUT);

  vw_rx_start(); // Start the receiver PLL running
}
void loop(){
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)){ // Non-blocking
    if(buf[0]=='1'){
      digitalWrite(13,1);
    } 
    if(buf[0]=='0'){
      digitalWrite(13,0);
    }
  }
 }