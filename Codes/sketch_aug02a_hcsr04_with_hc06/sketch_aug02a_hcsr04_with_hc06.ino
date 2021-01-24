#define trigPin 3
#define echoPin 2

#include <SoftwareSerial.h> 
SoftwareSerial BTserial(18, 19); // RX, TX
//D18 is A4
//D19 is A5

void setup() {
  Serial.begin(9600);
  Serial.println("Serial started at 9600");
  
  BTserial.begin(9600);  
  Serial.println("BTserial started at 9600");
  
  pinMode(trigPin, OUTPUT); //Pin, do którego podłączymy trig jako wyjście
  pinMode(echoPin, INPUT); //a echo, jako wejście
  pinMode(4, OUTPUT); //Wyjście dla buzzera
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
}
 
void loop() {  
  zakres(10, 25); //Włącz alarm, jeśli w odległości od 10 do 25 cm od czujnika jest przeszkoda  
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

//  Serial.print("Distance: ");
//  Serial.println(dystans);
  return dystans;
}
 
void zakres(int a, int b) {
  int jakDaleko = zmierzOdleglosc();

//  Serial.print("jakDaleko: ");
  Serial.println(String(jakDaleko) + "#");
  BTserial.print(String(jakDaleko) + "#");
  delay(100);
//  if (jakDaleko <= 5) {
//      digitalWrite(4, HIGH); //Włączamy buzzer
//  } else if ((jakDaleko > 5) && (jakDaleko <= 15)) {
//      beepXTimes(9);
//  } else if ((jakDaleko > 15) && (jakDaleko <= 30)) {
//      beepXTimes(6);
//  } else if ((jakDaleko > 30) && (jakDaleko <= 40)) {
//      beepXTimes(4);
//  } else if ((jakDaleko > 40) && (jakDaleko <= 70)) {
//      beepXTimes(2);
//  } else {
//      digitalWrite(4, LOW);
//  }
}

void beepXTimes(int times) {
  float calculatedDelay = 1.0f/((float)(times));
//  Serial.print("calculatedDelay: ");
//  Serial.println(calculatedDelay);
//  Serial.print("times: ");
//  Serial.println(times);

  for(int i = 0; i < times; i++) {
    digitalWrite(4, HIGH);
    delay(calculatedDelay * 500);
    digitalWrite(4, LOW);
    delay(calculatedDelay * 500);
  }
}
