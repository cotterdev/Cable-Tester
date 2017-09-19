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
//4017 Reset        All tied together then tied to arduino pin #2
//4017 Clock        All tied together then tied to arduino pin #3
//4017 CE chip #1   Tied to QA of 74HC595
//4017 CE chip #2   Tied to QB of 74HC595   
//
//74HC165 CLK         = Arduino #4
//74HC165 PL(snapshot)= Arduino #5
//74HC165 CE          = Arduino #6
//74HC165 DataOut     = Arduino #7

//*** Variables ***
int runCount = 0;
int numOf_165 = 3;
int id_4017[] = {254, 253, 251, 247, 239, 223, 191, 127};
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
//troubleshoot
int trblDelayPin = 8;


void setup() {
  Serial.begin(9600);   //print comments
  SetupPinModes();
  ResetChips();
  Serial.println("Started Test");
}//End setup

void loop() {
  if(runCount < 1){
    Serial.println("Just entered loop");
    //Begin Cable Test
    for(int num4017=0; num4017 < 2; num4017++){   //TODO The 2 needs to be changed to 8    
      SendSerialData_595(id_4017[num4017]);       //Enables 1 of 8 4017 via the 74HC595
      digitalWrite(resetPin_4017, LOW);  
        
      //We do this for each wire to clock on 4017
      for(int i=0; i < 8; i++){
        Clock4017();                              //Clocks the 4017 once
        WaitForButtonPress();
        TakeReceiveSnapshot();                    //Toggles 74HC165 pl pin to take parallel snapshot
        ReadAll_165();
        DisplayData_165();
        delay(500);
      }//End for
      ResetAll4017();
      runCount++;
    }//End for
  }//End if
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
  //Troubleshoot
  pinMode(trblDelayPin, INPUT);
}//End SetupPinModes


void ResetChips(){
  //Reset 595
  SendSerialData_595(255);
  //4017
  ResetAll4017();
  //Reset 74HC165
  digitalWrite(clockPin_165, HIGH); 
  digitalWrite(plPin_165, HIGH);
  digitalWrite(cePin_165, HIGH);
  delay(20);
}//End ResetChips


void Clock4017(){
  digitalWrite(clockPin_4017, LOW);
  delay(100);
  digitalWrite(clockPin_4017, HIGH);
  delay(100);
}//End Clock4017


void TakeReceiveSnapshot(){
  digitalWrite(plPin_165, LOW);  
  delay(20);
  digitalWrite(plPin_165, HIGH);
}//End TakeReceiveSnapshot 


void ReadAll_165(){  
  digitalWrite(cePin_165, LOW);       //Enable 74HC165
  
  for(int i=0; i<numOf_165; i++){
    serialIn_from165[i] = shiftIn(serialDataPin_165, clockPin_165, MSBFIRST);   //This reads one 74HC165
    delay(10);
  }
  digitalWrite(clockPin_165, HIGH);  //Without this line we were one bit shift off when reading 74HC165 
}//End ReadAll_165


void DisplayData_165(){ 
  Serial.println("Here comes the 74HC165 data...");
  for(int i = 0; i<numOf_165; i++){
    Serial.println(serialIn_from165[i]);
  }
}//End DisplayData_165


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


void WaitForButtonPress(){
  int buttonPress = 1;
  while(buttonPress == 1){
    //Serial.println("We entered button press while loop");
    buttonPress = digitalRead(trblDelayPin);
    delay(250);
  }
}//End WaitForButtonPress
