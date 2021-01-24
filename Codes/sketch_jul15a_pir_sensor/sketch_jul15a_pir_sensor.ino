    /*
     * PIR sensor tester
     */
    int inputPin = 0;               // choose the input pin (for PIR sensor)
    int redLedPin = 1;
    int greenLedPin = 2;                // choose the pin for the LED
    int controlLedPin = 3;
    int pirState = LOW;             // we start, assuming no motion detected
    int val = 0;                    // variable for reading the pin status

    int flag = true;
     
    void setup() {
      pinMode(redLedPin, OUTPUT);      // declare LED as output
      pinMode(greenLedPin, OUTPUT);      // declare LED as output
      pinMode(controlLedPin, OUTPUT);
      pinMode(inputPin, INPUT);     // declare sensor as input
     
      Serial.begin(9600);
    }
     
    void loop(){
      
      if (flag) {
        digitalWrite(controlLedPin, HIGH);
        flag = false;
      } else {
        digitalWrite(controlLedPin, LOW);
        flag = true;
      }
      
      val = digitalRead(inputPin);  // read input value
      if (val == HIGH) {            // check if the input is HIGH
        digitalWrite(redLedPin, HIGH);  // turn LED ON
        digitalWrite(greenLedPin, LOW);  // turn LED ON
        if (pirState == LOW) {
          // we have just turned on
          Serial.println("Motion detected!");
          // We only want to print on the output change, not state
          pirState = HIGH;
        }
      } else {
        digitalWrite(redLedPin, LOW); // turn LED OFF
        digitalWrite(greenLedPin, HIGH);  // turn LED ON
        if (pirState == HIGH){
          // we have just turned of
          Serial.println("Motion ended!");
          // We only want to print on the output change, not state
          pirState = LOW;
        }
      }

      delay(100);
    }
