#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
 
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
 
Adafruit_BMP280 bme; // I2C
//Adafruit_BMP280 bme(BMP_CS); // hardware SPI
//Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

float bmeTemperature = 0;
float bmePressure = 0;
float bmeAltitude = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));
   
  if (!bme.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  } 
}
 
void loop() { 
  updateReadings();
  printReadings();
  
  Serial.println();
  delay(2000);
}

void updateReadings() {
  bmeTemperature = bme.readTemperature();
  bmePressure = bme.readPressure();
  bmeAltitude = bme.readAltitude();
}

void printReadings() {
  Serial.print("Temperature = ");
  Serial.print(bmeTemperature);
  Serial.println(" *C");
   
  Serial.print("Pressure = ");
  Serial.print(bmePressure);
  Serial.println(" Pa");
   
  Serial.print("Approx altitude = ");
  Serial.print(bmeAltitude);
  Serial.println(" m");
}
