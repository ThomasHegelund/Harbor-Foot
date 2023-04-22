#include <RCSwitch.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

RCSwitch mySwitch = RCSwitch();

const char* ssid = "LyckBryllePetersen_5G"
const char* password = "Nykredit"

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.43.182:8000/return_date/2c6205db-06f1-4277-9432-d512583a2b2a";

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

void loop() {
  if (mySwitch.available()) {
    Serial.println(mySwitch.getReceivedValue());
    //API post
    if(WiFi.status() == WL_CONNECTED){
      WiFiclient client;
      HTTPClient http;

      String serverPath = servername + "?temperature=24.37";

      http.begin(client,serverPath.c_str());
      
      int httpResponseCode = http.GET();

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
    }
    http.end();
    else{
      Serial.println("WiFi Disconnected");
    }
    mySwitch.resetAvailable();
  }
}
