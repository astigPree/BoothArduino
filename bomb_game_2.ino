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

int wireSelections[20][3] = {
  {5, 6, 7}, {5, 6, 8}, {5, 6, 9}, {5, 6, 10}, {5, 7, 8},
  {5, 7, 9}, {5, 7, 10}, {5, 8, 9}, {5, 8, 10}, {5, 9, 10},
  {6, 7, 8}, {6, 7, 9}, {6, 7, 10}, {6, 8, 9}, {6, 8, 10},
  {6, 9, 10}, {7, 8, 9}, {7, 8, 10}, {7, 9, 10}, {8, 9, 10}
};

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

public:
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

public:
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

public:
  int wires[6];
  int correct_wires[3], wrong_wires[3];

  GameWires(int pins[6]){
    for (int i = 0 ; i < 6 ; i++ ){
      wires[i] = pins[i];
    }
  }

  void setTheCorrectWires(){
    int selected = random(0, 19);
    for ( size_t i = 0 ; i < 3 ; i++){
      correct_wires[i] = ::wireSelections[selected][i];
    }

    size_t wireUsed = 0; // check how many used wires to handle distributing wrong wires
    for (size_t i = 0; i < 6 ; i++ ){
      int correct = false; // check if in the correct_wires
      for (size_t j = 0; j < 3 ; j++ ){
        if (correct_wires[j] == wires[i]){
          correct = true;
        }
      }
      if (!correct && wireUsed < 3){
        wrong_wires[wireUsed] = wires[i];
        wireUsed = wireUsed + 1;
      }
    }

  }

};

int ECHO = 2; // Use for sensor
int TRIGGER = 3; // Use for sensor
int POPPER_PIN = 4; // Use for servo
int WIRES[] = {5,6,7,8,9,10}; // Wires available
int BUZZER_PIN = 11; // Buzzer used for timer ( lower to higher sound)

int WIN_LEDS[] = {A0 , A1, A2}; // Leds for the winning strikes 
int LOSE_LEDS[] = {A3 , A4, A5}; // Leds for the lossing strikes

int TIME = 6000; // The minimum time for playing

CustomSensor sensor(::ECHO, ::TRIGGER, 10); // sensor object
GameWires gWires(::WIRES); // wires game logic object
CustomTimer cTimer(::BUZZER_PIN, 20); // buzzer timer object

Servo popper;
int POPPER_START_POS = 0 , POPPER_END_POS = 90; // The angle of servo when popping a ballon


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
  for ( int pin : ::WIRES){
    pinMode(pin , INPUT);
  }

  pinMode(::BUZZER_PIN , OUTPUT);
  pinMode(::TRIGGER, OUTPUT);
  pinMode(::ECHO , INPUT);

  ::popper.attach(POPPER_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  ::popper.write(POPPER_START_POS); // Put the servo back to its position

  ::sensor.detect(); // Check if there is a player hand , then start the timer

  if (::sensor.detected){
    
  }
  

}




