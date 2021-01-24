#define trigPin 2
#define echoPin 3
#define ledPin 1
 
void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT); //Pin, do którego podłączymy trig jako wyjście
  pinMode(echoPin, INPUT); //a echo, jako wejście
  pinMode(ledPin, OUTPUT);
  pinMode(5, OUTPUT); //Wyjście dla buzzera
}
 
void loop() {  
  zakres(10, 25); //Włącz alarm, jeśli w odległości od 10 do 25 cm od czujnika jest przeszkoda
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
} 
 
int zmierzOdleglosc() {
  long czas, dystans;
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  czas = pulseIn(echoPin, HIGH);
  dystans = czas * 0.034/2;

  Serial.println("Distance: ");
  Serial.println(dystans);
  return dystans;
}
 
void zakres(int a, int b) {
  int jakDaleko = zmierzOdleglosc();
  if (jakDaleko <= 5) {
      digitalWrite(5, HIGH); //Włączamy buzzer
  } else if ((jakDaleko > 5) && (jakDaleko <= 15)) {
      beepXTimes(9);
  } else if ((jakDaleko > 15) && (jakDaleko <= 30)) {
      beepXTimes(6);
  } else if ((jakDaleko > 30) && (jakDaleko <= 40)) {
      beepXTimes(4);
  } else if ((jakDaleko > 40) && (jakDaleko <= 70)) {
      beepXTimes(2);
  } else {
      digitalWrite(5, LOW);
  }
}

void beepXTimes(int times) {
  float calculatedDelay = 1.0f/((float)(times));
  Serial.print("calculatedDelay: ");
  Serial.println(calculatedDelay);
  Serial.print("times: ");
  Serial.println(times);

  for(int i = 0; i < times; i++) {
    digitalWrite(5, HIGH);
    delay(calculatedDelay * 500);
    digitalWrite(5, LOW);
    delay(calculatedDelay * 500);
  }
}
