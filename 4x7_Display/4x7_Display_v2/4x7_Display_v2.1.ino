/* ***************************************************
 *                Global Constants                   *
 *************************************************** */
 
const int dataPin  = 12;  // 74HC595 pin 8 DS
const int latchPin = 11;  // 74HC595 pin 9 STCP
const int clockPin = 9;   // 74HC595 pin 10 SHCP
const int digit0   = 7;   // 7-Segment pin D4
const int digit1   = 6;   // 7-Segment pin D3
const int digit2   = 5;   // 7-Segment pin D2
const int digit3   = 4;   // 7-Segment pin D1 

/* ***************************************************
 *                Global Variables                   *
 *************************************************** */
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
    };
byte digitDP = 32;
byte controlDigits[] = { digit0, digit1, digit2, digit3 };  // pins to turn off & on digits
byte displayDigits[] = { 0,0,0,0,0 }; // ie: { 1, 0, 7, 13, 0} == d701 (all values from table array)
    /* Each array value holds digit values as table array index, or raw byte
     *  parameters: digit0, digit1, digit2, digit3, digitSwitch
     *  
     * digitSwitch: the four least significant bits controls data handling, 
     *              each bit controls associated digit
     *              starting with least-significant bit 0, 
     *              i.e. B1010, digit1 & digit3 are raw, 
     *                          digit0 & digit2 use table array 

/* ***************************************************
 *           Global Adjustable Variables             *
 *************************************************** */
int digitDelay = 50;                  // delay between incrementing digits (ms)
int brightness = 100;                  // valid range of 0-100, 100=brightest
unsigned int ShowSegCount = 250;      // number of RawDisplay loops before switching again 

/* ***************************************************
 *                   Void Setup                      *
 *************************************************** */
void setup() {
    //DDRD=0xff;                      // make pins 0-7 outputs
    //DDRB=0xff;                      // make pins 8-13 outputs
    //PORTD=0xf0;                     // make pins 4-7 HIGH
    
    pinMode(latchPin,OUTPUT);
    pinMode(clockPin,OUTPUT);
    pinMode(dataPin,OUTPUT);
    for (int x=0; x<4; x++){
        pinMode(controlDigits[x],OUTPUT);
        digitalWrite(controlDigits[x],LOW);  // Turns off the digit  
    }
}

/* ***************************************************
 *                   Functions                       *
 *************************************************** */    
void DisplaySegments(){
    // Display will send out all four digits
     
    
    for (int x=0; x<4; x++){
        for (int j=0; j<4; j++){
            digitalWrite(controlDigits[j],LOW);    // turn off digits
        }
        digitalWrite(latchPin,LOW);
        // table array value is sent to the shift register
        shiftOut(dataPin,clockPin,MSBFIRST,table[displayDigits[x]]);
        
        
        digitalWrite(latchPin,HIGH);
        digitalWrite(controlDigits[x],HIGH);   // turn on one digit
        delay(1);
    }
    for (int j=0; j<4; j++){
        digitalWrite(controlDigits[j],LOW);    // turn off digits
    }
}

void DateTest(){
  displayDigits[3] = table[0];
  displayDigits[2] = table[7] + B00100000;
  displayDigits[1] = table[0];
  displayDigits[0] = table[8]; 
  displayDigits[4] = B1111;
}



/* ***************************************************
 *                   Void Loop                       *
 *************************************************** */
void loop() {

    DisplaySegments();
    
    /* *************************************
     *         Control Brightness          *
     * *********************************** */
    delayMicroseconds(1638*((100-brightness)/10));         // largest value 16383
    
    /* *************************************
     *        Selects Display Type         *
     * *********************************** */

    DateTest();
}