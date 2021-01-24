

/* Sending Sensor Data to Nordic BLE android app  by CircuitDigest(www.circuitdigest.com)
  works with nRF24L01. and the works for Nordic's
   It reads temperature from a DHT11 and sends it via BTLE.
   Works with Nordic Semiconductor apps such as
   "nRF Connect for Mobile" and "nRF Temp 2.0 for BLE"
   Pin Mapping:
  GND -> GND on the Arduino
  VCC -> 3.3v on the Arduino
  CE -> PIN 9 on the Arduino
  CSN -> PIN 10 on the Arduino
  SCK -> PIN 13 on the Arduino Uno
  MOSI -> PIN 11 on the Arduino Uno
  MISO -> PIN 12 on the Arduino Uno
  IRQ -> not used
*/

#include <SPI.h>
#include <RF24.h>
#include <BTLE.h>
#include <DHT.h>                                                            // dht11 temperature and humidity sensor library

#define DHTPIN 4                                                 // select dht type as DHT 11 or DHT22
DHT dht;

RF24 radio(9, 10); // CE, CSN
BTLE btle(&radio);

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.print("BLE and DHT Starting... ");
  Serial.println("Send Temperature Data over BTLE");
  dht.setup(DHTPIN);
  btle.begin("CD Temp");    // 8 chars max
  Serial.println("Successfully Started");
}

void loop() {                                              
  float temp = dht.getTemperature();   //read temperature data
  if (isnan(temp)) {
    temp = 0.0;
  }
  
  Serial.print(" Temperature: ");  Serial.print(temp);  Serial.println(" °C ");
  nrf_service_data buf;
  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;
  buf.value = BTLE::to_nRF_Float(temp);

  if (!btle.advertise(0x16, &buf, sizeof(buf))) {
    Serial.println("BTLE advertisement failed..!");
  }
  btle.hopChannel(); 
  delay(500);
}
