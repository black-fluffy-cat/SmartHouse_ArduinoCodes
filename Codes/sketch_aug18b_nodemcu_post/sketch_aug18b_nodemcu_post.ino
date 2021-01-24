#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "TP-LINK_E6565C"; // WiFi name
const char* password = ""; // WiFi password

const char* serverName = "http://192.168.0.108:8080/sensors/data";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected, my IP address is: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      http.begin(serverName);
      
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"message\":\"Message from nodemcu\"}");

      Serial.print("Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    }
    else {
      Serial.println("Disconnected");
    }
    lastTime = millis();
  }
}
