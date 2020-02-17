#include <Password.h>
#include <Keypad.h>
#include <elapsedMillis.h>

//KEYPAD
const byte ROWS = 4; 
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

//BUZZER
const int buzzer = 11;
int buzzOn=0;

//MOTION SENSOR
int sensor = 13; 
int val = 0;

//LED
int led = 12;  

//PASSWORD
Password password = Password("1234");
int guess=999;

void setup() {
  //KEYPAD
  keypad.addEventListener(keypadEvent);
  //BUZZER
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  //MOTION SENSOR
  pinMode(sensor, INPUT);
  //LED
  pinMode(led, OUTPUT);      // initalize LED as an output
  
  Serial.begin(9600);

}

void loop() {
  //CHECK FOR MOVEMENT
  val = digitalRead(sensor);
  if (val == HIGH) {
    //COUNTDOWN LED AND WAIT FOR PWORD
    digitalWrite(led, HIGH);
    elapsedMillis timeElapsed;
    unsigned int interval = 10000;//ms
    while(timeElapsed < interval){
      digitalWrite(led, HIGH);
      keypad.getKey();
    }
    if(guess==1){
      digitalWrite(led,LOW);
      noTone(buzzer);
      guess=999;
    }
    else{
      //START BUZZER
      tone(buzzer, 1000);
      buzzOn=1;
    }

    while(buzzOn){
      keypad.getKey();
      if(guess==1){
        digitalWrite(led,LOW);
        noTone(buzzer);
        guess=999;
        buzzOn=0;
      }
    }
  }
}



void keypadEvent(KeypadEvent eKey){
 switch (keypad.getState()){
   case PRESSED:
     Serial.print("Pressed: ");
     Serial.println(eKey);
     switch (eKey){
       case '*': guessPassword(); password.reset(); break;
       case '#': password.reset(); break;
        default:
              password.append(eKey);
    }
 }
}

void guessPassword(){
     if (password.evaluate()){
                Serial.print("ACCEPTED\n");
                guess=1;
     }else{
                Serial.print("NOT ACCEPTED\n");
                guess=0;
     }
}
