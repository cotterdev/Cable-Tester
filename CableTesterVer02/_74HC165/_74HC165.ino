//Author : Toby Cotter
//Description:  This program clocks in data from the 74HC165 (parallel in serial out shift register)
//
//74HC165 CLK            = Arduino Pin 4
//74HC165 PL (snapshot)  = Arduino Pin 5
//74HC165 CE Chip Enable = Arduino Pin 6
//74HC165 Data Out       = Arduino Pin 7

//Variables
int CLK_165 = 4;
int PL_165 = 5;
int CE_165 = 6;
int SerialData_165 = 7;
int runCount = 0;



void setup() {
  //Print comments
  Serial.begin(9600);
  Serial.println("Starting Test...");
  //74HC165
  pinMode(CLK_165, OUTPUT);
  pinMode(PL_165, OUTPUT);
  pinMode(CE_165, OUTPUT);
  pinMode(SerialData_165, INPUT);
  //Init
  digitalWrite(CLK_165, HIGH);
  digitalWrite(PL_165, HIGH);
  digitalWrite(CE_165, HIGH);
  delay(100);
  //Snapshot
  digitalWrite(PL_165, LOW);  
  delay(50);
  digitalWrite(PL_165, HIGH);
}

void loop() {
  if(runCount < 4){ 
    digitalWrite(CE_165, LOW);

    //Read + print serial data
    byte serialIn_from165 = shiftIn(SerialData_165, CLK_165, MSBFIRST);
    delay(50);
    Serial.println(serialIn_from165);

    runCount++;
    delay(500);
  }
}
