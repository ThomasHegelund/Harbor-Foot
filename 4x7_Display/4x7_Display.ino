/*
koden er inspirret af https://projecthub.arduino.cc/SAnwandter1/5c461711-5c20-46f3-8d60-732ea59e6677
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---
    D
 */
 

// Pins for the the 7 segments
int pinA = 2;
int pinB = 3;
int pinC  = 4; 
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
//____________
// for activating the display segment
int  D1 = 9;
int D2 = 10;
int D3 = 11;
int D4 = 12;

// the setup routine  runs once when you press reset:
void setup() {                
  // initialize  the digital pins as outputs.
  Serial.begin(9600);
  pinMode(pinA, OUTPUT);     
  pinMode(pinB,  OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG,  OUTPUT);   
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3,  OUTPUT);  
  pinMode(D4, OUTPUT);  
}

// 7 segment numbers
void zero(){
  digitalWrite(pinA,  HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF,  HIGH);   
  digitalWrite(pinG, LOW); 
}
void one(){
    digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD,  LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);
}
void two(){
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD,  HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, HIGH);
}
void three(){
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB,  HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG,  HIGH);
}
void four(){
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB,  HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG,  HIGH);
}
void five(){
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB,  LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG,  HIGH);
}
void six(){
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB,  LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG,  HIGH);
}
void seven(){
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB,  HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG,  LOW);
}
void eight(){
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB,  HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG,  HIGH);
}
void nine(){
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB,  HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG,  HIGH);
}

//choose display
void DisOne(){
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
}
void DisTwo(){
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
}
void DisThree(){
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
}
void DisFour(){
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
}

void loop() {

  //diplay one
  DisOne();
  eight();
  delay(1);               // a "delay" so it displays on all of the displays at once
  
  //display 2
  DisTwo();
  nine();   
  delay(1);               
  
  //display 3
  DisThree();
  six();
  delay(1);               

  //display 4
  DisFour();
  seven();     
  delay(1);               

}


