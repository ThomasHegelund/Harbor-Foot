// Import library
#include "SR04.h"

// Define super ultrasonic sensor pins
#define TRIG_PIN 8
#define ECHO_PIN 10

// Define ultrasonic sensor
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distance;
unsigned int light_intensity;

// Define variable for tracking spot occupation
bool SPOT_OCCUPIED = false;
bool NIGHT_MODE = false;
const int measure_interval = 5000;
unsigned long last_measure_time = millis() - measure_interval;

// Define pins
const int GREEN_LED_PIN = 2;
const int RED_LED_PIN = 3;
const int PHOTORESISTOR_PIN = A0;
const int dataPin  = 12;  // 74HC595 pin 8 DS
const int latchPin = 11;  // 74HC595 pin 9 STCP
const int clockPin = 9;   // 74HC595 pin 10 SHCP
const int digit0   = 7;   // 7-Segment pin D4
const int digit1   = 6;   // 7-Segment pin D3
const int digit2   = 5;   // 7-Segment pin D2
const int digit3   = 4;   // 7-Segment pin D1 

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
        0x20,  // = decimal point
    };  //Hex shown

int brightness = 100; 

byte controlDigits[] = { digit0, digit1, digit2, digit3 };  // pins to turn off & on digits
byte displayDigits[] = { 0,0,0,0,0 }; // ie: { 1, 0, 7, 13, 0} == d701 (all values from table array)

bool is_dark(int light_intensity, int threshold = 200)
{
  return light_intensity < threshold;
}

bool distance_in_range(int distance, int min_distance = 5, int max_distance = 150)
{
  return distance > min_distance && distance < max_distance;
}

bool is_time_to_make_measurement()
{
  return (millis() - last_measure_time) > measure_interval;
}

void update_night_mode(int light_intensity)
{
  if (is_dark(light_intensity, 200)) {
      NIGHT_MODE = true;
      return;
    }
    NIGHT_MODE = false; 
}

void update_leds()
{
  if (SPOT_OCCUPIED == true) {
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);
      return;
    }
    
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
}

void update_occupation_status(int distance, int min_distance = 5, int max_distance = 150)
{
  if (distance_in_range(distance, min_distance, max_distance))
  {
    SPOT_OCCUPIED = true;
    return;
  }
  SPOT_OCCUPIED = false;
}

void DisplaySegments(){
    /* Display will send out all four digits
     * one at a time.  Elegoo kit only has 1 74HC595, so
     * the Arduino will control the digits
     *   displayDigits[4] = the right nibble controls output type
     *                      1 = raw, 0 = table array
     *                  upper (left) nibble ignored
     *                  starting with 0, the least-significant (rightmost) bit
     */
    
    for (int x=0; x<4; x++){
        for (int j=0; j<4; j++){
            digitalWrite(controlDigits[j],LOW);    // turn off digits
        }
        digitalWrite(latchPin,LOW);
        // table array value is sent to the shift register
        if (x == 2) {
          shiftOut(dataPin,clockPin,MSBFIRST,table[displayDigits[x]] + B00100000);
        }
        else {
          shiftOut(dataPin,clockPin,MSBFIRST,table[displayDigits[x]]);
        }
        
        
        
        digitalWrite(latchPin,HIGH);
        digitalWrite(controlDigits[x],HIGH);   // turn on one digit
        delay(1);                              // 1 or 2 is ok
    }
    for (int j=0; j<4; j++){
        digitalWrite(controlDigits[j],LOW);    // turn off digits
    }
}

void DateTest(){
  displayDigits[3] = 0;
  displayDigits[2] = 7;
  displayDigits[1] = 0;
  displayDigits[0] = 8;
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
    for (int x=0; x<4; x++){
        pinMode(controlDigits[x],OUTPUT);
        digitalWrite(controlDigits[x],LOW);  // Turns off the digit  
    }
   delay(1000);
}

void loop() {

  // Check if it is time to make a measurement
  if (is_time_to_make_measurement()) {

    // Measure the distance with ultrasonic sensor
    distance = sr04.Distance();

    // If an object is detected between 5 and 150 cm away, the spot is occupied
    update_occupation_status(distance, 5, 150);

    // If the spot is occupied, turn on the green LED, otherwise turn on red LED
    update_leds();

    // Measue light intensity
    light_intensity = analogRead(PHOTORESISTOR_PIN);

    // Switch to nightmode if it is dark
    update_night_mode(light_intensity);

    // Update time of the latest measure
    last_measure_time = millis();

    // Print all measurements
    Serial.print(distance);
    Serial.println("cm");
    Serial.print("Light intensity: ");
    Serial.println(light_intensity);
    }

    DisplaySegments();                                      // Caution: Avoid extra delays
    
    /* *************************************
     *         Control Brightness          *
     * *********************************** */
    delayMicroseconds(1638*((100-brightness)/10));         // largest value 16383
    
    /* *************************************
     *        Selects Display Type         *
     * *********************************** */

    DateTest();
}


