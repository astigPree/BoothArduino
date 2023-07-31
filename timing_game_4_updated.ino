/* 
  This program is used to play the X lighted game;
  How it works?
    The light is switching in different leds in X's lines when the button
    clicked the light stop. If the light stop in the center of X's leds then 
    the player win the game.
*/


bool isClicked(int pin){
  if ( digitalRead(pin) == HIGH) return true;
  return false;
}

void sound1(int pinSound , int pinButton){

}

struct Leds{
  int leds[9];
  int pos = 1;  // it represent zero but i use 1 to represent the counting in leds[9]
  int center = 5; // it represent 4 but i use 1 to represent the counting in leds[9]

  void offAllLights(){
    for(int led : leds){
      digitalWrite(led , LOW);
    }
    pos = 1;
  }

  bool isStoppedInCenter(){
    if (pos == center) return true;
    return false;
  }

  void switchToForwardLight(){
    digitalWrite(leds[pos - 1] , LOW); // Off the past Light
    Serial.println("Off : " + (String)leds[pos - 1]);
    pos++; // Move the position of the Light
    if (pos > 9) pos = 1; // If the position is greater than the number of leds , go back to start
    digitalWrite(leds[pos - 1], HIGH); // On the new positioned light
    Serial.println("On : " + (String)leds[pos - 1]);
  }

  void switchToBackwardLight(){
    digitalWrite( leds[pos - 1] , LOW );// Off the past Light
    Serial.println("Off : " + (String)leds[pos - 1]);
    pos--; // Move the position of the Light
    if (pos < 1 ) pos = 9; // If the position is less than the number of leds , go back to highest position
    digitalWrite(leds[pos - 1], HIGH); // On the new positioned light
    Serial.println("On : " + (String)leds[pos - 1]);
  }

};



int led_pins[9] = { 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10};
int button_pin = 11;
int buzzer_pin = 12;
Leds game_leds;


void GAME(){
  /* 
    this is where the game happen
  */

  int DELAY = 500;

  while (true){
    Serial.println("[?] The game starting ...");
    int backOrForward = random(0,2);
    Serial.println("[?] Position of lights " + (String)backOrForward );
    
    
    bool donePlaying = false;
    ::game_leds.offAllLights();

    while (!donePlaying){
      Serial.println("---------------- Counting Start In Game Start ------------------");
      for ( int mil = 0 ; mil <= DELAY ; mil++ ){
        delay(1);
        if ( ::isClicked(::button_pin) ){
          donePlaying = true;
          break;
        }
      }
      if (!donePlaying) {
        if (backOrForward == 0) {
          ::game_leds.switchToBackwardLight();
        } else {
          ::game_leds.switchToForwardLight();
        }
      }
    }

    if (::game_leds.isStoppedInCenter()){
      Serial.println("[!] Player Wins !!!!!!!!!!!!!!!!!!!!!!!!! ");

    } else {
      Serial.println("[!] Player Lose !!!!!!!!!!!!!!!!!!!!!!!!! ");
    }

  }


}


void setup() {
  // put your setup code here, to run once:
  for( int i = 0 ; i < 9 ; i++){
    pinMode(::led_pins[i] , OUTPUT);
    ::game_leds.leds[i] = ::led_pins[i];
  }
  pinMode(::button_pin , OUTPUT);
  pinMode(::buzzer_pin, OUTPUT);

  Serial.begin(9600);
  randomSeed(analogRead(A0)); // Use to seed so the randomness occur

}

void loop() {
  // put your main code here, to run repeatedly:
  ::GAME();

}
