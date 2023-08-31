/*
  This game is a bomb game, A player should chose the correct wires to remove
  Because you might cut off the wrong wire then pop the ballon!
  The player recieve a clue based on the lights on in the bomb leds
  It has also a timer which is a buzzer that the increase the sound if its comming to end!
  If the player cut off the correct wires, Players Wins
*/

/*
  Possible Combinations ;
    (5, 6, 7), (5, 6, 8), (5, 6, 9), (5, 6, 10), (5, 7, 8)
    (5, 7, 9), (5, 7, 10), (5, 8, 9), (5, 8, 10), (5, 9, 10)
    (6, 7, 8), (6, 7, 9), (6, 7, 10), (6, 8, 9), (6, 8, 10)
    (6, 9, 10), (7, 8, 9), (7, 8, 10), (7, 9, 10), (8, 9, 10)
*/

#include <Servo.h>

void delayServoMovement(Servo servo, int startPos , int endPos , int delayNum = 3){
  for ( startPos ; startPos <= endPos ; startPos++ ){
    servo.write(startPos);
    delay(delayNum);
  }
}

void buzzerSound( int pin , int volume){
  tone(pin , volume);
}

bool isCutOff(int pin){
  if ( digitalRead(pin) == HIGH){
    return true;
  }
  return false;
}

class CustomSensor {
  int echo = 0;
  int trig = 0;
  int range = 0;
  bool detected = false;

  CustomSensor(int pin1, int pin2, int val) {
    echo = pin1;
    trig = pin2;
    range = val;
  }

  void detect() {
    // check if the an object enter the range
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    int duration = pulseIn(echo, HIGH);
    int distance = duration * 0.034 / 2;
    if (distance <= range) {
      detected = true;
      Serial.println("Game Starting ! " + (String)distance + " cm");
    }
  }

};

class CustomTimer{
  int timer_pin = 0;
  int timer_time = 0;
  int buzzer_sound = 0 , incresed_volume = 5;

  CustomTimer(int pin , int timer){
    timer_pin = pin;
    timer_time = timer;
  }

  void decreaseTimer(){
    buzzer_sound = buzzer_sound + incresed_volume;
    timer_time = timer_time - 1;
  }

  void sound(){
    ::buzzerSound(timer_pin, buzzer_sound);
  }


};

class GameWires{

  int wires[6];
  int correct_wires[3], wrong_wires[3];

  GameWires(int pins[6]){
    for (int i = 0 ; i < 6 ; i++ ){
      wires[i] = pins[i];
    }
  }

  void setTheCorrectWires( int pins[3] ){
    int numberOfSet = 0;
    for (int i = 0; i < 6 ; i++ ){
      bool setted = false;
      for ( int j = 0 ; j < 3 ; j++ ){
        if (wires[i] == pins[j]){
          correct_wires[numberOfSet] = wires[j];
          setted = true;
        }
      }
      if (!setted){
        
      }

    }
  }

};

int ECHO = 2;
int TRIGGER = 3;
int POPPER_PIN = 4;
int LEDS[] = {5,6,7,8,9,10};
int BUZZER_PIN = 11;

int WIN_LEDS[] = {A0 , A1, A2};
int LOSE_LEDS[] = {A3 , A4, A5};

Servo popper;
int POPPER_START_POS = 0 , POPPER_END_POS = 90;


void setup() {
  // A0 is not input so i can
  Serial.begin(9600);
  randomSeed(analogRead(A0));

  int range = random(0, 1000);
  for (int i = 0; i < range; i++) {
    randomSeed(analogRead(A0));
  }

  for ( int pin : ::WIN_LEDS){
    pinMode(pin , OUTPUT);
  }
  for ( int pin : ::LOSE_LEDS){
    pinMode(pin , OUTPUT);
  }
  for ( int pin : ::LEDS){
    pinMode(pin , OUTPUT);
  }

  pinMode(::BUZZER_PIN , OUTPUT);
  pinMode(::TRIGGER, OUTPUT);
  pinMode(::ECHO , INPUT);

  ::popper.attach(POPPER_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  ::popper.write(POPPER_START_POS); // Put the servo back to its position
  

}




