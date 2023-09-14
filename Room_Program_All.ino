
// First Activity ------------- Variables
const int ECHO = 2, TRIGGER = 3; // Pins for handling the lights inside the room
const int BUTTON = 4; // Button pin for force off or on the lights inside the room
const int LEDS[] = { A5 , A4 , A3 , A2 }; // ( Red , Blue , Yellow, White )
const int activityPin = 5; // Pin for opening all the lights

const int LAPTOP_RANGE = 10;
int light_turn = 0; // Represent the light turn, 0 = off, 1 = Sleeping , 2 = Active , 3 = Random , 4 = Full Activity
int clickDelay = 25; // microsecond delay from button
int clickCounter = 0; // microsecond counter from button
int isActionIsButton = false; // if the action is in the button
int detectDelay = 40; // microsecond delay from button
int detectCounter = 0; // microsecond counter from button
int isActionIsDetection = false; // if the action is in the button

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
    } else {
      detected = false;
    }
  }

};

CustomSensor sensor(::ECHO , ::TRIGGER , ::LAPTOP_RANGE );

// Second Activity ------------- Variables
const int FAN_BUTTON = 7; // Button to open the celling fan
const int FAN_TRANSISTOR = 8; // Transistor pin to control the flow of volts
const int PIR_SENSOR = 9; // Pin for sensoring outside person
const int BUZZER_PIN = 10; // Pin for buzzering when sensor detected

bool isFanOpen = false; // Use to check if the fan button is clicked
const int BUZZER_SOUND = 325; // The pitch of the buzzer
const int buzzerDelay = 150; // The microsecond for buzzer to sound
bool doneBuzzerDelay = false; // Used to check if done buzzering
bool doneDetecting = false; // Check if done detecting an object
const int stopingBuzzerDelay = 350; // The microsecond for buzzer to stop sound
int countedDelay = 0; // Used to cound the microsecond pass

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
  pinMode(::TRIGGER, OUTPUT);
  pinMode(::ECHO , INPUT);

  // Second Activity Variables
  pinMode(::FAN_BUTTON , INPUT);
  pinMode(::FAN_TRANSISTOR , OUTPUT);
  pinMode(::PIR_SENSOR , INPUT);
  pinMode(::BUZZER_PIN , OUTPUT);

  Serial.begin(9600);

}

void loop() {

  // FIRST ACTIVITY -----------------------------------------------------
  ::sensor.detect(); // check if using laptop

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

  bool detect = false;
  if (!::isActionIsDetection){
    detect = ::sensor.detected;
    if (detect){
      ::isActionIsDetection = true;
    }
  } else {
    delayMicroseconds(1);
    ::detectCounter = ::detectCounter + 1;
    if (::detectCounter == ::detectDelay){
      ::isActionIsDetection = false;
      ::detectCounter = 0;
    }
    Serial.println("Detect Counter : " + (String)::detectCounter);
  }
  
  if ( clicked || detect ){
    // If clicked , Start the activity

    
      // Change every turn when clicked
    ::light_turn = ::light_turn + 1;
    if (::light_turn > 4){
      ::light_turn = 0;
    }

    if (::sensor.detected){
      // Convert to active if detected using laptop
      ::light_turn = 4;
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
  
  if ( ::isClicked(::FAN_BUTTON) ){
    ::isFanOpen = !::isFanOpen;
  }

  if (::isFanOpen){
    digitalWrite(::FAN_TRANSISTOR , HIGH);
    Serial.println("FAN OPEN <-------- ");
  } else {
    digitalWrite(::FAN_TRANSISTOR , LOW);
    Serial.println("FAN CLOSE -------->");
  }

}










