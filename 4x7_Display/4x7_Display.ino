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
const int DISPLAY_ACTIVATION_PIN_1 = 9;
const int DISPLAY_ACTIVATION_PIN_2 = 10;
const int DISPLAY_ACTIVATION_PIN_3 = 11;
const int DISPLAY_ACTIVATION_PIN_4 = 12;

const int DISPLAY_ACTIVATION_PINS[] = {DISPLAY_ACTIVATION_PIN_1, DISPLAY_ACTIVATION_PIN_2, DISPLAY_ACTIVATION_PIN_3, DISPLAY_ACTIVATION_PIN_4};

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
  pinMode(DISPLAY_ACTIVATION_PIN_1, OUTPUT);  
  pinMode(DISPLAY_ACTIVATION_PIN_2, OUTPUT);  
  pinMode(DISPLAY_ACTIVATION_PIN_3,  OUTPUT);  
  pinMode(DISPLAY_ACTIVATION_PIN_4, OUTPUT);  
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

void switch_display(int display_pin)
{
  for (int display_activation_pin : DISPLAY_ACTIVATION_PINS)
  {
    digitalWrite(display_activation_pin, HIGH);
  }
  digitalWrite(display_pin, LOW);
}

void write_digit(int digit)
{
  switch (digit) {
    case 0:
      zero();
      break;
    case 1:
      one();
      break;
    case 2:
      two();
      break;
    case 3:
      three();
      break;
    case 4:
      four();
      break;
    case 5:
      five();
      break;
    case 6:
      six();
      break;
    case 7:
      seven();
      break;
    case 8:
      eight();
      break;
    case 9:
      nine();
      break;
  }
}

void write_digit_to_display(int digit, int display_activation_pin)
{
  switch_display(display_activation_pin);
  write_digit(digit); // a "delay" so it displays on all of the displays at once
  delay(1);
}

void loop() {
  write_digit_to_display(8, DISPLAY_ACTIVATION_PIN_1);
  write_digit_to_display(9, DISPLAY_ACTIVATION_PIN_2);
  write_digit_to_display(6, DISPLAY_ACTIVATION_PIN_3);
  write_digit_to_display(7, DISPLAY_ACTIVATION_PIN_4);            
}