

const int CUSTOM_DELAY = 5; // 5 Seconds
const int BUTTON_OUTSIDE = 2; // Button to move the curtains outside
const int BUTTON_INSIDE = 3; // Button to move curtains inside
const int BUZZER = 7; // Buzzer when opening the curtain
// const int CURTAIN_CONTROLLER = PENDING  // Currently understanding the tools

bool isCurtainOpen = false; // Use to identify if the curtain is open

bool buttonIsClicked(int pin){
  if ( digitalRead(pin) == HIGH){
    return true;
  }
  return false;
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (::buttonIsClicked(::BUTTON_INSIDE) || ::buttonIsClicked(::BUTTON_OUTSIDE)){
    // If the buttons clicked move the curtain based if the curtain is open or close
    ::isCurtainOpen = !::isCurtainOpen; // Change the moving of the curtain to open or close

    while (::buttonIsClicked(::BUTTON_OUTSIDE)){
      // Move the curtain until not pressed the Button Outside
      if (::isCurtainOpen){
        // If the is opening, Make sound to alert the inside
        tone(::BUZZER, 1000);
      }
      // Put here the code of moving the servor motor
    }

    while (::buttonIsClicked(::BUTTON_INSIDE)){
      // Move the curtain until not pressed the Button Outside
      if (::isCurtainOpen){
        // If the is opening, Make sound to alert the inside
        tone(::BUZZER, 1000);
      }
      // Put here the code of moving the servor motor
    }

  } else {
    // Stop the moving of the Curtain
  }

}


