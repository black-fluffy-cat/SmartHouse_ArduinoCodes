#include "DHT.h"
#include <SoftwareSerial.h> 

#define echoPin 2
#define trigPin 3
#define DHT11_PIN 4

unsigned long actualTime = 0;
unsigned long hcsr04LastTime = 0;
unsigned long dhtLastTime = 0;

int dhtMinimumSamplingPeriod = 1000;

int distance = 0;
int temperature = 0;

DHT dht;

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
  pinMode(LED_BUILTIN, OUTPUT);
  dht.setup(DHT11_PIN);

  dhtMinimumSamplingPeriod = dht.getMinimumSamplingPeriod();
  Serial.println("DHT minimum sampling period " + String(dhtMinimumSamplingPeriod));
}
 
void loop() {
  actualTime = millis();

  if (abs(actualTime - hcsr04LastTime) >= 100UL) {
    distance = measureDistance();
  }
  
  actualTime = millis();
  
  if (abs(actualTime - dhtLastTime) >= dhtMinimumSamplingPeriod) {
    temperature = dht.getTemperature();  
  }
  
  Serial.println(String(distance) + "/" + String(temperature) + "#");
  BTserial.print(String(distance) + "/" + String(temperature) + "#");
  
  delay(100);
} 
 
int measureDistance() {
  long pulse, distance;
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  pulse = pulseIn(echoPin, HIGH);
  distance = pulse * 0.034/2;
  return distance;
}
