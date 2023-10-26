
// First Activity ------------- Variables
const int BUTTON = 4; // Button pin for force off or on the lights inside the room
const int LEDS[] = { A5 , A4 , A3 , A2 }; // ( Red , Blue , Yellow, White )
const int activityPin = 5; // Pin for opening study lights

int light_turn = 0; // Represent the light turn, 0 = off, 1 = Sleeping , 2 = Active , 3 = Random , 4 = Full Activity
int clickDelay = 25; // microsecond delay from button
int clickCounter = 0; // microsecond counter from button
int isActionIsButton = false; // if the action is in the button

// Second Activity ------------- Variables
const int PIR_SENSOR = 9; // Pin for sensoring outside person
const int BUZZER_PIN = 10; // Pin for buzzering when sensor detected
const int OUTSIDE_LIGHTS = 11; // Pin for outside lights when sensor detected someone

const int BUZZER_SOUND = 440 ; // The pitch of the buzzer
const int buzzerDelay = 150; // The microsecond for buzzer to sound
bool doneBuzzerDelay = false; // Used to check if done buzzering
bool doneDetecting = false; // Check if done detecting an object
const int stopingBuzzerDelay = 350; // The microsecond for buzzer to stop sound
int countedDelay = 0; // Used to cound the microsecond pass

const int SLEEPING_COLOR[4][4] = {
  {225 , 180 , 0 , 0},
  {0 , 190, 150, 225},
  {180 , 0 , 225 , 0},
  {225, 150 , 225, 0}
};

const int ACTIVE_COLOR[4][4] = {
  {0 , 0, 0 , 225},
  {0 , 180 , 150 , 225},
  {0 , 150 , 120 , 225},
  {0 , 180, 0 , 225}
};


bool detected(){
  if ( digitalRead(::PIR_SENSOR) == HIGH){
    return true;
  }
  return false;
}

bool isClicked(int pin){
  if (digitalRead(pin) == HIGH){
    return true;
  }
  return false;
}


void setup() {

  Serial.begin(9600);
  randomSeed(analogRead(A0));

  int range = random(0, 1000);
  for (int i = 0; i < range; i++) {
    randomSeed(analogRead(A0));
  }

  // First Activity Variables
  pinMode(::BUTTON , INPUT);
  pinMode(::activityPin , OUTPUT);

  // Second Activity Variables
  pinMode(::PIR_SENSOR , INPUT);
  pinMode(::BUZZER_PIN , OUTPUT);

  Serial.begin(9600);

}

void loop() {

  // FIRST ACTIVITY -----------------------------------------------------

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
  
  if ( clicked  ){
    // If clicked , Start the activity

    
      // Change every turn when clicked
    ::light_turn = ::light_turn + 1;
    if (::light_turn > 4){
      ::light_turn = 0;
    }
    
    if (::light_turn == 1 ){
      // Sleeping Lights open
      int pos = random(0,4);
      for ( int i = 0 ; i < 4 ; i++ ){
        analogWrite( ::LEDS[i] ,::SLEEPING_COLOR[pos][i]);
      }
      Serial.println("Night Time ... : " + (String)pos);
    } else if ( ::light_turn == 2){
      // Active Lights open
      int pos = random(0,4);
      for ( int i = 0 ; i < 4 ; i++ ){
        analogWrite( ::LEDS[i] ,::ACTIVE_COLOR[pos][i]);
      }
      Serial.println("Active Time ... : " + (String)pos);
    } else if ( ::light_turn == 3 ){
      // Random Lights Open
      int val1 = random(0 , 225);
      int val2 = random(0 , 225);
      int val3 = random(0 , 225);
      int val4 = random(0 , 225);
      analogWrite( ::LEDS[0] , val1 );
      analogWrite( ::LEDS[1] , val2 );
      analogWrite( ::LEDS[2] , val3 );
      analogWrite( ::LEDS[3] , val4 );
      
      Serial.println("Random Time ..." + (String)val1 + ", " + (String)val2 + ", " + (String)val3 + ", " + (String)val4 );
    } else if (::light_turn == 4){
      int pos = random(0,4);
      for ( int i = 0 ; i < 4 ; i++ ){
        analogWrite( ::LEDS[i] ,::ACTIVE_COLOR[pos][i]);
      }
      digitalWrite(::activityPin , HIGH);
    } else {
      // Close lights 
      analogWrite( ::LEDS[0] , 0 );
      analogWrite( ::LEDS[1] , 0 );
      analogWrite( ::LEDS[2] , 0 );
      analogWrite( ::LEDS[3] , 0 );
      digitalWrite(::activityPin , LOW);
      Serial.println("Close Time ...");
    }
  }

  // SECOND ACTIVITY -----------------------------------------------------

  // check if there a person outside
  if (!::doneDetecting){
    // check if detected
    Serial.println("Detecting .... ");
    if (::detected()){
      // if person detected make sound 
      ::doneDetecting = true;
      ::countedDelay = 0;
    } 
  }

  // Add microsecond to the buzzer
  if (::doneDetecting && !::doneBuzzerDelay){
    tone(::BUZZER_PIN , ::BUZZER_SOUND);
    delayMicroseconds(1);
    ::countedDelay = ::countedDelay + 1;
    Serial.println("Open The Buzzer <----");
  }

  // Add microsecond to the time out of buzzer
  if (::doneBuzzerDelay ){
    noTone(::BUZZER_PIN);
    delayMicroseconds(1);
    ::countedDelay = ::countedDelay + 1;
    Serial.println("Close The Buzzer <----");
  }

  // check if the buzzer done buzzering
  if (::doneDetecting){
    if (::countedDelay == ::buzzerDelay && !::doneBuzzerDelay){
      ::doneBuzzerDelay = true;
      ::countedDelay = 0;
    }
    if (::countedDelay == ::stopingBuzzerDelay){
      ::doneDetecting = false;
      ::doneBuzzerDelay = false;
      ::countedDelay = 0;
    }
  }
  

}










