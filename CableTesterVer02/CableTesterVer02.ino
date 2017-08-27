//Author: Toby Cotter
//Description:  Getting 74HC595 to operate
//
//74HC595 Serial Data = Arduino pin #10
//74HC595 Latch       = Arduino pin #11
//74HC595 Clock       = Arduino pin #12
//
//4017 Reset      All tied together then tied to arduino pin #2
//4017 Clock      All tied together then tied to arduino pin #3
//4017 CE chip #1   Tied to QA of 74HC595
//4017 CE chip #2   Tied to QB of 74HC595   


//*** 74HC595 Variables ***
int serialDataPin_595 = 10;
int latchPin_595 = 11;
int clockPin_595 = 12;
//*** 4017 Variables ***
int resetPin_4017 = 2;
int clockPin_4017 = 3;

int resetValue = 255;
int serialData = 255;
int firstTime = 1;

void setup() {
  //print comments
  Serial.begin(9600);
  //595
  pinMode(serialDataPin_595, OUTPUT);
  pinMode(latchPin_595, OUTPUT);
  pinMode(clockPin_595, OUTPUT);
  //4017
  pinMode(resetPin_4017, OUTPUT);
  pinMode(clockPin_4017, OUTPUT);
  //Reset 595
  SendSerialData_595(255);
  //4017
  ResetAll4017();
}//End setup

void loop() {
    //Serial.print("Sending: ");
    //Serial.println(resetValue);
    //4017 #1
    SendSerialData_595(254);
    digitalWrite(resetPin_4017, LOW);
    for(int i=0; i < 5; i++){
      Clock4017();
    }
    ResetAll4017();
    //4017 #2
    SendSerialData_595(253);
    digitalWrite(resetPin_4017, LOW);
    for(int i=0; i < 5; i++){
      Clock4017();
    }
    ResetAll4017();
    delay(100);
}//End loop

void Clock4017(){
  digitalWrite(clockPin_4017, LOW);
  delay(100);
  digitalWrite(clockPin_4017, HIGH);
  delay(100);
}//End Clock4017

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
