#include <Servo.h>

const int reset = 2;
const int one = 4;
const int two = 5;
const int three = 6;
const int led = 7;
Servo myServo;
volatile int pressedflag = 0;
int i=0;
int numbers[5]={};
int numberskey[5]={1,1,2,3,3};
//unsigned long times[5]={};

//Percentage Thresholds
float perckey[8] = { 
                  .05,.15,       //between 1-2
                  .35,.45,       //between 2-3
                  .35,.45,       //between 3-4
                  .05,.15        //between 4-5
                };

//To store times
float perc[4] = {};

/*
unsigned long timesminkey[5]={53,240,63,245,201};//{63,250,73,255,211};
unsigned long timesmaxkey[5]={113,360,123,355,291};//{103,350,113,345,281};
*/


unsigned long buttontime = 0;
unsigned long lastbuttontime = 0;
int flag1=0;
int flag2=0; 
int flag3=0;
int flagreset=0;
int wrongflag=0;
int wrongcount=0;
int x=0;
long temp=0;

//To speed up normalization  Might be faster in crucial area to leave this out.  ie. Where do we need to be fast? button recording or checking?
float runningtotal=0;

void setup() {
  myServo.attach(3);
  myServo.write(0);
  delay(1000);
  pinMode(one, INPUT);
  pinMode(two, INPUT);
  pinMode(three, INPUT);
  pinMode(led, OUTPUT);
  pinMode(reset, INPUT);
  digitalWrite(led, LOW);
  //attachInterrupt(0, changeflag, RISING);
  Serial.begin(9600);
  for(i=0;i<5;i++)
    numbers[i]=0;
  for(i=0;i<4;i++)
    perc[i]=0;
  i=-1;
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(1){
    if(validpress()){
      if(flag1){
        numbers[i]=1;
        //flag1=0;
        break;
      }
      if(flag2){
        numbers[i]=2;
        //flag2=0;
        break;
      }
      if(flag3){
        numbers[i]=3;
        //flag3=0;
        break;
      } 
      if(flagreset){
        digitalWrite(led, LOW);
        flagreset=0;
        for(i=0;i<5;i++){
          numbers[i]=0;
        }
        for(i=0;i<4;i++){
          perc[i]=0;
        }
        myServo.write(0);
        i=-1;  
        runningtotal=0;
      }        
    } 
  }

  Serial.print(numbers[0]);
  Serial.print(numbers[1]);
  Serial.print(numbers[2]);
  Serial.print(numbers[3]);
  Serial.print(numbers[4]);
  //Serial.print(numbers[5]);
  Serial.println();
  Serial.print(numberskey[0]);
  Serial.print(numberskey[1]);
  Serial.print(numberskey[2]);
  Serial.print(numberskey[3]);
  Serial.print(numberskey[4]);
  Serial.println();
  //Serial.println(numberskey[5]);
  if(i==4){
    for(x=0;x<5;x++){
      if(numbers[x]!=numberskey[x]){
        Serial.println("wrong code");
        wrongflag=1;
      }
    }
    Serial.println();
    //Normalize here!
    for(x=0; x<4; x++){
       runningtotal=runningtotal+perc[x];
    }
    Serial.println(runningtotal);
    for(x=0;x<4;x++){
        Serial.print(perc[x]);
        Serial.print("  /   ");
        Serial.print(perc[x]/runningtotal);
        Serial.print(" for x = ");
        Serial.println(x);
      if( ((perc[x]/runningtotal) < perckey[2*x]) || (perc[x]/runningtotal > perckey[2*x+1])){
        Serial.println("wrong time");
        wrongcount++;
      }
    }

    if(wrongcount>1){
      wrongflag=1;
    }
    Serial.print("wrongflag = ");
    Serial.println(wrongflag);

    if(wrongflag==0){
      digitalWrite(led, HIGH);
      myServo.write(50);
    }

    else{
      digitalWrite(led, HIGH);
      delay(100);
      digitalWrite(led, LOW);
      delay(100);
      digitalWrite(led, HIGH);
      delay(100);
      digitalWrite(led, LOW);
      for(i=0;i<5;i++){
        numbers[i]=0;
      }
      for(i=0;i<4;i++){
        perc[i]=0;
      }
      myServo.write(0);
      i=-1;
      wrongflag=0;
      wrongcount=0;
      runningtotal=0;
    }
  }

//  Serial.print(numbers[6]);
//  Serial.print(numbers[7]);
//  Serial.print(numbers[8]);
//  Serial.print(numbers[9]);

  Serial.println();
  delay(10);
}

boolean validpress(){
  
  if(digitalRead(one) && flag1==0){
    buttontime = millis();
    flag1=1;
  }
  else if(digitalRead(two) && flag2==0){
    buttontime = millis();
    flag2=1;
  }
  else if(digitalRead(three)&& flag3==0){
    buttontime = millis();
    flag3=1;
  }
  else if(digitalRead(reset)){
    buttontime = millis();
    flagreset=1;
  }
  
  if(!digitalRead(one)){
    flag1=0; 
  }
  if(!digitalRead(two)){
    flag2=0; 
  }
  if(!digitalRead(three)){
    flag3=0; 
  }
  
  if(buttontime-lastbuttontime>0){
    i++;
    Serial.print("press ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(buttontime-lastbuttontime);
    if(i>0 && i<5){
      perc[i-1] = buttontime-lastbuttontime;
    }
    lastbuttontime = buttontime;
    return true;
  }
  else{
    return false;
  }
}
//void changeflag(){
//  pressedflag=1;
//}
