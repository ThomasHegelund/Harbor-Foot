// Define pins
const int GREEN_LED_PIN = 9;
const int RED_LED_PIN = 3;
const int PHOTORESISTOR_PIN = A0;
const int dataPin  = 12;  // 74HC595 pin 8 DS
const int latchPin = 11;  // 74HC595 pin 9 STCP
const int clockPin = 10;   // 74HC595 pin 10 SHCP

// Digit control pins
const int digit0   = 7;   // 7-Segment pin D4
const int digit1   = 6;   // 7-Segment pin D3
const int digit2   = 5;   // 7-Segment pin D2
const int digit3   = 4;   // 7-Segment pin D1
byte controlDigits[] = { digit0, digit1, digit2, digit3 };  // pins to turn off & on digits

// Display status variables
bool blink_for_reserved = false;
bool display_date = false;
byte display_byte;
byte displayDigits[] = { 0,0,0,0 }; // current date to be displayed
unsigned long previousMillis = 0; // Timer for blinking red light to idecate reservation.
int brightness = 100;
String data = "100000";

// Byte codes for shiftregister and 7-segment display
byte table[]= 
    {   0x5F,  // = 0
        0x44,  // = 1
        0x9D,  // = 2
        0xD5,  // = 3
        0xC6,  // = 4
        0xD3,  // = 5
        0xDB,  // = 6
        0x45,  // = 7
        0xDF,  // = 8
        0xC7,  // = 9
    };

// Define functions
void updateDigits(String date){
  displayDigits[3] = date[0] - '0';
  displayDigits[2] = date[1] - '0';
  displayDigits[1] = date[2] - '0';
  displayDigits[0] = date[3] - '0';
}

void checkUpdates() {
  if (Serial.available()) { // if there is data coming in from the ESP8266
    data = Serial.readStringUntil('\n'); // read the data until a newline character is received

    // Update shown availability based on 
    if (data.charAt(0) == '0') { // 0 means available
      blink_for_reserved = false;
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
    }
    else if (data.charAt(0) == '1') { // 1 means unavailable
      blink_for_reserved = false;
      digitalWrite(GREEN_LED_PIN, LOW);
      analogWrite(RED_LED_PIN, 200);

    }
    else if (data.charAt(0) == '2') { // 2 means reserved
      blink_for_reserved = true;
      digitalWrite(GREEN_LED_PIN, LOW);
      analogWrite(RED_LED_PIN, 200);    
    }


    // Update whether or not date should be displayed
    if (data.charAt(1) == '0') { // 0 means do not display date
      display_date = false;
    }
    else if (data.charAt(1) == '1') { // 1 means display date
      display_date = true;
    }

    // Update the return date
    updateDigits(data.substring(2));

  }
}


void DisplaySegments(){

    for (int x=0; x<4; x++){

      // Turn off all digits
      for (int j=0; j<4; j++){
          digitalWrite(controlDigits[j],LOW);   
      }

      digitalWrite(latchPin,LOW);
      
      // Select display byte code from table
      display_byte = table[displayDigits[x]];

      // Add . to signify dd/mm seperation
      if (x == 2) {
        display_byte += B00100000;
      }

      // Send data to shift register
      shiftOut(dataPin,clockPin,MSBFIRST,display_byte);
      
      digitalWrite(latchPin,HIGH);
      digitalWrite(controlDigits[x],HIGH);   // turn on one digit
      delay(1);                              // 1 or 2 is ok
    }

    // Turn off all digits
    for (int j=0; j<4; j++){
      digitalWrite(controlDigits[j],LOW);    
    }
}

void blink() {
  if (millis() - previousMillis >= 500) {
    previousMillis = millis();
    digitalWrite(RED_LED_PIN, !digitalRead(RED_LED_PIN));
  }

}

void setBrightness() {
  if (analogRead(PHOTORESISTOR_PIN) < 300) {
    brightness = 2;
  }
  else {
    brightness = 100;
  }

  if (data.charAt(0) == '0') {
    analogWrite(GREEN_LED_PIN, round(255 * brightness / 100));
  }
  else if (data.charAt(0) == '1') {
    analogWrite(RED_LED_PIN, round(200 * brightness / 100));
  }
}


void setup() {
  // Setup pins and serial
  Serial.begin(9600);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(PHOTORESISTOR_PIN, INPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);

  // Initialize digit control pins
  for (int x=0; x<4; x++){
      pinMode(controlDigits[x],OUTPUT);
      digitalWrite(controlDigits[x],LOW);  // Turns off the digit  
    }

  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  checkUpdates();

  if (display_date) {
    DisplaySegments();
    delayMicroseconds(1638*((100-brightness)/10));
  }

  if (blink_for_reserved) {
    blink();
  }
  else {
    setBrightness();
  }

}
