#include <Servo.h>
Servo myServoNeck;
Servo myServoOne;
Servo myServoTwo;

int wingLeft = 9;
int wingRight = 11;
int servoNeck = 3;
int pos = 0;
int posNeck = 0;
int maxDistance = 100;
const int echoPin = 6;
const int trigPin = 5;


void setup() {
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  myServoNeck.attach(servoNeck);
  myServoOne.attach(wingLeft);
  myServoTwo.attach(wingRight);
  Serial.begin(9600);
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int cm = duration / 29 / 2;
  int angle = map(cm, 0, maxDistance, 0, 100);
  Serial.println(cm);
  myServoOne.write(90);
  myServoTwo.write(0);
  // myServoNeck.write(0);
  delay(10);
  // moveWing();
  // moveNeck(); 
  delay(500);
}

void moveWing() {
  for (pos = 20; pos <= 120; pos += 4) {
    myServoOne.write(90 - pos);
    myServoTwo.write(pos-25);
    delay(100);
  }

  for (pos = 120; pos >= 20; pos -= 2) {
    myServoOne.write(90 - pos);
    myServoTwo.write(pos-25);
    delay(50);
  }
}


void moveNeck() {
  for (posNeck = 70; posNeck <= 160; posNeck += 1) {
    myServoNeck.write(posNeck);
    delay(20);
  }

  for (posNeck = 160; posNeck >= 70; posNeck -= 1) {
    myServoNeck.write(posNeck);
    delay(20);
  }
}