#define PIR_INPUT_PIN 5
#define LED_PIN 13

bool movementAlreadyDetected = false;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_INPUT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  checkForPirInput();
}

void checkForPirInput() {
  if (isPirInputHigh() && !movementAlreadyDetected) {
    onPirMovementDetected();
  } else if (isPirInputLow()) {
    onPirMovementGone();
  }
}

bool isPirInputHigh() {
  return isPinHigh(PIR_INPUT_PIN);
}

bool isPirInputLow() {
  return isPinLow(PIR_INPUT_PIN);
}

bool isPinHigh(int pinNumber) {
  return digitalRead(pinNumber) == HIGH;
}

bool isPinLow(int pinNumber) {
  return !isPinHigh(pinNumber);
}

void onPirMovementDetected() {
  movementAlreadyDetected = true;
//  lightLed();
  tripleBlink();
  Serial.println("Motion detected");
}

void onPirMovementGone() {
  movementAlreadyDetected = false;
  lightDownLed();
  Serial.println("Motion gone");
}

void tripleBlink() {
  for(int i = 0; i <= 3; i++) {
    lightLed();
    delay(75);
    lightDownLed();  
    delay(75);
  }
}

void lightLed() {
  digitalWrite(LED_PIN, HIGH);
}

void lightDownLed() {
    digitalWrite(LED_PIN, LOW);
}
