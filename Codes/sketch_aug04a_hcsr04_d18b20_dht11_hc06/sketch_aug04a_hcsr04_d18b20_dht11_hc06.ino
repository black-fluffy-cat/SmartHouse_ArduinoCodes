#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <SoftwareSerial.h> 

#define echoPin 2
#define trigPin 3
#define DHT11_PIN 4
#define ONE_WIRE_BUS 5

unsigned long actualTime = 0;
unsigned long hcsr04LastTime = 0;
unsigned long dhtLastTime = 0;
unsigned long d18b20LastTime = 0;

int hcsr04SamplingPeriod = 100;
int dhtMinimumSamplingPeriod = 1000;
int d18b20SamplingPeriod = 2000;

int distance = 0;
int dhtTemperature = 0;
int d18b20Temperature = 0;

DHT dht;
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature d18b20Sensor(&oneWire);

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
  d18b20Sensor.begin();
  dht.setup(DHT11_PIN);

  dhtMinimumSamplingPeriod = dht.getMinimumSamplingPeriod();
  Serial.println("DHT minimum sampling period " + String(dhtMinimumSamplingPeriod));
}
 
void loop() {
  actualTime = millis();

  Serial.println("Reading from hcsr04");
  if (abs(actualTime - hcsr04LastTime) >= hcsr04SamplingPeriod) {
    hcsr04LastTime = actualTime;
    distance = measureDistance();
  }
  
  actualTime = millis();
  
  Serial.println("Reading from dht");
  if (abs(actualTime - dhtLastTime) >= dhtMinimumSamplingPeriod) {
    dhtLastTime = actualTime;
    dhtTemperature = dht.getTemperature();  
  }

  actualTime = millis();

  Serial.println("Reading from d18b20");
  if (abs(actualTime - d18b20LastTime) >= d18b20SamplingPeriod) {
    d18b20LastTime = actualTime;
    d18b20Sensor.requestTemperatures();
    d18b20Temperature = d18b20Sensor.getTempCByIndex(0);
  }
  
  Serial.println(String(distance) + "/" + String(dhtTemperature) + "/" + String(d18b20Temperature) + "#");
  BTserial.print(String(distance) + "/" + String(dhtTemperature) + "/" + String(d18b20Temperature) + "#");
  
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
