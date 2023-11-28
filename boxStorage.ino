#include <Servo.h>

const int triggerPin = 2;   // Connect the trigger pin of the distance sensor to digital pin 2
const int echoPin = 3;      // Connect the echo pin of the distance sensor to digital pin 3
const int servoPin = 9;     // Connect the signal pin of the servo to digital pin 9
bool opened = false;

Servo myServo;

void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
    if (isObjectDetected(30)) {
    // Step 2: If there is, slowly move the servo to a 90-degree angle
    if (!::opened){
      ::opened = true;
      moveServo(90);
    }

    // Step 3: Steady the servo angle for 5 seconds or until there is an object in the perimeter
    delay(5000);
    
    // Move the servo back to the initial position
    if (!isObjectDetected(30) && ::opened){
      ::opened = false;
      moveServo(0);
    }
  }
}


bool isObjectDetected(int rangeDistance) {
  // Trigger the distance sensor to send a pulse
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Measure the pulse duration on the echo pin
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters
  int distance = duration * 0.034 / 2;

  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if an object is within a certain perimeter (adjust the distance threshold as needed)
  return distance < rangeDistance; // Change 30 to the desired distance threshold in centimeters
}

void moveServo(int angle) {
  // Move the servo to the specified angle
  if (::opened){
    ::Serial.println("Close Box ....");
  } else {
    ::Serial.println("Open Box ....");
  }
  myServo.write(angle);
  delay(15); // Give the servo some time to reach the target angle
}


