#ifndef SERVO_H
#define SERVO_H

#include <ESP32Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo

void setupServo() {
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    myservo1.setPeriodHertz(50);    // standard 50 hz servo
    myservo1.attach(38, 1000, 2000); // attaches the servo on pin 18 to the servo object
    
    myservo2.setPeriodHertz(50);    // standard 50 hz servo
    myservo2.attach(39, 1000, 2000); // attaches the servo on pin 18 to the servo object
    
}

void dance() {
  while (1){
    int pos1 = 180;    // variable to store the servo position
    int pos2 = 0;    // variable to store the servo position
    while (pos1 <= 180) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo1.write(pos1);              // tell servo to go to position in variable 'pos'    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
      myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
      pos1++;
      pos2--;
    }
    while (pos1 >= 0) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
      myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
      pos1--;
      pos2++;
    }
  }
}

#endif // SERVO_H
