/*
  Logic:
    When I Click the button it open the night time activity
    then if I Click again it open the serialized lights until it close
*/

const int BUTTON = 5; // Button pin for opening and closing the activity lights
const int NIGHT_ACTIVITY_TRANSISTOR = 6; // Transistor to handle 5 volts to night time activity lights

const int LEDS[] = { A5 , A4 , A3 , A2 }; // ( Red , Blue , Yellow, White )
int clickDelay = 30; // microsecond delay from button
// Button Activity
int clickCounter = 0; // microsecond counter from button
bool isActionIsButton = false; // if the action is in the button
bool isNightLightOpen = false; // If the night time activity light is open
int selectedAction = 0 ; // if the action is 0 = close, 1 = night time and 2 = serialized lights
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

  pinMode(::BUTTON , INPUT);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  // Night Time Activity Logic
  bool clicked = false; 
  if (!::isActionIsButton){
    clicked = ::isClicked(::BUTTON);
    if (clicked){
      ::isActionIsButton = true;
    }
  } else {
    delayMicroseconds(1);
    ::clickCounter = ::clickCounter + 1;
    if (::clickCounter == ::clickDelay){
      ::isActionIsButton = false;
      ::clickCounter = 0;
    }
    Serial.println("Button Counter : " + (String)::clickCounter);
  }

  if ( clicked ){
    
    // modify ::selectedAction
    ::selectedAction = ::selectedAction + 1;

    if (::selectedAction == 1){
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

    } else if (::selectedAction == 2){
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
    } else{
      // Check if the button clicked does not match the actions
      ::selectedAction = 0;
    }
    
  }

}












