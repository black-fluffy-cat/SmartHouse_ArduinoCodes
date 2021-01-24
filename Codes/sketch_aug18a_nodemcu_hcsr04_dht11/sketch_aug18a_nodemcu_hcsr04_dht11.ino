#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "DHT.h"

#define DHT11_PIN 15
#define trigPin 16 // D0 on Nodemcu
#define echoPin 5 // D1 on Nodemcu

const char* ssid = "TP-LINK_E6565C"; // WiFi name
const char* password = ""; // WiFi password

const char* serverName = "http://192.168.0.108:8080/sensors/data";

unsigned long actualTime = 0;
unsigned long requestLastTime = 0;
unsigned long dhtLastTime = 0;

unsigned long timerDelay = 100;


int dhtMinimumSamplingPeriod = 1000;
int temperature = 0;

DHT dht;
void setup() {
  Serial.begin(115200);
  
  dht.setup(DHT11_PIN);

  dhtMinimumSamplingPeriod = dht.getMinimumSamplingPeriod();
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("");
  Serial.print("Connected, my IP address is: ");
  Serial.println(WiFi.localIP());
 }

void loop() {
  int distance = calculateDistance();
  
  actualTime = millis();

  if (abs(actualTime - dhtLastTime) >= dhtMinimumSamplingPeriod) {
    temperature = dht.getTemperature();  
  }
  
  if (abs(actualTime - requestLastTime) >= timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      http.begin(serverName);
      
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"deviceName\":\"Nodemcu_1\",\"message\":\"" + String(distance) + " " + String(temperature) + "\"}");
//      int httpResponseCode = http.POST("{\"message\":\"" + String(distance) + "\"}");
     
      Serial.print("Response code: ");
      Serial.println(httpResponseCode);
        
      http.end();
    }
    else {
      Serial.println("Disconnected");
    }
    requestLastTime = millis();
  }
}

int calculateDistance() {
  long measuredTime, distance;
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  measuredTime = pulseIn(echoPin, HIGH);
  distance = measuredTime * 0.034/2;

  return distance;
}
