/*
  Logic:
    When I Click the Red button it open the serialized lights until it close
    When I Click the Blue buttonn it open the night time activity
*/

const int BUTTON_2 = 4; // Button pin for opening and closing the serialized lights
const int BUTTON_1 = 5; // Button pin for opening and closing the night time activity lights
const int NIGHT_ACTIVITY_TRANSISTOR = 6; // Transistor to handle 5 volts to night time activity lights

const int LEDS[] = { A5 , A4 , A3 , A2 }; // ( Red , Blue , Yellow, White )
int clickDelay = 30; // microsecond delay from button
// Button for Night Time Activity
int clickCounter1 = 0; // microsecond counter from button
bool isActionIsButton1 = false; // if the action is in the button
bool isNightLightOpen = false; // If the night time activity light is open
// Button for serialized lights
int clickCounter2 = 0; // microsecond counter from button
bool isActionIsButton2 = false; // if the action is in the button
/*
  The leds must have different colors ( Red , Blue , Yellow, White )

  Sleeping Color:
     Deep Red = 225 , 25 , 0 , 0
     Warm White = 0 , 100 , 50 , 225
     Peach = 100 , 0 , 225, 0
     Burnt Orange = 225, 150 , 225, 0

*/
int row = 4;
int cols = 4;
int pos = -1 ;
int SLEEPING_COLOR[4][4] = {
  {225 , 180 , 0 , 0},
  {0 , 190, 150, 225},
  {180 , 0 , 225 , 0},
  {225, 150 , 225, 0}
};

bool isClicked(int pin){
  if (digitalRead(pin) == HIGH){
    return true;
  }
  return false;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  int range = random(0, 1000);
  for (int i = 0; i < range; i++) {
    randomSeed(analogRead(A0));
  }

  for ( int pin : ::LEDS ){
    pinMode(pin , OUTPUT);
  }

  pinMode(::BUTTON_1 , INPUT);
  pinMode(::BUTTON_2 , INPUT);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  // Night Time Activity Logic
  bool clicked1 = false; 
  if (!::isActionIsButton1){
    clicked1 = ::isClicked(::BUTTON_1);
    if (clicked1){
      ::isActionIsButton1 = true;
    }
  } else {
    delayMicroseconds(1);
    ::clickCounter1 = ::clickCounter1 + 1;
    if (::clickCounter1 == ::clickDelay){
      ::isActionIsButton1 = false;
      ::clickCounter1 = 0;
    }
    Serial.println("Night Time Button Counter : " + (String)::clickCounter1);
  }

  if ( clicked1 ){
    // Check if the night time button is clicked

    if (::isNightLightOpen){
      // If open then close it
      ::isNightLightOpen = false;
      digitalWrite(::NIGHT_ACTIVITY_TRANSISTOR , LOW);
    } else {
      // If close then open it
      ::isNightLightOpen = true;
      digitalWrite(::NIGHT_ACTIVITY_TRANSISTOR, HIGH);
    }
  }


  // Serialized Activity Logic
  bool clicked2 = false; 
  if (!::isActionIsButton2){
    clicked2 = ::isClicked(::BUTTON_2);
    if (clicked2){
      ::isActionIsButton2 = true;
    }
  } else {
    delayMicroseconds(1);
    ::clickCounter2 = ::clickCounter2 + 1;
    if (::clickCounter2 == ::clickDelay){
      ::isActionIsButton2 = false;
      ::clickCounter2 = 0;
    }
    Serial.println("Serialized Button Counter : " + (String)::clickCounter2);
  }

  if (clicked2){
    // Check if the serialized button is clicked
    ::pos = ::pos + 1;

    // Check if the pos is greater than rows, if true then close the lights and change the pos to -1
    if (::pos >= ::row){
      for (int i = 0 ; i < ::cols ; i++){
        analogWrite(::LEDS[i], 0);
      }
      ::pos = -1;
    } else {
      for (int i = 0 ; i < ::cols ; i++){
        analogWrite(::LEDS[i], ::SLEEPING_COLOR[pos][i]);
      }
    }

  }

}












