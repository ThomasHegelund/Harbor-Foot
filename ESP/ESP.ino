#include <RCSwitch.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "SR04.h"

// Define super ultrasonic sensor pins
#define TRIG_PIN 16
#define ECHO_PIN 5
// Define ultrasonic sensor
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

//RCSwitch class
RCSwitch mySwitch = RCSwitch();

//The UID for the boat who owns the berth space
String MY_BERTH = "53fe7f71-2f4b-4b6d-970b-465944c516a0";

//WiFi info
const char* ssid = "LyckBryllePetersen";
const char* password = "Nykredit";

//IP of server host
String serverName = "http://192.168.8.112:8000/update_berth_status";
String main_boat_in_harbor;
String code;

String SPOT_OCCUPIED;
//1 er LED state
//0 - grøn
//1 - rød

//2 - blink

//2 er display state
//0 - display er tænd
//1 - display er slukket

//Sidste 4 er dato

const int measure_interval = 5000;
unsigned long last_measure_time = millis() - measure_interval;

bool distance_in_range(int distance, int min_distance = 5, int max_distance = 150){
  return distance > min_distance && distance < max_distance;
}

String occupation_status(int distance, int min_distance = 5, int max_distance = 150){
  
  if (distance_in_range(distance, min_distance, max_distance)){
    SPOT_OCCUPIED = "true";
  }
  else{
    SPOT_OCCUPIED = "false";
  }
  return SPOT_OCCUPIED; 
}

//Function used to send a code to the API
void sendCode(String package){
  //API post
  if(WiFi.status() == WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    //Er code en korrekt string
    String serverPath = serverName;
    http.begin(client,serverPath.c_str());
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(package);

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else{
    Serial.println("WiFi Disconnected");
  }
}

bool is_time_to_make_measurement(){
  return (millis() - last_measure_time) > measure_interval;
}

void setup() {
  Serial.begin(9600);

  //WiFi setup
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  //Wireless transmission setup
  //Data is read on D3
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  //Serial.println(mySwitch.getReceivedValue());
  if (is_time_to_make_measurement()){
    if (mySwitch.available()){
      int received = mySwitch.getReceivedValue();
      Serial.println("Received from RF");
      Serial.println(received);

      if (received == 3){
        main_boat_in_harbor = "true";
      }
      else{
        main_boat_in_harbor = "false";
      }
      mySwitch.resetAvailable();
    }
    else{
      main_boat_in_harbor = "false";
    }
  
    int distance = sr04.Distance();
    String occupied = occupation_status(distance, 5, 150);

    Serial.println("Distance");
    Serial.println(distance);

    Serial.println("Occupied?");
    Serial.println(occupied);

    Serial.println("Home boat?");
    Serial.println(main_boat_in_harbor);

    last_measure_time = millis();
    code = "{\"berth_uuid\": \"" + MY_BERTH + "\",\"occupied\": " + occupied + ", \"main_boat_in_harbor\": " + main_boat_in_harbor +"}";
    //code = "{\"berth_uuid\": \"53fe7f71-2f4b-4b6d-970b-465944c516a0\",\"occupied\": true, \"main_boat_in_harbor\": true}";
    //code = "{\"berth_uuid\": \"53fe7f71-2f4b-4b6d-970b-465944c516a0\",\"occupied\": true, \"main_boat_in_harbor\": true}";
    //code = "{\"berth_uuid\": \"" + MY_BERTH + "\",\"occupied\": " + occupied + "\",\"main_boat_in_harbor\": " + main_boat_in_harbor + "}";
    sendCode(code);
  }
}
