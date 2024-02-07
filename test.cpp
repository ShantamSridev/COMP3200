#include <Arduino.h>
#include <stdio.h>

int pinA = 7;
int pinB = 8;

long int pA1 = 0;
int pA2 = 0;
int pB1 = 0;
int pB2 = 0;
int direction = 0;
double avg1;
double avg2;

void setup() {
  Serial.begin(9600);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);

}

void loop() {
  //digitalWrite(32,HIGH);
  int val1 = digitalRead(pinA);
  int val2 = digitalRead(pinB);
  if ((val1 != 0) && (val2 != 0)){
    if (val1>val2){
      direction = 1;
      Serial.println("CW");
    }
    else{
      direction = 2;
      Serial.println("CCW");
    }
  }
  else {
    direction = 0;
    Serial.println("STOP");
  }
  pA1 = pulseIn(pinA, HIGH);
  pA2 = pulseIn(pinA, LOW);
  pB1 = pulseIn(pinB, HIGH);
  pB2 = pulseIn(pinB, LOW);
  Serial.println((1000000*60)/((pA1+pA2)*7));
  avg1 = (1000000*60)/((pA1+pA2)*7);
  avg2 = (1000000*60)/((pB1+pB2)*7);

  double avg = (avg1 + avg2)/2;
  //Serial.println(duration);
  //Serial.println(1000/duration); 
  Serial.println(avg);
}

// #include <Servo.h>

// Servo myservo;  // create servo object to control a servo
// // twelve servo objects can be created on most boards

// int pos = 0;    // variable to store the servo position

// void setup() {
//   myservo.attach(9,1000,2000);  // attaches the servo on pin 9 to the servo object
//   myservo.write(90);
// }

// void loop() {
//   delay(2000);
//   for (pos = 90; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15ms for the servo to reach the position
//   }
//   for (pos = 180; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15ms for the servo to reach the position
//   }
//   delay(2000);
//   for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15ms for the servo to reach the position
//   }
//   for (pos = 0; pos <= 90; pos += 1) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15ms for the servo to reach the position
//   }
// }