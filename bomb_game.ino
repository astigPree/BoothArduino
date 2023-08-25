#include <Servo.h>

struct CustomSensor {
  int echo = 0;
  int trig = 0;
  int range = 0;
  bool detected = false;

  void set_pins(int pin1, int pin2, int val) {
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

struct Result {

  int leds[3];
  int counter = 0;
  bool done = false;
  bool dpins = false;  // if the leds are conected to digital pin

  void set_pins(int pins[], bool digital) {
    dpins = digital;
    if (digital) {
      for (int i = 0; i < 3; i++) {
        leds[i] = pins[i];
        digitalWrite(pins[i], LOW);
      }
    } else {
      for (int i = 0; i < 3; i++) {
        leds[i] = pins[i];
        analogWrite(pins[i], 0);
      }
    }
  }

  bool activate() {
    // on the leds and check if all leds are on
    if (dpins) {
      digitalWrite(leds[counter], HIGH);
    } else {
      analogWrite(leds[counter], 255);
    }
    counter = counter + 1;

    if (counter > 2) {
      done = true;
      return true;
    } else {
      return false;
    }
  }
};

struct CustomTimer {
  int leds[3];
  int counter = 0;


  void set_pins(int pins[]) {
    for (int i = 0; i < 3; i++) {
      leds[i] = pins[i];
    }
  }

  void on_lights() {
    for (int i = 0; i < 3; i++) {
      analogWrite(leds[i], 255);
    }
  }

  void decrease_minute() {
    for (int i = 255; i > -1; i--) {
      analogWrite(leds[counter], i);
      delay(10);
    }
    counter = counter + 1;
  }
};

struct GameWires {
  Result win, lose;

  int wires[6];
  int correct_wires[3] = { 0, 0, 0 };
  int wrong_wires[3] = { 0, 0, 0 };

  void set_pins(int pins[]) {
    for (int i = 0; i < 6; i++) {
      wires[i] = pins[i];
    }
  }

  void generate_pins() {
    // Use to generate a random number to make a guessing number
    int shufpins[6];
    for (int i = 0; i < 6; i++) {
      shufpins[i] = wires[i];
    }

    for (int i = 0; i < 6; i++) {
      int j = random(i, 6);
      int temp = shufpins[i];
      shufpins[i] = shufpins[j];
      shufpins[j] = temp;
    }

    Serial.print("Correct pins : ");
    for (int i = 0; i < 3; i++) {
      correct_wires[i] = shufpins[i];
      Serial.print(" " + (String)correct_wires[i]);
    }
    Serial.println();

    Serial.print("Wrong pins : ");
    for (int i = 0; i < 3; i++) {
      wrong_wires[i] = shufpins[i + 3];
      Serial.print(" " + (String)wrong_wires[i]);
    }
    Serial.println();
  }

  bool check_wire() {
    for (int i = 0; i < 6; i++) {
      if (digitalRead(wires[i]) == 0) {
        bool result = checking(wires[i]);
        if (result) {
          return true;
        }
      }
    }
    return false;
  }

  bool checking(int wire) {
    for (int i = 0; i < 3; i++) {
      if (wrong_wires[i] == wire) {
        wrong_wires[i] = 0;
        Serial.println("Lose : " + (String)wire);
        bool result = lose.activate();
        return result;
      }
    }

    for (int i = 0; i < 3; i++) {
      if (correct_wires[i] == wire) {
        correct_wires[i] = 0;
        Serial.println("Win : " + (String)wire);
        bool result = win.activate();
        return result;
      }
    }

    return false;
  }
};

int win_pins[] = { A0, A1, A2 };
int lose_pins[] = { 5, 6, 7 };

GameWires gwires;
int wire_pins[] = { 8, 9, 10, 11, 12, 13 };

CustomSensor start;
int echo = 2;  // to make my random work must be INPUT the pin
int trigger = 3;


CustomTimer timer;
int timer_pins[] = { A3, A4, A5 };

Servo popper;
int popper_pin = 4;

// Additional Features



void setup() {
  // A0 is not input so i can
  Serial.begin(9600);
  randomSeed(analogRead(A0));

  int range = random(0, 1000);
  for (int i = 0; i < range; i++) {
    randomSeed(analogRead(A0));
  }

  // Initializing pins for every objects
  for (int pin : win_pins) {
    pinMode(pin, OUTPUT);
  }
  gwires.win.set_pins(win_pins, false);

  for (int pin : lose_pins) {
    pinMode(pin, OUTPUT);
  }
  gwires.lose.set_pins(lose_pins, true);

  for (int pin : wire_pins) {
    pinMode(pin, INPUT);
  }
  gwires.set_pins(wire_pins);


  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  start.set_pins(echo, trigger, 20);


  for (int pin : timer_pins) {
    pinMode(pin, OUTPUT);
  }
  timer.set_pins(timer_pins);

  popper.attach(popper_pin);
  popper.write(0);

  // configuring to ready the activity
}


void loop() {
  // put your main code here, to run repeatedly:
  if (!start.detected) {
    start.detect();
    gwires.generate_pins();
    Serial.println();
  } else {
    gwires.generate_pins();
    timer.on_lights();
    for (int minute = 0; minute < 3; minute++) {
      //  minute pass
      for (int sec = 0; sec < 60; sec++) {
        // second pass
        for (int mil = 0; mil < 1000; mil++) {
          // millisecond pass
          run_activity();
          delay(1);
        }
        Serial.println((String)sec + "s");
      }
      Serial.println((String)minute + " minute time pass ");
      timer.decrease_minute();
    }
    game_over(90);
  }
}

void game_over(int pos) {

  for (int i = 0; i < 3; i++) {
    analogWrite(win_pins[i], 0);
  }

  for (int i = 0; i < 3; i++) {
    digitalWrite(lose_pins[i], 0);
  }

  for (int i = 0; i < 3; i++) {
    analogWrite(timer_pins[i], 0);
  }
  for ( int i = 0 ; i <= pos ; i++ ){
    popper.write(i);
    Serial.println("Tiyog + :" + (String)i);
  }
  popper.write(0);
  Serial.println("Player Lose !");
  while (true) {
    digitalWrite(lose_pins[0] , HIGH);
    digitalWrite(lose_pins[1] , LOW);
    digitalWrite(lose_pins[2], HIGH);
    delay(100);
    digitalWrite(lose_pins[0] , LOW);
    digitalWrite(lose_pins[1] , HIGH);
    digitalWrite(lose_pins[2], LOW);
    delay(100);
  }
}


void run_activity() {
  // Contain the activity related to wirings or games logic

  bool result = gwires.check_wire();
  //Serial.println("Activity Done : " + (String)result);
  if (!result) { return; }  // the objective is not done yet

  if (gwires.lose.done) {
    // pop the ballons
    Serial.println("Player Lose ");
    game_over(90);
  } else if (gwires.lose.done && gwires.win.done) {
    // pop the ballon if it draw
    Serial.println("Player Lose ");
    game_over(90);
  } else if (gwires.win.done) {
    // show the winning design
    Serial.println("Player Win ");
    while (true) {
      wave_lights(timer.leds, false);
      wave_lights(gwires.win.leds, false);
      wave_lights(gwires.lose.leds, true);
    }
  } else {
    // Do nothingh here
  }
}



void wave_lights(int pins[], bool digital) {
  // wave a the lights
  if (digital) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(pins[i], LOW);
    }
    delay(50);
    for (int i = 0; i < 3; i++) {
      digitalWrite(pins[i], HIGH);
      delay(50);
    }
  } else {
    for (int i = 0; i < 3; i++) {
      analogWrite(pins[i], 0);
    }
    delay(50);
    for (int i = 0; i < 3; i++) {
      analogWrite(pins[i], 255);
      delay(50);
    }
  }
}
