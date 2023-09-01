
/*
  Logic:
    When I click the button it give me a serialized lights ( Sleeping , Nightime Activity, Random )
    But when the sensor found out that a working my laptop its atomatic turn to Nightime Activity
*/

const int ECHO = 2, TRIGGER = 3; // Pins for handling the lights inside the room
const int BUTTON = 4; // Button pin for force off or on the lights inside the room
const int LEDS[] = { A5 , A4 , A3 , A2 }; // ( Red , Blue , Yellow, White )
const int LAPTOP_RANGE = 225;
int light_turn = 0; // Represent the light turn, 0 = off, 1 = Sleeping , 2 = Active , 3 = Random
/*
  The leds must have different colors ( Red , Blue , Yellow, White )

  Sleeping Color:
     Deep Red = 225 , 25 , 0 , 0
     Warm White = 0 , 100 , 50 , 225
     Peach = 100 , 0 , 225, 0
     Burnt Orange = 225 , 100 , 225 , 0

  Nightime Activity Color:
    Daylight = 0 , 0, 100, 0 , 225
    Art Galleries = 0 , 100 , 70 , 225
    Workspaces = 0 , 50 , 30 , 225
    Fluorescent Lights = 0 , 150, 0 , 225

  Random Color :
    just randomize color intensity

*/
const int SLEEPING_COLOR[4][4] = {
  {0, 100, 0 , 225},
  {0 , 100, 50, 225},
  {100 , 0 , 225 , 0},
  {225, 100 , 225, 0}
};

const int ACTIVE_COLOR[4][4] = {
  {225 , 25, 0 , 0},
  {0 , 100 , 70 , 225},
  {0 , 50 , 30 , 225},
  {0 , 150, 0 , 225}
};

bool isClicked(int pin){
  if (digitalRead(pin) == HIGH){
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
    } else {
      detected = false;
    }
  }

};


CustomSensor sensor(::ECHO , ::TRIGGER , ::LAPTOP_RANGE );

void setup() {

  Serial.begin(9600);
  randomSeed(analogRead(A0));

  int range = random(0, 1000);
  for (int i = 0; i < range; i++) {
    randomSeed(analogRead(A0));
  }

  for ( int pin : ::LEDS ){
    pinMode(pin , OUTPUT);
  }

  pinMode(::BUTTON , INPUT);
  pinMode(::TRIGGER, OUTPUT);
  pinMode(::ECHO , INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  ::sensor.detect(); // check if using laptop

  if (::isClicked(::BUTTON) || ::sensor.detected){
    // If clicked , Start the activity

    if (::light_turn < 4){
      // Change every turn when clicked
      ::light_turn = ::light_turn + 1;
    } else {
      ::light_turn = 0;
    }

    if (::sensor.detected){
      // Convert to active if detected using laptop
      ::light_turn = 2;
    }

    
    if (::light_turn == 1 ){
      // Sleeping Lights open
      int pos = random(0,4);
      for ( int i = 0 ; i < 4 ; i++ ){
        analogWrite( ::LEDS[i] ,::SLEEPING_COLOR[pos][i]);
      }
    } else if ( ::light_turn == 2){
      // Active Lights open
      int pos = random(0,4);
      for ( int i = 0 ; i < 4 ; i++ ){
        analogWrite( ::LEDS[i] ,::ACTIVE_COLOR[pos][i]);
      }
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
    } else {
      // Close lights 
      analogWrite( ::LEDS[0] , 0 );
      analogWrite( ::LEDS[1] , 0 );
      analogWrite( ::LEDS[2] , 0 );
      analogWrite( ::LEDS[3] , 0 );
    }
    
  } 


}







