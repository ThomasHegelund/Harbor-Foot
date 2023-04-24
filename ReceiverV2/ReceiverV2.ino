#include <RCSwitch.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

RCSwitch mySwitch = RCSwitch();

//The boat corresponding to the harbort slot
String MY_BOAT = "2c6205db-06f1-4277-9432-d512583a2b2a";
const char* ssid = "Gringo_Factor_Maximized";
const char* password = "soy gringo";

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.43.182:8000/update_berth_occupation_status";
//1 er LED state
//0 - grøn
//1 - rød

//2 - blink

//2 er display state
//0 - display er tænd
//1 - display er slukket

//Sidste 4 er dato

//Tjek format for code
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
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

String home_boat_in_harbor;

void loop() {
  String code = "{\"berth_uuid\": \"2c6205db-06f1-4277-9432-d512583a2b2a\",\"new_occupation_status\": true}";
  
  //sendCode(code);
  
  if (mySwitch.available()){
    int received = mySwitch.getReceivedValue();
    Serial.println(received);

    if (received == 3){
      home_boat_in_harbor = "1";
    }
    else{
      home_boat_in_harbor = "0";
    }
    mySwitch.resetAvailable();
  }
  
  else{
    home_boat_in_harbor = "0";
  }

  //String berth_occupied = "false";
  //Tjek json struktur
  //String code = "{\"berth_uuid\": \"2c6205db-06f1-4277-9432-d512583a2b2a\",\"new_occupation_status\": " 
  //if (strcmp(berth_occupied, "1")){
  //  code = code + "true}";
  //}

  //else{
  //  code = code + "false}";
  //}

  //Read distance sensor

  //Code is structered 
  
  //First bit is boat state
  //

  //sendCode(code)
  delay(1000);
}
