#include "LedControl.h"
// #include <Servo.h>

LedControl led = LedControl(6,5,3,2);
int speakerPin = 8;

int blinkDelay = 300;
int blinkLength = 20;
bool blinking = false;
int counter = 0;

const int echoPin = 11; 
const int trigPin = 12;

byte blank = B00000000;
byte frame = B01111000;
byte inner = B10000100;
byte pupil = B10110100;

// Servo myServoOne;
// Servo myServoTwo;
// int wingLeft = 9; 
// int wingRight = 10;
// int servoNeck = 10;
// int pos = 0;

void writeEyesCute (int addr) {
  led.setRow(addr, 0, B00100000);
  led.setRow(addr, 1, B00010000);
  led.setRow(addr, 2, B00001000);
  led.setRow(addr, 3, B00000100);
  led.setRow(addr, 4, B00000100);
  led.setRow(addr, 5, B00001000);
  led.setRow(addr, 6, B00010000);
  led.setRow(addr, 7, B00100000);
  led.setRow(addr, 8, blank);
}

void writeEyesJudging(int addr) {
  led.setRow(addr, 0, B00100100);
  led.setRow(addr, 1, B00100100);
  led.setRow(addr, 2, B00111100);
  led.setRow(addr, 3, B00100100);
  led.setRow(addr, 4, B00100100);
  led.setRow(addr, 5, B00111100);
  led.setRow(addr, 6, B00100100);
  led.setRow(addr, 7, B00100100);
  led.setRow(addr, 8, B00100100);
}

void writeEyesSleep (int addr) {
  led.setRow(addr, 0, B00000100);
  led.setRow(addr, 1, B00001000);
  led.setRow(addr, 2, B00010000);
  led.setRow(addr, 3, B00100000);
  led.setRow(addr, 4, B00100000);
  led.setRow(addr, 5, B00010000);
  led.setRow(addr, 6, B00001000);
  led.setRow(addr, 7, B00000100);
  led.setRow(addr, 8, B00000010);
}
void writeEyesShifted(int addr, int shiftX, int shiftY) {
    led.setRow(addr, 0,  blank);
    led.setRow(addr, 1,  blank);
    led.setRow(addr, 6,  blank);
    led.setRow(addr, 7,  blank);

    led.setRow(addr, 0 + shiftY, frame >> shiftX);
    led.setRow(addr, 1 + shiftY, inner >> shiftX);

    if (blinking) {
        led.setRow(addr, 2 + shiftY, inner >> shiftX);
        led.setRow(addr, 3 + shiftY, inner >> shiftX);

    } else {
        led.setRow(addr, 2 + shiftY, pupil >> shiftX);
        led.setRow(addr, 3 + shiftY, pupil >> shiftX);
    }

    led.setRow(addr, 4 + shiftY, inner >> shiftX);
    led.setRow(addr, 5 + shiftY, frame >> shiftX);
}

void setup() {
  pinMode(echoPin, INPUT); 
  pinMode(trigPin, OUTPUT);
    for (int i = 0; i < 2; i++) {
        led.shutdown(i, false);
        led.setIntensity(i, 2);
        led.clearDisplay(i);
    }
  Serial.begin(9600);
}

void loop() {
    if (blinking && counter > blinkLength) {
        blinking = false;
        counter = 0;

    } else if (!blinking && counter > blinkDelay) {
        blinking = true;
        counter = 0;
    }

  counter++;
    
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5); 
  digitalWrite(trigPin,LOW);

  long duration = pulseIn(echoPin, HIGH);
  int cm = duration / 29 / 2;
  Serial.println(cm);

  if (cm > 100) {
    writeEyesSleep(0);
    writeEyesSleep(1);
    delay(100);
    playOwlGrowl();
  } else if (cm <= 100 && cm >= 75) {
    writeEyesShifted(0,1,1);
    writeEyesShifted(1,1,1);
    playOwlSound();
    delay(100); 
  } else if (cm <=75 && cm >=50) {
    writeEyesCute(0);
    writeEyesCute(1);
    playWhistle();
    delay(100);
  } else if (cm < 50) { 
    writeEyesJudging(0);
    writeEyesJudging(1);
    Serial.println(cm);
    delay(100);
    playOwlGrowl();
    }

  }
  // int frequency = map(cm, 0, 100, 200, 400);
  // tone(speakerPin, frequency);

void playOwlGrowl() {
  for (int i = 0; i <200; i=i+20) {
    int durations[] = {300, 100, 200};
    tone(speakerPin, i, durations[i]);
    delay(100);
  }
}


void playOwlSound() {
  int frequencies[] = {330, 330, 330};
  int durations[] = {300, 500, 200};
  for (int i = 0; i < sizeof(frequencies) / sizeof(frequencies[0]); i++) {
    tone(speakerPin, frequencies[i], durations[i]);
    delay(durations[i] + 50);
  }
  noTone(speakerPin);
  delay(50);
}


void playWhistle() {
  int frequencies[] = {500, 550, 560, 600, 570};
  int durations[] = {300, 200, 150, 100, 50};
  for (int i = 0; i < sizeof(frequencies) / sizeof(frequencies[0]); i++) {
    tone(speakerPin, frequencies[i], durations[i]);
    delay(durations[i]);
  }
  noTone(speakerPin);
  delay(100);
}
