// Import library
#include "SR04.h"

// Define super ultrasonic sensor pins
#define TRIG_PIN 12
#define ECHO_PIN 11

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


void setup() {
  // Setup pins and serial
   Serial.begin(9600);
   pinMode(GREEN_LED_PIN, OUTPUT);
   pinMode(RED_LED_PIN, OUTPUT);
   pinMode(PHOTORESISTOR_PIN, INPUT);
   delay(1000);
}

void loop() {

  // Check if it is time to make a measurement
  if ((millis() - last_measure_time) > measure_interval) {

    // Measure the distance with ultrasonic sensor
    distance=sr04.Distance();

    // If an object is detected between 5 and 150 cm away, the spot is occupied
    if (distance < 150 && distance > 5) {
      SPOT_OCCUPIED = true;
    }
    else {
      SPOT_OCCUPIED = false;
    }

    // If the spot is occupied, turn on the green LED, otherwise turn on red LED
    if (SPOT_OCCUPIED) {
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);
    }
    else {
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
    }

    // Measue light intensity
    light_intensity = analogRead(PHOTORESISTOR_PIN);

    // Switch to nightmode if it is dark
    if (light_intensity < 200) {
      NIGHT_MODE = true;
    }
    else {
      NIGHT_MODE = false;
    }

    // Update time of the latest measure
    last_measure_time = millis();

    // Print all measurements
    Serial.print(distance);
    Serial.println("cm");
    Serial.print("Light intensity: ")
    Serial.println(light_intensity);
    }
}
