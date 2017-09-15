//Author: Toby Cotter
//Description:  We are using the 74HC595 to enable one 4017
// decade counter to count up.  The 74hc165 will receive the
// decade count and send the read data to the arduino.  If
// rec'd data is good then the DUT is good.
//            
//74HC595 Serial Data = Arduino pin #10
//74HC595 Latch       = Arduino pin #11
//74HC595 Clock       = Arduino pin #12
//
//4017 Reset      All tied together then tied to arduino pin #2
//4017 Clock      All tied together then tied to arduino pin #3
//4017 CE chip #1   Tied to QA of 74HC595
//4017 CE chip #2   Tied to QB of 74HC595   
//
//74HC165 CLK         = Arduino #4
//74HC165 PL(snapshot)= Arduino #5
//74HC165 CE          = Arduino #6
//74HC165 DataOut     = Arduino #7

//*** Variables ***
int runCount = 0;
byte serialIn_from165[8];
//*** 74HC595 Variables ***
int serialDataPin_595 = 10;     //serial data pin of the 74hc595 connects to arduino pin 10
int latchPin_595 = 11;          //latch pin of 74hc595 connects to arduino pin 11
int clockPin_595 = 12;
//*** 4017 Variables ***
int resetPin_4017 = 2;
int clockPin_4017 = 3;
//*** 74HC165 Variables ***
int clockPin_165 = 4;
int plPin_165 = 5;
int cePin_165 = 6;
int serialDataPin_165 = 7;



void setup() {
  Serial.begin(9600);   //print comments
  SetupPinModes();
  ResetChips();
  Serial.println("Started Test");
}//End setup

void loop() {
  if(runCount > 2){
    //Cable Test #1
    SendSerialData_595(254);
    digitalWrite(resetPin_4017, LOW);    
    for(int i=0; i < 5; i++){
      Clock4017();            //Clocks the 4017 once
      TakeReceiveSnapshot();  //Toggles 74HC165 pl pin to take parallel snapshot
      ReadAll_165();
    }
    ResetAll4017();
    
    //4017 #2
    SendSerialData_595(253);
    digitalWrite(resetPin_4017, LOW);
    for(int i=0; i < 5; i++){
      //Cable Test #2
      Clock4017();
    }
    ResetAll4017();
    delay(100);
    runCount++;
  }
}//End loop

void SetupPinModes(){
  //595
  pinMode(serialDataPin_595, OUTPUT);
  pinMode(latchPin_595, OUTPUT);
  pinMode(clockPin_595, OUTPUT);
  //4017
  pinMode(resetPin_4017, OUTPUT);
  pinMode(clockPin_4017, OUTPUT);
  //74hc165
  pinMode(clockPin_165, OUTPUT);
  pinMode(plPin_165, OUTPUT);
  pinMode(cePin_165, OUTPUT);
  pinMode(serialDataPin_165, INPUT);
}//End SetupPinModes


void ResetChips(){
  //Reset 595
  SendSerialData_595(255);
  //4017
  ResetAll4017();
  //Reset 74HC165
  digitalWrite(clockPin_165, HIGH);
  digitalWrite(plPin_165, HIGH);
  digitalWrite(cePin_165, OUTPUT);
  delay(10);
}//End ResetChips


void Clock4017(){
  digitalWrite(clockPin_4017, LOW);
  delay(100);
  digitalWrite(clockPin_4017, HIGH);
  delay(100);
}//End Clock4017


void TakeReceiveSnapshot(){
  digitalWrite(plPin_165, LOW);  
  delay(50);
  digitalWrite(plPin_165, HIGH);
}//End TakeReceiveSnapshot 


void ReadAll_165(){
  int numOf_165 = 3;
  digitalWrite(cePin_165, LOW);   //Enable 74HC165
  
  for(int i=0; i<numOf_165; i++){
    serialIn_from165[i] = shiftIn(serialDataPin_165, clockPin_165, MSBFIRST);
    delay(50);
   //TODO - this is where we left off on Friday morning
  }
}//ReadAll_165


void SendSerialData_595(int val){
  digitalWrite(latchPin_595, LOW);
  shiftOut(serialDataPin_595, clockPin_595, MSBFIRST,val);
  digitalWrite(latchPin_595,HIGH);
  delay(100);
}//End SendSerialData


void ResetAll4017(){ 
  digitalWrite(resetPin_4017, HIGH);
  digitalWrite(clockPin_4017, HIGH);
  delay(50);
}//End ResetAll4017
