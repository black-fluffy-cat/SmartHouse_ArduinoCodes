// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 3; 
// Create a servo object 
Servo Servo1; 
void setup() { 
   // We need to attach the servo to the used pin number 
   Servo1.attach(servoPin); 
}
void loop(){ 
  int i = 0;
  for(i = 0; i < 180; i = i + 30) {
    Servo1.write(i);
    delay(1000);
  }
  for(i = 180; i > 0; i = i - 30) {
   Servo1.write(i); 
   delay(1000);
  }
}
