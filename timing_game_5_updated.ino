/*
  This Arduino is a game of pairing there is 5 lights in upper and also the bottom
  First is select the 1 light in upper position by clicking the bottom
  Second is select the paired in the bottom position by clicking
  If the selected has the same position then the player wins
*/

bool isClicked(int pin){
  if ( digitalRead(pin) == HIGH) return true;
  return false;
}

void sound1(int pinSound , int pinButton){

}

struct Leds{

  int bottom_pos = 0, upper_pos = 0;
  int leds_bottom[5] , leds_upper[5];

  void setAllTheLeds( int setBottom[5] , int setUpper[5] ){
    for (int i = 0; i < 5 ; i++ ){
      leds_bottom[i] = setBottom[i];
    }
    for (int i = 0; i < 5 ; i++ ){
      leds_upper[i] = setUpper[i];
    }
  }

  void resetLeds(){
    bottom_pos = 0;
    upper_pos = 0;
    for (int i = 0; i < 5 ; i++ ){
      digitalWrite(leds_bottom[i] , LOW);
    } 
    for (int i = 0; i < 5 ; i++ ){
      digitalWrite(leds_upper[i] , LOW);
    } 
  }

  void moveThelight( bool isBottom){
    int current_pos = (isBottom) ? bottom_pos : upper_pos; // if isBottom get bottom_pos else upper_pos
    
    if (current_pos == 0){
      (isBottom) ? digitalWrite(leds_bottom[current_pos], HIGH) : digitalWrite(leds_bottom[current_pos] , HIGH);
    } else if ( current_pos > 0 ){
      
      (isBottom) ? digitalWrite(leds_bottom[current_pos], HIGH) : digitalWrite(leds_bottom[current_pos] , HIGH);

    }

    (isBottom) ? digitalWrite(leds_bottom[current_pos], LOW) : digitalWrite(leds_bottom[current_pos] , LOW);
  }

  void incrementPos(bool isBottom){
    if (isBottom){
      // if bottom position
      bottom_pos = bottom_pos + 1;
      if (bottom_pos == 5){
        bottom_pos = 0;
      }

    } else{
      // if not bottom position
      upper_pos = upper_pos + 1;
      if (upper_pos == 5){
        upper_pos = 0;
      }
    }
  }


};

int BOTTOM_LEDS[] = {2,3,4,5,6};
int UPPER_LEDS[] = {7,8,9,10,11};
int BUTTON_PIN = 12;
int BUZZER_PIN = 13;

int TIME_PASS = 60;



void setup() {
  // put your setup code here, to run once:
  for (int pin : ::UPPER_LEDS){
    pinMode(pin , OUTPUT);
  }
  for (int pin : ::BOTTOM_LEDS){
    pinMode(pin , OUTPUT);
  }

  pinMode(::BUZZER_PIN , OUTPUT);
  pinMode(::BUTTON_PIN , INPUT);

  Serial.begin(9600);
  randomSeed(analogRead(A0));

}

void loop() {
  // put your main code here, to run repeatedly:
  
  ::Leds game;
  game.setAllTheLeds(::BOTTOM_LEDS, ::UPPER_LEDS);

  bool upper_turn = false, bottom_turn = false;

  Serial.println("The game is starting ......... ");

  Serial.println("The upper leds is open !");
  while (!upper_turn){
    game.moveThelight(false);
    game.incrementPos(false);
    for ( int i = 0 ; i < ::TIME_PASS ; i++){
      if (::isClicked(::BUTTON_PIN)){
        upper_turn = true;
        break;
      }
      delay(1);
    }
  }

  Serial.println("The upper position is " + (String)game.upper_pos);

  delay(random(0, 5));

  while (!bottom_turn){
    game.moveThelight(true);
    game.incrementPos(true);
    for ( int i = 0 ; i < ::TIME_PASS ; i++){
      if (::isClicked(::BUTTON_PIN)){
        bottom_turn = true;
        break;
      }
      delay(1);
    }
  }


  Serial.println("The bottom position is " + (String)game.bottom_pos);

  
  if (game.bottom_pos == game.upper_pos){
    Serial.print("The Player Wins !!!!!!!!!!!!!!!!!!!!!!! ");
    // Player Win
  } else {
    Serial.print("The Player Lose !!!!!!!!!!!!!!!!!!!!!!! ");
    // Player Lose
  }


}









