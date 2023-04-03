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
}


