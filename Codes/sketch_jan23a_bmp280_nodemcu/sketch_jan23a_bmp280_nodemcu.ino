#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
 
Adafruit_BMP280 bme; // I2C
//Adafruit_BMP280 bme(BMP_CS); // hardware SPI
//Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

float bmeTemperature = 0;
float bmePressurePa = 0;
float bmePressurehPa = 0;
float bmeAltitude = 0;

const char* networkSSID = "TP-LINK_E6565C_RASP"; // WiFi name
const char* networkPwd = ""; // WiFi password

String deviceName = "NodeMCU_+_BME280_second_sensor";

String serverUrl = "http://10.42.0.1:8080";
String dataEndpoint = serverUrl + "/bme280Data";
String heartbeatEndpoint = serverUrl+ "/heartbeat";

unsigned long lastHeartbeatTime = 0;
unsigned int heartbeatSendIntervalMs = 30000;
unsigned long startTime = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("BME280 test"));
   
  startTime = millis();
  
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
    
  WiFi.begin(networkSSID, networkPwd);
  Serial.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected, my IP address is: ");
  Serial.println(WiFi.localIP());
}
 
void loop() { 
  updateReadings();
//  printReadings();

  sendReadings();
  
  if (shouldSendHeartbeat()) {
    lastHeartbeatTime = millis();
    sendHeartbeat();
  }
  
  Serial.println();
  delay(10000);
}

void updateReadings() {
  bmeTemperature = bme.readTemperature();
  bmePressurePa = bme.readPressure();
  bmePressurehPa = bmePressurePa / 100;
  bmeAltitude = bme.readAltitude();
}

void printReadings() {
  Serial.print("Temperature = ");
  Serial.print(bmeTemperature);
  Serial.println(" *C");
   
  Serial.print("Pressure = ");
  Serial.print(bmePressurehPa);
  Serial.println(" Pa");
   
  Serial.print("Approx altitude = ");
  Serial.print(bmeAltitude);
  Serial.println(" m");
}

void sendReadings() {
  sendRequest(dataEndpoint, getDataJson());
}

bool shouldSendHeartbeat() {
  return (millis() - lastHeartbeatTime) > heartbeatSendIntervalMs;
}

void sendHeartbeat() {
  sendRequest(heartbeatEndpoint, getHeartbeatJson());  
}

void sendRequest(String url, String json) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      
      http.begin(url);
      
      http.addHeader("Content-Type", "application/json");
      Serial.print("Sending JSON: ");
      Serial.println(json);
      int httpResponseCode = http.POST(json);

      Serial.print("Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    } else {
      Serial.println("Disconnected");
    }
}

String getDataJson() {  
  String firstPart = "{\"deviceName\":\"";
  String thirdPart = "\",\"temperatureC\":\"";
  String fourthPart = "\",\"pressurehPa\":\"";
  String fifthPart = "\",\"altitudeM\":\"";
  String sixthPart = "\"}";
  return firstPart + deviceName + thirdPart + String(bmeTemperature) + fourthPart + String(bmePressurehPa) + fifthPart + String(bmeAltitude) + sixthPart;
}

String getHeartbeatJson() {
  unsigned long currentTime = millis();
  unsigned long timeFromStart = currentTime - startTime;
  String firstPart = "{\"deviceName\":\"";
  String secondPart = "\",\"timeFromStart\":\"";
  String timeFromStartString = String(timeFromStart);
  String thirdPart = "\",\"timeFromAlert\":\"";
  String timeFromAlertString = String(-1);
  String fourthPart = "\"}";
  return firstPart + deviceName + secondPart + timeFromStartString + thirdPart + timeFromAlertString + fourthPart;
}
