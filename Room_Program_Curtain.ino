

const int BUTTON_OUTSIDE = 2; // Button to move the curtains outside
const int BUTTON_INSIDE = 3; // Button to move curtains inside
const int BUZZER = 4; // Buzzer when opening the curtain
const int OPEN_PIN = A5 , CLOSE_PIN = A4; // Pins for moving the turbine
const int PIR_SENSOR = 5; // Sensor for detecting person outside
// const int CURTAIN_CONTROLLER = PENDING  // Currently understanding the tools


const int BUZZER_SOUND = 1000; // The pitch of the buzzer
const int TURBINE_SPEED = 100; // Voltage for moving the turbine
bool isCurtainOpen = false; // Use to identify if the curtain is open
const int buzzerDelay = 500000; // The microsecond for buzzer to sound
bool doneBuzzerDelay = false; // Used to check if done buzzering
bool doneDetecting = false; // Check if done detecting an object
const int stopingBuzzerDelay = 2000000; // The microsecond for buzzer to stop sound
int countedDelay = 0; // Used to cound the microsecond pass

bool buttonIsClicked(int pin){
  if ( digitalRead(pin) == HIGH){
    return true;
  }
  return false;
}

bool detected(){
  if ( digitalRead(::PIR_SENSOR) == HIGH){
    return true;
  }
  return false;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(::BUTTON_INSIDE , INPUT);
  pinMode(::BUTTON_OUTSIDE , INPUT);
  pinMode(::PIR_SENSOR , INPUT);

  pinMode(::BUZZER , OUTPUT);
  pinMode(::OPEN_PIN , OUTPUT);
  pinMode(::CLOSE_PIN , OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

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
    tone(::BUZZER , ::BUZZER_SOUND);
    delayMicroseconds(1);
    ::countedDelay = ::countedDelay + 1;
    Serial.println("Open The Buzzer <----");
  }

  // Add microsecond to the time out of buzzer
  if (::doneBuzzerDelay ){
    noTone(::BUZZER);
    delayMicroseconds(1);
    ::countedDelay = ::countedDelay + 1;
    Serial.println("Close The Buzzer <----");
  }

  // check if the buzzer done buzzering
  if (::doneDetecting){
    if (::countedDelay == ::buzzerDelay){
      ::doneBuzzerDelay = true;
      ::countedDelay = 0;
    }
    if (::countedDelay == ::stopingBuzzerDelay){
      ::doneDetecting = false;
      ::doneBuzzerDelay = false;
      ::countedDelay = 0;
    }
  }
  
  
  if (::buttonIsClicked(::BUTTON_INSIDE) || ::buttonIsClicked(::BUTTON_OUTSIDE)){
    // If the buttons clicked move the curtain based if the curtain is open or close
    ::isCurtainOpen = !::isCurtainOpen; // Change the moving of the curtain to open or close

    // Off the buzzer when the button is clicked
    
    if (::isCurtainOpen){
      Serial.println("Opening The Curtain ---------> ");
    } else {
      Serial.println("Closing The Curtain ---------> ");
    }

    if (::doneDetecting){
      ::doneDetecting = false;
      ::doneBuzzerDelay = false;
      ::countedDelay = 0;
      tone(::BUZZER , 1000);
    }

    while (::buttonIsClicked(::BUTTON_OUTSIDE)){
      // Move the curtain until not pressed the Button Outside
      if (::isCurtainOpen){
        // If the is opening, Make sound to alert the inside
        tone(::BUZZER, ::BUZZER_SOUND);
      }
      // Put here the code of moving the servor motor
      if (::isCurtainOpen){
        digitalWrite(::OPEN_PIN , ::TURBINE_SPEED);
      } else {
        digitalWrite(::CLOSE_PIN , ::TURBINE_SPEED);
      }
    }

    while (::buttonIsClicked(::BUTTON_INSIDE)){
      // Move the curtain until not pressed the Button Outside
      if (::isCurtainOpen){
        // If the is opening, Make sound to alert the inside
        tone(::BUZZER, ::BUZZER_SOUND);
      }
      // Put here the code of moving the servor motor
      if (::isCurtainOpen){
        digitalWrite(::OPEN_PIN , ::TURBINE_SPEED);
      } else {
        digitalWrite(::CLOSE_PIN , ::TURBINE_SPEED);
      }
    }

    noTone(::BUZZER); // Done using buzzer

  } else {
    // Stop the moving of the Curtain
    digitalWrite(::OPEN_PIN, LOW);
    digitalWrite(::CLOSE_PIN , LOW);
  }



}


